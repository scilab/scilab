#mac (or, more recently, pc) style sbindings


#delete "native" text sbindings in the textarea -- we want all text
# manipulation to pass through our procs, so that e.g. colorization and
# edit while debug are correctly treated
sbind Text <Control-h> {backspacetext}
sbind Text <Control-k> {}
sbind Text <Control-t> {}
sbind Text <Control-i> {}
# remove the default sbind ctrl-d=delete char
sbind Text <Control-d> ""
sbind Text <KeyPress>  {if {{%A} != {{}}} {puttext %W %A}}

sbind $pad <Control-o>     {showopenwin currenttile}
sbind $pad <Control-Key-4> {showopenwin horizontal}
sbind $pad <Control-Key-5> {showopenwin vertical}

sbind Text <Control-z> {undo [gettextareacur]}
sbind Text <Control-Z> {redo [gettextareacur]}


sbind $pad <Control-f> {findtextdialog find}
sbind $pad <Control-r> {findtextdialog replace}
sbind $pad <F3> {findnext}

sbind Text <Control-x> {cuttext}
sbind $pad <Control-c> {copytext}
sbind Text <Control-v> {pastetext}
sbind Text <Delete>    {deletetext}

#cut-copy-paste for entries
sbind Entry <Control-x> [bind Entry <<Cut>>]
sbind Entry <Control-c> [bind Entry <<Copy>>]
sbind Entry <Control-v> [bind Entry <<Paste>>]

sbind Text <Control-slash> ""
sbind $pad <Control-a> {selectall}


sbind $pad <Control-w> {closecur yesnocancel} 
sbind $pad <Control-n> {filesetasnew}
sbind $pad <Control-q> {idleexitapp}
sbind $pad <Control-g> {gotoline}
sbind $pad <Control-p> {selectprint [gettextareacur]}
sbind $pad <Control-P> {printsetup}
sbind $pad <Control-s> {filetosavecur}
sbind $pad <Control-S> {filesaveascur}

sbind $pad <Control-m> {CommentSel}
sbind $pad <Control-M> {UnCommentSel}

sbind $pad <Control-l> {execfile}
sbind $pad <Control-y> {execselection}

sbind Text <Button-3> {showpopup2}
sbind Text <Shift-Button-3> {showpopup3}
sbind Text <Control-Button-3> {showpopupfont}

sbind $pad <Control-B> {showwhichfun}

sbind $pad <F5> {filetosavecur; execfile}
sbind $pad <F2> {filetosavecur}

sbind $pad <F1> {helpme}
sbind $pad <Control-F1> {helpword}

sbind $pad <Control-d> {IndentSel}
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
