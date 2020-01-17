#include "storage.h"
#include <iostream>

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
           std::cout<<"  "<<d.first<<std::hex<<std::endl;
           for (auto b:*d.second)
             std::cout<<"0x"<<(int) b<<" ";
           std::cout <<std::dec <<std::endl;

         }

      }
}
