/*--------------------------------------------------------------
 * Gtk version 
 * main menu and graphic window menus 
 *--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "../graphics/Math.h"
#include "../graphics/periGtk-bcg.h" 
#include "jpc_global.h"
#include "../sun/h_help.h" 
#include "../sun/Sun.h" 
#include "All-extern-x1.h" 
#include "../graphics/Graphics.h" 

#define PI0 (integer *) 0
#define PD0 (double *) 0

extern char GetDriver();

/*--------------------------------------------------------------
 * main menu 
 *--------------------------------------------------------------*/

void create_main_menu() 
{
  menu_entry * main_menu_entries;
  GtkWidget *Plug;
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  GtkWidget *menubar;
  char * plug_info = getenv("SCIWIN");

  accel_group = gtk_accel_group_new ();

  /* This function initializes the item factory.
   *   Param 1: The type of menu - can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU,
   *           or GTK_TYPE_OPTION_MENU.
   *  Param 2: The path of the menu.
   *  Param 3: A pointer to a gtk_accel_group.  The item factory sets up
   *           the accelerator table while generating menus.
   */

  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
				       accel_group);

  if ( plug_info == NULL) return ;
  Plug = gtk_plug_new(atoi(getenv("SCIWIN")));
  
  main_menu_entries =  /* graphic_initial_menu(-1); XXXXXX */ sci_window_initial_menu();
  if ( main_menu_entries == NULL) return;

  /* This function generates the menu items from scilab description */
  /* Attention il faut aussi gerer les menu en set unest XXXXX */

  sci_menu_to_item_factory(item_factory, main_menu_entries);
  
  /* Attach the new accelerator group to the window. */
  /* XXXX gtk_window_add_accel_group (GTK_WINDOW (dd->window), accel_group); */
  
  /* Finally, return the actual menu bar created by the item factory. */ 

  menubar = gtk_item_factory_get_widget (item_factory, "<main>");
  gtk_container_add(GTK_CONTAINER(Plug),menubar);
  gtk_widget_show_all(Plug);

}

/*--------------------------------------------------------------
 * Graphic window menu in main menu 
 *--------------------------------------------------------------*/

static integer lab_count = 0;
static char gwin_name[100];
static Widget GWinButMenu = (Widget) 0;

void MenuFixCurrentWin(ivalue)
     int ivalue;
{
  static int firstentry=0;
  int  i;
  if ( firstentry == 0) 
    {
      strcpy(gwin_name, "Graphic Window 0");
      firstentry++;
    }
  if ( lab_count != ivalue )
    {
      C2F(xscion)(&i);
      if (i==1)
	{
	  lab_count = ivalue;
	  sprintf( gwin_name, "Graphic Window %d", (int) ivalue );
	  /* XXXX A FAIRE */
	}
    }
} 
/* 

static void 
Countp(widget, closure, callData)
     Widget widget;
     XtPointer closure, callData;
{
  Arg arg[1];
  sprintf( gwin_name, "Graphic Window %d ", (int) ++lab_count );
  XtSetArg( arg[0], XtNlabel, gwin_name );
  XtSetValues( (Widget)closure, arg, ONE );
}

static void 
Countm(widget, closure, callData)
     Widget widget;
     XtPointer closure, callData;
{
   Arg arg[1];
   lab_count = (lab_count == 0) ? 0 : lab_count-1;
   sprintf( gwin_name, "Graphic Window %d ", (int) lab_count);
   XtSetArg( arg[0], XtNlabel, gwin_name );
   XtSetValues( (Widget)closure, arg, ONE );
}

static void 
SendCountSet(widget, closure, callData)
     Widget widget;
     XtPointer closure, callData;
{
  char c ;
  if ((c=GetDriver())=='R' || c == 'X' || c == 'W')
    {
      C2F(dr)("xset","window",&lab_count,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    };
}

static void 
SendCountRaise(widget, closure, callData)
     Widget widget;
     XtPointer closure, callData;
{
  char c ;
  if ((c=GetDriver())=='R' || c == 'X' || c == 'W')
    {
      / * C2F(dr)("xsetdr","Rec",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);* /
      C2F(dr)("xset","window",&lab_count,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(dr)("xselect","v",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    };
}

static void 
SendCountDelete(widget, closure, callData)
     Widget widget;
     XtPointer closure, callData;
{
  C2F(deletewin)(&lab_count);
}
*/

