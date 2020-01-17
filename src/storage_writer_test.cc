#include "storage.h"

#include <string>

std::string branchnames[]={"first","second","third","forth"};


int main()
{

  storage::writer  writer("test.dat");


  //we'll write 10 events, with 4 branches each

  for (int event=0;event<10;event++)
    {
      for (int branch=0;branch<4;branch++)
        {
          auto brv= writer.add(branchnames[branch]);
          brv->resize(4);
          unsigned int *val= (unsigned int *) brv->data();
          *val=event*branch;
        }
      writer.writeEvent(event*event);
    }


  return 0;

}
