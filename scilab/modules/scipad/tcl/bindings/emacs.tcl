#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#
#emacs style bindings

#prefixes: show them in the showinfo and validate the continuation
sbind $pad <Control-x> "+showinfo Ctrl-x-"
sbind $pad <Control-h> "+showinfo Ctrl-h-"

sbind $pad <Control-x><KeyPress> {validpostfix %A %s "hk"; break}
sbind Text <Control-x><KeyPress> {validpostfix %A %s "hk"; break}
sbind $pad <Control-h><KeyPress> {validpostfix %A %s "h"; break}
sbind Text <Control-h><KeyPress> {validpostfix %A %s "h"; break}


sbind $pad <Control-x><Control-KeyPress> {validpostfix %A %s "swfc"; break}
sbind Text <Control-x><Control-KeyPress> {validpostfix %A %s "swfc"; break}
sbind $pad <Control-h><Control-KeyPress> {validpostfix %A %s "f"; break}
sbind Text <Control-h><Control-KeyPress> {validpostfix %A %s "f"; break}


#nb: compound bindings like C-x-k defined for $pad need to be defined 
# also particularly for Text so that the specific long binding overrides 
# that to puttext; the second binding needs to end with break to prevent
# double firing, the first not, so that the accelerator lookup finds it
#(that seems to be the easiest way to get out of the prefix troubles)


#delete "native" text sbindings in the textarea -- we want all text
# manipulation to pass through our procs, so that e.g. colorization and
# edit while debug are correctly treated
#sbind Text <Control-h> {backspacetext}
sbind Text <Control-k> {}
sbind Text <Control-t> {}
sbind Text <Control-i> {}
sbind Text <Control-f> {} ; # avoids selection deletion on find box open


sbind $pad <Control-x><Control-f> {showopenwin currenttile}
sbind $pad <Control-Key-4> {showopenwin horizontal}
sbind $pad <Control-Key-5> {showopenwin vertical}

sbind Text <Control-underscore> {undo [gettextareacur]}
sbind Text <Control-Z> {redo [gettextareacur]}

#sbind $pad <Control-f> {findtextdialog find}
sbind $pad <Alt-percent> {findtextdialog replace}
sbind $pad <F3> {findnext}

sbind Text <Control-w> {cuttext normal}
sbind Text <Control-W> {cuttext block}
sbind $pad <Alt-w>     {copytext}
sbind Text <Control-y> {pastetext normal}
sbind Text <Control-Y> {pastetext block}
sbind Text <Control-d> {deletetext}

#cut-copy-paste for entries
sbind Entry <Control-w> [bind Entry <<Cut>>]
sbind Entry <Alt-w> [bind Entry <<Copy>>]
sbind Entry <Control-y> [bind Entry <<Paste>>]

sbind Text <Control-slash> ""
sbind Text <Control-x><h> {selectall}


sbind $pad <Control-x><Key-k> {closecurfile yesnocancel}
sbind Text <Control-x><Key-k> {closecurfile yesnocancel; break}
#sbind $pad <Control-x><Control-f> {filesetasnew}
sbind $pad <Control-x><Control-c> {idleexitapp}
sbind $pad <Control-g> {gotoline}
#sbind $pad <Control-p> {selectprint [gettextareacur]}
sbind $pad <Control-P> {printsetup}
sbind $pad <Control-x><Control-s> {filetosavecur}
sbind $pad <Control-x><Control-w> {filesaveascur}

sbind $pad <Control-m> {CommentSel}
sbind $pad <Control-M> {UnCommentSel}

#sbind $pad <Control-l> {execfile}

sbind $pad <Alt-exclam> {execselection}

sbind Text <Button-3> {showpopup2}
sbind Text <Shift-Button-3> {showpopup3}
sbind Text <Control-Button-3> {showpopupfont}

#sbind $pad <Control-B> {showwhichfun}

sbind $pad <F5> {filetosavecur; execfile}
sbind $pad <F2> {filetosavecur}

sbind $pad <Control-h><Key-h> {helpme}
sbind Text <Control-h><Key-h> {helpme; break}
sbind $pad <Control-h><Control-f> {helpword}

# remove the default sbind ctrl-d=delete char
#sbind Text <Control-d> ""
sbind $pad <Control-Alt-backslash> {IndentSel}
sbind $pad <Control-D> {UnIndentSel}

sbind $pad <F7> {nextbuffer all}
sbind $pad <F6> {prevbuffer all}
sbind $pad <Control-F7> {nextbuffer visible}
sbind $pad <Control-F6> {prevbuffer visible}

sbind $pad <F4> {importmatlab}

sbind $pad <Control-R> {revertsaved [gettextareacur]}

sbind Text <Control-slash> {openlibfunsource [[gettextareacur] index insert]}


## not exactly emacs recenter:
sbind Text <Control-l> {[gettextareacur] see insert}

#emacs pure movements: duplicate movement keys, hopefully more robust
sbind Text <Control-v> [bind Text <Next>]
sbind Text <Alt-v>     [bind Text <Prior>]
sbind all <Control-a>  [bind Text <Home>]
sbind all <Control-e>  [bind Text <End>]
sbind all <Control-p>  [bind Text <Up>]
sbind all <Control-n>  [bind Text <Down>]
sbind all <Control-b>  [bind Text <Left>]
sbind all <Control-f>  [bind Text <Right>]
sbind all <Alt-b>      [bind Text <Control-Left>]
sbind all <Alt-f>      [bind Text <Control-Right>]
sbind Text <Alt-less>  [bind Text <Control-Home>]
sbind Text <Alt-greater> [bind Text <Control-End>]
