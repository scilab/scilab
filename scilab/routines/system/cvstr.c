#include "../stack-c.h"
#include "../graphics/Math.h"
#define TRUE_ 1
#define FALSE_ 0

/* Table of constant values */

static integer cx1 = 1;
static integer c_n1 = -1;

integer C2F(getfastcode)(unsigned char *c, unsigned long c_len) ;

/*------------------------------------------------
 *   converts from ascii to Scilab internal coding 
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
 * ascii->code is performed from end to the begining 
 ------------------------------------------------ */

int C2F(cvstr1)(integer *n,integer * line,char * str,integer * job,
		unsigned long  str_len)
{
  if (*job == 0) 
    C2F(asciitocode)(n, line, str, &c_n1, str_len);
  else 
    C2F(codetoascii)(n, line, str, str_len);
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
    else if (Abs(m) > csiz) 
      {
	if (m > eol) {
	  str[j] = (m - (eol + 1));
	} else {
	  str[j] = '!';
	}
      } 
    else if (m < 0) 
      {
	str[j] = C2F(cha1).alfb[Abs(m)];
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
      line[j] = C2F(getfastcode)(str+j, 1L);
    }
  } else {
    for (j = *n -1 ; j >= 0; --j) {
      line[j] = C2F(getfastcode)(str+j, 1L);
    }
  }
  return 0;
} 

/*--------------------------------------------- 
 *   converts one ascii to Scilab internal code 
 *   Copyright INRIA/ENPC 
 *   Obsolete replaced by getfascode 
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
  case 0:  return  100; break; 
  case 9:  return 109; break; 
  case 10: return 110 ; break; 
  default : 
    return  *mc + eol + 1;
  }
} 


#include "../machine.h" 

static integer taba2s[128] = 
{ 100,101,102,103,104,105,106,107,108,-40,
  110,111,112,113,114,115,116,117,118,119,
 120,121,122,123,124,125,126,127,128,129,
 130,131, 40, 38,-53, 37, 39, 56, 58, 53,
  41, 42, 47, 45, 52, 46, 51, 48,  0,  1,
   2,  3,  4,  5,  6,  7,  8,  9, 44, 43,
  59, 50, 60,-38,-61,-10,-11,-12,-13,-14,
 -15,-16,-17,-18,-19,-20,-21,-22,-23,-24,
 -25,-26,-27,-28,-29,-30,-31,-32,-33,-34,
 -35, 54, 49, 55, 62, 36,-59, 10, 11, 12,
  13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
  23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
  33, 34, 35,-54, 57,-55, 61,227 };

/*--------------------------------------------------
 * Convert one ascii char to Scilab internal code 
 *     Copyright INRIA/ENPC 
 *      Modified by Bruno Pincon 
 *     the big table (pure) ascii -> scilab code 
 *--------------------------------------------------*/

integer C2F(getfastcode)(unsigned char *c, unsigned long c_len) 
{
  int k = *c ;
  if (k <= 127) {
    return taba2s[*c];
  } else {
    return k + 100;
  }
} 









