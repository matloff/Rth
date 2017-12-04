#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP c_R_mix_96(SEXP, SEXP, SEXP);
extern SEXP c_rthcolsums(SEXP, SEXP);
extern SEXP c_rthdist(SEXP, SEXP);
extern SEXP c_Rth_get_backend();
extern SEXP c_Rth_get_num_threads();
// extern SEXP c_rthgini(SEXP, SEXP, SEXP, SEXP);
// extern SEXP c_rthhist(SEXP, SEXP, SEXP, SEXP);
// extern SEXP c_rthkendall(SEXP, SEXP, SEXP);
 // extern SEXP c_rthma(SEXP, SEXP, SEXP);
extern SEXP c_rthmean(SEXP, SEXP);
extern SEXP c_rthnorm(SEXP, SEXP, SEXP);
extern SEXP c_rthorder(SEXP, SEXP, SEXP);
extern SEXP c_rthpdist(SEXP, SEXP, SEXP);
extern SEXP c_rthpearson(SEXP, SEXP, SEXP);
// extern SEXP c_rth_rnorm(SEXP, SEXP, SEXP, SEXP, SEXP);
// extern SEXP c_rth_runif(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP c_rthsort_double(SEXP, SEXP, SEXP, SEXP);
extern SEXP c_rthsort_int(SEXP, SEXP, SEXP, SEXP);
extern SEXP c_rthtable(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP c_rthxpos(SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"c_R_mix_96",            (DL_FUNC) &c_R_mix_96,            3},
    {"c_rthcolsums",          (DL_FUNC) &c_rthcolsums,          2},
    {"c_rthdist",             (DL_FUNC) &c_rthdist,             2},
    {"c_Rth_get_backend",     (DL_FUNC) &c_Rth_get_backend,     0},
    {"c_Rth_get_num_threads", (DL_FUNC) &c_Rth_get_num_threads, 0},
//    {"c_rthgini",             (DL_FUNC) &c_rthgini,             4},
//    {"c_rthhist",             (DL_FUNC) &c_rthhist,             4},
//    {"c_rthkendall",          (DL_FUNC) &c_rthkendall,          3},
//    {"c_rthma",               (DL_FUNC) &c_rthma,               3},
    {"c_rthmean",             (DL_FUNC) &c_rthmean,             2},
    {"c_rthnorm",             (DL_FUNC) &c_rthnorm,             3},
    {"c_rthorder",            (DL_FUNC) &c_rthorder,            3},
    {"c_rthpdist",            (DL_FUNC) &c_rthpdist,            3},
    {"c_rthpearson",          (DL_FUNC) &c_rthpearson,          3},
//     {"c_rth_rnorm",           (DL_FUNC) &c_rth_rnorm,           5},
//     {"c_rth_runif",           (DL_FUNC) &c_rth_runif,           5},
    {"c_rthsort_double",      (DL_FUNC) &c_rthsort_double,      4},
    {"c_rthsort_int",         (DL_FUNC) &c_rthsort_int,         4},
    {"c_rthtable",            (DL_FUNC) &c_rthtable,            5},
    {"c_rthxpos",             (DL_FUNC) &c_rthxpos,             1},
    {NULL, NULL, 0}
};

void R_init_Rth(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
