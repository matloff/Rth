#include <R.h>
#include <Rdefines.h>

#define INT(x) INTEGER(x)[0]


// The hash function from the thrust devs
unsigned int thrust_hash(unsigned int a)
{
  a = (a+0x7ed55d16) + (a<<12);
  a = (a^0xc761c23c) ^ (a>>19);
  a = (a+0x165667b1) + (a<<5);
  a = (a+0xd3a2646c) ^ (a<<9);
  a = (a+0xfd7046c5) + (a<<3);
  a = (a^0xb55a4f09) ^ (a>>16);
  return a;
}



// Robert Jenkins' 96 bit Mix Function
int mix_96(int a, int b, int c)
{
  a=a-b;  a=a-c;  a=a^(c >> 13);
  b=b-c;  b=b-a;  b=b^(a << 8);
  c=c-a;  c=c-b;  c=c^(b >> 13);
  a=a-b;  a=a-c;  a=a^(c >> 12);
  b=b-c;  b=b-a;  b=b^(a << 16);
  c=c-a;  c=c-b;  c=c^(b >> 5);
  a=a-b;  a=a-c;  a=a^(c >> 3);
  b=b-c;  b=b-a;  b=b^(a << 10);
  c=c-a;  c=c-b;  c=c^(b >> 15);

  return c;
}


SEXP c_R_mix_96(SEXP a, SEXP b, SEXP c)
{
  SEXP ret;
  PROTECT(ret = allocVector(INTSXP, 1));

  INT(ret) = mix_96(INT(a), INT(b), INT(c));

  UNPROTECT(1);
  return ret;
}

