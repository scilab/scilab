#include "../stack-c.h"

#define TRUE_ 1
#define FALSE_ 0

/* Table of constant values */

static integer cx1 = 1;
static integer c_n1 = -1;

/*------------------------------------------------
 *     converts from ascii to Scilab internal coding 
 *   call cvstr(n,line,str,job) 
 *   n: integer, length of the string to be converted entier 
 *   line: integer array (where Scilab coded string are stored ) 
 *   string: string 
 *   job: integer flag 
 *       1: code-->ascii 
 *       0: ascii-->code 
 *   Copyright INRIA/ENPC 
 ------------------------------------------------ */

int C2F(cvstr)(integer * n,integer * line,char * str,integer * job,
	       unsigned long str_len)
{
  if (*job == 0) 
    C2F(asciitocode)(n, line, str, &cx1, str_len);
  else 
    C2F(codetoascii)(n, line, str, str_len);
  return 0;
} 

/*------------------------------------------------
 * very similar to cvstr but the conversion 
 *    ascii->code is performed from end to the begining 
 ------------------------------------------------ */

int C2F(cvstr1)(integer *n,integer * line,char * str,integer * job,
		unsigned long  str_len)
{
  if (*job == 0) 
    C2F(asciitocode)(n, &line[1], str, &c_n1, str_len);
  else 
    C2F(codetoascii)(n, &line[1], str, str_len);
  return 0;
} 

/*--------------------------------------------- 
 *   converts from Scilab internal coding to ascii 
 *   Copyright INRIA/ENPC 
 --------------------------------------------- */

int C2F(codetoascii)(integer *n,integer * line,char * str,
		     unsigned long str_len)
{
  static integer eol = 99;
  int j, m;
  /*     conversion code ->ascii */
  for (j = 0; j < *n; ++j) {
    m = line[j];
    if (m == eol) 
      {
	str[j] = '!';
      } 
    else if (abs(m) > csiz) 
      {
	if (m > eol) {
	  str[j] = (m - (eol + 1));
	} else {
	  str[j] = '!';
	}
      } 
    else if (m < 0) 
      {
	str[j] = C2F(cha1).alfb[abs(m)];
      } 
    else
      {
	str[j] = C2F(cha1).alfa[m];
      }
  }
  return 0;
} 

/*--------------------------------------------- 
 *   converts from ascii to  Scilab internal coding 
 *   flag can be 1 or -1 and this is used when the 
 *   conversion is made with line and str stored at overlapping 
 *   memory zone 
 *   Copyright INRIA/ENPC 
 *--------------------------------------------- */

int C2F(asciitocode)(integer * n,integer * line,char * str,integer * flagx,
		     unsigned long  str_len)
{
  integer j;
  if (*flagx == 1) {
    for (j = 0; j < *n ; ++j) {
      line[j] = C2F(getcode)(str+j, 1L);
    }
  } else {
    for (j = *n -1 ; j >= 0; --j) {
      line[j] = C2F(getcode)(str+j, 1L);
    }
  }
  return 0;
} 

/*--------------------------------------------- 
 *   converts one ascii to Scilab internal code 
 *   Copyright INRIA/ENPC 
 *--------------------------------------------- */

integer C2F(getcode)(unsigned char * mc,unsigned long mc_len)
{
  static integer eol = 99;
  static integer blank = 40;
  integer k;
  for (k = 0 ; k < csiz; ++k) {
    if ( *mc == C2F(cha1).alfa[k]) 
      {
	return k ;
      }
    else if ( *mc == C2F(cha1).alfb[k]) {
      return - k;
    }
  }
  /*     special characters */
  switch ( *mc ) {
  case 0:  return  eol; break; 
  case 9:  return blank+1; break; 
  case 10: return eol ; break; 
  default : 
    return  *mc + eol + 1;
  }
} 



