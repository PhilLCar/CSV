#ifndef CSV_POINTERARRAY_HEADER
#define CSV_POINTERARRAY_HEADER

#include <osal.h>

namespace csv {
  CSV_LINK class CSV_API PointerArray {
  public:
    PointerArray(int size = 1);
    ~PointerArray();

  public:
    void   operator += (void* element);
    void   operator -= (void* element);
    void** operator [](int index);
    int    size();

  private:
    int    _capacity;
    int    _size;
    void** pointers;
  };
}

#endif