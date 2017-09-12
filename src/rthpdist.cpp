#include <stdio.h>
#include <math.h>
#include <thrust/device_vector.h>

#include "Rth.h"
#include "rthutils.h"

// Rth substitute for R's pdist package

// note that in addition to R using column-major order, Rcpp uses that
// format too when converting an R matrix to an Rcpp vector

// in output matrix, element in row i, column j gives the distance from
// row i in dm1 to row j of dm2

// TODO: review this
struct calc_pdist
{
  const thrust::device_vector<flouble>::iterator d_x_matrix;
  const thrust::device_vector<flouble>::iterator d_y_matrix;
  const thrust::device_vector<flouble>::iterator d_out;

  int nr1;
  int nc;
  int nr2;

  flouble *m1;
  flouble *m2;
  flouble *out;

  calc_pdist(
    thrust::device_vector<flouble>::iterator _dm1,
    thrust::device_vector<flouble>::iterator _dm2,
    thrust::device_vector<flouble>::iterator _dout,
    int _nr1, int _nc, int _nr2
  ) : d_x_matrix(_dm1), d_y_matrix(_dm2), d_out(_dout), nr1(_nr1), nc(_nc),
      nr2(_nr2)
  {
    m1 = thrust::raw_pointer_cast(&d_x_matrix[0]);
    m2 = thrust::raw_pointer_cast(&d_y_matrix[0]);
    out = thrust::raw_pointer_cast(&d_out[0]);
  }

  __device__
  void operator()(const int index)
  // will compute and store all the distances from m1's row i
  // to rows of m2
  {
    flouble sum;
    flouble tmp;
    for (int row = 0; row < nr2; row++)
    {
      sum = 0;
      for (int col = 0; col < nc; col++)
      {
        int m1_index = rth::row_major_index(col, nr1, index);
        int m2_index = rth::row_major_index(col, nr2, row);
        tmp = m1[m1_index] - m2[m2_index];
        sum += tmp * tmp;
      }

      int out_index = rth::row_major_index(row, nr1, index);
      out[out_index] = sqrt(sum);
    }
  }
};

// for unknown reasons, this code does not work under TBB
// compute distances from rows of r_x_matrix to rows of r_y_matrix
extern "C" SEXP rthpdist(SEXP r_x_matrix, SEXP r_y_matrix, SEXP nthreads)
{
  SEXP r_out;
  int x_rows = nrows(r_x_matrix);
  int y_rows = nrows(r_y_matrix);
  int cols = ncols(r_x_matrix);
  int x_size = x_rows * cols;
  int y_size = y_rows * cols;

  RTH_GEN_NTHREADS(nthreads);

  thrust::device_vector<flouble> d_x_matrix = rth::to_device_vector<flouble>(
      r_x_matrix,
      x_size
  );
  thrust::device_vector<flouble> d_y_matrix = rth::to_device_vector<flouble>(
      r_y_matrix,
      y_size
  );

  thrust::device_vector<flouble> d_out = rth::make_device_vector<flouble>(
      x_rows * y_rows
  );

  thrust::for_each(
    thrust::make_counting_iterator(0),
    thrust::make_counting_iterator(x_rows),
    calc_pdist(
      d_x_matrix.begin(),
      d_y_matrix.begin(),
      d_out.begin(),
      x_rows,
      cols,
      y_rows
    )
  );

  PROTECT(r_out = allocMatrix(REALSXP, x_rows, y_rows));
  thrust::copy(d_out.begin(), d_out.end(), REAL(r_out));

  UNPROTECT(1);
  return r_out;
}

