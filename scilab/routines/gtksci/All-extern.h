#ifndef SCI_WIN_FN 
#define SCI_WIN_FN 

#define True 1
#define False 0


/*** elsewhere **/

extern int C2F (scilab) (int *nostartup);  
extern int C2F (scilines) (int *nl, int *nc);  
extern int C2F (sciquit) (void);  
extern void C2F(setfbutn)  (char *name,int *rep);
extern void  controlC_handler(int n);

extern void  reset_scig_handler (void);
extern void  reset_scig_click_handler (void);
extern void  reset_scig_deletegwin_handler (void);
extern void  reset_scig_command_handler (void);

/* 
typedef int (*Scig_click_handler) (int,int,int,int,int,int);
extern Scig_click_handler set_scig_click_handler (Scig_click_handler f);
typedef void (*Scig_deletegwin_handler) (int);
extern Scig_deletegwin_handler set_scig_deletegwin_handler (Scig_deletegwin_handler f);
*/

extern int PushClickQueue (int,int ,int y,int ibut,int m,int r);
extern int CheckClickQueue(int *win,int *x,int *y,int *ibut,int *motion,int *release);		       
extern int ClearClickQueue  (integer);  

extern int C2F (deletewin) (integer *number);  
extern int C2F (delbtn) (integer *win_num, char *button_name);  
extern void AddMenu  (integer *win_num, char *button_name, char **entries, integer *ne, integer *typ, char *fname, integer *ierr);  
extern int C2F (addmen) (integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *typ, char *fname, integer *ierr);  
extern int C2F (setmen) (integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *ierr);  
extern int C2F (unsmen) (integer *win_num, char *button_name, integer *entries, integer *ptrentries, integer *ne, integer *ierr);  

typedef int (*Scig_command_handler) (char *);
extern Scig_command_handler set_scig_command_handler (Scig_command_handler f);
extern void SetXsciOn  (void);  
extern int C2F (xscion) (int *i);  
extern int Xorgetchar  (int interrupt);  
extern int C2F (sxevents) (void);  
extern int StoreCommand  (char *command);  
extern int GetCommand  (char *str);  
extern integer C2F (ismenu) (void);  
extern int C2F (getmen) (char *btn_cmd, integer *lb, integer *entry);  
extern void MenuFixCurrentWin  (int ivalue);  
extern void write_scilab  (char *s);  
extern void menu_command  ();  
extern int C2F(scirun)(char * startup, int lstartup);
extern int C2F(inisci)(int *,int *,int *);

/* zzledt.c */ 

extern void C2F(zzledt)(char *buffer,int * buf_size,int * len_line,
			int * eof,int * interrupt, int * modex, long int  dummy1); 

void sci_get_screen_size (int *rows,int *cols); 
void C2F(setprlev)(int *pause) ; 
extern int using_readline(void);
extern int get_one_char(char *prompt);

/* xxx */

extern void sciprint(char *fmt, ...);
extern void sciprint_nd(char *fmt, ...);
extern int sciprint2(int v,char *fmt,...);
extern void Scistring(char *str);
extern int  Scierror(int iv,char *fmt,...);
extern int C2F(clearexit)(integer *n);
void  C2F(setfbutn)(char *buf,int *rep);
void  C2F(fbutn)(char *buf,int *win,int *ent);

/* x_main.c */ 

extern void sci_winch_signal(int);
extern void C2F(tmpdirc)();
extern void sci_clear_and_exit (int);
extern void sci_usr1_signal(int);
extern void start_sci_gtk(void);

/* ../os_specific/inffic.c */ 
extern char *get_sci_data_strings(int n);

/* menu.c */ 

#endif 
