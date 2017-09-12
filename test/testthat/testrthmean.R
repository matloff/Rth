describe("rthmean", {
  it("behaves like mean", {
    x <- c(0:10, 50)

    rthval <- rthmean(x)
    rval <- mean(x)

    expect_equal(rthval, rval, tolerance = 0.1)
  })
})
