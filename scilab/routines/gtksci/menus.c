/*------------------------------------------------------------------------
 *    Copyright (C) 2001-2003 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 *    main menu and graphic window menus 
 *--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "../graphics/Math.h"
#include "../graphics/bcg.h" 
#include "../graphics/Graphics.h" 
#include "../os_specific/Os_specific.h" 
#include "../os_specific/men_Sutils.h"
#include "../graphics/Events.h"
#include "All-extern.h"

#define PI0 (integer *) 0
#define PD0 (double *) 0

extern char *sci_convert_to_utf8(char *str, int *alloc);

void menu_entry_show(menu_entry *m);  /* only used for debug */ 
extern void create_scilab_about(void); 
extern int prtdlg  __PARAMS((integer *flag, char *printer, integer *colored, integer *orientation, char *file, integer *ok));
extern int sci_get_file_window __PARAMS((char *, char **, char *, int,int, int *,char *));

extern char GetDriver();
static void *sci_window_initial_menu() ;
static void sci_menu_to_item_factory(GtkItemFactory *ifactory,menu_entry *m);
static void sci_menu_delete(menu_entry **m, char *name) ;
static int sci_menu_add(menu_entry **m,int winid,char *name,char** entries,int ne, 
			int action_type,char *fname);
static menu_entry * sci_menu_set_status(menu_entry *m,int winid,char *name,
					int subid,int status);
static int call_predefined_callbacks(char *name, int winid);

static void sci_factory_add_menu_entry(GtkItemFactory *ifactory,menu_entry *m);
static void sci_factory_add_last_menu_entry(GtkItemFactory *ifactory,menu_entry *m);
/*--------------------------------------------------------------
 * main menu. i.e the menu of the main scilab window 
 *            this menu is attached to a zterm through a plug widget
 *--------------------------------------------------------------*/

static menu_entry *main_menu_entries = NULL;
static GtkItemFactory  *main_item_factory= NULL;

/*
 * used when the menu is plugged 
 */

void create_plugged_main_menu() 
{
  static GtkWidget *menubar = NULL; 
  static int first = 0; 
  static GtkWidget *Plug;
  static GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group = NULL ; 
  char * plug_info = getenv("SCIWIN");

  if ( plug_info == NULL) return ;

  main_item_factory= item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
							  accel_group);
  
  if ( first == 0 ) {
    Plug = gtk_plug_new(atoi(getenv("SCIWIN")));
    main_menu_entries = sci_window_initial_menu();
    if ( main_menu_entries == NULL) return;
    first = 1;
  }

  /* This function generates the menu items from scilab description */
  /* Attention il faut aussi gerer les menu en set unest XXXXX */

  sci_menu_to_item_factory(item_factory, main_menu_entries);
  
  /* Attach the new accelerator group to the window. */
  /* gtk_window_add_accel_group (GTK_WINDOW (window), accel_group); */ 
  
  /* Finally, return the actual menu bar created by the item factory. */ 

  if ( menubar != NULL) gtk_widget_destroy(menubar);
  menubar = gtk_item_factory_get_widget (item_factory, "<main>");
  gtk_container_add(GTK_CONTAINER(Plug),menubar);
  if ( accel_group != NULL ) 
    gtk_window_add_accel_group (GTK_WINDOW (Plug), accel_group);
  gtk_widget_show_all(Plug);

}



/*
 * used when zterm is plugged 
 */

GtkWidget *create_main_menu( GtkWidget  *window)
{
  static int first = 0;
  static GtkWidget *menubar = NULL; 
  static GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group = NULL ; 
  
  /* Make an accelerator group (shortcut keys) */
  if ( window != NULL)  accel_group = gtk_accel_group_new ();

  main_item_factory= item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
							  accel_group);
  if ( first == 0 ) {
    main_menu_entries = sci_window_initial_menu();
    if ( main_menu_entries == NULL) return NULL;
    first = 1;
  }

  /* This function generates the menu items from scilab description */
  /* Attention il faut aussi gerer les menu en set unset XXXXX */

  sci_menu_to_item_factory(item_factory, main_menu_entries);
  
  /* Finally, return the actual menu bar created by the item factory. */ 
  if ( menubar != NULL) gtk_widget_destroy(menubar);
  menubar = gtk_item_factory_get_widget (item_factory, "<main>");
  if ( window != NULL )  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
  return menubar;

}



