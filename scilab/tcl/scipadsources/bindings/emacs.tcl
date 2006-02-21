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
# also particulatly for Text so that the specific long binding overrides 
# that to puttext; the second binding needs to end with break to prevent
# double firing, the first not, so that the accelerator lookup finds it
#(that seems to be the easiest way to get out of the prefix troubles)


sbind Text <KeyPress>  {if {{%A} != {{}}} {puttext %W %A}}



#delete "native" text sbindings in the textarea -- we want all text
# manipulation to pass through our procs, so that e.g. colorization and
# edit while debug are correctly treated
#sbind Text <Control-h> {backspacetext}
sbind Text <Control-k> {}
sbind Text <Control-t> {}
sbind Text <Control-i> {}


sbind $pad <Control-x><Control-f> {showopenwin currenttile}
sbind $pad <Control-Key-4> {showopenwin horizontal}
sbind $pad <Control-Key-5> {showopenwin vertical}

sbind Text <Control-underscore> {undo [gettextareacur]}
sbind Text <Control-Z> {redo [gettextareacur]}

#sbind $pad <Control-f> {findtextdialog find}
sbind $pad <Alt-percent> {findtextdialog replace}
sbind $pad <F3> {findnext}

sbind Text <Control-w> {cuttext}
sbind $pad <Alt-w>     {copytext}
sbind Text <Control-y> {pastetext}
sbind Text <Control-d> {deletetext}

#cut-copy-paste for entries
sbind Entry <Control-w> [bind Entry <<Cut>>]
sbind Entry <Alt-w> [bind Entry <<Copy>>]
sbind Entry <Control-y> [bind Entry <<Paste>>]

sbind Text <Control-slash> ""
sbind Text <Control-x><h> {selectall}


sbind $pad <Control-x><Key-k> {closecur yesnocancel}
sbind Text <Control-x><Key-k> {closecur yesnocancel; break}
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

sbind $pad <F7> {nextbuffer}
sbind $pad <F6> {prevbuffer}
sbind $pad <Control-F7> {nextbuffer}
sbind $pad <Control-F6> {prevbuffer}

sbind $pad <Control-plus> {set FontSize [expr round($FontSize*1.11)]; \
                            setfontscipad $FontSize}
sbind $pad <Control-minus> {set FontSize [expr round($FontSize*0.9)]; \
                            setfontscipad $FontSize}

sbind $pad <F4> {importmatlab}

sbind $pad <Control-R> {revertsaved [gettextareacur]}

sbind Text <Control-slash> {openlibfunsource [[gettextareacur] index insert]}


## not exactly emacs recenter:
sbind Text <Control-l> {[gettextareacur] see insert}

#emacs pure movements: duplicate movement keys, hopefully more robust
sbind Text <Control-v> [bind Text <Next>]
sbind Text <Alt-v>     [bind Text <Prior>]
sbind All <Control-a>  [bind Text <Home>]
sbind All <Control-e>  [bind Text <End>]
sbind All <Control-p>  [bind Text <Up>]
sbind All <Control-n>  [bind Text <Down>]
sbind All <Control-b>  [bind Text <Left>]
sbind All <Control-f>  [bind Text <Right>]
sbind All <Alt-b>      [bind Text <Control-Left>]
sbind All <Alt-f>      [bind Text <Control-Right>]
sbind Text <Alt-less>  [bind Text <Control-Home>]
sbind Text <Alt-greater> [bind Text <Control-End>]