/*--------------------------------------------------------------
 * Graphic window menu 
 *--------------------------------------------------------------*/

void create_graphic_window_menu(struct BCG *dd)
{
  GtkAccelGroup *accel_group;
  accel_group = gtk_accel_group_new ();

  /* This function initializes the item factory.
   *   Param 1: The type of menu - can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU,
   *           or GTK_TYPE_OPTION_MENU.
   *  Param 2: The path of the menu.
   *  Param 3: A pointer to a gtk_accel_group.  The item factory sets up
   *           the accelerator table while generating menus.
   */

  dd->item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
				       accel_group);

  /* This function generates the menu items from scilab description */

  /* Attention il faut aussi gerer les menu en set unest XXXXX */
  sci_menu_to_item_factory(dd->item_factory,dd->menu_entries);
  
  /* Attach the new accelerator group to the window. */
  gtk_window_add_accel_group (GTK_WINDOW (dd->window), accel_group);
  
  /* Finally, return the actual menu bar created by the item factory. */ 

  if ( dd->menubar != NULL) 
    {
      /* free old menu bar XXX */
      gtk_widget_destroy(dd->menubar);
      dd->menubar = gtk_item_factory_get_widget (dd->item_factory, "<main>");
      gtk_box_pack_start (GTK_BOX (dd->vbox),dd->menubar, FALSE, TRUE, 0);
      gtk_widget_show (dd->menubar);
    }
  else 
    {
      dd->menubar = gtk_item_factory_get_widget (dd->item_factory, "<main>");
      gtk_box_pack_start (GTK_BOX (dd->vbox),dd->menubar, FALSE, TRUE, 0);
      gtk_widget_show (dd->menubar);
    }
  return ;
}

/**********************************************************
 * General routines for dynamic menu item creation and deletion
 ***********************************************************/

/*---------------------------------------------------
 * Delete the button named button_name in window 
 * number win_num 
 *---------------------------------------------------*/

int C2F(delbtn)(win_num,button_name)
     integer *win_num;
     char *button_name;
{
  BCG *dd = GetWindowXgcNumber(*win_num);
  if ( dd == NULL) return;
  if ( dd->menu_entries != NULL) 
    {
      /* changes the stored menus_entries */
      sci_menu_delete(&dd->menu_entries,button_name);
      create_graphic_window_menu(dd);
    }
  return(0);
}

/*----------------------------------------------------------------
 *  Add dynamically buttons and menus in The Scilab Graphic Window
 * or in the Scilab main window 
 * Scilab interface for the AddMenu function 
 *  Add a menu in  window  number win_num or in Main window
 *
 *  win_num     : graphic window number or -1 for main scilab window
 *  button_name : label of button
 *  entries     : labels of submenus if any (in scilab code)
 *  ptrentries  : table of pointers on each entries
 *  ne          : number of submenus
 *  typ         : Action mode
 *                typ==0 : interpreted (execution of scilab instruction
 *                typ!=0 : hard coded a routine is called
 *  fname;      : name of the action function  
 *----------------------------------------------------------------*/

int C2F(addmen)(win_num,button_name,entries,ptrentries,ne,typ,fname,ierr)
     integer *win_num,*entries,*ptrentries,*ne,*ierr,*typ;
     char *button_name,*fname;
{
  char ** menu_entries;
  *ierr =0;
  if (*ne!=0) {
    ScilabMStr2CM(entries,ne,ptrentries,&menu_entries,ierr);
    if ( *ierr == 1) return(0);
  }
  AddMenu(win_num,button_name,menu_entries,ne,typ,fname,ierr);
  return(0);
}


