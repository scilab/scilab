/*
 *	$XConsortium: input.c,v 1.16 91/05/10 16:57:16 gildea Exp $
 */

/*
 * Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts.
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Digital Equipment
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior permission.
 *
 *
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/* input.c */

#include "x_ptyxP.h"		/* gets Xt headers, too */

#include "../machine.h"
#include "All-extern-x.h"

#include <X11/keysym.h>
#include <X11/DECkeysym.h>
#include <X11/Xutil.h>
#include <stdio.h>

extern Boolean sunFunctionKeys;

static XComposeStatus compose_status = {NULL, 0};
static char *kypd_num = " XXXXXXXX\tXXX\rXXXxxxxXXXXXXXXXXXXXXXXXXXXX*+,-./0123456789XXX=";
static char *kypd_apl = " ABCDEFGHIJKLMNOPQRSTUVWXYZ??????abcdefghijklmnopqrstuvwxyzXXX";
static char *cur = "DACB";

static void AdjustAfterInput  __PARAMS((register TScreen *screen));  
static int funcvalue  __PARAMS((int keycode));  
static int sunfuncvalue  __PARAMS((int keycode));  

static void
AdjustAfterInput (screen)
register TScreen *screen;
{
	if(screen->scrollkey && screen->topline != 0)
		WindowScroll(screen, 0);
	if(screen->marginbell) {
		int col = screen->max_col - screen->nmarginbell;
		if(screen->bellarmed >= 0) {
			if(screen->bellarmed == screen->cur_row) {
			    if(screen->cur_col >= col) {
				Bell();
				screen->bellarmed = -1;
			    }
			} else
			    screen->bellarmed =
				screen->cur_col < col ? screen->cur_row : -1;
		} else if(screen->cur_col < col)
			screen->bellarmed = screen->cur_row;
	}
}

void Input (keyboard, screen, event, eightbit)
    register TKeyboard	*keyboard;
    register TScreen	*screen;
    register XKeyEvent *event;
    Bool eightbit;
{

#define STRBUFSIZE 100

	char strbuf[STRBUFSIZE];
	register char *string;
	register int key = FALSE;
	int	scr = screen->respond;
	int	nbytes;
	KeySym  keysym;
	ANSI	reply;
	char	tmp[STRBUFSIZE];
	char	*bp = strbuf;
	int	count;
	XEvent	ev;

	count = XLookupString (event, tmp, STRBUFSIZE,
			       &keysym, &compose_status);
	strncpy(bp, tmp, count);
	bp += count;
	nbytes = count;
	/*
	 * Make sure we get every keystroke available
	 * before we start doing the update, so can issue a single
	 * DrawString instead of several.
	 */
	while (nbytes < STRBUFSIZE && 
	       XCheckMaskEvent(screen->display, KeyPressMask, &ev))	{
	    count = XLookupString ((XKeyEvent *)&ev, tmp, STRBUFSIZE,
				   &keysym, &compose_status);
	    if (nbytes + count <= STRBUFSIZE) {
		strncpy(bp, tmp, count);
		bp += count;
		nbytes += count;
	    } else {
		XPutBackEvent(screen->display, &ev);
		break;
	    }
	}

	string = &strbuf[0];
	reply.a_pintro = 0;
	reply.a_final = 0;
	reply.a_nparam = 0;
	reply.a_inters = 0;

	if (IsPFKey(keysym)) {
		reply.a_type = SS3;
		unparseseq(&reply, scr);
		unparseputc((char)(keysym-XK_KP_F1+'P'), scr);
		key = TRUE;
	} else if (IsKeypadKey(keysym)) {
	  	if (keyboard->flags & KYPD_APL)	{
			reply.a_type   = SS3;
			unparseseq(&reply, scr);
			unparseputc(kypd_apl[keysym-XK_KP_Space], scr);
		} else
			unparseputc(kypd_num[keysym-XK_KP_Space], scr);
		key = TRUE;
        } else if (IsCursorKey(keysym) &&
        	keysym != XK_Prior && keysym != XK_Next) {
       		if (keyboard->flags & CURSOR_APL) {
			reply.a_type = SS3;
			unparseseq(&reply, scr);
			unparseputc(cur[keysym-XK_Left], scr);
		} else {
			reply.a_type = CSI;
			reply.a_final = cur[keysym-XK_Left];
			unparseseq(&reply, scr);
		}
		key = TRUE;
	 } else if (IsFunctionKey(keysym) || IsMiscFunctionKey(keysym) ||
	 	keysym == XK_Prior || keysym == XK_Next ||
	 	keysym == DXK_Remove) {
		reply.a_type = CSI;
		reply.a_nparam = 1;
		if (sunFunctionKeys) {
		    reply.a_param[0] = sunfuncvalue ((int) keysym);
		    reply.a_final = 'z';
		} else {
		    reply.a_param[0] = funcvalue ((int) keysym);
		    reply.a_final = '~';
		}
		if (reply.a_param[0] > 0)
			unparseseq(&reply, scr);
		key = TRUE;
	} else if (nbytes > 0) {
		if ((nbytes == 1) && eightbit) {
		    if (screen->input_eight_bits)
		      *string |= 0x80;	/* turn on eighth bit */
		    else
		      unparseputc (033, scr);  /* escape */
		}
		while (nbytes-- > 0)
			unparseputc(*string++, scr);
		key = TRUE;
	}
	if(key ) AdjustAfterInput(screen);
	return;
      }

