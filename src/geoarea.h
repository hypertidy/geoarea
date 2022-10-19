#ifndef GEOAREA_H
#define GEOAREA_H

#include <math.h>

#include <R.h>
#include <Rinternals.h>

#include "WSG84-defs.h"
#include "geodesic.h"

double haversine (double x);

SEXP R_one_geoarea (SEXP lons_, SEXP lats_);
SEXP R_one_geoarea_cheap (SEXP lons_, SEXP lats_);

#endif /* GEOAREA_H */
