#include "geoarea.h"

// Core calculations for a single geodesic area.

//' Karney (2013) geodesic
//' https://geographiclib.sourceforge.io/geod.html
//' https://link.springer.com/content/pdf/10.1007/s00190-012-0578-z.pdf
SEXP R_one_geoarea (SEXP lons_, SEXP lats_)
{
    size_t n = (size_t) length (lons_);

    double *rlons, *rlats, *rout;

    SEXP out = PROTECT (allocVector (REALSXP, 2));

    rlons = REAL (lons_);
    rlats = REAL (lats_);
    rout = REAL (out);

    struct geod_geodesic g;
    double A = 0, P = 0;

    geod_init(&g, earth, flattening);
    // Need to copy to vectors to pass to Karney's code, so call code directly
    // as in `geod_polygonarea`:
    struct geod_polygon p;
    geod_polygon_init (&p, FALSE);
    for (int i = 0; i < n; ++i)
        geod_polygon_addpoint (&g, &p, rlats [i], rlons [i]);
    geod_polygon_compute (&g, &p, FALSE, TRUE, &A, &P);
    //Rprintf("%.0f %.2f\n", A, P);

    rout [0] = A;
    rout [1] = P;

    UNPROTECT (1);

    return out;
}
