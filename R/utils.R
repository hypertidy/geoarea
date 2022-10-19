
#' find_xy_cols
#'
#' Find the lon and lat cols of a rectangular object
#' @param obj Rectangular object
#' @return Vector of two column indices of longitude and latitude
#' @noRd
find_xy_cols <- function (obj) {

    nms <- names (obj)
    if (is.null (nms))
        nms <- colnames (obj)

    if (!is.null (nms)) {

        ix <- match_xy_pattern (nms, value = "x")
        iy <- match_xy_pattern (nms, value = "y")
        if (length (ix) != 1 | length (iy) != 1)
            stop ("Unable to determine longitude and latitude columns; ",
                  "perhaps try re-naming columns.")
    } else {

        message ("object has no named columns; assuming order is lon then lat")
        ix <- 1
        iy <- 2
    }
    c (ix, iy)
}

match_xy_pattern <- function (nms, value = "x") {

    value <- match.arg (value, c ("x", "y"))
    lonlat <- ifelse (value == "x", "lon", "lat")

    ptn <- paste0 ("^", value, "|", value, "$|^", lonlat, "|", lonlat, "$")
    i <- grep (ptn, nms, ignore.case = TRUE)

    if (length (i) > 1) {
        # exclude any with :alpha: before or after x/lon:
        ptn <- paste0 ("^", value, "[[:alpha:]]|",
                       "[[:alpha:]]", value, "$|",
                       "^", lonlat, "[[:alpha:]]|",
                       "[[:alpha:]]", lonlat, "$")
        i <- i [which (!seq_along (i) %in% grep (ptn, nms [i]))]
    }

    if (length (i) == 0) {
        # search for match to initial part only
        ptn <- paste0 ("^", value, "|^", lonlat)
        i <- grep (ptn, nms, ignore.case = TRUE)
    }

    if (length (i) != 1) {
        # try initial or terminal punct characters before/after x/y/lon/lat:
        ptn <- paste0 ("^[[:punct:]]+", value, "|", value, "[[:punct:]]+$|",
                       "^[[:punct:]]+", lonlat, "|", lonlat, "[[:punct:]]+$")
        i <- grep (ptn, nms, ignore.case = TRUE)
    }

    return (i)
}

#' convert_to_matrix
#'
#' Convert a rectangular object to a matrix of two columns, lon and lat.
#'
#' @param obj Rectagular object
#' @return Numeric matrix of two columns: lon and lat
#' @noRd
convert_to_matrix <- function (obj) {

    xy_cols <- find_xy_cols (obj)
    if (is.vector (obj))
        obj <- matrix (obj, nrow = 1)
    if (is.numeric (obj)) {

        cbind (obj [, xy_cols [1]], obj [, xy_cols [2]])
    } else {

        if (!(is.numeric (obj [, xy_cols [1], drop = TRUE]) &
              is.numeric (obj [, xy_cols [2], drop = TRUE]))) {

            cbind (as.numeric (obj [, xy_cols [1], drop = TRUE]),
                   as.numeric (obj [, xy_cols [2], drop = TRUE]))
        } else {

            cbind (obj [[xy_cols [1] ]], obj [[xy_cols [2] ]]) # nolint
        }
    }
}
