describe("rthxpos", {
  it("transposes a vector", {
    x <- matrix(runif(30), 5, 6)

    rthval <- rthxpos(x)
    rval <- t(x)

    expect_equal(rthval, rval, tolerance = 0.1)
  })
})
