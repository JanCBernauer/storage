#include "storage.h"
#include <iostream>
#include "serialize_root.h"

int main()
{
  storage::reader reader("test.dat");

  auto ev=reader.getEvents();

    for (auto e:ev)
      {
       std::cout<<"Event: "<<e<<std::endl;
       auto data=reader.getEvent(e);
       for (auto d:data)
         {
           TBufferStorage buf(d.second);
           TString str;
           str.Streamer(buf);
           std::cout<<"Read from bank "<<d.first<<" : "<<str<<std::endl;

         }

      }
}
