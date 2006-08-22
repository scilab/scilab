#include "GetExceptionCode.h"
/* Allan CORNET */
/* INRIA 2005 */
#include <string.h>
/*-----------------------------------------------------------------------------------*/
char *GetExceptionString(DWORD ExceptionCode)
{
	char *StringOuput=(char *)MALLOC((strlen("EXCEPTION_NONCONTINUABLE_EXCEPTION")+1)*sizeof(char));
		
	switch (ExceptionCode) 
	{
  		case EXCEPTION_ACCESS_VIOLATION:
  			wsprintf(StringOuput,"%s","EXCEPTION_ACCESS_VIOLATION");
  		break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			wsprintf(StringOuput,"%s","EXCEPTION_DATATYPE_MISALIGNMENT");
  		break;			
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			wsprintf(StringOuput,"%s","EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
  		break;			
		case EXCEPTION_IN_PAGE_ERROR:
			wsprintf(StringOuput,"%s","EXCEPTION_IN_PAGE_ERROR");
  		break;			
		case EXCEPTION_GUARD_PAGE:
			wsprintf(StringOuput,"%s","EXCEPTION_GUARD_PAGE");
			break;
		case EXCEPTION_STACK_OVERFLOW:
			wsprintf(StringOuput,"%s","EXCEPTION_STACK_OVERFLOW");
  		break;			
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			wsprintf(StringOuput,"%s","EXCEPTION_ILLEGAL_INSTRUCTION");
  		break;			
		case EXCEPTION_PRIV_INSTRUCTION:
			wsprintf(StringOuput,"%s","EXCEPTION_PRIV_INSTRUCTION");
  		break;			
		case EXCEPTION_INVALID_DISPOSITION:
			wsprintf(StringOuput,"%s","EXCEPTION_INVALID_DISPOSITION");
  		break;			
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			wsprintf(StringOuput,"%s","EXCEPTION_NONCONTINUABLE_EXCEPTION");
  		break;			
		case EXCEPTION_BREAKPOINT:
			wsprintf(StringOuput,"%s","EXCEPTION_BREAKPOINT");
  		break;			
		case EXCEPTION_SINGLE_STEP:
			wsprintf(StringOuput,"%s","EXCEPTION_SINGLE_STEP");
  		break;			
		case EXCEPTION_INVALID_HANDLE:
			wsprintf(StringOuput,"%s","EXCEPTION_INVALID_HANDLE");
  		break;			
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			wsprintf(StringOuput,"%s","EXCEPTION_INT_DIVIDE_BY_ZERO");
  		break;			
		case EXCEPTION_INT_OVERFLOW:
			wsprintf(StringOuput,"%s","EXCEPTION_INT_OVERFLOW");
  		break;			
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			wsprintf(StringOuput,"%s","EXCEPTION_FLT_DENORMAL_OPERAND");
  		break;			
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			wsprintf(StringOuput,"%s","EXCEPTION_FLT_DIVIDE_BY_ZERO");
  		break;			
		case EXCEPTION_FLT_INEXACT_RESULT:
			wsprintf(StringOuput,"%s","EXCEPTION_FLT_INEXACT_RESULT");
  		break;			
		case EXCEPTION_FLT_INVALID_OPERATION:
			wsprintf(StringOuput,"%s","EXCEPTION_FLT_INVALID_OPERATION");
  		break;			
		case EXCEPTION_FLT_OVERFLOW:
			wsprintf(StringOuput,"%s","EXCEPTION_FLT_OVERFLOW");
  		break;			
		case EXCEPTION_FLT_STACK_CHECK:
			wsprintf(StringOuput,"%s","EXCEPTION_FLT_STACK_CHECK");
  		break;			
		case EXCEPTION_FLT_UNDERFLOW:
			wsprintf(StringOuput,"%s","EXCEPTION_FLT_UNDERFLOW");
  		break;
  		default:
  		wsprintf(StringOuput,"%s","unknown exception");
  		break;				
  }

	return StringOuput;
}
/*-----------------------------------------------------------------------------------*/
