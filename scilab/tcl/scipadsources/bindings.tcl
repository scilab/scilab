#bindings
bind All <Alt-F> {}
bind All <Alt-E> {}
bind All <Alt-S> {}
bind ALL <Alt-H> {}
bind $pad <F3> {findnext find}
bind $pad <Control-x> {cuttext}
bind $pad <Control-c> {copytext}
bind $pad <Control-s> {filetosave %W}
bind Text <Control-o> {}
bind Text <Control-f> {}


#Added by Matthieu PHILIPPE 21/11/2001
bind Text <KeyPress> { if {{%A} != {{}}} {puttext %W %A}}
bind Text <Delete> { deletetext}
bind Text <BackSpace> { backspacetext}
bind Text <Return> {insertnewline %W}

bind Text <parenright> { if {{%A} != {{}}} {insblinkbrace %W %A}}
bind Text <bracketright> { if {{%A} != {{}}} {insblinkbrace %W %A}} 
bind Text <braceright>  { if {{%A} != {{}}} {insblinkbrace %W %A}}
#######

bind $pad <Control-o> {filetoopen %W}
bind $pad <Control-z> {undo_menu_proc}
bind $pad <Control-Z> {redo_menu_proc}
bind $pad <Control-f> {findtext find}
bind $pad <Control-r> {findtext replace}
##ES: I do not know why the following. Commenting it I get at once standard
### behaviors: double click=word select, triple click line select, 
### 2nd button click copy.
##bind $pad <Button> {selection clear}
# because windows is 'different' and mac is unknown
# if [ expr [string compare $tcl_platform(platform) "unix"] ==0] {
# 	#events
# 	set tk_strictMotif 0
# 	event delete <<Cut>> <Control-x>
# 	event delete <<Paste>> <Control-v>
#         event delete <<Paste>> <Control-Key-y>
# 	# more bindings
# 	bind Text <Control-v> {}
# 	bind All <Control-v> {pastetext}
#     } else {
# 	#events
# 	set tk_strictMotif 0
# 	event delete <<Cut>> <Control-x>
# 	event delete <<Paste>> <Control-v>
#         event delete <<Paste>> <Control-Key-y>
# 	# more bindings
# 	bind Text <Control-v> {}
# 	bind All <Control-v> {pastetext}
#     }

bind Text <Control-v> {}
bind Text <Control-v> {pastetext}
event delete <<Paste>> <Button-2>
bind Text <Button-2> {button2copypaste %W %x %y}

#Francois VOGEL, 17/10/04 - Added catch {} to avoid error popup message in case
#listoffile("$textarea",language) has already been unset in proc byebye which
#is called on ctrl-w to close the current buffer
bind $textareacur <KeyRelease> {catch {keyposn %W}}
bind $textareacur <ButtonRelease> {catch {keyposn %W}}

###### added by ES 24/7/2003
event delete <<Cut>> <Control-w>
bind Text <Control-w> {} 
bind $pad <Control-w> {closecur} 
bind $pad <Control-n> {filesetasnewmat}
bind $pad <Control-q> {exitapp}
bind $pad <Control-g> {gotoline}
bind $pad <Control-p> {selectprint $textareacur}
bind $pad <Control-P> {printseupselection}
bind $pad <Control-S> {filesaveascur}

##ES 17/9/03
bind $pad <Control-m> {CommentSel}
bind $pad <Control-M> {UnCommentSel}
##ES 22/9/03
bind $pad <Control-l> {execfile}
event delete <<Paste>> <Control-y>
event delete <<Paste>> <Control-Key-y>
bind $pad <Control-y> {execselection}
bind $pad <F1> {helpme}
bind Text <Button-3> {showpopup2}
#ES 30/9/03
bind Text <Shift-Button-3> {showpopup3}
bind Text <Control-Button-3> {showpopupfont}
bind $pad <Control-B> {showwhichfun}

#ES 16/10/03
bind $pad <F5> {filetosave %W; execfile}

bind $pad <F2> {filetosave %W}
bind $pad <Control-F1> {helpword}
bind $pad <Shift-F1> {aboutme}

#ES 25/12/03
# remove the default bind ctrl-d=delete char
bind Text <Control-d> ""
bind $pad <Control-d> {IndentSel}
bind $pad <Control-D> {UnIndentSel}

# Francois VOGEL 16/04/04, ES 19/4/04, FV 21/04/04
bind $pad <F7> {nextbuffer}
bind $pad <F6> {prevbuffer}
bind $pad <Control-F7> {nextbuffer}
bind $pad <Control-F6> {prevbuffer}

bind $pad <Control-plus> {set FontSize [expr round($FontSize*1.11)]; \
                             setfontscipad $FontSize}
bind $pad <Control-minus> {set FontSize [expr round($FontSize*0.9)]; \
                              setfontscipad $FontSize}

bind $pad <F4> {importmatlab}

#ES 10/11/04
bind $pad <Control-R> {revertsaved}

bind Text <Control-a> ""
bind $pad <Control-a> {selectall}

bind Text <F8> {set ind [%W index insert]; openlibfunsource $ind}
bind Text <Control-Button-1> {set ind [%W index current]; showpopupsource $ind}
