#ifndef MEN_SCILAB 
#define MEN_SCILAB 

#define GTK_ENABLE_BROKEN

/* Copyright ENPC */

#include <stdio.h>
#include "../machine.h"
#include "../graphics/Math.h"
#include "../graphics/Graphics.h"
#include "../sun/men_Sutils.h"

#if defined(__MWERKS__)||defined(THINK_C)
#define Widget int
#define TRUE 1
#define FALSE 0
#else
#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/List.h>
#include <X11/cursorfont.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Toggle.h>
#endif

#include <string.h>

/* used only for message and dialog boxes */

/* choose */

typedef struct {
  char *description;    /** Title **/
  char **strings;       /** items **/
  int nstrings;         /** number of items **/
  char **buttonname;    /** buttons **/
  int nb;               /** number of buttons **/
  int choice;           /** number of selected item **/
}  ChooseMenu ;

/* dialog */

extern char *dialog_str;

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
  Widget toggle;
} SciData;

typedef struct {
  struct {
    char *name;
    char *text;
    int   num_toggles;
    int   columns;
    int  (*function)();
    int  default_toggle; /* and is set dynamically to the selected value */
    Widget label;
  } choice;
  SciData *data;
} SciStuff;

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

#define NPAGESMAX 10000
#define NITEMMAXPAGE 3000

typedef struct {
  char *labels;           /** Title **/
  char **pszTitle;        /** items **/
  char **pszName;         /** buttons **/
  int  nv;                /** number of items: when nv # 0 this means that 
			   MDialog is used **/
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

extern void C2F(xchoices) __PARAMS((int *,int *,int *,int *,int *,int *,int *,int *,int *));  
extern int TestChoice  __PARAMS((void));  
extern int SciChoice  __PARAMS((char *, char **, int *, int ));  

/* "men_choose-n.c.X1" */

extern int TestChoose  __PARAMS((void));  
extern void C2F(xchoose) __PARAMS((int *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *));  

/* "men_dialog-n.c.X1" */

extern int TestDialog  __PARAMS((void));  
extern void C2F(xdialg) __PARAMS((int *value, int *ptrv, int *nv, int *, int *ptrdesc, int *nd, int *btn, int *ptrbtn, int *nb, int *res, int *ptrres, int *nr, int *ierr));  
extern void xdialg1  __PARAMS((char *, char *valueinit, char **pButName, char *value, int *ok));  

/* "men_getfile-n.c.X1" */

extern int TestGetFile  __PARAMS((void));  
extern void C2F(xgetfile) __PARAMS((char *filemask, char *dirname, char **res, integer *ires, integer *ierr, integer *idir,integer *desc,integer *ptrdesc,integer *nd));

/* "men_madial-n.c.X1" */

extern void C2F(xmatdg) __PARAMS((int *, int *ptrlab, int *nlab, int *value, int *ptrv, int *v, int *ptrdescv, int *h, int *ptrdesch, int *nl, int *nc, int *res, int *ptrres, int *ierr));  
extern int TestMatrixDialogWindow  __PARAMS((void));  

/* "men_mdial-n.c.X1" */

extern int TestmDialogWindow  __PARAMS((void));  
extern void C2F(xmdial) __PARAMS((int *, int *ptrlab, int *nlab, int *value, int *ptrv, int *, int *ptrdesc, int *nv, int *res, int *ptrres, int *ierr));  

/* "men_message-n.c.X1" */

extern int TestMessage  __PARAMS((int n));  
extern void C2F(xmsg) __PARAMS((int *basstrings, int *ptrstrings, int *nstring, int *btn, int *ptrbtn, int *nb, int *nrep, int *ierr));  
extern void C2F(xmsg1) __PARAMS((int *basstrings, int *ptrstrings, int *nstring, int *btn, int *ptrbtn, int *nb, int *ierr));  

/* "men_print-n.c.X1" */

extern int prtdlg  __PARAMS((integer *flag, char *printer, integer *colored, integer *orientation, char *file, integer *ok));  
extern int TestPrintDlg  __PARAMS((void));  
extern int SetPrinterList  __PARAMS((int));  

/* "xmen_Utils-n.c.X1" */

extern void XtMyLoop  __PARAMS((Widget , Display *, int, int *));  
extern void ShellFormCreate  __PARAMS((char *, Widget *, Widget *, Display **));  
extern int ButtonCreate  __PARAMS((Widget, Widget *, XtCallbackProc, XtPointer, char *, char *));  
extern int ViewpLabelCreate  __PARAMS((Widget, Widget *, Widget *, char *));  
extern int ViewpListCreate  __PARAMS((Widget, Widget *, Widget *, char **, int));  
extern int LabelSize  __PARAMS((Widget, int, int , Dimension *, Dimension *));  
extern int AsciiSize  __PARAMS((Widget, int, int , Dimension *, Dimension *));  
extern int SetLabel  __PARAMS((Widget, char *, Dimension , Dimension ));  
extern int SetAscii  __PARAMS((Widget, char *, Dimension , Dimension ));  

/* "xmen_choice-n.c.X1" */

extern int SciChoiceI  __PARAMS((char *, int *, int ));  
extern int SciChoiceCreate  __PARAMS((char **, int *, int ));  
extern int AllocAndCopy  __PARAMS((char **, char *));  
extern int SciChoiceFree  __PARAMS((int ));  
extern Widget create_choices  __PARAMS((Widget, Widget,int));  

/* "xmen_choose-n.c.X1" */

extern int ExposeChooseWindow  __PARAMS((ChooseMenu *));  

/* "xmen_dialog-n.c.X1" */

extern int DialogWindow  __PARAMS((void));  

/* "xmen_getfile-n.c.X1" */

extern int GetFileWindow  __PARAMS((char *, char **, char *, int, int *,char *));  
extern int sci_get_file_window __PARAMS((char *, char **, char *, int,int, int *,char *));  
extern void XtSpecialLoop  __PARAMS((void));  
extern void cancel_getfile  __PARAMS((void));  
extern int write_getfile  __PARAMS((char *, char *));  
extern int popup_file_panel1  __PARAMS((Widget,char* ));  

/* "xmen_madial-n.c.X1" */

extern int MatrixDialogWindow  __PARAMS((void));  

/* "xmen_mdial-n.c.X1" */

extern int mDialogWindow  __PARAMS((void));  

/* "xmen_message-n.c.X1" */

extern int ExposeMessageWindow  __PARAMS((void));  
extern int ExposeMessageWindow1  __PARAMS((void));  

/* "xmen_print-n.c.X1" */

extern void PrintDlgOk  __PARAMS((Widget w, caddr_t , caddr_t ));  
extern void SaveDlgOk  __PARAMS((Widget w, caddr_t , caddr_t ));  
extern void PrintDlgCancel  __PARAMS((Widget w, caddr_t , caddr_t ));  
extern int ExposePrintdialogWindow  __PARAMS((int flag, int *, int *)); 


#endif 
