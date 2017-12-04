#include <thrust/device_vector.h>
#include <thrust/sort.h>
#include <thrust/reduce.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>

#include "Rth.h"
#include "rthutils.h"

/* mean */
struct div_by_n
{
  const int n;

  div_by_n(int _n) : n(_n) {}

  __host__ __device__
  flouble operator()(flouble val) const
  {
    return val/((flouble) n);
  }
};

// FIXME very slow
extern "C" SEXP c_rthmean(SEXP r_input, SEXP nthreads)
{
  SEXP r_avg;
  PROTECT(r_avg = allocVector(REALSXP, 1));
  const int length = LENGTH(r_input);

  RTH_GEN_NTHREADS(nthreads);

  thrust::device_vector<flouble> d_array = rth::to_device_vector<flouble>(
    r_input,
    length
  );

  REAL(r_avg)[0] = (flouble) thrust::transform_reduce(
    d_array.begin(),
    d_array.end(),
    div_by_n(length),
    (flouble) 0.0,
    thrust::plus<flouble>()
  );

  UNPROTECT(1);
  return r_avg;
}