/*---------------------------------------------------
 * Add a menu in  window  number wun_num or in Main window
 *  win_num     : graphic window number or -1 for main scilab window
 *  button_name : label of button
 *  entries     : labels of submenus if any
 *  ne          : number of submenus
 *  typ         : Action mode
 *                typ==0 : interpreted (execution of scilab instruction
 *  typ!=0 : hard coded a routine is called
 *  fname;      : name of the action function  
 *---------------------------------------------------*/

void AddMenu(win_num, button_name, entries, ne, typ, fname, ierr)
     integer *win_num;
     char *button_name;
     char **entries;
     integer *ne;
     integer *typ;
     char *fname;
     integer *ierr;
{ 
  /* reste a voir le cas du menu General Scilab XXXXXXXX */
  BCG *dd = GetWindowXgcNumber(*win_num);
  if ( dd == NULL) return;
  if ( sci_menu_add(&dd->menu_entries,*win_num,button_name,entries,*ne,*typ,fname) == 1 ) 
    {
      *ierr=1;
      return ;
    }
  create_graphic_window_menu (dd);
}

/*--------------------------------------------------
 * Activate or deactivate a menu 
 *---------------------------------------------------*/



void SetUnsetMenu(win_num, button_name, ne,flag)
     integer *win_num;
     char *button_name;
     integer *ne;
     int flag;
{ 
  menu_entry *e;
  /* reste a voir le cas du menu General Scilab XXXXXXXX */
  BCG *dd = GetWindowXgcNumber(*win_num);
  if ( dd == NULL) return;
  e = sci_menu_set_status(dd->menu_entries,*win_num,button_name,*ne,flag);
  if ( e != NULL) 
    {
      GtkWidget *w;
      char buf[128];
      if ( *ne == 0)
	{ 
	  /* top menu */ 
	  char *loc = e->name, *pbuf; 
	  strcpy(buf,"<main>/");
	  pbuf = buf + strlen(buf);
	  while ( *loc != '\0' ) 
	    { 
	      if ( *loc != '_' ) { *pbuf = *loc ; pbuf++; loc++;} 
	      else loc++;
	    }
	  *pbuf = '\0';
	}
      else 
	{
	  /* sub_menu */
	  char *loc = e->menu->name, *pbuf; 
	  strcpy(buf,"<main>/");
	  pbuf = buf + strlen(buf);
	  while ( *loc != '\0' ) 
	    { 
	      if ( *loc != '_' ) { *pbuf = *loc ; pbuf++; loc++;} 
	      else loc++;
	    }
	  *pbuf = '/';pbuf++;
	  loc = e->name ; 
	  while ( *loc != '\0' ) 
	    { 
	      if ( *loc != '_' ) { *pbuf = *loc ; pbuf++; loc++;} 
	      else loc++;
	    }
	  *pbuf = '\0';
	}
      w = gtk_item_factory_get_widget (dd->item_factory,buf);
      /* rend le menu non sensitif */
      if ( w != NULL) 
	{
	  if ( flag == TRUE ) 
	    gtk_widget_set_sensitive (w, TRUE);
	  else 
	    gtk_widget_set_sensitive (w, FALSE);
	}
    }
}

/** activate a menu (scilab interface) **/

int C2F(setmen)(win_num,button_name,entries,ptrentries,ne,ierr)
     integer *win_num,*entries,*ptrentries,*ne,*ierr;
     char *button_name;
{
  SetUnsetMenu(win_num,button_name,ne,TRUE);
  return(0);
}

int C2F(unsmen)(win_num,button_name,entries,ptrentries,ne,ierr)
     integer *win_num,*entries,*ptrentries,*ne,*ierr;
     char *button_name;
{
  SetUnsetMenu(win_num,button_name,ne,FALSE);
  return(0);
}

/*--------------------------------------------------------
 * Set of function to make 
 * addmenu/delmenu/set-unsetmenu 
 * work with itemfactory menus of gtk 
 * It seams that toplevel menu buttons cannot 
 * be deleted in itemfactory. 
 * thus we recreate a new menubar when menus are 
 * deleted 
 * Note: addmenu should be changed to accept accelerators.
 * 
 * XXXX A finir gere les noms avec ou sans _ 
 *--------------------------------------------------------*/

