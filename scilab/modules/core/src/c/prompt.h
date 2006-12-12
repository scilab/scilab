/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Ledru Sylvestre */
/*-----------------------------------------------------------------------------------*/ 

/* Define prompts of Scilab */

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



void C2F(setprlev)( int *pause);
void GetCurrentPrompt(char *CurrentPrompt);
