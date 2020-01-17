#include "storage.h"
#include "serialize_root.h"
#include <string>

std::string branchnames[]={"first","second","third","forth"};


int main()
{

  storage::writer  writer("test.dat");


  
  for (int event=0;event<1;event++)
    {
      for (int branch=0;branch<4;branch++)
        {
          auto brv= writer.add(branchnames[branch]);
          TBufferStorage buf(brv);
          TString test("Teststringtoserialize");
          test.Streamer(buf);
          buf.compact(); // Have to do it to write only necessary amount

        }
      writer.writeEvent(event*event);
    }


  return 0;

}
