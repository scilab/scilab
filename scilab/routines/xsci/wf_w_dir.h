/* This file is part of xdir, an X-based directory browser.
 *
 *	Created: 13 Aug 88
 *
 *	Win Treese
 *	Cambridge Research Lab
 *	Digital Equipment Corporation
 *	treese@crl.dec.com
 *
 *	    COPYRIGHT 1990 DIGITAL EQUIPMENT CORPORATION MAYNARD, MASSACHUSETTS
 *	  ALL RIGHTS RESERVED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
 * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
 * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE
 * FOR ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED
 * WARRANTY.
 *
 * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT
 * RIGHTS, APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN
 * ADDITION TO THAT SET FORTH ABOVE.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Digital Equipment Corporation not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 *	Modified: 4 Dec 91 - Paul King (king@cs.uq.oz.au)
 */

/* From the C library. */

char	       *re_comp();

/* Useful constants. */

#define EOS	'\0'		/* End-of-string. */

#define NENTRIES	100	/* chunk size for allocating filename space */

/* Useful macros. */

#define streq(a, b)	(! strcmp((a), (b)))

extern Widget	popup_dir_text;

/* Xdir function declarations. */


#ifdef __STDC__
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		paramlist
#endif
#else	
#ifndef  __PARAMS
#define  __PARAMS(paramlist)		()
#endif
#endif

extern void FileSelected  __PARAMS((Widget w, XtPointer client_data, XtPointer ret_val));  
extern void DirSelected  __PARAMS((Widget w, XtPointer client_data, XtPointer ret_val));  
extern void GoHome  __PARAMS((Widget w, XtPointer client_data, XtPointer ret_val));  
extern void GoScilab  __PARAMS((Widget w, XtPointer client_data, XtPointer ret_val));  
extern void SetDir  __PARAMS((Widget widget, XEvent *event, String *params, Cardinal *num_params));  
extern void create_dirinfo  __PARAMS((Widget parent, Widget below, Widget *ret_beside, Widget *ret_below, Widget *mask_w, Widget *dir_w, Widget *flist_w, Widget *dlist_w));  
extern Boolean MakeFileList  __PARAMS((char *dir_name, char *mask, char ***dir_list1, char ***file_list1));  
extern void DoChangeDir  __PARAMS((char *dir));  
extern void CallbackRescan  __PARAMS((Widget widget, XtPointer closure, XtPointer call_data));  
extern void Rescan  __PARAMS((Widget widget, XEvent *event, String *params, Cardinal *num_params));  
extern void NewList  __PARAMS((Widget listwidget, String *list));  
extern char *SaveString  __PARAMS((char *string));  
extern Boolean IsDirectory  __PARAMS((char *root, char *path));  
extern void MakeFullPath  __PARAMS((char *root, char *filename, char *pathname));  
extern int wild_match  __PARAMS((char *string, char *pattern));  
