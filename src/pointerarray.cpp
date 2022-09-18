#include <pointerarray.h>

namespace csv {
  PointerArray::PointerArray(int size)
  {
    _capacity = size;
    _size     = 0;
    pointers  = new void* [size] {nullptr};
  }

  PointerArray::~PointerArray()
  {
    delete[] pointers;
  }

  void PointerArray::operator +=(void *element)
  {
    if (_size == _capacity) {
      void** np = new void* [(unsigned long long)_capacity << 1] {nullptr};
      if (np != nullptr) {
        _capacity <<= 1;
        for (int i = 0; i < _size; i++) np[i] = pointers[i];
        delete[] pointers;
        pointers = np;
      }
    }
    pointers[_size++] = element;
  }

  void PointerArray::operator-=(void* element)
  {
    for (int i = 0, d = 0; i + d < _size; i++) {
      if (pointers[i] == element) ++d;
      if (d != 0) pointers[i] = pointers[i + d];
    }
    if (_size < _capacity >> 1) {
      if (_size == _capacity) {
        void** np = new void* [(unsigned long long)_capacity >> 1] {nullptr};
        if (np != nullptr) {
          _capacity >>= 1;
          for (int i = 0; i < _size; i++) np[i] = pointers[i];
          delete[] pointers;
          pointers = np;
        }
      }
    }
  }

  void** PointerArray::operator[] (int index)
  {
    return &pointers[index];
  }

  int PointerArray::size()
  {
    return _size;
  }
}