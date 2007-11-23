/*--------------------------------------------------------------------------*/
/* Sylvestre LEDRU */
/* Allan CORNET */
/* INRIA 2006 */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "SetScilabEnvironment.h"
#include "SCIHOME.h"
#include "inisci-c.h" /* SetSci */
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
	extern void SciEnvForWindows(void);
#endif
/*--------------------------------------------------------------------------*/
/**
* Define SCI and some others Environments variables 
*/
void SetScilabEnvironment(void) 
{
#ifdef _MSC_VER
	SciEnvForWindows(); 
#else
	SetSci();
#endif
	setSCIHOME();
}
/*--------------------------------------------------------------------------*/
