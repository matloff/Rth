// author: N. Matloff

// Rth replacements for R order(), rank()

#include <thrust/device_vector.h>
#include <thrust/sequence.h>
#include <thrust/sort.h>

#include "Rth.h"
#include "rthutils.h"

/*
TODO: extract these to a different file so we don't have to type
thrust::device_vector ...
typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<double> doublevec;
typedef intvec::iterator intveciter;
typedef doublevec::iterator doubleveciter;
*/

// returns order(x) in out, unless *rnk=TRUE, in which case rank(x) is returned
extern "C" SEXP rthorder(SEXP r_array, SEXP r_rank, SEXP nthreads) 
{
  const int length = LENGTH(r_array);
  bool rank = (bool) INTEGER(r_rank)[0];
  
  SEXP r_ret;
  PROTECT(r_ret = allocVector(INTSXP, length));
  
  RTH_GEN_NTHREADS(nthreads);
  
  thrust::device_vector<flouble> d_array = rth::to_device_vector<flouble>(
    r_array,
    length
  );
  
  thrust::device_vector<int> d_sequence = rth::make_device_vector<int>(length);
  thrust::sequence(d_sequence.begin(), d_sequence.end());
  thrust::sort_by_key(d_array.begin(), d_array.end(), d_sequence.begin());

  if (!rank) 
  {
    thrust::copy(d_sequence.begin(), d_sequence.end(), INTEGER(r_ret));
  }
  else 
  {
    thrust::device_vector<int> d_out = rth::make_device_vector<int>(length);
    thrust::scatter(
      thrust::make_counting_iterator(0),
      thrust::make_counting_iterator(length),
      d_sequence.begin(),
      d_out.begin()
    ); 
    thrust::copy(d_out.begin(), d_out.end(), INTEGER(r_ret));
  }
  
  UNPROTECT(1);
  return r_ret;
}

