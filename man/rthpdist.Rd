% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/rthpdist.R
\name{rthpdist}
\alias{rthpdist}
\title{Parallel Computaiton of Dinctances Between Matricies}
\usage{
rthpdist(m1, m2, nthreads = rth.nthreads())
}
\arguments{
\item{m1}{First matrix/data frame for between-matrices distance computation.}

\item{m2}{Second matrix/data frame for between-matrices distance
computation.}

\item{nthreads}{An optional argument for specifying the number of threads
 (in the case of OpenMP or TBB backends) that the routine should (at most)
use.  Does nothing with a CUDA backend.  See \link{nthreads}.}
}
\value{
Object of R class \code{"matrix"}.  Duplicate computation is avoided
in \code{rthdist()} but the entire matrix is constructed.
}
\description{
Similar to R's \code{pdist()}.
The call \code{rthpdist(m1,m2)} returns the distance from all rows of
\code{m1} to all in \code{m2}.
}
\examples{

\dontrun{
library(Rth)
m1 <- rbind(1:2,3:4)
m2 <- matrix(c(1,0,1,6),ncol=2)
m1
m2
rthpdist(m1,m2)
}

}
