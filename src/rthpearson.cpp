// author: N. Matloff

// Rth implementation of Pearson product-moment correlation

// single-pass, subject to increased roundoff error

#include <thrust/device_vector.h>
#include <thrust/inner_product.h>
#include <math.h>

#include "Rth.h"
#include "rthutils.h"

extern "C" SEXP rthpearson(SEXP r_x, SEXP r_y, SEXP nthreads)
{
  SEXP r_correlation;

  int length = LENGTH(r_x);
  thrust::device_vector<flouble> d_x = rth::to_device_vector<flouble>(
    r_x,
    length
  );
  thrust::device_vector<flouble> d_y = rth::to_device_vector<flouble>(
    r_y,
    length
  );

  flouble zero = (flouble) 0.0;

  RTH_GEN_NTHREADS(nthreads);

  flouble xy = thrust::inner_product(d_x.begin(), d_x.end(), d_y.begin(), zero);
  flouble x2 = thrust::inner_product(d_x.begin(), d_x.end(), d_x.begin(), zero);
  flouble y2 = thrust::inner_product(d_y.begin(), d_y.end(), d_y.begin(), zero);
  flouble xt = thrust::reduce(d_x.begin(), d_x.end());
  flouble yt = thrust::reduce(d_y.begin(), d_y.end());
  flouble xm = xt/length;
  flouble ym = yt/length;
  flouble xsd = sqrt(x2/length - xm*xm);
  flouble ysd = sqrt(y2/length - ym*ym);

  PROTECT(r_correlation = allocVector(REALSXP, 1));
  REAL(r_correlation)[0] = (xy/length - xm*ym) / (xsd*ysd);

  UNPROTECT(1);
  return r_correlation;
}


