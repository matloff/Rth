# ensure R_LIBS_USER is set
library(testthat)
library(Rth, lib.loc="~/R/Rth-OpenMP")

test_check("Rth")
