#include "geoarea.h"

// used in geoarea_cheap
double haversine (double x)
{
    return ((1. - cos (x)) / 2.0);
}

// Core calculations for a single geodesic area.

// Karney (2013) geodesic
// https://geographiclib.sourceforge.io/geod.html
// https://link.springer.com/content/pdf/10.1007/s00190-012-0578-z.pdf
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

    rout [0] = fabs (A); // geodesic can return -ve areas
    rout [1] = P;

    UNPROTECT (1);

    return out;
}

// From geosphere/R/areaPolygon.R, which includes an old R version with these
// comments:
// Based on code by Jason_Steven (http://forum.worldwindcentral.com/showthread.php?p=69704)
// Reference: Bevis, M. and G. Cambareri, 1987. Computing the area of a
// spherical polygon of arbitrary shape. Mathematical Geology 19: 335-346 SEXP
// This code is lightly adapted from the above link.
SEXP R_one_geoarea_cheap (SEXP lons_, SEXP lats_)
{
    size_t n = (size_t) length (lons_);

    double *rlons, *rlats, *rout;

    SEXP out = PROTECT (allocVector (REALSXP, 1));

    rlons = REAL (lons_);
    rlats = REAL (lats_);
    rout = REAL (out);

    double lam1 = 0, lam2 = 0, beta1 =0, beta2 = 0, cosB1 =0, cosB2 = 0;
    double hav = 0;
    double sum = 0;

    // convert lons & lats to radians:
    for (int i = 0 ; i < n; i++ )
    {
        rlons [i] = rlons [i] * M_PI / 180;
        rlats [i] = rlats [i] * M_PI / 180;
    }

    for (int j = 0 ; j < n; j++ )
    {
        int k = j + 1;
        if (j == 0)
        {
            lam1 = rlons [j];
            beta1 = rlats [j];
            lam2 = rlons [j + 1];
            beta2 = rlats [j + 1];
            cosB1 = cos (beta1);
            cosB2 = cos (beta2);
        }
        else
        {
            k = (j + 1) % n;
            lam1 = lam2;
            beta1 = beta2;
            lam2 = rlons [k];
            beta2 = rlats [k];
            cosB1 = cosB2;
            cosB2 = cos (beta2);
        }
        if (lam1 != lam2)
        {
            hav = haversine (beta2 - beta1) + 
                cosB1 * cosB2 * haversine (lam2 - lam1);
            double a = 2 * asin (sqrt (hav));
            double b = M_PI / 2 - beta2;
            double c = M_PI / 2 - beta1;
            double s = 0.5 * ( a + b + c );
            double t = tan ( s / 2 ) * tan ( ( s - a ) / 2 ) *  
                tan ( ( s - b ) / 2 ) * tan ( ( s - c ) / 2 );

            double excess = fabs (4 * atan (sqrt (fabs (t))));

            if (lam2 < lam1)
            {
                excess = -excess;
            }

            sum += excess;
        }
    }

    rout [0] = fabs (sum) * earth * earth;

    UNPROTECT (1);

    return out;
}
