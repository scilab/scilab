/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_wintools.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
 static gw_generic_table Tab[]=
 {
  {C2F(sci_ddeopen),"ddeopen"},
  {C2F(sci_ddeclose),"ddeclose"},
  {C2F(sci_ddeexec),"ddeexec"},
  {C2F(sci_ddepoke),"ddepoke"},
  {C2F(sci_ddereq),"ddereq"},
  {C2F(sci_ddeisopen),"ddeisopen"},
  {C2F(sci_clipboard),"ClipBoard"},
  {C2F(sci_hidetoolbar),"hidetoolbar"},
  {C2F(sci_toolbar),"toolbar"},
  {C2F(sci_setlanguagemenu),"setlanguagemenu"},
  {C2F(sci_about),"about"},
  {C2F(sci_settextcolor),"settextcolor"},
  {C2F(sci_settextbackgroundcolor),"settextbackgroundcolor"},
  {C2F(sci_filesassociationbox),"filesassociationbox"},
  };
/*-----------------------------------------------------------------------------------*/
int C2F(gw_wintools)()
{  
  Rhs = Max(0, Rhs);
  callFunctionFromGateway(Tab);
  return 0;
}
/*-----------------------------------------------------------------------------------*/
