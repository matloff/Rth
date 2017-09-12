describe("rthcolsums", {
  it("behaves like colsums", {
    m <- 100
    n <- 10
    x <- matrix(rnorm(m * n), m, n)

    rthval <- rthcolsums(x)
    rval <- colSums(x)

    expect_equal(rthval[1], rval[1], tolerance = 0.1)
    expect_equal(rthval[5], rval[5], tolerance = 0.1)
    expect_equal(rthval[10], rval[10], tolerance = 0.1)
  })
})