/*--------------------------------------------------------------
 * Graphic window menu in main menu 
 *--------------------------------------------------------------*/

static integer lab_count = 0;
static char gwin_name[100];


void MenuFixCurrentWin(int ivalue)
{
  int w=-1;
  char *graphic_entries[] = { "Create or Select||$gwselect",
			      "Raise||$gwraise", 
			      "Delete||$gwdelete",
			      "+||$gwplus" ,
			      "-||$gwminus" } ;
  if ( ivalue == lab_count ) return ; 
  if ( main_item_factory == NULL ) return;
  sprintf( gwin_name, "Graphic Window %d", (int) lab_count );
  C2F(delbtn)(&w, gwin_name);
  sprintf( gwin_name, "Graphic Window %d", (int) ivalue );
  lab_count = ivalue;
  sci_menu_add(&main_menu_entries,-1,gwin_name,
	       graphic_entries,5,0,"$graphic_window");
  sci_factory_add_last_menu_entry(main_item_factory,main_menu_entries);
}


/*--------------------------------------------------------------
 * Graphic window menu 
 *--------------------------------------------------------------*/

void create_graphic_window_menu(struct BCG *dd)
{
  GtkAccelGroup *accel_group=  gtk_accel_group_new ();

  /* Attach the new accelerator group to the window. */
  gtk_window_add_accel_group (GTK_WINDOW (dd->window), accel_group);

  dd->item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
					   accel_group);
  
  /* This function generates the menu items from scilab description */
  /* Attention il faut aussi gerer les menu en set unest XXXXX */
  
  sci_menu_to_item_factory(dd->item_factory,dd->menu_entries);
  
  /* Finally, return the actual menu bar created by the item factory. */ 

  dd->menubar = gtk_item_factory_get_widget (dd->item_factory, "<main>");
  gtk_box_pack_start (GTK_BOX (dd->vbox),dd->menubar, FALSE, TRUE, 0);
  gtk_widget_show (dd->menubar);
  return ;
}

/**********************************************************
 * General routines for dynamic menu item creation and deletion
 ***********************************************************/

/*---------------------------------------------------
 * Delete the button named button_name in the menu of window 
 * number win_num 
 *---------------------------------------------------*/

int C2F(delbtn)(int *win_num,char *button_name)
{
  GtkItemFactory  *item_factory;
  static char btn[64];
  char *p,*but= button_name;
  char *btn_utf8;
  int alloc;
  p = btn ; 
  *(p++) = '/';
  while ( *but != '\0' ) {
    if ( *but == '/') break ; 
    else if ( *but == '_') but++ ; 
    else { *(p++)= *(but++);}
  }
  *p = '\0';
  if ( *win_num == -1 ) 
    {
      item_factory = main_item_factory; 
      sci_menu_delete(&main_menu_entries,button_name);
    }
  else 
    {
      BCG *dd = GetWindowXgcNumber(*win_num);
      if ( dd == NULL || dd->item_factory == NULL) return 0;
      item_factory = dd->item_factory;
      sci_menu_delete(&dd->menu_entries,button_name);
    }
  btn_utf8 = sci_convert_to_utf8(btn,&alloc);
  gtk_item_factory_delete_item (item_factory,btn_utf8);
  if ( alloc == 1) free(btn_utf8);
  return 0;
}

