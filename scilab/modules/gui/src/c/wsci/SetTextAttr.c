#include "SetTextAttr.h"
#include "wgnuplib.h"

extern TW textwin;

#define TEXTUSER 0xf1
#define TEXTGNUPLOT 0xf0

void SetTextAttr(int AttrType)
{
	switch (AttrType)
	{
	case 1 :
		TextAttr (&textwin, TEXTUSER);
		break;
	case 2 :
		TextAttr (&textwin, TEXTGNUPLOT);
		break;
	default :
		break;
	}

}