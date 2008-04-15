#include "basic_functions.h"

double dexps(double _dblVal)
{
	return exp(_dblVal);
}

void zexps(double _dblRealIn, double _dblImgIn, double *_pdblRealOut, double *_pdblImgOut)
{
	*_pdblRealOut	= dexps(_dblRealIn) * dcoss(_dblImgIn);
	*_pdblImgOut	= dexps(_dblRealIn) * dsins(_dblImgIn);
}