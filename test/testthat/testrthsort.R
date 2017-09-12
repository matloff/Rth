describe("rthsort", {
  it("implements sort for ints", {
    x <- c(10:1)

    rthval <- rthsort(x)
    rval <- sort(x)

    expect_equal(rthval, rval)
  })

  it("implements sort for doubles", {
    x <- runif(10)

    rthval <- rthsort(x)
    rval <- sort(x)

    expect_equal(rthval, rval, tolerance = 0.1)
  })
})
