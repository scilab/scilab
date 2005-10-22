/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "GraphWindows.h"
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
GW graphwin;
/*-----------------------------------------------------------------------------------*/
LPGW GetGraphWinScilab(void)
{
	return &graphwin;
}
/*-----------------------------------------------------------------------------------*/
GW InitGWStruct(void)
{
	GW GWStruct;

	GWStruct.lpr=NULL;
	GWStruct.hInstance=NULL;
	GWStruct.hPrevInstance=NULL;
	GWStruct.Title=NULL;
	GWStruct.xmax=0;
	GWStruct.ymax=0;
	GWStruct.lptw=NULL;

	GWStruct.Origin.x=0;
	GWStruct.Origin.y=0;

	GWStruct.Size.x=0;
	GWStruct.Size.y=0;

	GWStruct.hPopMenu=NULL;
	GWStruct.resized=FALSE;
	GWStruct.graphtotop=FALSE;
	GWStruct.locked=FALSE;
	GWStruct.ButtonHeight=0;
	GWStruct.szMenuName=NULL;
	
	return GWStruct;
}
/*-----------------------------------------------------------------------------------*/

