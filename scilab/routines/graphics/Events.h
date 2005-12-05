
typedef int (*Scig_click_handler) (int,int,int,int,int,int);
typedef void (*Scig_deletegwin_handler) (int);

extern int PushClickQueue (int win,int x,int y,int ibut,int motion,int release);
extern int CheckClickQueue(int *win,int *x,int *y,int *ibut,int *motion,int *release);			 
extern int ClearClickQueue (int win);

extern int scig_click_handler_none (int win,int x,int y,int ibut,int motion,int release);
extern int scig_click_handler_sci (int win,int x,int y,int ibut,int motion,int release);
extern Scig_click_handler set_scig_click_handler (Scig_click_handler f);
extern void reset_scig_click_handler (void);

extern void scig_deletegwin_handler_none (int win);
extern void scig_deletegwin_handler_sci (int win);
extern Scig_deletegwin_handler set_scig_deletegwin_handler (Scig_deletegwin_handler f);
extern void reset_scig_deletegwin_handler ();

extern void seteventhandler(int *win_num,char *name,int *ierr);

extern void set_delete_win_mode(void);
extern void set_no_delete_win_mode(void);
extern int  get_delete_win_mode(void);

extern void set_wait_click(int val);
extern int  get_wait_click();

Scig_deletegwin_handler scig_deletegwin_handler;