static void submenu_entry_set_menu(menu_entry *subs,menu_entry *father);

/* checks that name and name1 are the same taking care of _ */

int is_menu_name(char *name,char *name1) 
{
  while ( *name != 0) 
    {
      if ( *name == '_' ) 
	{
	  name++;
	  if ( *name1 == '_' )  name1++;
	}
      else if ( *name != *name1 ) {
	return 1;
      }
      else {
	name++; name1++;
      }
    }
  if ( *name1 != 0) return 1;
  return 0;
}
      

void menu_entry_show(menu_entry *m)
{
  if ( m== NULL) return ;
  fprintf(stdout,"name = %s ",m->name);
  fprintf(stdout,"status %d nsub=%d win=%d action=%d fname=%s\n",
	  m->status,m->nsub,m->winid,m->action_type,m->fname);
  if ( m->subs != NULL) 
    {fprintf(stdout,"{\n");
    menu_entry_show(m->subs);
    fprintf(stdout,"}\n");
    }
  menu_entry_show(m->next);
}

menu_entry *new_menu_entry(char *name,char *accel,int status,int nsub,
			   menu_entry *subs,int winid,
			   int action_type,char *fname)
{
  menu_entry *loc; 
  if ((loc = MALLOC(sizeof(menu_entry)))== NULL) 
    return NULL;
  if ((loc->name = MALLOC((strlen(name)+1)*sizeof(char)))==NULL) 
    return NULL;
  strcpy(loc->name,name);

  if (accel != NULL) 
    {
      if ((loc->accel = MALLOC((strlen(accel)+1)*sizeof(accel)))==NULL) 
	return NULL;
      strcpy(loc->accel,accel);
    }
  else loc->accel = NULL;

  if (fname != NULL) 
    {
      if ((loc->fname = MALLOC((strlen(fname)+1)*sizeof(fname)))==NULL) 
	return NULL;
      strcpy(loc->fname,fname);
    }
  else loc->fname= NULL;

  loc->status = status;
  loc->nsub = nsub;
  loc->subs = subs;
  loc->winid = winid;
  loc->action_type = action_type;
  loc->menu= NULL;
  loc->next = NULL;
  submenu_entry_set_menu(loc->subs,loc);
  return loc;
}

static void submenu_entry_set_menu(menu_entry *subs,menu_entry *father)
{
  if ( subs == NULL) return ;
  subs->menu = father ;
  submenu_entry_set_menu(subs->next,father);
}

void menu_entry_delete(menu_entry *me)
{
  if ( me == NULL) return ;
  /* recursive free  */
  FREE(me->name); 
  FREE(me->fname); 
  FREE(me->accel); 
  menu_entry_delete(me->subs);
  menu_entry_delete(me->next);
  FREE(me);
}

void submenu_add(menu_entry *me,menu_entry *more)
{
  menu_entry *loc = me->subs;
  if (loc == NULL) me->subs = more;
  while ( loc->next != NULL) loc = loc->next ;
  loc->next = more;
}

/****************************************************
 * Add a menu in a menu_item list
 *  win_num     : graphic window number or -1 for main scilab window
 *  name        : label of menu button
 *  entries     : labels of submenus if any. each entry is a string 
 *                "entry_name" or "entry_name|accelerator|specific_action"
 * 
 *  ne          : number of submenus
 *  action_type : Action mode
 *                action_type==0 : interpreted (execution of scilab instruction
 *                action_type!=0 : hard coded a routine is called
 *  fname;      : name of the action function  
 ****************************************************/

