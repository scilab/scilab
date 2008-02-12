/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/* Jean-Baptiste SILVY */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_graphic_export.h"
#include "stack-c.h"
#include "scilabmode.h"
#include "callFunctionFromGateway.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]= 
{
  {sci_xs2bmp,"xs2bmp"},
  {sci_xs2gif,"xs2gif"},
  {sci_xs2jpg,"xs2jpg"},
  {sci_xs2png,"xs2png"},
  {sci_xs2ppm,"xs2ppm"},
  {sci_xs2eps,"xs2eps"},
  {sci_xs2pdf,"xs2pdf"},
  {sci_xs2svg,"xs2svg"},
  {sci_xs2ps,"xs2ps"},
  {sci_xs2emf,"xs2emf"},
  {sci_xs2fig,"xs2fig"},  
  {sci_xg2ps,"xg2ps"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_graphic_export)()
{  
	Rhs = Max(0, Rhs);
	if ( getScilabMode() != SCILAB_NWNI )
	{
		callFunctionFromGateway(Tab);
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("Scilab graphic module disabled -nogui or -nwni mode.\n"));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