/*----------------------------------------------------------------
 * Add dynamically buttons and menus in The Scilab Graphic Window
 * or in the Scilab main window 
 * Scilab interface for the AddMenu function 
 * Add a menu in  window  number win_num or in Main window
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
#if GTK_MAJOR_VERSION == 2 
  int button_alloc,i,*ent_alloc;
  char *button_name_utf8;
#endif 
  char ** menu_entries;
  *ierr =0;
  if (*ne!=0) {
    ScilabMStr2CM(entries,ne,ptrentries,&menu_entries,ierr);
    if ( *ierr == 1) return(0);
  }
#if GTK_MAJOR_VERSION == 2 
  /* switch to utf8 if necessary */
  button_name_utf8 = sci_convert_to_utf8(button_name,&button_alloc);
  ent_alloc = malloc((*ne)*sizeof(int));
  if ( ent_alloc == NULL) return 0;
  for (i = 0; i < *ne ; i++)
    menu_entries[i] = sci_convert_to_utf8(menu_entries[i],&ent_alloc[i]);
  AddMenu(win_num,button_name_utf8,menu_entries,ne,typ,fname,ierr);
  if ( button_alloc == TRUE) free(button_name_utf8);
  for ( i = 0; i < *ne ; i++ ) if (ent_alloc[i]==TRUE) free(menu_entries[i]);
  free(ent_alloc);
#else 
  AddMenu(win_num,button_name,menu_entries,ne,typ,fname,ierr);
#endif
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
  if ( *win_num == -1 ) 
    {
      /* Scilab main menu */ 
      if ( main_item_factory == NULL ) return;
      if ( sci_menu_add(&main_menu_entries,*win_num,button_name,entries,*ne,
			*typ,fname) == 1 ) 
	{
	  *ierr=1;
	  return ;
	}
      sci_factory_add_last_menu_entry(main_item_factory,main_menu_entries);
    }
  else 
    {
      BCG *dd = GetWindowXgcNumber(*win_num);
      if ( dd == NULL || dd->item_factory == NULL ) return;
      if ( sci_menu_add(&dd->menu_entries,*win_num,button_name,entries,
			*ne,*typ,fname) == 1 ) 
	{
	  *ierr=1;
	  return ;
	}
      sci_factory_add_last_menu_entry(dd->item_factory,dd->menu_entries);
    }
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
  menu_entry *e,*entries;
  GtkItemFactory  *item_factory;
  if ( *win_num == -1 ) 
    {
      item_factory = main_item_factory; 
      entries =main_menu_entries;
    }
  else 
    {
      BCG *dd = GetWindowXgcNumber(*win_num);
      if ( dd == NULL || dd->item_factory == NULL) return ;
      item_factory = dd->item_factory;
      entries = dd->menu_entries;
    }
  
  e = sci_menu_set_status(entries,*win_num,button_name,*ne,flag);
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
      w = gtk_item_factory_get_widget (item_factory,buf);
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
 * A Set of functions to make addmenu/delmenu/set-unsetmenu 
 * work with itemfactory menus of gtk 
 * It seams that toplevel menu buttons cannot 
 * be deleted in itemfactory. 
 * thus we recreate a new menubar when menus are deleted 
 *--------------------------------------------------------*/

static void submenu_entry_set_menu(menu_entry *subs,menu_entry *father);

/* checks that name1 is a menu name
 * take care that name can be UTF8 because of previous conversion 
 * if so we must also convert name1 
 */

extern char *sci_convert_to_utf8(char *str, int *alloc);


static int is_menu_name( char *name, char *name1) 
{
  char *name_utf8,*name_kp;
  char *name1_utf8,*name1_kp;
  int alloc1,alloc,rep=0;
  name1_kp = name1_utf8 = sci_convert_to_utf8(name1,&alloc1);
  name_kp = name_utf8 = sci_convert_to_utf8(name,&alloc);
  while ( *name_utf8 != 0) 
    {
      if ( *name_utf8 == '_' ) 
	{
	  name_utf8++;
	  if ( *name_utf8 == '_' )  name1_utf8++;
	}
      else if ( *name_utf8 != *name1_utf8 ) {
	rep=1; goto end;
      }
      else {
	name_utf8++; name1_utf8++;
      }
    }
  if ( *name1_utf8 != 0) {rep=1; goto end;};
 end: 
  if ( alloc1 == 1) free(name1_kp);
  if ( alloc == 1) free(name_kp);
  return rep;
}