int sci_menu_add(menu_entry **m,int winid,char *name,char** entries,int ne, 
		  int action_type,char *fname)
{  
  int i;
  /* here we must find the menu_entry associated to win_num */
  menu_entry *me1,*me2,*top,*subs=NULL;
  /* first build the sub_menus */
  for (i=0 ; i < ne ;i++) 
    {
      char *accel;
      accel = strchr(entries[i],'|');
      if (accel != NULL) 
	{
	  char * action =  strchr(accel+1,'|');
	  *accel = '\0';
	  if ( action != NULL ) 
	    { 
	      *action = '\0';
	      if ( action == accel + 1 ) 
		me2 = new_menu_entry(entries[i],NULL,1,i+1,NULL,winid,
				     action_type,action+1);
	      else 
		me2 = new_menu_entry(entries[i],accel+1,1,i+1,NULL,winid,
				     action_type,action+1);
	      *action = '|';
	    }
	  else 
	    {
	      me2 = new_menu_entry(entries[i],accel+1,1,i+1,NULL,winid,
				   action_type,fname);
	    }
	  *accel='|';
	}
      else 
	{
	  me2 = new_menu_entry(entries[i],NULL,1,i+1,NULL,winid,
			       action_type,fname);
	}
      if ( me2 == NULL) 
	{
	  /* XXXXX clean and return */
	  return 1;
	}
      if ( i != 0) me1->next = me2;
      else { subs = me2;}
      me1=me2;
    }
  /* now the menu entry */
  top = new_menu_entry(name,NULL,1,1,subs,winid,action_type,fname);
  if ( top == NULL) 
    {
      /* XXXXX clean and return */
      return 1;
    }
  if ( *m == NULL) *m = top ;
  else 
    { 
      menu_entry *loc= *m ; 
      while (loc->next != NULL) loc=loc->next;
      loc->next = top;
    }
  return 0;
}

/****************************************************
 *Delete the menu name in menu_entry list 
 ****************************************************/

void sci_menu_delete(menu_entry **m, char *name) 
{ 
  menu_entry *loc,*nloc;
  if ( *m == NULL ) return ;
  /* we want to delete a toplevel menu */
  loc = *m ;
  if ( is_menu_name((*m)->name,name) ==0) 
    {
      /* we delete the first entry of m */
      *m = (*m)->next ; 
      loc->next = NULL;
      menu_entry_delete(loc);
      return ;
    }
  nloc = loc->next;
  while ( nloc != NULL) 
    {
      if ( is_menu_name(nloc->name,name)==0) 
	{
	  loc->next = nloc->next ;
	  nloc->next = NULL;
	  menu_entry_delete(nloc);
	  return ;
	}
      loc = nloc;
      nloc= nloc->next ;
    }
}

/****************************************************
 * Set the status of a menu 
 ****************************************************/

menu_entry * sci_menu_set_status(menu_entry *m,int winid,char *name,int subid,int status)
{  
  menu_entry *loc = m ;
  while ( loc != NULL) 
    {
      if ( is_menu_name(loc->name,name)==0) 
	{
	  if ( subid == 0) 
	    {
	      loc->status = status ;
	      return loc;
	    }
	  else 
	    {
	      int count;
	      /* walk to find submenu number subid */
	      loc = loc->subs; 
	      for ( count = 0 ; count < subid -1 ; count++) 
		loc = (loc == NULL) ? NULL : loc->next ;
	      if ( loc == NULL) return NULL ;
	      loc->status = status;
	      return loc;
	    }
	}
      loc = loc->next ;
    }
  return NULL;
}

/*------------------------------------------------------
 * menu callback 
 *------------------------------------------------------*/

static void sci_menu_default_callback (gpointer  callback_data,
				       guint     callback_action,
				       GtkWidget  *widget)
{
  static char buf[256];
  menu_entry *m = (menu_entry *) callback_data;

  fprintf(stdout,"menu activated \"%s\"", gtk_item_factory_path_from_widget (widget));
  if ( m== NULL) return ;
  /* 
     fprintf(stdout,"name = %s ",m->name);
     fprintf(stdout,"status %d nsub=%d win=%d action=%d fname=%s\n",
     m->status,m->nsub,m->winid,m->action_type,m->fname);
  */

  if ( call_predefined_callbacks(m->fname, m->winid)==1) return ;

  if (m->action_type == 0) 
    { 
      /* Interpreted mode : we store the action on a queue */
      if ( m->winid < 0 ) 
	sprintf(buf,"execstr(%s(%d))",m->fname,m->nsub);
      else 
	sprintf(buf,"execstr(%s_%d(%d))",m->fname,m->winid,m->nsub);
      StoreCommand(buf);
    }
  else 
    { 
      /* hard coded mode */
      int rep ;
      C2F(setfbutn)(m->fname,&rep);
      if ( rep == 0) 
	F2C(fbutn)((m->fname),&(m->winid),&(m->nsub));
    }
}


