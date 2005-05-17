#bindings
bind All <Alt-F> {}
bind All <Alt-E> {}
bind All <Alt-S> {}
bind ALL <Alt-H> {}

bind Text <KeyPress> { if {{%A} != {{}}} {puttext %W %A}}
bind Text <Delete> { deletetext}
bind Text <BackSpace> { backspacetext}
bind Text <Return> {insertnewline %W}
bind Text <Tab> {inserttab %W}

bind Text <parenright> { if {{%A} != {{}}} {insblinkbrace %W %A}}
bind Text <bracketright> { if {{%A} != {{}}} {insblinkbrace %W %A}} 
bind Text <braceright>  { if {{%A} != {{}}} {insblinkbrace %W %A}}

bind Text <Control-o> {}
bind $pad <Control-o> {showopenwin}

bind $pad <Control-z> {undo_menu_proc}
bind $pad <Control-Z> {redo_menu_proc}

bind Text <Control-f> {}
bind $pad <Control-f> {findtext find}
bind $pad <Control-r> {findtext replace}
bind $pad <F3> {findnext find}

bind Text <Control-x> {cuttext}
bind $pad <Control-c> {copytext}
bind Text <Control-v> {}
bind Text <Control-v> {pastetext}
event delete <<Paste>> <Button-2>
bind Text <Button-2> {button2copypaste %W %x %y}

#Added catch {} is here to avoid error popup message in case
#listoffile("$textarea",language) has already been unset in proc byebye which
#is called on ctrl-w to close the current buffer
bind $textareacur <KeyRelease> {catch {keyposn %W}}
bind $textareacur <ButtonRelease> {catch {keyposn %W}}

event delete <<Cut>> <Control-w>
bind Text <Control-w> {} 
bind $pad <Control-w> {closecur yesnocancel} 
bind $pad <Control-n> {filesetasnew}
bind $pad <Control-q> {exitapp yesnocancel}
bind $pad <Control-g> {gotoline}
if {"$tcl_platform(platform)" == "unix"} {
    bind $pad <Control-p> {selectprint %W}
    bind $pad <Control-P> {printseupselection}
}
bind $pad <Control-s> {filetosave %W}
bind $pad <Control-S> {filesaveascur}

bind $pad <Control-m> {CommentSel}
bind $pad <Control-M> {UnCommentSel}

bind $pad <Control-l> {execfile}
event delete <<Paste>> <Control-y>
event delete <<Paste>> <Control-Key-y>
bind $pad <Control-y> {execselection}

bind Text <Button-3> {showpopup2}
bind Text <Shift-Button-3> {showpopup3}
bind Text <Control-Button-3> {showpopupfont}

bind $pad <Control-B> {showwhichfun}

bind $pad <F5> {filetosave %W; execfile}
bind $pad <F2> {filetosave %W}

bind $pad <F1> {helpme}
bind $pad <Control-F1> {helpword}

# remove the default bind ctrl-d=delete char
bind Text <Control-d> ""
bind $pad <Control-d> {IndentSel}
bind $pad <Control-D> {UnIndentSel}

bind $pad <F7> {nextbuffer}
bind $pad <F6> {prevbuffer}
bind $pad <Control-F7> {nextbuffer}
bind $pad <Control-F6> {prevbuffer}

bind $pad <Control-plus> {set FontSize [expr round($FontSize*1.11)]; \
                            setfontscipad $FontSize}
bind $pad <Control-minus> {set FontSize [expr round($FontSize*0.9)]; \
                            setfontscipad $FontSize}

bind $pad <F4> {importmatlab}

bind $pad <Control-R> {revertsaved}

bind Text <Control-/> ""
bind Text <Control-a> ""
bind $pad <Control-a> {selectall}

bind Text <Control-slash> {set ind [%W index insert]; openlibfunsource $ind}
bind Text <Shift-Control-Button-1> {set ind [%W index current]; showpopupsource $ind}

# The following are (unfortunately) platform/os-dependent keysyms
if {"$tcl_platform(platform)" == "unix"} {
    switch $tcl_platform(os) {
        Linux {
            # XF86 4.3 modmap has the following keysyms
            # these work at least with Red Hat 9 and Mandrake 10.1
            set Shift_Tab   "ISO_Left_Tab"
            set Shift_F1    "XF86_Switch_VT_1"
            set Shift_F8    "XF86_Switch_VT_8"
            set Shift_F11   "XF86_Switch_VT_11"
            set Shift_F12   "XF86_Switch_VT_12"
        }
        SunOS {
            # Solaris 2.6 on SunOS 5.8 or 5.9
            set Shift_Tab   "Shift-Tab"
            set Shift_F1    "Shift-F1"
            set Shift_F8    "Shift-F8"
            set Shift_F11   "Shift-SunF36"
            set Shift_F12   "Shift-SunF37"
        }
        default {
            # untested for other unices (HP-UX, IRIX64...)
            set Shift_Tab   "Shift-Tab"
            set Shift_F1    "Shift-F1"
            set Shift_F8    "Shift-F8"
            set Shift_F11   "Shift-F11"
            set Shift_F12   "Shift-F12"
        }
    }
} else {
    # $tcl_platform(platform) should be "windows" (tested on XP)
    # or "macintosh" (untested)
    set Shift_Tab   "Shift-Tab"
    set Shift_F1    "Shift-F1"
    set Shift_F8    "Shift-F8"
    set Shift_F11   "Shift-F11"
    set Shift_F12   "Shift-F12"
}
bind Text <$Shift_Tab> {UnIndentSel}
bind $pad <$Shift_F1> {aboutme}
