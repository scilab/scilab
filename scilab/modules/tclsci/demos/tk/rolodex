# rolodex --
# This script was written as an entry in Tom LaStrange's rolodex
# benchmark.  It creates something that has some of the look and
# feel of a rolodex program, although it's lifeless and doesn't
# actually do the rolodex application.
#
# SCCS: @(#) rolodex 1.7 96/02/16 10:49:23

foreach i [winfo child .top] {
    catch {destroy $i}
}

#------------------------------------------
# Phase 0: create the front end.
#------------------------------------------

frame .top.frame -relief flat
pack .top.frame -side top -fill y -anchor center

set names {{} Name: Address: {} {} {Home Phone:} {Work Phone:} Fax:}
foreach i {1 2 3 4 5 6 7} {
    frame .top.frame.$i
    pack .top.frame.$i -side top -pady 2 -anchor e

    label .top.frame.$i.label -text [lindex $names $i] -anchor e
    entry .top.frame.$i.entry -width 30 -relief sunken
    pack .top.frame.$i.entry .top.frame.$i.label -side right
}

frame .top.buttons
pack .top.buttons -side bottom -pady 2 -anchor center
button .top.buttons.clear -text Clear
button .top.buttons.add -text Add
button .top.buttons.search -text Search
button .top.buttons.delete -text "Delete ..."
pack .top.buttons.clear .top.buttons.add .top.buttons.search .top.buttons.delete \
	-side left -padx 2

#------------------------------------------
# Phase 1: Add menus, dialog boxes
#------------------------------------------

frame .top.menu -relief raised -borderwidth 1
pack .top.menu -before .top.frame -side top -fill x

menubutton .top.menu.file -text "File" -menu .top.menu.file.m -underline 0
menu .top.menu.file.m
.top.menu.file.m add command -label "Load ..." -command fileAction -underline 0
.top.menu.file.m add command -label "Exit" -command {destroy .top} -underline 0
pack .top.menu.file -side left

menubutton .top.menu.help -text "Help" -menu .top.menu.help.m -underline 0
menu .top.menu.help.m
pack .top.menu.help -side right

proc deleteAction {} {
    if {[tk_dialog .top.delete {Confirm Action} {Are you sure?} {} 0  Cancel]
	    == 0} {
	clearAction
    }
}
.buttons.delete config -command deleteAction