/*------------------------------------------------------
 * fill an item factory with a menu_entry description 
 *------------------------------------------------------*/

void sci_menu_to_item_factory(GtkItemFactory *ifactory,menu_entry *m)
{
  char buf_path[128];
  static GtkItemFactoryEntry entry = { NULL,NULL, sci_menu_default_callback,0,NULL};

  while ( m != NULL) 
    {
      sprintf(buf_path,"/%s",m->name);
      entry.path = buf_path;
      entry.accelerator = m->accel;
      if ( m->subs == NULL) 
	{
	  entry.item_type = "<Item>";
	  gtk_item_factory_create_item(ifactory,&entry,(void *)m,1);
	}
      else 
	{
	  menu_entry *loc;
	  entry.item_type = "<Branch>";
	  entry.callback = NULL;
	  gtk_item_factory_create_item(ifactory,&entry,(void *)m,1);
	  loc =  m->subs ; 
	  entry.item_type = "<Item>";
	  while ( loc != NULL) 
	    {
	      sprintf(buf_path,"/%s/%s",m->name,loc->name);
	      entry.path = buf_path;
	      entry.accelerator = loc->accel;
	      entry.callback = sci_menu_default_callback;
	      gtk_item_factory_create_item(ifactory,&entry,(void*)loc,1);
	      loc = loc->next;
	    }
	}
      m= m->next;
    }
}


void * graphic_initial_menu(int winid) {
  menu_entry *m = NULL;
  char *file_entries[] = { "_Clear||$clear",
			   "_Select||$select",
			   "_Print|<control>P|$print",
			   "_Export|<control>E|$export",
			   "S_ave|<control>S|$save",
			   "L_oad|<control>L|$load",
			   "C_lose||$close" };
  sci_menu_add(&m,winid,"_File",file_entries,7,0,"$file");
  sci_menu_add(&m,winid,"2D _Zoom",NULL,0,0,"$zoom");
  sci_menu_add(&m,winid,"_UnZoom",NULL,0,0,"$unzoom");
  sci_menu_add(&m,winid,"3D _Rot.",NULL,0,0,"$rot3d");
  return m;
}

void * sci_window_initial_menu() {
  menu_entry *m = NULL;
  int winid = -1;
  char *file_entries[] = { "File _Operations||$fileops",
			   "_Kill||$kill",
			   "_Quit|<control>Q|$quit" } ;
  char *control_entries[] = { "Resume||$resume",
			      "Abort||$abort",
			      "Restart||$restart",
			      "Stop||$stop" } ;
  char *graphic_entries[] = { "Create or Select",
			      "Raise", 
			      "Delete",
			      "+" ,
			      "-" } ;
			      
  sci_menu_add(&m,winid,"_File",file_entries,3,0,"$file");
  sci_menu_add(&m,winid,"_Control",control_entries,4,0,"$zoom");
  sci_menu_add(&m,winid,"_Demos",NULL,0,0,"$demos");
  sci_menu_add(&m,winid,"Graphic Window",graphic_entries,5,0,"$graphic_window");
  sci_menu_add(&m,winid,"Help",NULL,0,0,"$help");
  return m;
}

/**************************************************************
 * predefined actions for menus 
 **************************************************************/

/*-----------------------------------------------------------------
 * To clear the graphic window and clear the recorded graphics 
 * w and client_data are unused 
 *-----------------------------------------------------------------*/

static void scig_menu_erase(int winid) 
{
  scig_erase(winid);
}

/*----------------------------------------------
 * To select the graphic window 
 -----------------------------------------------*/

static void scig_menu_select(int winid)
{
  scig_sel(winid);
}

/*----------------------------------------------
 * To delete the graphic window 
 -----------------------------------------------*/

/* add handlers for delete action */

void scig_deletegwin_handler_none (win)int win; {};

