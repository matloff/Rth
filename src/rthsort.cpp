#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/sort.h>

#include "Rth.h"
#include "rthutils.h"

extern "C" SEXP c_rthsort_double(
  SEXP r_input,
  SEXP r_decreasing,
  SEXP r_inplace,
  SEXP nthreads
)
{
  SEXP r_out;
  int length = LENGTH(r_input);
  int decreasing = INTEGER(r_decreasing)[0];
  int inplace = INTEGER(r_inplace)[0];

  RTH_GEN_NTHREADS(nthreads);

  thrust::device_vector<flouble> d_x = rth::to_device_vector<flouble>(
    r_input,
    length
  );

  if (decreasing)
  {
    thrust::sort(d_x.begin(), d_x.end(), thrust::greater<flouble>());
  }
  else
  {
    thrust::sort(d_x.begin(), d_x.end());
  }

  if (inplace)
  {
    thrust::copy(d_x.begin(), d_x.end(), REAL(r_input));
    return R_NilValue;
  }
  else
  {
    PROTECT(r_out = allocVector(REALSXP, length));
    thrust::copy(d_x.begin(), d_x.end(), REAL(r_out));

    UNPROTECT(1);
    return r_out;
  }
}

extern "C" SEXP c_rthsort_int(
   SEXP r_input,
   SEXP r_decreasing,
   SEXP r_inplace,
   SEXP nthreads
)
{
  SEXP r_out;
  int length = LENGTH(r_input);
  int decreasing = INTEGER(r_decreasing)[0];
  int inplace = INTEGER(r_inplace)[0];

  RTH_GEN_NTHREADS(nthreads);

  thrust::device_vector<int> d_x = rth::to_device_vector_int<int>(
    r_input,
    length
  );

  if (decreasing)
  {
    thrust::sort(d_x.begin(), d_x.end(), thrust::greater<int>());
  }
  else
  {
    thrust::sort(d_x.begin(), d_x.end());
  }

  if (inplace)
  {
    thrust::copy(d_x.begin(), d_x.end(), INTEGER(r_input));
    return R_NilValue;
  }
  else
  {
    PROTECT(r_out = allocVector(INTSXP, length));
    thrust::copy(d_x.begin(), d_x.end(), INTEGER(r_out));

    UNPROTECT(1);
    return r_out;
  }
}
