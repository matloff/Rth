### slow, don't use

#' Calculate the mean of a one dimensional vector
#'
#' Computes the mean like \code{mean()}
#'
#' @param x The data array
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return The mean of the array
#' @author Norm Matloff <matloff@@cs.ucdavis.edu> and Drew Schmidt
#' <schmidt@@math.utk.edu>
#' @examples
#' 
#' \dontrun{
#' x <- c(1:10, 50)
#' x
#' rthmean(x)
#' }
#' 
#' @export
rthmean <- function(x, nthreads=rth.nthreads())
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rthmean", x, as.integer(nthreads), PACKAGE="Rth")
  
  return( ret )
}