static Scig_deletegwin_handler scig_deletegwin_handler = scig_deletegwin_handler_none;

Scig_deletegwin_handler set_scig_deletegwin_handler(f) 
     Scig_deletegwin_handler f;
{
  Scig_deletegwin_handler old = scig_deletegwin_handler;
  scig_deletegwin_handler = f;
  return old;
}

void reset_scig_deletegwin_handler() 
{
  scig_deletegwin_handler = scig_deletegwin_handler_none;
}

/* delete action */

static void scig_menu_delete(int winid) 
{
  scig_erase(winid);
  DeleteSGWin(winid);
  scig_deletegwin_handler(winid);
}

/* for Fortran call */

int C2F(deletewin)(int *winid) 
{
  scig_menu_delete(*winid);
  return(0);
}

/*-----------------------------------------------------------------*
 * Replot in Postscript style and send to printer 
 *-----------------------------------------------------------------*/

static  char bufname[256];
static  char printer[128];
static  char file[256];

static void scig_menu_print(int winid)
{
  char *p1;
  integer colored,orientation,flag=1,ok;
  prtdlg(&flag,printer,&colored,&orientation,file,&ok);
  if (ok==1) 
    {
      if ( ( p1 = getenv("TMPDIR"))  == (char *) 0 )
	{
	  sciprint("Cannot find environment variable TMPDIR\r\n");
	}

      sprintf(bufname,"%s/scilab-%d",p1,(int)winid);
      scig_tops(winid,colored,bufname,"Pos");
      sprintf(bufname,"$SCI/bin/scilab -%s %s/scilab-%d %s",
	      (orientation == 1) ? "print_l" : "print_p",
	      p1,(int)winid,printer);
      system(bufname);
  }
}

/* for use inside menus */

void scig_print(int winid) 
{
  scig_menu_print( winid);
}

/*-----------------------------------------------------------------
 * Replot in Postscript or Xfig style and save 
 *-----------------------------------------------------------------*/

static void scig_menu_saveps(int winid) 
{
  integer colored,orientation,flag=2,ok;
  prtdlg(&flag,printer,&colored,&orientation,file,&ok);
  if (ok==1) 
    {
      if (strncmp(printer,"Postscript",10)==0 ) 
	{
	  /** Postscript*   **/
	  scig_tops(winid,colored,file,"Pos");
	}
      else if (strcmp(printer,"Xfig")==0)
	{
	  /** Xfig   **/
	  scig_tops(winid,colored,file,"Fig");
	}
      else if (strcmp(printer,"Gif")==0)
	{
	  /** Gif file **/
	  scig_tops(winid,colored,file,"GIF");
	}
      if ( strcmp(printer,"Postscript No Preamble") != 0)
	{
	  sprintf(bufname,"$SCI/bin/scilab -%s %s %s",
		  ( orientation == 1) ? "save_l" : "save_p",file,printer);
	  system(bufname);
	}
  }
}

/* for use inside menus */

void scig_export(int winid)
{
  scig_menu_saveps(winid) ;
}

/*-----------------------------------------------------------------*
 * Binary File save 
 *-----------------------------------------------------------------*/

static void scig_menu_save(int winid) 
{
  char *filename;
  int ierr=0,rep;
  static char *init ="*.scg";
  rep=GetFileWindow(init,&filename,".",0,&ierr,"Save Graphic File");
  if ( ierr == 0 && rep == TRUE )
    {
      C2F(xsaveplots)(&winid,filename,0L);
    }
}

/*-----------------------------------------------------------------*
 * Binary File load 
 *-----------------------------------------------------------------*/

