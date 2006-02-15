#bindings
bind All <Alt-F> {}
bind All <Alt-E> {}
bind All <Alt-S> {}
bind ALL <Alt-H> {}

#delete "native" text bindings in the textarea -- we want all text
# manipulation to pass through our procs, so that e.g. colorization and
# edit while debug are correctly treated
#bind Text <Control-h> {backspacetext}
bind Text <Control-k> {}
bind Text <Control-t> {}
bind Text <Control-i> {}

bind Text <KeyPress>  {if {{%A} != {{}}} {puttext %W %A}}
bind Text <BackSpace> {backspacetext}
bind Text <Return>    {insertnewline %W}
# break prevents from triggering the default Tk
# binding: bind all <Key-Tab> tk::TabToWindow [tk_focusNext %W], which
# is harmful when displaying more than one buffer at the same time
# warning: this binding might be erased by the completion binding
# set later in this file, depending on the user preference (completion
# options menu). Moreover, if the binded script changes, proc
# SetCompletionBinding must be updated accordingly
bind Text <Tab>       {inserttab %W ; break}

bind Text <parenright>   {if {{%A} != {{}}} {insblinkbrace %W %A}}
bind Text <bracketright> {if {{%A} != {{}}} {insblinkbrace %W %A}} 
bind Text <braceright>   {if {{%A} != {{}}} {insblinkbrace %W %A}}


bind Text <<Modified>> {changedmodified %W}


bind Text <ButtonRelease-2> {button2copypaste %W %x %y}


#Added catch {} is here to avoid error popup message in case
#listoffile("$textarea",language) has already been unset in proc byebye which
#is called on ctrl-w to close the current buffer
bind $textareacur <KeyRelease> {catch {keyposn %W}}

event delete <<Cut>> <Control-w>
event delete <<Cut>> <Control-x>
event delete <<Cut>> <Control-Key-x>

event delete <<Paste>> <Control-y>
event delete <<Paste>> <Control-Key-y>


bind Text <Button-3> {showpopup2}
bind Text <Shift-Button-3> {showpopup3}
bind Text <Control-Button-3> {showpopupfont}

bind $pad <Control-B> {showwhichfun}




# remove the default bind ctrl-d=delete char
bind Text <Control-d> ""

bind $pad <F7> {nextbuffer}
bind $pad <F6> {prevbuffer}
bind $pad <Control-F7> {nextbuffer}
bind $pad <Control-F6> {prevbuffer}

bind $pad <Control-plus> {set FontSize [expr round($FontSize*1.11)]; \
                            setfontscipad $FontSize}
bind $pad <Control-minus> {set FontSize [expr round($FontSize*0.9)]; \
                            setfontscipad $FontSize}


bind Text <Shift-Control-Button-1> {set ind [%W index current]; showpopupsource $ind}

bind $pad <Control-Key-1> "$pad.filemenu.wind invoke 1"
bind $pad <Control-Key-2> "$pad.filemenu.wind invoke 2"
bind $pad <Control-Key-3> "$pad.filemenu.wind invoke 3"
# For Tk 8.5 and above, the behavior on external resize is driven by
# the option -stretch always
# For Tk before 8.5, proc spaceallsasheskeeprelsizes emulates this option
bind $pad <Configure> {if {"%W"=="$pad"} {
                           if {!$Tk85} {
                               catch {spaceallsasheskeeprelsizes}
                           }
                       }}
bind Panedwindow <Double-Button-1> {spacesashesevenly %W}

bind $pad <FocusIn> {checkifanythingchangedondisk %W}

# The following are (unfortunately) platform/os-dependent keysyms
set Shift_Tab {"ISO_Left_Tab" "Shift-Tab"}
set Shift_F1  {"XF86_Switch_VT_1" "Shift-F1"}
set Shift_F3  {"XF86_Switch_VT_3" "Shift-F3"}
set Shift_F8  {"XF86_Switch_VT_8" "Shift-F8"}
set Shift_F11  {"XF86_Switch_VT_11" "Shift-F11" "Shift-SunF36"}
set Shift_F12  {"XF86_Switch_VT_12" "Shift-F12" "Shift-SunF37"}

# warning: this binding might be erased by the completion binding
# set later in this file, depending on the user preference (completion
# options menu). Moreover, if the binded script changes, proc
# SetCompletionBinding must be updated accordingly
pbind Text $Shift_Tab {UnIndentSel ; break}

pbind $pad $Shift_F1 {aboutme}
pbind $pad $Shift_F3 {reversefindnext}

# the following call sets a binding for the completion popup menu
# it must be sourced after all the possible bindings proposed in the
# options/completion menu that are also present above since it might
# replace bindings previously set e.g. Tab and Shift-Tab
SetCompletionBinding
