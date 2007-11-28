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
  int j = 0;

  /* conversion code ->ascii */
  for (j = 0; j < *n; ++j)
  {
	str[j] = convertScilabCodeToAsciiCode(line[j]);
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(asciitocode)(integer * n,integer * line,char * str,integer * flagx,unsigned long  str_len)
{
	integer j = 0;
	if (*flagx == 1) 
	{
		for (j = 0; j < *n ; ++j) 
		{
			unsigned char current_char = str[j];
			line[j] = convertAsciiCodeToScilabCode(current_char);
		}
	} 
	else 
	{
		for (j = *n -1 ; j >= 0; --j) 
		{
			unsigned char current_char = str[j];
			line[j] = convertAsciiCodeToScilabCode(current_char);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
