#include <thrust/device_vector.h>

#include "Rth.h"

typedef thrust::device_vector<int> intvec;
typedef thrust::device_vector<int>::iterator intveciter;

struct do1chunk {  // handle one chunk of the data
  intveciter dx; intveciter dvalcounts;
  int nobsv,ndim,ncells,nch,*lb,*lurng,*bases; 
  int *wx, *wcellcounts;
  do1chunk(intveciter _dx,  // data
           intveciter _dvalcounts,  // space for counts
           int *_lb,  // vector of lower bds on data vals
           int *_lurng,  // vector of lower-upper bd rnages
           int *_bases,  // range products for linear cell indices
           int _nobsv,  //  number of data pts
           int _ndim,  //  number of variables
           int _ncells,  // number of possible values of data items
           int _nch):   // number of chunks
           dx(_dx),dvalcounts(_dvalcounts),lb(_lb),lurng(_lurng), bases(_bases),
           nobsv(_nobsv),ncells(_ncells),nch(_nch), ndim(_ndim)
           {  wx = thrust::raw_pointer_cast(&dx[0]);  
              wcellcounts = thrust::raw_pointer_cast(&dvalcounts[0]);  
           }
  __device__
  void operator()(int chunknum)
  {
    int chunksize = nobsv / nch,
      // where does this chunk of the data start and end?
      first = chunknum * chunksize,
      last = first + chunksize - 1;
    if (chunknum == nch - 1) last = nobsv - 1;
    int i;  // observation number, i.e. row number in dx
    int varnum, // column number in x
        varval;  // value of in that column, in obs i, minus lb
    for (i = first; i <= last; i++) {
      int cellnum = 0;
      for (varnum = 0; varnum < ndim; varnum++) {
        varval = wx[varnum*nobsv + i] - lb[varnum];
        cellnum += varval * bases[varnum];
      }
      wcellcounts[chunknum * ncells + cellnum]++;
    }
  }
};

// data x has lower, upper bds given in ub_; count computation will be
// divided into nch_ chunks, performed by nthreads threads
// for unknown reasons, this code does not work under TBB
extern "C" SEXP rthtable(SEXP x, SEXP lb_, SEXP ub_, 
  SEXP nch_, SEXP nthreads)
{
  SEXP rvalcounts;
  
  // x is the data matrix, one observation per row (but stored in
  // column-major order); each column is a separate variable, i.e.
  // a separate dimension in the output table
  const int nobsv = nrows(x);  // number of observations
  const int ndim = ncols(x);  // e.g. 2 for an m x n table
  intvec dx(REAL(x), REAL(x)+nobsv*ndim);  // input data copied to device
  
  // configuration of output table
  //
  // R tables are stored in a generalized column-major format; e.g. 
  // for an m x n x k table, row index varies most rapidly, then column 
  // index and finally layer index
  // 
  // set up the ndim lower- and upper-bound pairs; variable in 
  // dimension i of the table takes on values in lb[i} through ub[i]
  int *lb = INTEGER(lb_);
  int *ub = INTEGER(ub_);
  
  // find number of table cells, ncells, and the range for each
  // variable, in lurng
  int ncells = 1,i;
  int lurng[ndim];  // "lower-upper range"
  for (i = 0; i < ndim; i++) {
    lurng[i] = ub[i] - lb[i] + 1;
    ncells *= lurng[i];
  }
  
  // counts for the table; will eventually be the output
  PROTECT(rvalcounts = allocVector(INTSXP, ncells));
  
  // partitioning of work
  const int nch = INTEGER(nch_)[0];
  
  RTH_GEN_NTHREADS(nthreads);
  
  // form matrix of cell counts, one row per chunk, row-major order
  intvec dcellcounts(nch*ncells);
  
  // products needed to compute linear cell indices
  int bases[ndim],prod=1;
  for (i = 0; i < ndim; i++) {
    bases[i] = prod;
    prod *= lurng[i];
  };
  
  // the heart of the computation, a for_each() loop, one iteration per
  // chunk (chunking should improve cache performance)
  thrust::counting_iterator<int> seqa(0);
  thrust::counting_iterator<int> seqb =  seqa + nch;
  thrust::for_each(seqa,seqb,
    do1chunk(dx.begin(),dcellcounts.begin(),lb,lurng,bases,
      nobsv,ndim,ncells,nch));
  
  // copy result to host and combine the subhistograms
  int hvalcounts[nch*ncells];
  thrust::copy(dcellcounts.begin(),dcellcounts.end(),hvalcounts);
  int cellnum,chunknum;
  for (cellnum = 0; cellnum < ncells; cellnum++) {
    int sum = 0;
    for (chunknum = 0; chunknum < nch; chunknum++) 
      sum += hvalcounts[chunknum*ncells + cellnum];
    
    INTEGER(rvalcounts)[cellnum] = sum;
  }
  
  UNPROTECT(1);
  return rvalcounts;
}

