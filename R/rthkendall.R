# rthkendall <- function(x,y, nthreads=rth.nthreads())
# {
#   if (!is.double(x))
#     storage.mode(x) <- "double"
#   if (!is.double(y))
#     storage.mode(y) <- "double"
#
#   ret <- .Call(c_rthkendall, x, y, as.integer(nthreads))
#
#   return(ret)
# }
NULL
