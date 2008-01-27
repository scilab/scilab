/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* ENPC */
/*--------------------------------------------------------------------------*/
#include "getfastcode.h"
#include "stack-c.h"
#include "stack-def.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/
/**
* Scilab not uses ASCII code in internal kernel
* TO DO : Remove this with a new kernel
* To understand Scilab coding style :
* scilab code --> ascii code
*           0 --> 0
*           9 --> 9
*          10 --> a
*          35 --> z
*         -10 --> A
*         -35 --> Z
*/
#define NUMBER_INTERNAL_CHARACTERS 128
static int INTERNAL_CHARACTERS_TABLE_CODES_FOR_SCILAB[NUMBER_INTERNAL_CHARACTERS] =  
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
/*--------------------------------------------------------------------------*/
/* Copyright INRIA / ENPC
 * Modified by Bruno Pincon for optimisation purpose */
integer C2F(getfastcode)(unsigned char *c, unsigned long c_len) 
{
	return convertAsciiCodeToScilabCode(c[0]);
} 
/*--------------------------------------------------------------------------*/
int convertAsciiCodeToScilabCode(unsigned char scilab_char)
{
	int k = (int)scilab_char ;
	int val = 0;
	if (k < NUMBER_INTERNAL_CHARACTERS) val = INTERNAL_CHARACTERS_TABLE_CODES_FOR_SCILAB[k];
	else val = k + 100;
	return val;
}
/*--------------------------------------------------------------------------*/
char convertScilabCodeToAsciiCode(int scilab_code)
{
	#define eol 99
	#define EXCLAMATION_CHAR '!'
	char ascii_code = 0;

	if (scilab_code == eol) ascii_code = EXCLAMATION_CHAR;
	else if (Abs(scilab_code) > csiz) 
	{
		if (scilab_code > eol) ascii_code = (char)(scilab_code - (eol + 1));
		else ascii_code = EXCLAMATION_CHAR;
	} 
	else if (scilab_code < 0) ascii_code = C2F(cha1).alfb[Abs(scilab_code)];
	else ascii_code = C2F(cha1).alfa[scilab_code];

	return ascii_code;
}
/*--------------------------------------------------------------------------*/
