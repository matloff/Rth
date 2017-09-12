describe("rthorder", {
  it("behaves like order", {
    x <- c(1.2, 0.3, 0.8, 2.8)

    rthval <- rthorder(x)
    rval <- order(x)

    expect_equal(rthval, rval, tolerance = 0.1)
  })

  it("behaves like rank", {
    x <- c(1.2, 0.3, 0.8, 2.8)

    rthval <- rthorder(x, TRUE)
    rval <- rank(x)

    expect_equal(rthval, rval, tolerance = 0.1);
  })
})
