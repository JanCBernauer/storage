#ifndef __SERIALIZE_ROOT_H_
#define __SERIALIZE_ROOT_H_
#include <TBufferFile.h>
#include <cstdint>
#include <flatbuffers/flatbuffers.h>

class TBufferStorage: public TBufferFile
{
private:
  std::vector<uint8_t> *outbuf;
  const flatbuffers::Vector<uint8_t> *inbuf;
  void AutoExpand(Int_t size_needed);
  void Expand(Int_t newsize, Bool_t copy);
public:

  TBufferStorage(std::vector<uint8_t>* backing);
  TBufferStorage(const flatbuffers::Vector<uint8_t>* backing);
  void compact();
  //  ~TBufferStorage();
  
};

#endif
