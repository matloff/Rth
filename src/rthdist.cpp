#include <math.h>
#include <thrust/device_vector.h>

#include "Rth.h"

// Rth substitute for R's dist() function

// note that in addition to R using column-major order, Rcpp uses that format
// too when converting an R matrix to an Rcpp vector

// in output matrix, element in row i, column j gives the distance from row i in
// dm to row j of dm; output is a full matrix, not a triangular version

struct distval
{
  const thrust::device_vector<double>::iterator dinmat;
  const thrust::device_vector<double>::iterator doutmat;
  int nrow, ncol;

  distval(
    thrust::device_vector<double>::iterator _dinmat,
    thrust::device_vector<double>::iterator _doutmat,
    int _nrow,
    int _ncol
  ): dinmat(_dinmat), doutmat(_doutmat), nrow(_nrow), ncol(_ncol)
  { }

  // will compute and store all the distances from m's row i to rows j of m,
  // for j > i
  __device__
  void operator()(const int i)
  {
    double tmp, sum;

    // find distance from row i to row j
    // j is row number, k is column number
    for (int j = i + 1; j < nrow; j++)
    {
      sum = 0;
      for (int k = 0; k < ncol; k++)
      {
        tmp = dinmat[k*nrow+i] - dinmat[k*nrow+j];
        sum += tmp * tmp;
      }

      // result goes into elements (i,j) and (j,i) of out, a matrix of
      // dimensions nr x nr
      tmp = sqrt(sum);
      doutmat[j*nrow+i] = doutmat[i*nrow+j] = tmp;
    }
  }
};

// compute distances from rows of inmat1 to rows of inmat2
extern "C" SEXP rthdist(SEXP inmat, SEXP nthreads)
{
  SEXP ret;
  int nrow = nrows(inmat);
  int ncol = ncols(inmat);

  RTH_GEN_NTHREADS(nthreads);

  thrust::device_vector<double> dinmat(REAL(inmat), REAL(inmat) + nrow * ncol);
  thrust::device_vector<double> doutmat(nrow * nrow);

  // find the distances from row i in inmat to all rows j of inmat, j > i
  // the counting_iterator will act as the index for the row
  thrust::for_each(
    thrust::make_counting_iterator(0),
    thrust::make_counting_iterator(nrow),
    distval(dinmat.begin(), doutmat.begin(), nrow, ncol)
  );

  PROTECT(ret = allocMatrix(REALSXP, nrow, nrow));
  thrust::copy(doutmat.begin(), doutmat.end(), REAL(ret));
  UNPROTECT(1);

  return ret;
}

