/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
#else
#include "men_scilab.h"
#endif

SciMess ScilabMessage;

/*************************************************     
 * Test function 
 **********************************************************/

int TestMessage(n)
     int n;
{
  int rep;
  static char* description = "Message \ntest";
  static char* buttonname[] = {
        "Label1",
	"Label2",
	NULL
    };
  ScilabMessage.nb= n;
  ScilabMessage.pButName = buttonname;
  ScilabMessage.string = description;
  rep= ExposeMessageWindow();
  sciprint("reponse[%d] \n",rep);
  return(rep);
}

/*************************************************     
 * Scilab Interface 
 **********************************************************/

void C2F(xmsg)(basstrings,ptrstrings,nstring,btn,ptrbtn,nb,nrep,ierr)
     int *basstrings,*nstring,*ptrstrings,*btn,*nb,*ptrbtn,*nrep,*ierr;
{
  int i,rep;;
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
}
/*************************************************     
 * Scilab Interface  for modeless message
 **********************************************************/

void C2F(xmsg1)(basstrings,ptrstrings,nstring,btn,ptrbtn,nb,ierr)
     int *basstrings,*nstring,*ptrstrings,*btn,*nb,*ptrbtn,*ierr;
{
  int i,rep;;
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
}

