#ifndef __RTH_UTILS__
#define __RTH_UTILS__

#include <thrust/device_vector.h>

#include "Rth.h"

namespace rth
{
  __device__
  inline int row_major_index(int index, int rows, int offset)
  {
    return index * rows + offset;
  }

  template<typename T>
  inline thrust::device_vector<T> make_device_vector (int size)
  {
    return thrust::device_vector<T>(size);
  }

  // TODO: find better name
  // can it be combined with other function?
  template<typename T>
  inline thrust::device_vector<T> to_device_vector_int (
    SEXP r_val,
    int size
  )
  {
    return thrust::device_vector<T>(INTEGER(r_val), INTEGER(r_val) + size);
  }

  template<typename T>
  inline thrust::device_vector<T> to_device_vector (
    SEXP r_val,
    int size
  )
  {
    return thrust::device_vector<T>(REAL(r_val), REAL(r_val) + size);
  }
}

#endif

