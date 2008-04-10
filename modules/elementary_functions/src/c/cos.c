#include "basic_functions.h"


/*cos*/
double dcoss(double _dblVal)
{
	return cos(_dblVal);
}

void zcoss(double _dblRealIn, double _dblImgIn, double* _dblRealOut, double* _dblImgOut)
{
	*_dblRealOut	= dcoss(_dblRealIn) * dcoshs(_dblImgIn);
	*_dblImgOut		= -dsins(_dblRealIn) * dsinhs(_dblImgIn);
}