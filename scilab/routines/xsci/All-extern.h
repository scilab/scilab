#ifdef __STDC__
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		()
#endif
#endif

/*** elsewhere **/

extern int C2F (scilab) __PARAMS((int *nostartup));  
extern int C2F (scilines) __PARAMS((int *nl, int *nc));  
extern int C2F (sciquit) __PARAMS((void));  
extern void C2F(setfbutn)  __PARAMS((char *name,int *rep));

/*  "h_help-n.c.X1"*/
extern void initHelpActions  __PARAMS((XtAppContext appContext));  
extern void popupHelpPanel __PARAMS((void));  
extern void changeHelpList  __PARAMS((int i));  
extern void setHelpShellState  __PARAMS((int state));  
extern char *getWidgetString  __PARAMS((Widget widget));  

/*  "jpc_SGraph-n.c.X1"*/

extern void   reset_scig_handler __PARAMS((void));
extern void  reset_scig_click_handler __PARAMS((void));
extern void  reset_scig_deletegwin_handler __PARAMS((void));
extern void  reset_scig_command_handler __PARAMS((void));

typedef int (*Scig_click_handler) __PARAMS((int,int,int,int,int,int));
extern Scig_click_handler set_scig_click_handler __PARAMS((Scig_click_handler f));

typedef void (*Scig_deletegwin_handler) __PARAMS((int));
extern Scig_deletegwin_handler set_scig_deletegwin_handler __PARAMS((Scig_deletegwin_handler f));


extern int PushClickQueue __PARAMS((int,int ,int y,int ibut,int m,int r));
extern int CheckClickQueue  __PARAMS((integer *,integer *x, integer *y, integer *ibut));  
extern int ClearClickQueue  __PARAMS((integer));  

extern int C2F (deletewin) __PARAMS((integer *number));  
extern void SGDeleteWindow  __PARAMS((Widget w, XEvent *event, String *params, Cardinal *num_params));  

extern void ChangeBandF  __PARAMS((int win_num, Pixel fg, Pixel bg));  
extern int C2F (delbtn) __PARAMS((integer *win_num, char *button_name));  
extern void AddMenu  __PARAMS((integer *win_num, char *button_name, char **entries, integer *ne, integer *typ, char *fname, integer *ierr));  
extern int C2F (addmen) __PARAMS((integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *typ, char *fname, integer *ierr));  
extern int C2F (setmen) __PARAMS((integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *ierr));  
extern int C2F (unsmen) __PARAMS((integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *ierr));  

/*  "jpc_Xloop-n.c.X1"*/


typedef int (*Scig_command_handler) __PARAMS((char *));
extern Scig_command_handler set_scig_command_handler __PARAMS((Scig_command_handler f));
extern void SetXsciOn  __PARAMS((void));  
extern int C2F (xscion) __PARAMS((int *i));  
extern void DisplayInit  __PARAMS((char *string, Display **dpy, Widget *toplevel));  
extern int Xorgetchar  __PARAMS((void));  
extern int C2F (sxevents) __PARAMS((void));  
extern int StoreCommand  __PARAMS((char *command));  
extern void GetCommand  __PARAMS((char *str));  
extern integer C2F (ismenu) __PARAMS((void));  
extern int C2F (getmen) __PARAMS((char *btn_cmd, integer *lb, integer *entry));  
/*  "jpc_coloredit-n.c.X1"*/
extern void popup_choice_panel  __PARAMS((Widget tool));  
extern void create_color_panel  __PARAMS((Widget form, Widget cancel));  
extern void cancel_color_popup  __PARAMS((Widget w, XtPointer dum1, XtPointer dum2));  
/*  "jpc_command-n.c.X1"*/
extern void FileG1  __PARAMS((Widget w, XtPointer closure, caddr_t call_data));  
extern void getMenuBut0  __PARAMS((Widget *w));  
extern void MenuFixCurrentWin  __PARAMS((int ivalue));  
extern void CreateCommandPanel  __PARAMS((Widget parent));  
/*  "jpc_inter-n.c.X1"*/
extern void write_scilab  __PARAMS((char *s));  
/*  "jpc_utils-n.c.X1"*/
extern void DisableWindowResize  __PARAMS((Widget w));  
extern void bell  __PARAMS((int volume));  
extern char *concat  __PARAMS((char *s1, char *s2));  