void menu_entry_show(menu_entry *m)
{
  if ( m== NULL) return ;
  fprintf(stdout,"name = %s ",m->name);
  fprintf(stdout,"status %d nsub=%d win=%d action=%d fname=%s\n",
	  m->status,m->nsub,m->winid,m->action_type,m->fname);
  if ( m->accel != NULL) 
    fprintf(stdout,"accel %s \n",m->accel);
  if ( m->subs != NULL) 
    {
      fprintf(stdout,"{\n");
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
 *                if entries is empty then name can be 
 *                   "name|accelerator|specific_action"
 *  entries     : labels of submenus if any. each entry is a string 
 *                "entry_name" or "entry_name|accelerator|specific_action"
 * 
 *  ne          : number of submenus
 *  action_type : Action mode
 *                action_type==0 : interpreted (execution of scilab instruction
 *                action_type!=0 : hard coded a routine is called
 *  fname;      : name of the action function  
 ****************************************************/


static menu_entry *gtksci_new_menu_entry(char *entry_name,int winid,int pos,int action_type,char *fname)
{
  menu_entry *menu ; 
  char *entry=NULL,*accel=NULL,*action=NULL;
  entry=strdup(entry_name);
  accel = strchr(entry,'|');
  if (accel != NULL) 
    {
      *accel = '\0'; accel +=1;
      action =  strchr(accel,'|');
      if ( action != NULL ) 
	{ 
	  *action = '\0'; action+=1;
	}
    }
  if (action == NULL) action = fname ;
  menu = new_menu_entry(entry,accel,1,pos+1,NULL,winid,action_type,action);
  if ( menu == NULL) 
    {
      /* XXXXX clean and return */
      if(entry != NULL) free(entry);
      return NULL;
    }
  return menu;
}



static int sci_menu_add(menu_entry **m,int winid,char *name,char** entries,int ne, 
			int action_type,char *fname)
{  
  int i;
  char *entry=NULL;
  /* here we must find the menu_entry associated to win_num */
  menu_entry *me1=NULL,*me2,*top,*subs=NULL;
  /* first build the sub_menus */
  for (i=0 ; i < ne ;i++) 
    {
      me2 = gtksci_new_menu_entry(entries[i],winid,i,action_type,fname);
      if ( me2 == NULL) 
	{
	  return 1;
	}
      if ( i != 0) me1->next = me2;
      else { subs = me2;}
      me1=me2;
    }
  if ( ne == 0 ) 
    {
      top = gtksci_new_menu_entry(name,winid,0,action_type,fname);
    }
  else
    {
      /* now the menu entry */
      top = new_menu_entry(name,NULL,1,1,subs,winid,action_type,fname);
    }
  if ( top == NULL) 
    {
      /* XXXXX clean and return */
      if(entry != NULL) free(entry);
      return 1;
    }
  if ( *m == NULL) *m = top ;
  else 
    { 
      menu_entry *loc= *m ; 
      while (loc->next != NULL) loc=loc->next;
      loc->next = top;
    }
  if(entry != NULL) free(entry);
  return 0;
}

/****************************************************
 *Delete the menu name in menu_entry list 
 ****************************************************/

static void sci_menu_delete(menu_entry **m, char *name) 
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


static menu_entry * sci_menu_set_status(menu_entry *m,int winid,char *name,
					int subid,int status)
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
#ifdef DEBUG
  fprintf(stdout,"menu activated \"%s\"", 
	  gtk_item_factory_path_from_widget (widget));
#endif 
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
  else if (m->action_type == 2) 
    { 
      /* Interpreted mode : we store the action on a queue */
      if ( m->winid < 0 ) 
	sprintf(buf,"%s(%d)",m->fname,m->nsub);
      else 
	sprintf(buf,"%s(%d,%d)",m->fname,m->nsub,m->winid);
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

static void sci_menu_to_item_factory(GtkItemFactory *ifactory,menu_entry *m)
{
  while ( m != NULL) 
    {
      sci_factory_add_menu_entry(ifactory,m);
      m= m->next;
    }
}

/*-------------------------------------------------------------------
 * build items associated to the last menu_entry contained in m 
 * and add them in the factory ifactory 
 *-------------------------------------------------------------------*/

static void sci_factory_add_last_menu_entry(GtkItemFactory *ifactory,menu_entry *m)
{
  if ( m == NULL ) return ;
  while ( m->next != NULL) m = m->next ; 
  sci_factory_add_menu_entry(ifactory,m);
}


/*-------------------------------------------------------------------
 * build items associated to the first menu_entry contained in m 
 * and add them in the factory ifactory 
 *-------------------------------------------------------------------*/

static void sci_factory_add_menu_entry(GtkItemFactory *ifactory,menu_entry *m)
{
  char buf_path[128];
  static GtkItemFactoryEntry entry = { NULL,NULL, sci_menu_default_callback,0,NULL};
  if ( m == NULL ) return ;
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
      if ( is_menu_name(m->name,"Help")==0) 
	entry.item_type = "<LastBranch>";
      else 
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
  char *graph_entries[] =  {"gtk_main_quit||$gtk_main_quit",
			    "test2D+xsetech||$test2DD",
			    "test2D||$test2D",
			    "plot2d with animation||$test2D2",
			    "test2D3||$test2D3",
			    "test2D4||$test2D4",
			    "test2DN 1||$test2DN1",
			    "test2DN 2||$test2DN2",
			    "test2DN 3||$test2DN3",
			    "test3D||$test3D",
			    "test3D animation||$test3DA",
			    "test3D1||$test3D1",
			    "test3D2||$test3D2",
			    "testArrows||$testArrows",
			    "testC 1||$testC1",
			    "testC 2||$testC2",
			    "testC 3||$testC3",
			    "testC2d||$TestC2d",
			    "testCh||$testCh",
			    "testG||$testG",
			    "testP3D||$testP3D",
			    "testPattern||$testPattern",
			    "testColor||$testColor",
			    "testPrim||$testPrim",
			    "testString||$testString",
			    "testXor||$testXormode",
			    "testXliness||$testXliness",
			    "testXrects||$testXrects",
			    "testXpoly||$testPoly",
			    "testMark||$testMarks"};


  sci_menu_add(&m,winid,"_File",file_entries,7,0,"$file");
  sci_menu_add(&m,winid,"_Zoom",NULL,0,0,"$zoom");
  sci_menu_add(&m,winid,"_UnZoom",NULL,0,0,"$unzoom");
  sci_menu_add(&m,winid,"3D _Rot.",NULL,0,0,"$rot3d");
  /* sci_menu_add(&m,winid,"Tests",graph_entries,30,0,"$testg");*/
  return m;
}

static void * sci_window_initial_menu() 
{
  menu_entry *m = NULL;
  int winid = -1;
  char *file_entries[] = { "File _Operations||$fileops",
			   "_Kill||$kill",
			   "_Quit|<control>Q|$quit" } ;
  char *control_entries[] = { "Resume||$resume",
			      "Abort||$abort",
			      "Restart||$restart",
			      "Stop||$stop",
                              "Clear history||$clear_history"};
  char *graphic_entries[] = { "Create or Select||$gwselect",
			      "Raise||$gwraise", 
			      "Delete||$gwdelete",
			      "+||$gwplus" ,
			      "-||$gwminus" } ;
	
  char *help_entries[] = { "Scilab Help||$help",
			   "About||$about"};
  sci_menu_add(&m,winid,"_File",file_entries,3,0,"$file");
  sci_menu_add(&m,winid,"_Control",control_entries,5,0,"$zoom");
  sci_menu_add(&m,winid,"_Demos",NULL,0,0,"$demos");
  sci_menu_add(&m,winid,"Graphic Window 0",graphic_entries,5,0,"$graphic_window");
  sci_menu_add(&m,winid,"Help",help_entries,2,0,"$help");
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


/* delete action */

extern void  delete_sgwin_entities(int winid,int v_flag);


static void scig_menu_delete(int winid) 
{
  int v_flag = 1;
  scig_erase(winid);
  if (version_flag() == 0) {DeleteObjs(winid); v_flag = 0;}
  scig_deletegwin_handler(winid);
  DeleteSGWin(winid);
  delete_sgwin_entities(winid,v_flag);
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
      else if (strcmp(printer,"PPM")==0)
	{
	  /** PPM file **/
	  scig_tops(winid,colored,file,"PPM");
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
  rep=sci_get_file_window(init,&filename,".",0,0,&ierr,"Save Graphic File");
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
  rep=sci_get_file_window(init,&filename,".",0,0,&ierr,"Load Graphic File");
  if ( ierr == 0 && rep == TRUE )
    {
      scig_loadsg(winid, filename);
    }
}

/*-----------------------------------------------------------------*
 * file operations 
 *-----------------------------------------------------------------*/

static void sci_menu_fileops()
{
  static char * file = NULL ;
  int rep,ierr;
  rep = sci_get_file_window(NULL,&file,".",0,1,&ierr,"File operations");
  if ( ierr == 0 && rep == TRUE ) 
    { 
      StoreCommand1(file,1);  /* we want the command to appear in the window */
      free(file); 
      file =NULL; 
    }
}

/*-----------------------------------------------------------------**
 * 2D Zoom calback 
 *-----------------------------------------------------------------*/

static void scig_menu_zoom(int winid) 
{
  integer ne=0;
  SetUnsetMenu(&winid,"Zoom",&ne,False);
  SetUnsetMenu(&winid,"3D Rot.",&ne,False);
  SetUnsetMenu(&winid,"UnZoom",&ne,False);
  SetUnsetMenu(&winid,"File",&ne,False);
  if ( scig_2dzoom(winid)==1) return; /* the window has been closed */
  SetUnsetMenu(&winid,"Zoom",&ne,True);
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
  SetUnsetMenu(&winid,"Zoom",&ne,False);
  SetUnsetMenu(&winid,"File",&ne,False);
  if (scig_3drot(winid)==1) return; /* the window has been closed */
  SetUnsetMenu(&winid,"3D Rot.",&ne,True);
  SetUnsetMenu(&winid,"UnZoom",&ne,True);
  SetUnsetMenu(&winid,"Zoom",&ne,True);
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
  /* if (get_is_reading()) write_scilab(""); */
}

/*-----------------------------------------------------------------
 * run the help 
 *-----------------------------------------------------------------*/

extern int  Sci_Help(char *name);

static void sci_menu_help()
{
  StoreCommand("help();");
}

/*-----------------------------------------------------------------
 * run the demos 
 *-----------------------------------------------------------------*/

static void sci_menu_demos()
{
  StoreCommand( get_sci_data_strings(2));
}


/*-----------------------------------------------------------------
 * Callbacks for the Graphic Window main menu 
 *-----------------------------------------------------------------*/

static void sci_menu_gwplus() 
{
  MenuFixCurrentWin(lab_count+1); 
}

static void sci_menu_gwminus() 
{
  MenuFixCurrentWin(lab_count-1); 
}

static void sci_menu_gwcreate_or_select() 
{
  scig_sel(lab_count);
}

static void sci_menu_gwraise() 
{
  scig_raise(lab_count);
}

static void sci_menu_gwdelete() 
{
  scig_menu_delete(lab_count);
}

extern void   reset_history(void);


static void sci_menu_clear_history() 
{
  reset_history();
}



/*-----------------------------------------------------------------
 * Execute predefined callbacks 
 *-----------------------------------------------------------------*/

extern int call_graphics_predefined_callbacks(char *name, int winid);

static int call_predefined_callbacks(char *name, int winid)
{
  /* if ( call_graphics_predefined_callbacks(name,winid) == 1 ) return 1;*/
  if      (strcmp(name,"$clear")== 0)  scig_menu_erase(winid) ;
  else if (strcmp(name,"$select")== 0) scig_menu_select(winid) ;
  else if (strcmp(name,"$print")== 0)  scig_menu_print(winid);
  else if (strcmp(name,"$export")== 0) scig_export(winid);
  else if (strcmp(name,"$save")== 0)   scig_menu_save(winid);
  else if (strcmp(name,"$load")== 0)   scig_menu_load(winid);
  else if (strcmp(name,"$close")== 0)  scig_menu_delete(winid);
  else if (strcmp(name,"$zoom")== 0)   scig_menu_zoom(winid);
  else if (strcmp(name,"$unzoom")== 0) scig_menu_unzoom(winid);
  else if (strcmp(name,"$rot3d")== 0)  scig_menu_rot3d(winid);
  else if (strcmp(name,"$help")== 0)   sci_menu_help();
  else if (strcmp(name,"$stop")== 0)   sci_menu_stop();
  else if (strcmp(name,"$kill")== 0)   sci_menu_kill();
  else if (strcmp(name,"$demos")== 0)  sci_menu_demos();
  else if (strcmp(name,"$fileops")== 0) sci_menu_fileops();
  else if (strcmp(name,"$gwselect")== 0) sci_menu_gwcreate_or_select();
  else if (strcmp(name,"$gwraise")== 0) sci_menu_gwraise();
  else if (strcmp(name,"$gwdelete")== 0) sci_menu_gwdelete();
  else if (strcmp(name,"$gwplus")== 0)  sci_menu_gwplus();
  else if (strcmp(name,"$gwminus")== 0)  sci_menu_gwminus();
  else if (strcmp(name,"$about")== 0)  create_scilab_about ();
  else if (strcmp(name,"$resume")== 0) StoreCommand1("resume",1);
  else if (strcmp(name,"$abort")== 0)  StoreCommand1("abort",1); 
  else if (strcmp(name,"$restart")== 0) StoreCommand1("exec SCI/scilab.star;",1);
  else if (strcmp(name,"$quit")== 0)  StoreCommand1("quit;",1);
  else if (strcmp(name,"$clear_history")== 0) sci_menu_clear_history();
  else return 0;
  return 1;
}


static int getNbSubMenus(int winid, char *menu_name )
{
  int count=0;
  menu_entry *loc, *entries;
  GtkItemFactory  *item_factory;
  if ( winid == -1 ) 
    {
      item_factory = main_item_factory; 
      entries =main_menu_entries;
    }
  else 
    {
      BCG *dd = GetWindowXgcNumber(winid);
      if ( dd == NULL || dd->item_factory == NULL) return ;
      item_factory = dd->item_factory;
      entries = dd->menu_entries;
    }
  loc = entries ;
  while ( loc != NULL) 
    {
      if ( is_menu_name(loc->name,menu_name)==0) 
	{
	  int count;
	  /* walk to count */
	  loc = loc->subs; 
	  while (loc != NULL) {count++; loc=loc->next;}
	}
      loc = loc->next ;
    }
  return count;
}

/*
 * put some menus in gray under old graphic style  
 * or put them back under new graphic style        
 */


void refreshMenus( struct BCG * ScilabGC )
{
  /* put some menu in grey in old graphic style */
  if ( version_flag() == 0 )
    {
      /* new graphic style */
      int        subMenuNumber = 0 ;
      int        nbChildren        ;
      /*SetUnsetMenu( &ScilabGC->CurWindow, "Insert", &subMenuNumber, True ) ; */
      /* get the number of children of the Edit menu */
      nbChildren = getNbSubMenus( ScilabGC->CurWindow, "Edit" ) ;
      for ( subMenuNumber = 5 ; subMenuNumber <= nbChildren ; subMenuNumber++ )
	{
	  SetUnsetMenu( &ScilabGC->CurWindow, "Edit", &subMenuNumber, True ) ;
	}
    }
  else
    {
      /* old graphic style */
      int        subMenuNumber = 0 ;
      int        nbChildren        ;
      /*SetUnsetMenu( &ScilabGC->CurWindow, "Insert", &subMenuNumber, False ) ; */
      nbChildren = getNbSubMenus( ScilabGC->CurWindow, "Edit" ) ;
      for ( subMenuNumber = 5 ; subMenuNumber <= nbChildren ; subMenuNumber++ )
	{
	  SetUnsetMenu( &ScilabGC->CurWindow, "Edit", &subMenuNumber, False ) ;
	}
    }
}

