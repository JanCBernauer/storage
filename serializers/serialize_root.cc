#include "serialize_root.h"


TBufferStorage::TBufferStorage(std::vector<uint8_t>* backing)
  :TBufferFile(TBuffer::kWrite)
{
  outbuf=backing;
  inbuf=NULL;
  SetBuffer(backing->data(),100,false,0);
  Expand(100,false);
}

TBufferStorage::TBufferStorage(const flatbuffers::Vector<uint8_t>* backing)
  :TBufferFile(TBuffer::kRead)
{
  inbuf=backing;
  outbuf=NULL;
  SetBuffer(NULL,100,false,0);
  fBuffer=(char *)inbuf->data();
  fBufSize = inbuf->Length();
  fBufCur  = fBuffer;
  fBufMax  = fBuffer + fBufSize;
}


void TBufferStorage::compact()
{
  if (!outbuf)
    Fatal ("compact","Cannot compact an input buffer");
  outbuf->resize(Length());
}


void TBufferStorage::Expand(Int_t newsize, Bool_t copy)
 {
   if (!outbuf)
     Fatal ("Expand","Cannot expand an input buffer");

   Int_t l  = Length();
  
   outbuf->resize(newsize);
   fBuffer=(char *)outbuf->data();
   fBufSize = newsize;
   fBufCur  = fBuffer + l;
   fBufMax  = fBuffer + fBufSize;
   
  
 }

