#include "storage.h"
#include <boost/functional/hash.hpp>
#include <boost/crc.hpp>
#include <iostream>

using namespace storage;
using namespace flatbuffers;

writer::writer(std::string filename)
{
  file.open(filename,std::ios_base::out|std::ios_base::binary);
}

std::vector<uint8_t> * writer::add(std::string name,uint32_t flags)
{
  std::unique_ptr<recordT> record(new recordT());


  if (recordmap.find(name)==recordmap.end())
    {
      //have to generate a hash
      boost::hash<std::string> string_hash;
      recordmap[name]=string_hash(name);
    }
  record->id=recordmap[name]; 
  record->flags=flags;

  event.records.push_back(std::move(record));
  return &event.records[event.records.size()-1]->data;
}

int writer::writeEvent(uint64_t eventnr)
{
  event.eventnr=eventnr;

  //we pack the event
  FlatBufferBuilder fbb;
  fbb.Finish(event::Pack(fbb,&event));

  uint32_t size= fbb.GetSize();
 

  //calculate the crc32
  boost::crc_optimal<32> crc_calculator;
  crc_calculator.process_bytes(fbb.GetBufferPointer(),size);
  uint32_t crc32 =crc_calculator.checksum();

  //we write the total size, the crc32 and then the object.
  file.write((const char *) &size,4);
  //we record the file pointer
  index.events.push_back(eventidx(eventnr,file.tellp(),size));

  file.write((const char *) fbb.GetBufferPointer(),size);
  file.write((const char *) &crc32,4);
  
  //clear record vector
  event.records.clear();
  return size+8; // Total size written
}

writer::~writer()
{
  // serialize name map

  for (auto map:recordmap)
    {
      std::unique_ptr<nameidT>name(new nameidT());
      name->id=map.second;
      name->name=map.first;
      index.names.push_back(std::move(name));
    }

  FlatBufferBuilder fbb;
  fbb.Finish(index::Pack(fbb,&index));

  uint32_t size= fbb.GetSize();
  file.write((const char *) fbb.GetBufferPointer(),size);
  file.write((const char *) &size,4);
  file.close();

}



reader::reader(std::string filename)
{
  file.open(filename,std::ios_base::in|std::ios_base::binary);

  //now, try to load the index
  file.seekg(-4,std::ios_base::end);
  uint32_t size;
  file.read((char *)&size,4);
  file.seekg(-(size_t)(size)-4,std::ios_base::end);
  indexbuf=new char[size];

  file.read(indexbuf,size);

 
  indx=flatbuffers::GetRoot<index>(indexbuf);
  auto names=indx->names();
  for (auto &&map:*names)
    {
    recordmap[map->id()]=map->name()->str();
    std::cout<<"Name Mapping:"<<map->id()<<" "<<map->name()->str()<<std::endl;
    }
  eventbuf=NULL;
}

std::vector<uint64_t> reader::getEvents()
{
  std::vector<uint64_t> res;
  res.resize(indx->events()->Length());
  for (int i=0;i<indx->events()->Length();i++)
    {
    res[i]=indx->events()->Get(i)->eventnr();
    std::cout<<"Event Mapping:"<<i<<" "<<indx->events()->Get(i)->eventnr()<<std::endl;
    }
  return res;
  }


std::map<std::string,const flatbuffers::Vector<uint8_t>*> reader::getEvent(uint64_t eventnr)
{
  std::map<std::string,const flatbuffers::Vector<uint8_t>*> res;
  //linear search -- do better todo
  int i;
  for (i=0;i<indx->events()->Length();i++)
    {
      auto evidx=indx->events()->Get(i);
      if (evidx->eventnr()==eventnr)
        break;
    }
  if (i==indx->events()->Length())
    {
      //todo
      printf("Event not found!\n");
      exit(-1);
    }
  auto evidx=indx->events()->Get(i);
  std::cout<<"Reading from offset:"<<evidx->offset()<<" Size:"<<evidx->size()<<std::endl;
  file.seekg(evidx->offset(),std::ios_base::beg);

  if (eventbuf)
    delete[] eventbuf;

  eventbuf=new char[evidx->size()];
  file.read(eventbuf,evidx->size());
  evnt=flatbuffers::GetRoot<event>(eventbuf);

  std::cout<<"Found "<<evnt->records()->Length()<< " records"<<std::endl;

  for (auto r:*evnt->records())
    {
    res[recordmap[r->id()]]=r->data();
    std::cout<<"  ID:"<<r->id()<<" name:"<<recordmap[r->id()]<<std::endl;
    }
  return res;
}

reader::~reader()
{
  delete[] indexbuf;
}
