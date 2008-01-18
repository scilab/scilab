/*--------------------------------------------------------------------------*/
/**
  * @INRIA 2008
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_graphic_export.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
typedef int (*Graphic_Export_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct
{
  Graphic_Export_Interf f;    /** function **/
  char *name;      /** its name **/
} Graphic_ExportTable;
/*--------------------------------------------------------------------------*/

static Graphic_ExportTable Tab[]=
{
  {sci_xs2gif,"xs2gif"},
  {sci_xs2ppm,"xs2ppm"},
  {sci_xs2ps,"xs2ps"},
  {sci_xs2bmp,"xs2bmp"},
  {sci_xs2emf,"xs2emf"},
  {sci_xs2fig,"xs2fig"},
  {sci_xs2fig,"xg2ps"}
};

/*--------------------------------------------------------------------------*/
int C2F(gw_graphic_export)()
{  
	Rhs = Max(0, Rhs);

#ifdef _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif

	return 0;
}
/*--------------------------------------------------------------------------*/
