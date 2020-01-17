#ifndef _STORAGE_H_
#define _STORAGE_H_
#include <string>
#include <map>
#include <cstdint>
#include <fstream>
#include "event_generated.h"
#include "index_generated.h"

namespace storage{


class reader
{
private:
  char *indexbuf,*eventbuf;
  std::ifstream file;
  const index *indx;
  const event *evnt;
  std::map<uint32_t,std::string> recordmap;
public:
  reader(std::string filename);
  ~reader();
  std::vector<uint64_t> getEvents();
  std::map<std::string,const flatbuffers::Vector<uint8_t>*> getEvent(uint64_t eventnr);
  
};




class writer
{
private:
  std::map<std::string, uint32_t> recordmap;
  std::ofstream file;
  eventT event;
  indexT index;
public:
  writer (std::string filename);
  ~writer ();

  std::vector<uint8_t> * add(std::string ); // this adds a buffer with the give name, returning a pointer to the std::vector buffer.
  int writeEvent(uint64_t eventnr);
};

}

#endif