void StringInput (screen, string, nbytes)
    register TScreen	*screen;
    register char *string;
    int nbytes;
{
  while (nbytes-- > 0)
    unparseputc(*string++, screen->respond);
  AdjustAfterInput(screen);
}

static int funcvalue (keycode)
	int keycode;
{
  switch (keycode) {
  case XK_F1:	return(11);
  case XK_F2:	return(12);
  case XK_F3:	return(13);
  case XK_F4:	return(14);
  case XK_F5:	return(15);
  case XK_F6:	return(17);
  case XK_F7:	return(18);
  case XK_F8:	return(19);
  case XK_F9:	return(20);
  case XK_F10:	return(21);
  case XK_F11:	return(23);
  case XK_F12:	return(24);
  case XK_F13:	return(25);
  case XK_F14:	return(26);
  case XK_F15:	return(28);
  case XK_Help:	return(28);
  case XK_F16:	return(29);
  case XK_Menu:	return(29);
  case XK_F17:	return(31);
  case XK_F18:	return(32);
  case XK_F19:	return(33);
  case XK_F20:	return(34);
    
  case XK_Find :	return(1);
  case XK_Insert:	return(2);
  case XK_Delete:	return(3);
  case DXK_Remove: return(3);
  case XK_Select:	return(4);
  case XK_Prior:	return(5);
  case XK_Next:	return(6);
  default:	return(-1);
  }
}


static int sunfuncvalue (keycode)
	int keycode;
  {
  	switch (keycode) {
		case XK_F1:	return(224);
		case XK_F2:	return(225);
		case XK_F3:	return(226);
		case XK_F4:	return(227);
		case XK_F5:	return(228);
		case XK_F6:	return(229);
		case XK_F7:	return(230);
		case XK_F8:	return(231);
		case XK_F9:	return(232);
		case XK_F10:	return(233);
		case XK_F11:	return(192);
		case XK_F12:	return(193);
		case XK_F13:	return(194);
		case XK_F14:	return(195);
		case XK_F15:	return(196);
		case XK_Help:	return(196);
		case XK_F16:	return(197);
		case XK_Menu:	return(197);
		case XK_F17:	return(198);
		case XK_F18:	return(199);
		case XK_F19:	return(200);
		case XK_F20:	return(201);

		case XK_R1:	return(208);
		case XK_R2:	return(209);
		case XK_R3:	return(210);
		case XK_R4:	return(211);
		case XK_R5:	return(212);
		case XK_R6:	return(213);
		case XK_R7:	return(214);
		case XK_R8:	return(215);
		case XK_R9:	return(216);
		case XK_R10:	return(217);
		case XK_R11:	return(218);
		case XK_R12:	return(219);
		case XK_R13:	return(220);
		case XK_R14:	return(221);
		case XK_R15:	return(222);
  
		case XK_Find :	return(1);
		case XK_Insert:	return(2);
		case XK_Delete:	return(3);
		case DXK_Remove: return(3);
		case XK_Select:	return(4);
		case XK_Prior:	return(5);
		case XK_Next:	return(6);
		default:	return(-1);
	}
}
