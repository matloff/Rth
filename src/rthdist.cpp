#include <math.h>
#include <thrust/device_vector.h>

#include "Rth.h"
#include "rthutils.h"

// Rth substitute for R's dist() function

// note that in addition to R using column-major order, Rcpp uses that format
// too when converting an R matrix to an Rcpp vector

// in output matrix, element in row i, column j gives the distance from row i in
// dm to row j of dm; output is a full matrix, not a triangular version

struct calculate_dist
{
  const thrust::device_vector<flouble>::iterator d_in_matrix;
  const thrust::device_vector<flouble>::iterator d_out_matrix;
  int rows;
  int cols;

  calculate_dist(
    thrust::device_vector<flouble>::iterator in_matrix,
    thrust::device_vector<flouble>::iterator out_matrix,
    int _rows,
    int _cols
  ): d_in_matrix(in_matrix), d_out_matrix(out_matrix), rows(_rows), cols(_cols)
  { }

  __device__
  void operator()(const int index)
  {
    flouble tmp;
    flouble sum;

    for (int row = index + 1; row < rows; row++)
    {
      sum = 0;
      for (int col = 0; col < cols; col++) {
        int current_index = rth::row_major_index(col, rows, index);
        int row_index = rth::row_major_index(col, rows, row);
        tmp = d_in_matrix[current_index] - d_in_matrix[row_index];
        sum += tmp * tmp;
      }

      tmp = sqrt(sum);
      int row_output_index = rth::row_major_index(row, rows, index);
      int current_output_index = rth::row_major_index(index, rows, row);
      d_out_matrix[row_output_index] = d_out_matrix[current_output_index] = tmp;
    }
  }
};

// compute distances from rows of inmat1 to rows of inmat2
extern "C" SEXP rthdist(SEXP r_matrix, SEXP nthreads)
{
  SEXP r_ret;
  int rows = nrows(r_matrix);
  int cols = ncols(r_matrix);
  int size = rows * cols;

  RTH_GEN_NTHREADS(nthreads);

  thrust::device_vector<flouble> d_matrix = rth::to_device_vector<flouble>(
    r_matrix,
    size
  );
  thrust::device_vector<flouble> d_out_matrix = rth::make_device_vector<flouble>(
    rows * rows
  );

  // find the distances from row i in inmat to all rows j of inmat, j > i
  // the counting_iterator will act as the index for the row
  thrust::for_each(
    thrust::make_counting_iterator(0),
    thrust::make_counting_iterator(rows),
    calculate_dist(d_matrix.begin(), d_out_matrix.begin(), rows, cols)
  );

  PROTECT(r_ret = allocMatrix(REALSXP, rows, rows));
  thrust::copy(d_out_matrix.begin(), d_out_matrix.end(), REAL(r_ret));
  UNPROTECT(1);

  return r_ret;
}

