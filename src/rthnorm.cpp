#include <thrust/functional.h>
#include <thrust/transform.h>
#include <thrust/transform_reduce.h>
#include <thrust/device_vector.h>

#include <cmath>

#include "Rth.h"
#include "rthutils.h"

struct ppow
{
  const int p;

  ppow(const int p) : p(p) {}

  __host__ __device__
  flouble operator() (flouble val)
  {
    return std::pow(std::abs(val), p);
  }
};


// Euclidean distance of two vectors
/*
double calc_dist(double *x, const double *y, const int len, const double p)
{
  int i;
  double *diff;
  double dist;

  diff = (double *) malloc(len * sizeof(diff));

  for (i=0; i<len; i++)
    diff[i] = x[i] - y[i];

  dist = calc_norm(diff, len, p);

  free(diff);

  return dist;
}
*/

#define ONE_NORM 1
#define TWO_NORM 2

extern "C" SEXP rthnorm(SEXP r_array, SEXP r_p, SEXP nthreads)
{
  SEXP r_norm;
  PROTECT(r_norm = allocVector(REALSXP, 1));

  const int length = LENGTH(r_array);
  const int p = REAL(r_p)[0];

  RTH_GEN_NTHREADS(nthreads);

  thrust::device_vector<flouble> d_array = rth::to_device_vector<flouble>(
    r_array,
    length
  );

  flouble norm = thrust::transform_reduce(
    d_array.begin(),
    d_array.end(),
    ppow(p),
    0.0,
    thrust::plus<flouble>()
  );

  if (p == TWO_NORM)
  {
    norm = std::sqrt(norm);
  }
  else if (p != ONE_NORM)
  {
    norm = std::pow(norm, 1.0/p);
  }
  
  REAL(r_norm)[0] = norm;

  UNPROTECT(1);
  return r_norm;
}

