
#ifndef _EVENTS_H_
#define _EVENTS_H_

#define SCI_EVENT_HANDLER_NAME_LENGTH 25

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
extern Scig_deletegwin_handler get_scig_deletegwin_handler ( void );
extern void reset_scig_deletegwin_handler (void);

extern void set_delete_win_mode(void);
extern void set_no_delete_win_mode(void);
extern int  get_delete_win_mode(void);

extern void set_wait_click(int val);
extern int  get_wait_click(void);



/*Scig_deletegwin_handler scig_deletegwin_handler;*/

extern void set_event_select(int val);
extern int get_event_select(void);

#endif /* _EVENTS_H_  */