proc fileAction {} {
    tk_dialog .top.fileSelection {File Selection} {This is a dummy file selection dialog box, which is used because there isn't a good file selection dialog built into Tk yet.} {} 0 OK
    puts stderr {dummy file name}
}

#------------------------------------------
# Phase 3: Print contents of card
#------------------------------------------

proc addAction {} {
    global names
    foreach i {1 2 3 4 5 6 7} {
	puts stderr [format "%-12s %s" [lindex $names $i] [.top.frame.$i.entry get]]
    }
}
.top.buttons.add config -command addAction

#------------------------------------------
# Phase 4: Miscellaneous other actions
#------------------------------------------

proc clearAction {} {
    foreach i {1 2 3 4 5 6 7} {
	.top.frame.$i.entry delete 0 end
    }
}
.buttons.clear config -command clearAction

proc fillCard {} {
    clearAction
    .top.frame.1.entry insert 0 "John Ousterhout"
    .top.frame.2.entry insert 0 "CS Division, Department of EECS"
    .top.frame.3.entry insert 0 "University of California"
    .top.frame.4.entry insert 0 "Berkeley, CA 94720"
    .top.frame.5.entry insert 0 "private"
    .top.frame.6.entry insert 0 "510-642-0865"
    .top.frame.7.entry insert 0 "510-642-5775"
}
.top.buttons.search config -command "addAction; fillCard"

#----------------------------------------------------
# Phase 5: Accelerators, mnemonics, command-line info
#----------------------------------------------------

.top.buttons.clear config -text "Clear    Ctrl+C"
bind .top <Control-c> clearAction
.top.buttons.add config -text "Add    Ctrl+A"
bind .top <Control-a> addAction
.top.buttons.search config -text "Search    Ctrl+S"
bind .top <Control-s> "addAction; fillCard"
.top.buttons.delete config -text "Delete...    Ctrl+D"
bind .top <Control-d> deleteAction

.top.menu.file.m entryconfig 1 -accel Ctrl+F
bind .top <Control-f> fileAction
.top.menu.file.m entryconfig 2 -accel Ctrl+Q
bind .top <Control-q> {destroy .top.}

focus .top.frame.1.entry

#----------------------------------------------------
# Phase 6: help
#----------------------------------------------------

proc Help {topic {x 0} {y 0}} {
    global helpTopics helpCmds
    if {$topic == ""} return
    while {[info exists helpCmds($topic)]} {
	set topic [eval $helpCmds($topic)]
    }
    if [info exists helpTopics($topic)] {
	set msg $helpTopics($topic)
    } else {
	set msg "Sorry, but no help is available for this topic"
    }
    tk_dialog .top.help {Rolodex Help} "Information on $topic:\n\n$msg" \
	    {} 0 OK
}

proc getMenuTopic {w x y} {
    return $w.[$w index @[expr $y-[winfo rooty $w]]]
}

bind .top <Any-F1> {Help [winfo containing %X %Y] %X %Y}
bind .top <Any-Help> {Help [winfo containing %X %Y] %X %Y}

# Help text and commands follow:

set helpTopics(.top.menu.file) {This is the "file" menu.  It can be used to invoke some overall operations on the rolodex applications, such as loading a file or exiting.}

set helpCmds(.top.menu.file.m) {getMenuTopic $topic $x $y}
set helpTopics(.top.menu.file.m.0) {The "Load" entry in the "File" menu posts a dialog box that you can use to select a rolodex file}
set helpTopics(.top.menu.file.m.1) {The "Exit" entry in the "File" menu causes the rolodex application to terminate}
set helpCmds(.top.menu.file.m.none) {set topic ".menu.file"}

set helpTopics(.top.frame.1.entry) {In this field of the rolodex entry you should type the person's name}
set helpTopics(.top.frame.2.entry) {In this field of the rolodex entry you should type the first line of the person's address}
set helpTopics(.top.frame.3.entry) {In this field of the rolodex entry you should type the second line of the person's address}
set helpTopics(.top.frame.4.entry) {In this field of the rolodex entry you should type the third line of the person's address}
set helpTopics(.top.frame.5.entry) {In this field of the rolodex entry you should type the person's home phone number, or "private" if the person doesn't want his or her number publicized}
set helpTopics(.top.frame.6.entry) {In this field of the rolodex entry you should type the person's work phone number}
set helpTopics(.top.frame.7.entry) {In this field of the rolodex entry you should type the phone number for the person's FAX machine}

set helpCmds(.top.frame.1.label) {set topic .top.frame.1.entry}
set helpCmds(.top.frame.2.label) {set topic .top.frame.2.entry}
set helpCmds(.top.frame.3.label) {set topic .top.frame.3.entry}
set helpCmds(.top.frame.4.label) {set topic .top.frame.4.entry}
set helpCmds(.top.frame.5.label) {set topic .top.frame.5.entry}
set helpCmds(.top.frame.6.label) {set topic .top.frame.6.entry}
set helpCmds(.top.frame.7.label) {set topic .top.frame.7.entry}

set helpTopics(context) {Unfortunately, this application doesn't support context-sensitive help in the usual way, because when this demo was written Tk didn't have a grab mechanism and this is needed for context-sensitive help.  Instead, you can achieve much the same effect by simply moving the mouse over the window you're curious about and pressing the Help or F1 keys.  You can do this anytime.}
set helpTopics(help) {This application provides only very crude help.  Besides the entries in this menu, you can get help on individual windows by moving the mouse cursor over the window and pressing the Help or F1 keys.}
set helpTopics(window) {This window is a dummy rolodex application created as part of Tom LaStrange's toolkit benchmark.  It doesn't really do anything useful except to demonstrate a few features of the Tk toolkit.}
set helpTopics(keys) "The following accelerator keys are defined for this application (in addition to those already available for the entry windows):\n\nCtrl+A:\t\tAdd\nCtrl+C:\t\tClear\nCtrl+D:\t\tDelete\nCtrl+F:\t\tEnter file name\nCtrl+Q:\t\tExit application (quit)\nCtrl+S:\t\tSearch (dummy operation)"
set helpTopics(version) {This is version 1.0.}

# Entries in "Help" menu

.top.menu.help.m add command -label "On Context..." -command {Help context} \
	-underline 3
.top.menu.help.m add command -label "On Help..." -command {Help help} \
	-underline 3
.top.menu.help.m add command -label "On Window..." -command {Help window} \
	-underline 3
.top.menu.help.m add command -label "On Keys..." -command {Help keys} \
	-underline 3
.top.menu.help.m add command -label "On Version..." -command {Help version}  \
	-underline 3
