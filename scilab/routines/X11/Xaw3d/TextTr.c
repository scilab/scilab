/* $XConsortium: TextTr.c,v 1.20 95/06/14 15:07:27 kaleb Exp $ */

/*

Copyright (c) 1991, 1994  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

*/

/* INTERNATIONALIZATION:

The OMRON R5 contrib added the following action to the old TextTr:

	Ctrl<Key>backslash:	reconnect-im()

This is needed when the im is killed or otherwise becomes unreachable.
This keystroke is evil (inconvenient, hard-to-remember, not obvious)
so I am adding one more translation:

	<Key>Kanji:		reconnect-im()

The Japanese user typically hits their Kanji key when they want to do
input.  This merely makes sure the input is connected. 
*/

char *_XawDefaultTextTranslations1 =
"\
Ctrl<Key>A:	beginning-of-line() \n\
Ctrl<Key>B:	backward-character() \n\
Ctrl<Key>C:	insert-selection(CUT_BUFFER0) \n\
Ctrl<Key>D:	delete-next-character() \n\
Ctrl<Key>E:	end-of-line() \n\
Ctrl<Key>F:	forward-character() \n\
Ctrl<Key>G:     multiply(Reset) \n\
Ctrl<Key>H:	delete-previous-character() \n\
Ctrl<Key>J:	newline-and-indent() \n\
Ctrl<Key>K:	kill-to-end-of-line() \n\
Ctrl<Key>L:	redraw-display() \n\
Ctrl<Key>M:	newline() \n\
Ctrl<Key>N:	next-line() \n\
Ctrl<Key>O:	newline-and-backup() \n\
Ctrl<Key>P:	previous-line() \n\
Ctrl<Key>R:	search(backward) \n\
Ctrl<Key>S:	search(forward) \n\
Ctrl<Key>T:     transpose-characters() \n\
Ctrl<Key>U:	multiply(4) \n\
Ctrl<Key>V:	next-page() \n\
Ctrl<Key>W:	kill-selection() \n\
Ctrl<Key>Y:	insert-selection(SECONDARY) \n\
Ctrl<Key>Z:	scroll-one-line-up() \n\
", *_XawDefaultTextTranslations2 = "\
Meta<Key>B:	backward-word() \n\
Meta<Key>F:	forward-word() \n\
Meta<Key>I:	insert-file() \n\
Meta<Key>K:	kill-to-end-of-paragraph() \n\
Meta<Key>Q:     form-paragraph() \n\
Meta<Key>V:	previous-page() \n\
Meta<Key>Y:	insert-selection(PRIMARY, CUT_BUFFER0) \n\
Meta<Key>Z:	scroll-one-line-down() \n\
:Meta<Key>d:	delete-next-word() \n\
:Meta<Key>D:	kill-word() \n\
:Meta<Key>h:	delete-previous-word() \n\
:Meta<Key>H:	backward-kill-word() \n\
:Meta<Key>\\<:	beginning-of-file() \n\
:Meta<Key>\\>:	end-of-file() \n\
:Meta<Key>]:	forward-paragraph() \n\
:Meta<Key>[:	backward-paragraph() \n\
~Shift Meta<Key>Delete:		delete-previous-word() \n\
 Shift Meta<Key>Delete:		backward-kill-word() \n\
~Shift Meta<Key>BackSpace:	delete-previous-word() \n\
 Shift Meta<Key>BackSpace:	backward-kill-word() \n\
", *_XawDefaultTextTranslations3 = "\
<Key>Home:	beginning-of-file() \n\
:<Key>KP_Home:	beginning-of-file() \n\
<Key>End:	end-of-file() \n\
:<Key>KP_End:	end-of-file() \n\
<Key>Next:	next-page() \n\
:<Key>KP_Next:	next-page() \n\
<Key>Prior:	previous-page() \n\
:<Key>KP_Prior: previous-page() \n\
<Key>Right:	forward-character() \n\
:<Key>KP_Right: forward-character() \n\
<Key>Left:	backward-character() \n\
:<Key>KP_Left:	backward-character() \n\
<Key>Down:	next-line() \n\
:<Key>KP_Down:	next-line() \n\
<Key>Up:	previous-line() \n\
:<Key>KP_Up:	previous-line() \n\
<Key>Delete:	delete-previous-character() \n\
:<Key>KP_Delete: delete-previous-character() \n\
<Key>BackSpace:	delete-previous-character() \n\
<Key>Linefeed:	newline-and-indent() \n\
<Key>Return:	newline() \n\
:<Key>KP_Enter:	newline() \n\
Ctrl<Key>backslash:	reconnect-im() \n\
<Key>Kanji:	reconnect-im()\n\
<Key>:		insert-char() \n\
", *_XawDefaultTextTranslations4 = "\
<EnterWindow>:	enter-window() \n\
<LeaveWindow>:	leave-window() \n\
<FocusIn>:	focus-in() \n\
<FocusOut>:	focus-out() \n\
<Btn1Down>:	select-start() \n\
<Btn1Motion>:	extend-adjust() \n\
<Btn1Up>:	extend-end(PRIMARY, CUT_BUFFER0) \n\
<Btn2Down>:	insert-selection(PRIMARY, CUT_BUFFER0) \n\
<Btn3Down>:	extend-start() \n\
<Btn3Motion>:	extend-adjust() \n\
<Btn3Up>:	extend-end(PRIMARY, CUT_BUFFER0) \
";
