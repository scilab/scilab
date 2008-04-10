#include "basic_functions.h"

/*abs*/
double dabss(double _dblVal)
{
	return fabs(_dblVal);
}

/*module of complex*/
double dabsz(double _dblRealVal, double _dblImgVal)
{
	double dblAbsReal	= dabss(_dblRealVal);
	double dblAbsImg	= dabss(_dblImgVal);
	double dblMax		= Max(dblAbsReal, dblAbsImg);
	double dblMin		= Min(dblAbsReal, dblAbsImg);
	if(dblMin == 0)
	{
		return dblMax;
	}
	else
	{
		return dblMax * (sqrt(1 + pow(dblMin / dblMax, 2)));
	}
	/*
	W = MAX( XABS, YABS )
	Z = MIN( XABS, YABS )
	*/

	return 0;
}