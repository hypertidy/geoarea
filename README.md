<!-- README.md is generated from README.Rmd. Please edit that file -->

[![R build
status](https://github.com/hypertidy/geoarea/workflows/R-CMD-check/badge.svg)](https://github.com/hypertidy/geoarea/actions?query=workflow%3AR-CMD-check)
[![Project Status: Active – The project has reached a stable, usable
state and is being actively
developed.](http://www.repostatus.org/badges/latest/active.svg)](http://www.repostatus.org/#active)
[![codecov](https://codecov.io/gh/hypertidy/geoarea/branch/master/graph/badge.svg)](https://codecov.io/gh/hypertidy/geoarea)
[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/geoarea)](http://cran.r-project.org/web/packages/geoarea)
![downloads](http://cranlogs.r-pkg.org/badges/grand-total/geoarea)

# geoarea

An ultra-lightweight, zero-dependency package for very fast calculation
of geodesic polygon areas. Calculation is based on the [“geographiclib”
code of Charles Karney](https://doi.org/10.1007/s00190-012-0578-z),
available at \[<https://github.com/geographiclib/geographiclib-c>\]
<https://github.com/geographiclib/geographiclib-c>\](<https://github.com/geographiclib/geographiclib-c>).
These calculations are available in many other libraries and R packages,
and are notably included in [the ‘PROJ’ library](https://proj.org/), and
used by many packages which bundles depend on that library, including
and . All such packages, as well as the package which includes the C++
version of the C code bundled here, are large and generally packages,
requiring extensive installation of a wide range of dependent libraries.
The `geoarea` package in contrast does not depend on any other packages,
and is much “lighter weight” than any of these alternatives.

The package contains one single function, `geoarea()`, which, like in
the sibling package, [`geodist`](https://github.com/hypertidy/geodist),
aims to provide the fastest way of calculating areas of polygons
expressed in longitude and latitude coordinates. This speed is primarily
through this package foregoing any pre-processing stages such as
coordinate reference system checking or transformations. All input
values are passed directly to the core C routines, and return a single
vector of two values quantifying the area and perimeter of the input
polygon.

The following example is taken from [the “geographiclib” source
code](https://github.com/geographiclib/geographiclib-c/blob/d908229507db95664aa4c735c6f5c79f94b0ecfc/src/geodesic.h#L789-L795),
with the coordinates approximately defining the perimeter of Antarctica.

``` r
library (geoarea)
lats <- c (-72.9, -71.9, -74.9, -74.3, -77.5, -77.4, -71.7, -65.9, -65.7,
           -66.6, -66.9, -69.8, -70.0, -71.0, -77.3, -77.9, -74.7)
lons <- c (-74, -102, -102, -131, -163, 163, 172, 140, 113, 88, 59, 25, -4,
           -14, -33, -46, -61)
xy <- cbind (lons, lats)
geoarea (xy)
#>           area      perimeter 
#> 13376856682207       14710425
```

## Even quicker area calculations

The `geoarea()` function has one additional parameter, “spherical”,
which defaults to `FALSE`. Setting to `TRUE` enables very
computationally efficient calculations of polygon areas presuming
spherical, as opposed to ellipsoidal, geometry. The resultant areas will
generally be less accurate than the default method, but are generally
much faster to compute, as demonstrated in the following benchmark
tests.

## Benchmark

The following benchmarks the calculations of this package with the
equivalent C++ routines included with [the `geosphere`
package](https://cran.r-project.org/package=geosphere), and [the `sf`
package](https://cran.r-project.org/package=sf).

``` r
library (bench)
library (sf)
library (geosphere)
lats <- c (-72.9, -71.9, -74.9, -74.3, -77.5, -77.4, -71.7, -65.9, -65.7,
           -66.6, -66.9, -69.8, -70.0, -71.0, -77.3, -77.9, -74.7)
lons <- c (-74, -102, -102, -131, -163, 163, 172, 140, 113, 88, 59, 25, -4,
           -14, -33, -46, -61)
xy <- cbind (lons, lats)
xys <- sf::st_polygon (list (rbind (xy, xy [1, ]))) |>
    sf::st_sfc (crs = 4326)

bench::mark (
    geoarea (xy),
    geoarea (xy, spherical = TRUE),
    geosphere::areaPolygon (xy),
    sf::st_area (xys),
    check = FALSE)
#> # A tibble: 4 × 6
#>   expression                         min   median `itr/sec` mem_alloc `gc/sec`
#>   <bch:expr>                    <bch:tm> <bch:tm>     <dbl> <bch:byt>    <dbl>
#> 1 geoarea(xy)                    21.48µs  22.98µs    42857.      368B     0   
#> 2 geoarea(xy, spherical = TRUE)   5.62µs   6.46µs   148198.      368B    14.8 
#> 3 geosphere::areaPolygon(xy)     23.99µs  25.36µs    39153.      368B     3.92
#> 4 sf::st_area(xys)                3.43ms   3.51ms      281.    2.95MB     6.38
```
