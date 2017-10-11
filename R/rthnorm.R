#' Parallel Vector Norm
#'
#' Compuetes the norm of a vector
#'
#' Computes the norm of a vector similar to \code{norm()}
#'
#' @param x a numeric vector
#' @param p type of norm to compute
#' @param nthreads An optional argument for specifying the number of threads
#' (in the case of OpenMP or TBB backends) that the routine should (at most)
#' use.  Does nothing with a CUDA backend.  See \link{nthreads}.
#' @return Norm of a vector
#'
#' @examples
#' 
#' \dontrun{
#' x <- c(5,2,12,13,8,0,50)
#' x
#' rthnorm(x)
#' }
#' 
#' @export
rthnorm <- function(x, p=2, nthreads=rth.nthreads())
{
  if (!is.double(x))
    storage.mode(x) <- "double"
  
  ret <- .Call("rthnorm", x, as.double(p), as.integer(nthreads), PACKAGE="Rth")
  
  return( ret )
}

