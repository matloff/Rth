#' Parallel Computaiton of Dinctances Between Matricies
#'
#' Similar to R's \code{pdist()}.
#' The call \code{rthpdist(m1,m2)} returns the distance from all rows of
#' \code{m1} to all in \code{m2}.
#'
#' @aliases rthpdist
#' @param m1 First matrix/data frame for between-matrices distance computation.
#' @param m2 Second matrix/data frame for between-matrices distance
#' computation.
#' @param nthreads An optional argument for specifying the number of threads
#'  (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return Object of R class \code{"matrix"}.  Duplicate computation is avoided
#' in \code{rthdist()} but the entire matrix is constructed.
#' @examples
#'
#' \dontrun{
#' library(Rth)
#' m1 <- rbind(1:2,3:4)
#' m2 <- matrix(c(1,0,1,6),ncol=2)
#' m1
#' m2
#' rthpdist(m1,m2)
#' }
#'
#' @export
rthpdist <- function(m1,m2, nthreads=rth.nthreads()) {
   if (is.data.frame(m1)) m1 <- as.matrix(m1)
   if (is.data.frame(m2)) m2 <- as.matrix(m2)
   .Call("rthpdist", m1, m2, as.integer(nthreads), PACKAGE="Rth")
}


