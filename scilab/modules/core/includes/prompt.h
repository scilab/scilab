/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/ 
#ifndef _PROMPT_H_
#define _PROMPT_H_
/* Define prompts of Scilab */

#include "machine.h" /* C2F */

#ifdef SCIPROMPT
#undef SCIPROMPT
#endif
#define SCIPROMPT "-->"

#ifdef SCIPROMPT_PAUSE
#undef SCIPROMPT_PAUSE
#endif
#define SCIPROMPT_PAUSE ">>"

#ifdef SCIPROMPT_INTERRUPT
#undef SCIPROMPT_INTERRUPT
#endif
#define SCIPROMPT_INTERRUPT "-%d->"

/*-----------------------------------------------------------------------------------*/ 
void C2F(setprlev)( int *pause);
void GetCurrentPrompt(char *CurrentPrompt);
/*-----------------------------------------------------------------------------------*/ 
#endif /* _PROMPT_H_ */
