#include <thrust/device_vector.h>

#include "Rth.h"

typedef thrust::device_vector<int> d_vec;
typedef thrust::device_vector<int>::iterator d_veciter;

struct dochunk
{
  int *data;
  int *output;
  int *lowerbounds;
  int *bases;

  int numobsv;
  int numdims;
  int numcells;
  int numchunks;

  dochunk(d_veciter xiter,
          d_veciter valcountsiter,
          d_veciter lowerbounditer,
          d_veciter basesiter,
          int h_numobsv,
          int h_numdims,
          int h_numcells,
          int h_numchunks)
  : numobsv(h_numobsv),
    numdims(h_numdims),
    numcells(h_numcells),
    numchunks(h_numchunks)
  {
    data = thrust::raw_pointer_cast(&xiter[0]);
    output = thrust::raw_pointer_cast(&valcountsiter[0]);
    lowerbounds = thrust::raw_pointer_cast(&lowerbounditer[0]);
    bases = thrust::raw_pointer_cast(&basesiter[0]);
  }

  __device__
  void operator()(int chunk)
  {
    int chunksize = numobsv / numchunks;
    int first = chunk * chunksize;
    int last = (chunk == numchunks - 1) ? numobsv - 1 : first + chunksize - 1;

    int varval = 0;

    for (int i = first; i <= last; i++)
    {
      int cellnum = 0;
      for (int varnum = 0; varnum < numdims; varnum++)
      {
        varval = data[varnum * numobsv + i] - lowerbounds[varnum];
        cellnum += varval * bases[varnum];
      }
      output[chunk * numcells + cellnum]++;
    }
  }
};

extern "C" SEXP rthtable(SEXP r_x,
                         SEXP r_lowerbound,
                         SEXP r_upperbound,
                         SEXP r_numchunks,
                         SEXP r_nthreads)
{
  SEXP r_valcounts;

  const int numobsv = nrows(r_x);
  const int numdim = ncols(r_x);

  d_vec d_x(INTEGER(r_x), INTEGER(r_x) + numobsv * numdim);
  d_vec d_lowerbound(INTEGER(r_lowerbound), INTEGER(r_lowerbound) + LENGTH(r_lowerbound));
  d_vec d_upperbound(INTEGER(r_upperbound), INTEGER(r_upperbound) + LENGTH(r_upperbound));

  int numcells = 1;
  d_vec d_lowerupperrange(numdim);
  for (int i = 0; i < numdim; i++)
  {
    d_lowerupperrange[i] = d_upperbound[i] - d_lowerbound[i] + 1;
    numcells *= d_lowerupperrange[i];
  }

  PROTECT(r_valcounts = allocVector(INTSXP, numcells));

  const int numchunks = INTEGER(r_numchunks)[0];
  int matrixsize = numchunks * numcells;

  RTH_GEN_NTHREADS(r_nthreads);

  d_vec d_cellcounts(matrixsize);

  d_vec d_bases(numdim);
  int product = 1;
  for (int i = 0; i < numdim; i++)
  {
    d_bases[i] = product;
    product *= d_lowerupperrange[i];
  }

  thrust::for_each(thrust::make_counting_iterator(0),
                   thrust::make_counting_iterator(numchunks),
                   dochunk(d_x.begin(),
                           d_cellcounts.begin(),
                           d_lowerbound.begin(),
                           d_bases.begin(),
                           numobsv,
                           numdim,
                           numcells,
                           numchunks
                     )
                  );

  int h_valcounts[matrixsize];
  thrust::copy(d_cellcounts.begin(), d_cellcounts.end(), h_valcounts);

  for (int cellnum = 0; cellnum < numcells; cellnum++)
  {
    int sum = 0;
    for (int chunknum = 0; chunknum < numchunks; chunknum++)
    {
      sum += h_valcounts[chunknum * numcells + cellnum];
    }

    INTEGER(r_valcounts)[cellnum] = sum;
  }

  UNPROTECT(1);
  return r_valcounts;
}

