describe("rthnorm", {
  it("behaves like norm", {
    x <- cbind(1, 1:10)

    rthval <- rthnorm(x)
    rval <- norm(x, type="2")

    expect_equal(rthval, rval, tolerance = 0.1)
  })
})
