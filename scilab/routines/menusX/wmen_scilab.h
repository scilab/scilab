/* Copyright ENPC */

#ifndef STRICT 
#define STRICT
#endif
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* #include "../wsci/wgnuplib.h"*/
#include "../wsci/wresource.h"
#include "../wsci/wcommon.h"
#include "../sun/men_Sutils.h"

#include "../stack-def.h" /* for IMPORT */
#include <prsht.h>      /* includes the property sheet functionality*/

extern TW textwin;

/*********************************
 * structure for choose 
 *********************************/

typedef struct {
  char *description;    /** Title **/
  char **strings;       /** items **/
  int nstrings;         /** number of items **/
  char **buttonname;    /** buttons **/
  int nb;               /** number of buttons **/
  int choice;           /** number of selected item **/
  int status;           /** status for return **/
}  ChooseMenu ;

/*********************************
 * structure for dialog 
 *********************************/

#define MAXSTR 512
IMPORT char *dialog_str;

typedef struct {
  char *description;      /** Title **/
  char *init;              /** initial value **/
  char **pButName;         /** buttons **/
  int  nb;                /** number of buttons **/
  int  ierr;
}  SciDialog ;


/* Data structure to deal with a set of choices */

typedef struct {
  char *name;
  char *cbinfo ; 
} SciData;

typedef struct {
  struct {
    char *name; /* name of item */
    char *text; /* idem ? */
    int   num_toggles; /* number of choices for this item */
    int  default_toggle; /* and is set dynamically to the selected value */
  } choice;
  SciData *data;
} SciStuff;

extern int SciChoiceCreate();
extern int SciChoiceFree();
extern SciStuff **Everything ;

/* Data structure to deal with message */

typedef struct {
  char *string;            /** texte  **/
  char **pButName;         /** buttons **/
  int  nb;                /** number of buttons **/
  int  ierr;
}  SciMess ;


/* Data structure to deal with mdialog */

/* WARNING: it's not enough to change the following
 * define in order to increase the number of possible items 
 */

#define NPAGESMAX 10
#define NITEMMAXPAGE 3 

typedef struct {
  int  NItPg[NPAGESMAX];  /* Number of Items in page i; */
  int  NPages;            /* Number of Pages  */
  int  CPage;             /* The number of current page **/
  char *labels;           /** Title **/
  char **pszTitle;        /** items **/
  char **pszName;         /** buttons **/
  int  nv;                /** number of items **/
  int  ierr;
}  MDialog ;


/** Data structure for MatDialog */

typedef struct {
  char *labels;           /** Title **/
  char **VDesc;           /* Vertical labels */
  char **HDesc;           /* Horizontal lables */
  char **data ;           /* values */
  int  nv;                /** number of items **/
  int  nl,nc;
  int  ierr;
}  MADialog ;


/** Data structure for printDialog **/

typedef struct {
  int numChoice ;
  char *filename ;
  char **PList ;
  char *Pbuffer ;
  int ns;
} PrintDial;


/* "men_choice-n.c.X1" */

extern void C2F(xchoices)(int *,int *,int *,int *,int *,int *,int *,int *,int *);  
extern int TestChoice (void);  
extern int SciChoice (char *, char **, int *, int );  

/* "men_choose-n.c.X1" */

extern int TestChoose (void);  
extern void C2F(xchoose)(int *, int *ptrdesc, int *nd, int *basstrings, int *ptrstrings, int *nstring, int *btn, int *ptrbtn, int *nb, int *nrep, int *ierr);  

/* "men_dialog-n.c.X1" */

extern int TestDialog (void);  
extern void C2F(xdialg)(int *value, int *ptrv, int *nv, int *, int *ptrdesc, int *nd, int *btn, int *ptrbtn, int *nb, int *res, int *ptrres, int *nr, int *ierr);  
extern void xdialg1 (char *, char *valueinit, char **pButName, char *value, int *ok);  

/* "men_getfile-n.c.X1" */

extern int TestGetFile (void);  

extern void C2F(xgetfile) (char *filemask, char *dirname, char **res, integer *ires, integer *ierr, integer *idir,integer *desc,integer *ptrdesc,integer *nd);


/* "men_madial-n.c.X1" */

extern void C2F(xmatdg)(int *, int *ptrlab, int *nlab, int *value, int *ptrv, int *v, int *ptrdescv, int *h, int *ptrdesch, int *nl, int *nc, int *res, int *ptrres, int *ierr);  
extern int TestMatrixDialogWindow (void);  

/* "men_mdial-n.c.X1" */

extern int TestmDialogWindow (void);  
extern void C2F(xmdial)(int *, int *ptrlab, int *nlab, int *value, int *ptrv, int *, int *ptrdesc, int *nv, int *res, int *ptrres, int *ierr);  

/* "men_message-n.c.X1" */

extern int TestMessage (int n);  
extern void C2F(xmsg)(int *basstrings, int *ptrstrings, int *nstring, int *btn, int *ptrbtn, int *nb, int *nrep, int *ierr);  

/* "men_print-n.c.X1" */

extern int prtdlg (integer *flag, char *printer, integer *colored, integer *orientation, char *file, integer *ok);  
extern int TestPrintDlg (void);  
extern int SetPrinterList (int);  

/*  "wmen_choice-n.c.X1" */

int SciChoiceI __PARAMS((char *label,int defval[],int nitems));
int SciChoiceCreate __PARAMS((char **items,int defval[],int nitems));
int AllocAndCopy __PARAMS((char **strh1,char *str2));
int SciChoiceFree __PARAMS((int nitems));
int mChoiceWindow __PARAMS((void));

/*  "wmen_choose-n.c.X1" */

EXPORT BOOL CALLBACK TestDlgProc  __PARAMS((HWND , UINT , WPARAM , LPARAM ));
   
extern int ExposeChooseWindow __PARAMS((ChooseMenu *PCh));

/*  "wmen_dialog-n.c.X1" */
EXPORT int CALLBACK  SciDialogDlgProc  __PARAMS((HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam));  
extern BOOL DialogWindow  __PARAMS((void));  

/*  "wmen_getfile-n.c.X1" */

extern int GetFileWindow  __PARAMS((char *filemask, char **szFile, char *dirname, int flag, int *ierr,char *title));  

/*  "wmen_madial-n.c.X1" */

extern int MatrixDialogWindow  __PARAMS((void));  

/*  "wmen_mdial-n.c.X1" */

int mDialogWindow __PARAMS((void)); 

/*  "wmen_message-n.c.X1" */

EXPORT BOOL CALLBACK SciMessageDlgProc  __PARAMS((HWND hdlg, UINT wmsg, WPARAM wparam, LPARAM lparam));
  
extern int ExposeMessageWindow  __PARAMS((void));  
extern int ExposeMessageWindow1  __PARAMS((void));  
/*  "wmen_print-n.c.X1" */

extern int ExposePrintdialogWindow  __PARAMS((int flag, int *colored, int *orientation));  

/*  Global variable to keep track of the last position of a popup menu */

extern RECT SciMenusRect ;
