/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
#else
#include "men_scilab.h"
#endif

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

SciMess ScilabMessage ={NULL,NULL,-1,0};


/*************************************************     
 * Scilab Interface 
 **********************************************************/

void C2F(xmsg)(int *basstrings, int *ptrstrings, int *nstring, int *btn, int *ptrbtn, int *nb, int *nrep, int *ierr)
{
  int i,rep;;
  if (ScilabMessage.nb >= 0) 
    {
      sciprint("Only one message window at a time please \r\n");
      return ;
    }
  *ierr=0;
  ScilabMStr2C(basstrings,nstring,ptrstrings,&(ScilabMessage.string),ierr);
  if ( *ierr == 1) return;
  ScilabMStr2CM(btn,nb,ptrbtn,&(ScilabMessage.pButName),ierr);
  if ( *ierr == 1) return;
  ScilabMessage.nb = *nb;
  rep = ExposeMessageWindow();
  FREE(ScilabMessage.string);
  for (i=0 ; i < *nb ; i++ )FREE(ScilabMessage.pButName[i]);
  FREE(ScilabMessage.pButName);
  *nrep= rep;
  ScilabMessage.nb = -1;
}
/*************************************************     
 * Scilab Interface  for modeless message
 **********************************************************/

void C2F(xmsg1)(int *basstrings, int *ptrstrings, int *nstring, int *btn, int *ptrbtn, int *nb, int *ierr)
{
  int i;
  if (ScilabMessage.nb >= 0) 
    {
      sciprint("Only one message window at a time please \r\n");
      return ;
    }
  *ierr=0;
  ScilabMStr2C(basstrings,nstring,ptrstrings,&(ScilabMessage.string),ierr);
  if ( *ierr == 1) return;
  ScilabMStr2CM(btn,nb,ptrbtn,&(ScilabMessage.pButName),ierr);
  if ( *ierr == 1) return;
  ScilabMessage.nb = *nb;
  ExposeMessageWindow1();
  FREE(ScilabMessage.string);
  for (i=0 ; i < *nb ; i++ )FREE(ScilabMessage.pButName[i]);
  FREE(ScilabMessage.pButName);
  ScilabMessage.nb = -1;
}