static void scig_menu_load(int winid) 
{
  char *filename;
  int ierr=0,rep;
  static char *init ="*.scg";
  rep=GetFileWindow(init,&filename,".",0,&ierr,"Load Graphic File");
  if ( ierr == 0 && rep == TRUE )
    {
      integer verb=0,cur,na;
      C2F(dr)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
      C2F(dr)("xset","window",&winid,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      C2F(xloadplots)(filename,0L);
      C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
}

/*-----------------------------------------------------------------**
 * 2D Zoom calback 
 *-----------------------------------------------------------------*/

static void scig_menu_zoom(int winid) 
{
  integer ne=0;
  SetUnsetMenu(&winid,"2D Zoom",&ne,False);
  SetUnsetMenu(&winid,"3D Rot.",&ne,False);
  SetUnsetMenu(&winid,"UnZoom",&ne,False);
  SetUnsetMenu(&winid,"File",&ne,False);
  scig_2dzoom(winid);
  SetUnsetMenu(&winid,"2D Zoom",&ne,True);
  SetUnsetMenu(&winid,"3D Rot.",&ne,True);
  SetUnsetMenu(&winid,"UnZoom",&ne,True);
  SetUnsetMenu(&winid,"File",&ne,True);
}


/*-----------------------------------------------------------------
 * Unzoom Callback 
 *-----------------------------------------------------------------*/

static void scig_menu_unzoom(int winid) 
{
  integer ne=0;
  SetUnsetMenu(&winid,"UnZoom",&ne,False);
  scig_unzoom(winid);
  SetUnsetMenu(&winid,"UnZoom",&ne,True);
}


/*-----------------------------------------------------------------
 * 3D Rotation callback 
 *-----------------------------------------------------------------*/

static void scig_menu_rot3d(int winid) 
{
  integer ne=0;
  SetUnsetMenu(&winid,"3D Rot.",&ne,False);
  SetUnsetMenu(&winid,"UnZoom",&ne,False);
  SetUnsetMenu(&winid,"2D Zoom",&ne,False);
  SetUnsetMenu(&winid,"File",&ne,False);
  scig_3drot(winid);
  SetUnsetMenu(&winid,"3D Rot.",&ne,True);
  SetUnsetMenu(&winid,"UnZoom",&ne,True);
  SetUnsetMenu(&winid,"2D Zoom",&ne,True);
  SetUnsetMenu(&winid,"File",&ne,True);
}


/*-----------------------------------------------------------------
 * kill scilab 
 *-----------------------------------------------------------------*/

static void sci_menu_kill()
{
  sci_clear_and_exit(1);
}

/*-----------------------------------------------------------------
 * make a stop 
 *-----------------------------------------------------------------*/

static void sci_menu_stop () 
{
  int j = SIGINT;
  C2F(sigbas)(&j);
  if (get_is_reading()) write_scilab("");
}

/*-----------------------------------------------------------------
/
 * run the help 
 *-----------------------------------------------------------------*/

extern void   popupHelpPanel();

static void sci_menu_help()
{
  popupHelpPanel();
}

/*-----------------------------------------------------------------
 * run the demos 
 *-----------------------------------------------------------------*/

static void sci_menu_demos()
{
  StoreCommand( get_sci_data_strings(2));
}

/*-----------------------------------------------------------------
 * Execute predefined callbacks 
 *-----------------------------------------------------------------*/

int call_predefined_callbacks(char *name, int winid)
{
  if      (strcmp(name,"$clear")== 0)  scig_menu_erase(winid) ;
  else if (strcmp(name,"$select")== 0)  scig_menu_select(winid) ;
  else if (strcmp(name,"$print")== 0) scig_menu_print(winid);
  else if (strcmp(name,"$export")== 0)  scig_export(winid);
  else if (strcmp(name,"$save")== 0)  scig_menu_save(winid);
  else if (strcmp(name,"$load")== 0) scig_menu_load(winid);
  else if (strcmp(name,"$close")== 0) scig_menu_delete(winid);
  else if (strcmp(name,"$zoom")== 0)  scig_menu_zoom(winid);
  else if (strcmp(name,"$unzoom")== 0)  scig_menu_unzoom(winid);
  else if (strcmp(name,"$rot3d")== 0) scig_menu_rot3d(winid);
  else if (strcmp(name,"$help")== 0) sci_menu_help();
  else if (strcmp(name,"$stop")== 0) sci_menu_stop();
  else if (strcmp(name,"$kill")== 0) sci_menu_kill();
  else if (strcmp(name,"$demos")== 0) sci_menu_demos();
  else return 0;
  return 1;
}

