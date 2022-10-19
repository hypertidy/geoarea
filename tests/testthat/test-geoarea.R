
test_all <- identical (Sys.getenv ("MPADGE_LOCAL"), "true")

test_that ("geoarea", {

    # values from geodesic.h area example (L#792); search for "Antarctica"
    lats <- c (-72.9, -71.9, -74.9, -74.3, -77.5, -77.4, -71.7, -65.9, -65.7,
               -66.6, -66.9, -69.8, -70.0, -71.0, -77.3, -77.9, -74.7)
    lons <- c (-74, -102, -102, -131, -163, 163, 172, 140, 113, 88, 59, 25, -4,
               -14, -33, -46, -61)
    xy <- cbind (lons, lats)

    res <- geoarea (xy)
    expect_type (res, "double")
    expect_length (res, 2L)
    expect_named (res)
    expect_identical (names (res), c ("area", "perimeter"))
})
