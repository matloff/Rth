#' Parallel Computation of Distances
#'
#' Similar to R's \code{dist()}.
#'
#' Calling \code{rthdist()} on the matrix \code{m} returns the euclidean
#' distances between all pairs of rows in \code{m}.
#'
#' @aliases rthdis
#' @param m Matrix/data frame for between-rows distance computation.
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return Object of R class \code{"matrix"}.  Duplicate computation is avoided
#' in \code{rthdist()} but the entire matrix is constructed.
#' @examples
#'
#' \dontrun{
#' library(Rth)
#' m1 <- rbind(1:2,3:4)
#' m1
#' rthdist(m1)
#' }
#'
#' @export
rthdist <- function(m, nthreads=rth.nthreads()) {
   if (is.data.frame(m)) m <- as.matrix(m)
   .Call(c_rthdist, m, as.integer(nthreads))
}


