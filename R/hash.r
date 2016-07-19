hash96 <- function(a, b, c)
{
  .Call("R_mix_96", as.integer(a), as.integer(b), as.integer(c), PACKAGE="Rth")
}

rth.getseed <- function()
{
  date <- as.integer(Sys.Date())
  time <- as.integer(Sys.time())
  pid <- as.integer(Sys.getpid())
  
  ret <- hash96(time, pid, date)
  ret <- as.integer(ret)
  
  return( ret )
}
