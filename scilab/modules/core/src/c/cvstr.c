/*--------------------------------------------------------------------------*/
#include "cvstr.h"
#include "stack-c.h"
#include "core_math.h"
#include "getfastcode.h"
/*--------------------------------------------------------------------------*/
/* Table of constant values */
static integer cx1 = 1;
static integer c_n1 = -1;
/*--------------------------------------------------------------------------*/
int C2F(cvstr)(integer * n,integer * line,char * str,integer * job,unsigned long str_len)
{
  if (*job == 0) C2F(asciitocode)(n, line, str, &cx1, str_len);
  else C2F(codetoascii)(n, line, str, str_len);
  return 0;
} 
/*--------------------------------------------------------------------------*/
int C2F(cvstr1)(integer *n,integer * line,char * str,integer * job,	unsigned long  str_len)
{
  if (*job == 0) C2F(asciitocode)(n, line, str, &c_n1, str_len);
  else C2F(codetoascii)(n, line, str, str_len);
  return 0;
} 
/*--------------------------------------------------------------------------*/
int C2F(codetoascii)(integer *n,integer * line,char * str, unsigned long str_len)
{
  static integer eol = 99;
  int j = 0, m = 0;

  /* conversion code ->ascii */
  for (j = 0; j < *n; ++j) 
  {
    m = line[j];
    if (m == eol) str[j] = '!';
    else if (Abs(m) > csiz) 
    {
		if (m > eol) str[j] = (char)(m - (eol + 1));
		else str[j] = '!';
    } 
    else if (m < 0) str[j] = C2F(cha1).alfb[Abs(m)];
    else str[j] = C2F(cha1).alfa[m];
  }
  return 0;
} 
/*--------------------------------------------------------------------------*/
int C2F(asciitocode)(integer * n,integer * line,char * str,integer * flagx,unsigned long  str_len)
{
  integer j = 0;
  if (*flagx == 1) 
  {
    for (j = 0; j < *n ; ++j) line[j] = C2F(getfastcode)(str+j, 1L);
  }
  else
  {
    for (j = *n -1 ; j >= 0; --j) line[j] = C2F(getfastcode)(str+j, 1L);
  }
  return 0;
} 
/*--------------------------------------------------------------------------*/
