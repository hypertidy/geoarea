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
    lons_ = PROTECT (Rf_coerceVector (lons_, REALSXP));
    lats_ = PROTECT (Rf_coerceVector (lats_, REALSXP));

    rlons = REAL (lons_);
    rlats = REAL (lats_);
    rout = REAL (out);

    // Need to copy to vectors to pass to Karney's code
    double lons_cp [n], lats_cp [n];
    for (int i = 0; i < n; i++)
    {
        lons_cp [i] = rlons [i];
        lats_cp [i] = rlats [i];
    }

    struct geod_geodesic g;
    double A = 0, P = 0;

    geod_init(&g, earth, flattening);
    geod_polygonarea(&g, lats_cp, rlons, (sizeof lats_cp) / (sizeof lats_cp[0]), &A, &P);
    //Rprintf("%.0f %.2f\n", A, P);

    rout [0] = A;
    rout [1] = P;

    UNPROTECT (3);

    return out;
}
