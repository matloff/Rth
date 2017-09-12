describe("rthdist", {
  it("behaves like dist", {
    x <- matrix(rnorm(100), nrow = 5)

    rthval <- rthdist(x)
    rval <- dist(x)

    # The value returned by Rth is a matrix. In order to get the right comparison
    # of values, we need to convert the index of Rth to the index of R
    expect_equal(rthval[2], rval[1], tolerance = 0.1)
    expect_equal(rthval[3], rval[2], tolerance = 0.1)
    expect_equal(rthval[5], rval[4], tolerance = 0.1)
  })
})
