#include "gw_pvm.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #include <Windows.h>
  #include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
extern int sci_gateway __PARAMS((char *fname, GatefuncS F));
extern int intspvm_error_mode(char *fname);
extern int intspvm_joingroup(char *fname);
extern int intspvm_lvgroup(char *fname);
extern int intspvm_gsize(char *fname);
extern int intspvm_gettid(char *fname);
extern int intspvm_getinst(char *fname);
extern int intspvm_barrier(char *fname);
extern int intspvm_bcast(char *fname);
extern int intspvm_tasks(char *fname);
extern int intspvm_config(char *fname);
extern int intspvm_addhosts(char *fname);
extern int intspvm_delhosts(char *fname);
extern int intspvm_parent(char *fname);
extern int intspvm_tidtohost(char *fname);
extern int intspvm_set_timer(char *fname);
extern int intspvm_get_timer(char *fname);
extern int intspvm_mytid(char *fname);
extern int intspvm_exit(char *fname);
extern int intspvm_kill(char *fname);
extern int intspvm_spawn(char *fname);
extern int intspvm_spawn_independent(char *fname);
extern int intspvm_recv(char *fname);
extern int intspvm_send(char *fname);
extern int intspvm_recv_var(char *fname);
extern int intspvm_send_var(char *fname);
extern int intspvm_reduce(char *fname);
extern int intspvm_start(char *fname);
extern int intspvm_halt(char *fname);
extern int intspvm_error(char *fname);
extern int intspvm_sci2f77(char *fname);
extern int intspvm_f772sci(char *fname);
extern int intspvm_probe(char *fname);
extern int intspvm_bufinfo(char *fname);
/*-----------------------------------------------------------------------------------*/
  static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,intspvm_joingroup,"pvm_joingroup"},
  {(Myinterfun)sci_gateway,intspvm_lvgroup,"pvm_lvgroup"},
  {(Myinterfun)sci_gateway,intspvm_gsize,"pvm_gsize"},
  {(Myinterfun)sci_gateway,intspvm_gettid,"pvm_gettid"},
  {(Myinterfun)sci_gateway,intspvm_getinst,"pvm_getinst"},
  {(Myinterfun)sci_gateway,intspvm_barrier,"pvm_barrier"},
  {(Myinterfun)sci_gateway,intspvm_bcast,"pvm_bcast"},
  {(Myinterfun)sci_gateway,intspvm_tasks,"pvm_tasks"},
  {(Myinterfun)sci_gateway,intspvm_config,"pvm_config"},
  {(Myinterfun)sci_gateway,intspvm_addhosts,"pvm_addhosts"},
  {(Myinterfun)sci_gateway,intspvm_delhosts,"pvm_delhosts"},
  {(Myinterfun)sci_gateway,intspvm_parent,"pvm_parent"},
  {(Myinterfun)sci_gateway,intspvm_tidtohost,"pvm_tidtohost"},
  {(Myinterfun)sci_gateway,intspvm_set_timer,"pvm_set_timer"},
  {(Myinterfun)sci_gateway,intspvm_get_timer,"pvm_get_timer"},
  {(Myinterfun)sci_gateway,intspvm_mytid,"pvm_mytid"},
  {(Myinterfun)sci_gateway,intspvm_exit,"pvm_exit"},
  {(Myinterfun)sci_gateway,intspvm_kill,"pvm_kill"},
  {(Myinterfun)sci_gateway,intspvm_spawn,"pvm_spawn"},
  {(Myinterfun)sci_gateway,intspvm_spawn_independent,"pvm_spawn_independent"},
  {(Myinterfun)sci_gateway,intspvm_recv,"pvm_recv"},
  {(Myinterfun)sci_gateway,intspvm_send,"pvm_send"},
  {(Myinterfun)sci_gateway,intspvm_recv_var,"pvm_recv_var"},
  {(Myinterfun)sci_gateway,intspvm_send_var,"pvm_send_var"},
  {(Myinterfun)sci_gateway,intspvm_reduce,"pvm_reduce"},
  {(Myinterfun)sci_gateway,intspvm_start,"pvm_start"},
  {(Myinterfun)sci_gateway,intspvm_halt,"pvm_halt"},
  {(Myinterfun)sci_gateway,intspvm_error,"pvm_error"},
  {(Myinterfun)sci_gateway,intspvm_sci2f77,"pvm_sci2f77"},
  {(Myinterfun)sci_gateway,intspvm_f772sci,"pvm_f772sci"},
  {(Myinterfun)sci_gateway,intspvm_probe,"pvm_probe"},
  {(Myinterfun)sci_gateway,intspvm_bufinfo,"pvm_bufinfo"},
  {(Myinterfun)sci_gateway,intspvm_error_mode,"pvm_error_mode"},
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_pvm)()
{
	Rhs = Max(0, Rhs);
	#ifdef _MSC_VER
		#ifndef _DEBUG
			_try
			{
				(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
			}
			_except (EXCEPTION_EXECUTE_HANDLER)
			{	
				ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
			}
		#else
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
		#endif
	#else
		(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
	#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
