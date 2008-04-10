#include "basic_functions.h"


/*sin*/
double dsins(double _dblVal)
{
	return sin(_dblVal);
}

void zsins(double _dblRealIn, double _dblImgIn, double* _dblRealOut, double* _dblImgOut)
{
	*_dblRealOut	= dsins(_dblRealIn) * dcoshs(_dblImgIn);
	*_dblImgOut		= dcoss(_dblRealIn) * dsinhs(_dblImgIn);
}
