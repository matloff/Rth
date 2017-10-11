#' Parallel Pearson Product Moment Correlation
#'
#' Computes Pearson Product Moment Correlation
#'
#' @param x a numeric vector
#' @param y a numeric vector 
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return The Pearson Moment Correlation
#' @examples
#' 
#' \dontrun{
#'  x <- c(44.4, 45.9, 41.9, 53.3, 44.7, 44.1, 50.7, 45.2, 60.1)
#'  y <- c( 2.6,  3.1,  2.5,  5.0,  3.6,  4.0,  5.2,  2.8,  3.8)
#'  rthpearson(x, y)
#' }
#
#' @export
rthpearson <- function(x,y, nthreads=rth.nthreads()) {
   if (!is.double(x))
      storage.mode(x) <- "double"
   if (!is.double(y))
      storage.mode(y) <- "double"
   
   .Call("rthpearson", x, y, as.integer(nthreads), PACKAGE="Rth")
}

