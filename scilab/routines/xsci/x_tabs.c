/*
 *	$XConsortium: tabs.c,v 1.4 91/05/06 17:12:18 gildea Exp $
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

/* tabs.c */

#include "x_ptyxP.h"

#include "../machine.h"
#include "All-extern-x.h"

/*
 * This file presumes 32bits/word.  This is somewhat of a crock, and should
 * be fixed sometime.
 */

/*
 * places tabstops at only every 8 columns
 */

void TabReset(tabs)
Tabs	tabs;
{
	register int i;

	for (i=0; i<TAB_ARRAY_SIZE; ++i)
		tabs[i] = 0;

	for (i=0; i<MAX_TABS; i+=8)
		TabSet(tabs, i);
}	


/*
 * places a tabstop at col
 */

void TabSet(tabs, col)
    Tabs	tabs;
    int		col;
{
	tabs[col >> 5] |= (1 << (col & 31));
}

/*
 * clears a tabstop at col
 */
void TabClear(tabs, col)
    Tabs	tabs;
    int		col;
{
	tabs[col >> 5] &= ~(1 << (col & 31));
}

/*
 * returns the column of the next tabstop
 * (or MAX_TABS - 1 if there are no more).
 * A tabstop at col is ignored.
 */
int TabNext (tabs, col)
    Tabs	tabs;
    int		col;
{
	extern XtermWidget term;
	register TScreen *screen = &term->screen;

	if(screen->curses && screen->do_wrap && (term->flags & WRAPAROUND)) {
		Index(screen, 1);
		col = screen->cur_col = screen->do_wrap = 0;
	}
	for (++col; col<MAX_TABS; ++col)
		if (tabs[col >> 5] & (1 << (col & 31)))
			return (col);

	return (MAX_TABS - 1);
}

/*
 * clears all tabs
 */

void TabZonk (tabs)
Tabs	tabs;
{
	register int i;

	for (i=0; i<TAB_ARRAY_SIZE; ++i)
		tabs[i] = 0;
}
