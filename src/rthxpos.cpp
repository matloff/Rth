// matrix transpose, for_each version

// see the alternate version, rthxposnv.cpp that follows the example in
// the Thrust package; the latter is an instructive example of the use
// of thrust::scatter()

#include <thrust/device_vector.h>

#include "Rth.h"
#include "rthutils.h"

// functor; holds iterators for the input and output matrices, and each
// invocation of the function copies from one element from the former to
// the latter
struct transpose
{
  int rows;
  int cols;
  const  thrust::device_vector<flouble>::iterator matrix;
  const  thrust::device_vector<flouble>::iterator transposed;

  transpose(
    thrust::device_vector<flouble>::iterator _m,
    thrust::device_vector<flouble>::iterator _mxp,
    int _nr,
    int _nc
  ) : matrix(_m), transposed(_mxp), rows(_nr), cols(_nc)
  { }

  __device__
  void operator()(const int i)
  {
    int col = i / rows;
    int row = i % rows;

    transposed[row * cols + col] = matrix[col * rows + row];
  }
};

extern "C" SEXP c_rthxpos(SEXP r_matrix)
{
  SEXP r_out;;
  int rows = nrows(r_matrix);
  int cols = ncols(r_matrix);
  int size = rows * cols;

  thrust::device_vector<flouble> d_matrix = rth::to_device_vector<flouble>(
    r_matrix,
    size
  );

  thrust::device_vector<flouble> d_transpose = rth::make_device_vector<flouble>(
    size
  );

  thrust::for_each(
    thrust::make_counting_iterator(0),
    thrust::make_counting_iterator(rows * cols),
    transpose(d_matrix.begin(), d_transpose.begin(), rows, cols)
  );

  PROTECT(r_out = allocVector(REALSXP, size));
  thrust::copy(d_transpose.begin(), d_transpose.end(), REAL(r_out));

  UNPROTECT(1);
  return r_out;
}