/*  "jpc_windows-n.c.X1"*/

extern void DefaultMessageWindow  __PARAMS((void));  
/*   extern XtermWidget CreateSubWindows  __PARAMS((Widget parent));   */
extern void set_scilab_icon  __PARAMS((void));  
extern XtEventHandler UseMessage  __PARAMS((Widget w, Widget child, XClientMessageEvent *e));  
extern void AddAcceptMessage  __PARAMS((Widget parent));  
extern void ReAcceptMessage  __PARAMS((void));  
/*  extern XtermWidget CreateTermWindow  __PARAMS((Widget parent));  */
extern void UpdateFileLabel  __PARAMS((char *string));  
extern void UpdateLineLabel  __PARAMS((Cardinal line));  
extern void UpdateMessageWindow  __PARAMS((char *format, char *arg));  

/*  "jpc_xwidgets-n.c.X1"*/
extern void AddNewMenu  __PARAMS((Widget parent, Widget drawbox));  
/*  "wf_e_edit-n.c.X1"*/
extern int panel_set_value  __PARAMS((Widget widg, char *val));  
extern char *panel_get_value  __PARAMS((Widget widg));  
extern void clear_text_key  __PARAMS((Widget w));  
extern void paste_panel_key  __PARAMS((Widget w, XKeyEvent *event));  

/*  "wf_f_read-n.c.X1"*/

extern void file_msg  __PARAMS((char *format, char *arg1));  
extern void clear_file_message  __PARAMS((Widget w, XButtonEvent *ev));  
extern void popup_file_msg  __PARAMS((void));  

/*  "wf_f_util-n.c.X1"*/
extern int emptyname  __PARAMS((char *name));  
extern int emptyname_msg  __PARAMS((char *name, char *msg));  
extern int change_directory  __PARAMS((char *path));  
extern int get_directory  __PARAMS((void));  

/*  "wf_w_cursor-n.c.X1"*/

extern void init_wf_cursor  __PARAMS((void));  
extern void reset_wf_cursor  __PARAMS((void));  
extern void set_temp_wf_cursor  __PARAMS((Cursor cursor));  
extern void set_wf_cursor  __PARAMS((Cursor cursor));  

/*  "wf_w_dir-n.c.X1"*/

extern  void parseuserpath __PARAMS((char *path,char *longpath));

/*  "wf_w_file-n.c.X1"*/

extern int exec_file  __PARAMS((char *dir, char *file));  
extern void do_exec  __PARAMS((Widget w, XButtonEvent *ev));  
extern void ok_file  __PARAMS((char *dir, char *file));  
extern void ok_end  __PARAMS((void));  
extern void ok_prep  __PARAMS((char *filemask, char *dirname, char *title,int flag, int *err));  
extern void parsescipath  __PARAMS((char *path, char *longpath));  
extern void do_ok  __PARAMS((Widget w, XButtonEvent *ev));  
extern int getfc_file  __PARAMS((char *dir, char *file));  
extern void do_getfc  __PARAMS((Widget w));  
extern int linkf_file  __PARAMS((char *dir, char *file));  
extern void do_linkf  __PARAMS((Widget w));  
extern int getf_file  __PARAMS((char *dir, char *file));  
extern void do_getf  __PARAMS((Widget w));  
extern int load_file  __PARAMS((char *dir, char *file));  
extern void do_load  __PARAMS((Widget w));  
extern void popup_file_panel  __PARAMS((Widget w,char *));  
extern void create_file_panel  __PARAMS((Widget w,char *));  

/*  "wf_w_init-n.c.X1"*/

extern void w_init  __PARAMS((Widget w));  

/*  "wf_w_msgpanel-n.c.X1"*/

extern void init_msg  __PARAMS((Widget tool,Widget vert_w, int ch, char *filename));  
extern void update_cur_filename  __PARAMS((char *newname));  
extern void put_msg  __PARAMS((char *,...)); 
extern void clear_message  __PARAMS((void));  
extern void FOpAddInfoHandler  __PARAMS((Widget widget, char *message));  

/*  "wf_w_setup-n.c.X1"*/

extern void setup_sizes  __PARAMS((int new_canv_wd, int new_canv_ht));  

/*  "wf_w_util-n.c.X1"*/

extern void  app_flush  __PARAMS((void));  

