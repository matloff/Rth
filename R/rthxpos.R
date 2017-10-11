#' Parallel Matrix Transpose
#'
#' Transposes a matrix
#'
#' Computes the transpose of a vector similar to  \code{t()}
#'
#' @param m a matrix to transpose
#' @return the transposed matrix
#' @examples
#'
#' \dontrun{
#' m <- matrix(runif(30), 5, 6)
#' rthxpos(m)
#' }
#'
#' @export
rthxpos <- function(m) {
   if (is.data.frame(m)) m <- as.matrix(m)
   tmp <- .Call("rthxpos",m,PACKAGE="Rth")
   dim(tmp) <- c(ncol(m), nrow(m))
   
   return(tmp)
}


