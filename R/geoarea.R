#' Calculate the area of a geodesic polygon.
#'
#' Calculation is based on the "geographiclib" code of Charles Karney
#' <doi:10.1007/s00190-012-0578-z>, available at
#' \url{https://github.com/geographiclib/geographiclib-c}. These calculations
#' are available in many other libraries and R packages, and are notably
#' included in the 'PROJ' library, and used by many packages which depend on
#' that library, including \pkg{sf}. The \pkg{geosphere} package also includes
#' the C++ version of this code, but depends on the obsolete \pkg{sp} package.
#'
#' In contrast to all other implementations, this function performs no
#' pre-processing or input checking whatsoever, and simply aims to provide the
#' fastest way to calculate areas of polygons expressed in longitudes and
#' latitudes.
#'
#' @param x Rectangular object (matrix, \code{data.frame}, \pkg{tibble},
#' whatever) containing longitude and latitude coordinates of a polygon.
#' @param spherical If `TRUE`, calculate approximate area presuming spherical
#' geometry, using method of Bevis and Cambareri (1987)
#' <doi:10.1007/BF00897843>.
#' @return A vector of two numbers quantifying the area and perimter of the
#' polygon 'x'. The `cheap = TRUE` method does not calculate perimeters, and
#' resturns `NA` for the second value.
#'
#' @examples
#' # Perimeter of Antarctica as documented in original 'geographiclib' code
#' lats <- c (-72.9, -71.9, -74.9, -74.3, -77.5, -77.4, -71.7, -65.9, -65.7,
#'            -66.6, -66.9, -69.8, -70.0, -71.0, -77.3, -77.9, -74.7)
#' lons <- c (-74, -102, -102, -131, -163, 163, 172, 140, 113, 88, 59, 25, -4,
#'            -14, -33, -46, -61)
#' xy <- cbind (lons, lats)
#' geoarea (xy)
#' @export
geoarea <- function (x, spherical = FALSE) {

    if (!spherical) {
        res <- .Call ("R_one_geoarea", x [, 1], x [, 2])
    } else {
        res <- c (.Call ("R_one_geoarea_cheap", x [, 1], x [, 2]), NA)
    }
    names (res) <- c ("area", "perimeter")

    return (res)
}
