/* $XConsortium: Initer.c,v 1.7 91/05/28 16:08:34 converse Exp $ */

/* 
 * Copyright 1988, 1989 by the Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided 
 * that the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of M.I.T. not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission. M.I.T. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 */

/* Created By:  Chris D. Peterson
 *              MIT X Consortium
 * Date:        May 8, 1989
 */

#include <X11/Intrinsic.h>
#include <X11/Xmu/Initer.h>

struct InitializerList {
  XmuInitializerProc function;	/* function to call */
  caddr_t data;			/* Data to pass the function. */
  XtAppContext * app_con_list;	/* a null terminated list of app_contexts. */
};
  
static struct InitializerList * init_list = NULL;
static Cardinal init_list_length = 0;

static Boolean AddToAppconList();

void
XmuAddInitializer(func, data) 
XmuInitializerProc func;
caddr_t data;
{
  init_list_length++;
  init_list = (struct InitializerList *) XtRealloc( (char *) init_list, 
					    (sizeof(struct InitializerList) * 
					     init_list_length) );

  init_list[init_list_length - 1].function = func;
  init_list[init_list_length - 1].data = data;
  init_list[init_list_length - 1].app_con_list = NULL;
}

void
XmuCallInitializers(app_con)
XtAppContext app_con;
{
  int i;

  for (i = 0 ; i < init_list_length ; i++) {
    if (AddToAppconList(&(init_list[i].app_con_list), app_con))
      (init_list[i].function) (app_con, init_list[i].data);
  }
}

/*	Function Name: AddToAppconList
 *	Description: Adds an action to the application context list and
 *                   returns TRUE, if this app_con is already on the list then
 *                   it is NOT added and FALSE is returned.
 *	Arguments: app_list - a NULL terminated list of application contexts.
 *                 app_con - an application context to test.
 *	Returns: TRUE if not found, FALSE if found.
 */

static Boolean
AddToAppconList(app_list, app_con)
XtAppContext **app_list, app_con;
{
  int i;
  XtAppContext *local_list;

  i = 0;
  local_list = *app_list;
  if (*app_list != NULL) {
    for ( ; *local_list != NULL ; i++, local_list++) {
      if (*local_list == app_con)
	return(FALSE);
    }
  }

  *app_list = (XtAppContext *)  XtRealloc((char *)(*app_list),
					  sizeof(XtAppContext *) * (i + 2) );
  (*app_list)[i++] = app_con;
  (*app_list)[i] = NULL;
  return(TRUE);
}
  
