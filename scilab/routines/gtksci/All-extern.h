#ifdef __STDC__
#ifndef  _PARAMS
#define  _PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  _PARAMS
#define  _PARAMS(paramlist)		()
#endif
#endif

/*** elsewhere **/

extern int C2F (scilab) _PARAMS((int *nostartup));  
extern int C2F (scilines) _PARAMS((int *nl, int *nc));  
extern int C2F (sciquit) _PARAMS((void));  
extern void C2F(setfbutn)  _PARAMS((char *name,int *rep));

/*  "h_help-n.c.X1"*/

extern void popupHelpPanel _PARAMS((void));  
extern void changeHelpList  _PARAMS((int i));  
extern void setHelpShellState  _PARAMS((int state));  

/*  "jpc_SGraph-n.c.X1"*/

extern void  reset_scig_handler _PARAMS((void));
extern void  reset_scig_click_handler _PARAMS((void));
extern void  reset_scig_deletegwin_handler _PARAMS((void));
extern void  reset_scig_command_handler _PARAMS((void));

typedef int (*Scig_click_handler) _PARAMS((int,int,int,int,int,int));
extern Scig_click_handler set_scig_click_handler _PARAMS((Scig_click_handler f));

typedef void (*Scig_deletegwin_handler) _PARAMS((int));
extern Scig_deletegwin_handler set_scig_deletegwin_handler _PARAMS((Scig_deletegwin_handler f));


extern int PushClickQueue _PARAMS((int,int ,int y,int ibut,int m,int r));
extern int CheckClickQueue  _PARAMS((integer *,integer *x, integer *y, integer *ibut));  
extern int ClearClickQueue  _PARAMS((integer));  

extern int C2F (deletewin) _PARAMS((integer *number));  
extern int C2F (delbtn) _PARAMS((integer *win_num, char *button_name));  
extern void AddMenu  _PARAMS((integer *win_num, char *button_name, char **entries, integer *ne, integer *typ, char *fname, integer *ierr));  
extern int C2F (addmen) _PARAMS((integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *typ, char *fname, integer *ierr));  
extern int C2F (setmen) _PARAMS((integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *ierr));  
extern int C2F (unsmen) _PARAMS((integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *ierr));  

/*  "jpc_Xloop-n.c.X1"*/


typedef int (*Scig_command_handler) _PARAMS((char *));
extern Scig_command_handler set_scig_command_handler _PARAMS((Scig_command_handler f));
extern void SetXsciOn  _PARAMS((void));  
extern int C2F (xscion) _PARAMS((int *i));  
extern int Xorgetchar  _PARAMS((void));  
extern int C2F (sxevents) _PARAMS((void));  
extern int StoreCommand  _PARAMS((char *command));  
extern void GetCommand  _PARAMS((char *str));  
extern integer C2F (ismenu) _PARAMS((void));  
extern int C2F (getmen) _PARAMS((char *btn_cmd, integer *lb, integer *entry));  
/*  "jpc_coloredit-n.c.X1"*/

extern void MenuFixCurrentWin  _PARAMS((int ivalue));  
/*  "jpc_inter-n.c.X1"*/
extern void write_scilab  _PARAMS((char *s));  
/*  "jpc_utils-n.c.X1"*/
extern void bell  _PARAMS((int volume));  
extern char *concat  _PARAMS((char *s1, char *s2));  

/*  "jpc_windows-n.c.X1"*/

extern void DefaultMessageWindow  _PARAMS((void));  
/*   extern XtermWidget CreateSubWindows  _PARAMS((Widget parent));   */
extern void set_scilab_icon  _PARAMS((void));  
extern void ReAcceptMessage  _PARAMS((void));  
extern void UpdateFileLabel  _PARAMS((char *string));  
extern void UpdateMessageWindow  _PARAMS((char *format, char *arg));  

/*  "wf_f_read-n.c.X1"*/

extern void file_msg  _PARAMS((char *format, char *arg1));  
extern void popup_file_msg  _PARAMS((void));  

/*  "wf_f_util-n.c.X1"*/
extern int emptyname  _PARAMS((char *name));  
extern int emptyname_msg  _PARAMS((char *name, char *msg));  
extern int change_directory  _PARAMS((char *path));  
extern int get_directory  _PARAMS((void));  

/*  "wf_w_cursor-n.c.X1"*/

extern void init_wf_cursor  _PARAMS((void));  
extern void reset_wf_cursor  _PARAMS((void));  

/*  "wf_w_dir-n.c.X1"*/

extern  void parseuserpath _PARAMS((char *path,char *longpath));

/*  "wf_w_file-n.c.X1"*/

extern int exec_file  _PARAMS((char *dir, char *file));  
extern void ok_file  _PARAMS((char *dir, char *file));  
extern void ok_end  _PARAMS((void));  
extern void ok_prep  _PARAMS((char *filemask, char *dirname, char *title,int flag, int *err));  
extern void parsescipath  _PARAMS((char *path, char *longpath));  
extern int getfc_file  _PARAMS((char *dir, char *file));  
extern int linkf_file  _PARAMS((char *dir, char *file));  
extern int getf_file  _PARAMS((char *dir, char *file));  
extern int load_file  _PARAMS((char *dir, char *file));  

/*  "wf_w_init-n.c.X1"*/
/*  "wf_w_msgpanel-n.c.X1"*/

extern void update_cur_filename  _PARAMS((char *newname));  
extern void put_msg  _PARAMS((char *,...)); 
extern void clear_message  _PARAMS((void));  

/*  "wf_w_setup-n.c.X1"*/

extern void setup_sizes  _PARAMS((int new_canv_wd, int new_canv_ht));  

/*  "wf_w_util-n.c.X1"*/

extern void  app_flush  _PARAMS((void));  

