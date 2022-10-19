#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* .Call calls */
extern SEXP R_one_geoarea (SEXP, SEXP);
extern SEXP R_one_geoarea_cheap (SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"R_one_geoarea",          (DL_FUNC) &R_one_geoarea,          2},
    {"R_one_geoarea_cheap",    (DL_FUNC) &R_one_geoarea_cheap,    2},
    {NULL, NULL, 0}
};

void R_init_geoarea(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
