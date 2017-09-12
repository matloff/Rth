/*
 * adapted from
 * https://github.com/thrust/thrust/blob/master/examples/sum_rows.cu
 *
 * reduce_by_key(), in the form here takes input keys, in this case column
 * numbers, and then does reductions based on their unique values; data that
 * have the same input keys will be reduced as a group (it's assumed that the
 * data ordering is such that all data with the same input keys are sontiguous).
 */

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>

#include "Rth.h"
#include "rthutils.h"

// note that R uses column-major order, and Rcpp vectors retain this ordering

// convert a linear index to a column index
struct lin_to_col: public thrust::unary_function<int,int>
{
  int num_rows;

  __host__ __device__
  lin_to_col(int num_rows) : num_rows(num_rows) {}

  __host__ __device__
  int operator()(int index)
  {
    return index / num_rows;
  }
};

extern "C" SEXP rthcolsums(SEXP r_matrix, SEXP nthreads)
{
  SEXP r_ret;

  int rows = nrows(r_matrix);
  int cols = ncols(r_matrix);
  int length = rows * cols;

  RTH_GEN_NTHREADS(nthreads);

  thrust::device_vector<flouble> d_matrix = rth::to_device_vector<flouble>(
    r_matrix,
    length
  );

  thrust::device_vector<flouble> d_col_sums = rth::make_device_vector<flouble>(
    cols
  );
  thrust::device_vector<int> d_col_indices = rth::make_device_vector<int>(rows);
  thrust::reduce_by_key
  (
    thrust::make_transform_iterator(
      thrust::counting_iterator<int>(0),
      lin_to_col(rows)
    ),
    thrust::make_transform_iterator(
      thrust::counting_iterator<int>(length),
      lin_to_col(rows)
    ),
    d_matrix.begin(),
    d_col_indices.begin(),
    d_col_sums.begin()
  );

  PROTECT(r_ret = allocVector(REALSXP, cols));
  thrust::copy(d_col_sums.begin(), d_col_sums.end(), REAL(r_ret));

  UNPROTECT(1);
  return r_ret;
}
