#!/bin/sh
# the next line restarts using wish \
# exec `which wish` "$0" "$@"

# Tk NotePad by Joseph Acosta
# modified by Matthieu PHILIPPE 01/01/2003
# Some bug corrected
# - cut/paste with ctrl-x/ctrl-v
# - select then unselect then type any key doesn't crash scipad any more !

# modified by Matthieu PHILIPPE 03/01/2002
# added
# - Number line
# - Goto Line
# - Higlighting Braces
# - Some Keys and Words Binding
# - Colorization for Scilab syntaxes
# - Recolorization of a cuted and pasted text
# - Multiple opened files.

# ES 28/7/2003: 
# changed text font from helvetica bold to courier medium
# added accelerators Ctrl-w, Ctrl-n, Ctrl-q, Ctrl-p, Ctrl-P, Ctrl-S, Ctrl-g; 
#  Alt-L
# fixed "unset pad" in closewin rather than exitapp - fixes a bug apparent when
#   exiting scipad by closing the last active buffer and trying to relaunch it
# colorization: added "punct", "operator"; changed colors of the colorization
# corrected "parencesis" in blinkbrace - fixes highlighting of ()
# "Scilab specials" in colorize -- added code to distinguish {} [], () from ;: 
# [external but connected: new list of keywords "words"]

#ES 4/9/2003
# * added colorization pattern for "number"; separated pattern "xmltag" from 
#   "rem2";
# * xmltags colorized only if filename contains ".xml"
# *corrected bug - scipad(newfilename) began in an unnamed buffer
#   [connected in scipad.sci: scipad(filename) does not open an additional 
#   "Untitled.sce"]

#ES 11/9/2003
# closing/reopening scipad remembers previous font size
# cursor doesn't disappear any more (it used to every second keypress or so or 
#  mouse move)
# cursor is red
# cursor is positioned at 1.1 when opening an existing file 

#ES 18/9/2003
# *changed $Size to Size in the option menu entries - the correct radiobutton  
#   is tagged at startup
# *commented #destroy $textarea in proc closefile - seems to solve the shrink
#   bug (if the initial Untitled.sce is closed, then the next setfontscipadN,
#   openfile,newfile command causes the window to shrink to its minimal size).
#    Not sure it is the right thing to do. The bug is cured and the only 
#   apparent side effect is that the window does not resize upon font change
#   (was never exact anyway) 
# *fixed tagging of radiobuttons in the windows menu when files are closed 
# *added comment selection command (glitch - adds // after the last newline 
#  also when the selection ends there)
# *added uncomment selection command (glitch - removes also the white space 
#  between beginning of line and //)

#ES 25/9/2003
# *the code is now free of TKSetCursor's (even in FindIt)
# *restored default mouseclicks actions (double-1=select word, triple-1=select 
#   line, 2=paste sel) 
# *deglitched "\n//" in comment selection and restore selection in uncomment 
#   (not yet 100.0% ok)
# *selection remains on after copytext
# *accelerator Ctrl-Z for Undo (more standard)
# *accelerator F1 for About
# *Help opens the scipad page in the helpbrowser
# *version -> 1.2
# *button3 popsup the edit menu (not so useful but why not - perhaps in future 
#   I'll replace it with a tailored menu)
# *"(modified)" appears on the titlebar, and the corresponding entry in the 
#   windows menu is colored (and of course things are reverted to normal when
#    the buffer is saved)
# *translated to french (!!?) some "file already open"
# *created the Execute menu; added the command execute selection and its 
#   accelerator Ctrl-y (inherits problems from ScilabEval/execstr() - most 
#   dangerous problem, goes amok when the selection contains a function without
#   endfunction) -- glitch: splits quoted strings containing // as if // starts
#   a comment
# *bug solved: Esc or Ctrl-c on a find/replace dialog used to hang scipad
# *copy/paste (Ctrl-{c,v}) possible on find/replace strings [but not cut nor 
#   replace-paste --why?]

#ES 2/10/2003
# * $pad.statusind goes Salmon when the buffer is modified
# * splitted long lines in the code for improved readability
# * the popup menu doesn't pop on the bottom bar
# * bug fixed: upon closing a buffer, the radiobutton of the one previously
#    opened (which becomes the current buffer) was not correctly checked
# * shift-button3 and ctrl-button3 popup execute and options
# * bug fixed: search and replace strings can begin with "-"
# * copytext doesn't tag the buffer as modified
# * glitch corrected: //..."string"... is all colorized as comment, "...//..."
#    as string
# * glitch corrected: gotoline didn't scroll to make the target line visible
# * begun proc whichfun

#ES 9/10/2003
# * corrected bad continued line "Annuler"
# * progress in whichfun, execselection, etc. 

#ES 19/10/2003
# * F5 Save and Run
# * cosmetic - compacted the command passed and displayed by execselection
# * pad remembers its geometry when quitting/reopening 
# * moved option stuff (colors, fontsize, geometry) at the top -- later I might
#   add that it is read from an options file
# * the background color of found text is $SELCOLOR, as selection
# * bug fixed: missing $textarea when printing with Ctrl-p

#ES 21/10/2003
# * added font micro (ok on my laptop)
# * rationalized setfontscipad procedures, font size changes also menu fonts
# * accelerator F2=Save
# * added catch to delinfo in proc showinfo - prevents a "missing variable
#   .pad" which appears ocasionally on reopening

#27/10/2003
# * proc helpword
# * message "file NNNN saved"

#around november: made colorization TIMES faster, by scanning only to
# wordend (not end-of-text) for scilab keywords in proc colorize.

#28/12/2003
# * added IndentSel, UnIndentSel (plain \s\s, no smart logical indent)
# * little deglitch in CommentSel, UncommentSel
# * rationalization of Alt-x-x keyboard menu activations
# * fixed Ctrl-w cut selection AND close window
# * text pasted with Button-2 is colorized

#4/1/2004
# * bug fixed - to interfering bindings from Ctrl-v caused no or double paste.
# * removed some "particular" bindings for Ctrl-c/v/x and transformed them
#   to All or Text: copy paste works better in find and filename dialogs
# * fixed Ctrl-y bind, does not paste selection surretptitiously
# * cursor position info (keyposn) updated when switching buffers

#15/2/2004
# * file dialogs for "all files" show also * files 

#30/2/2004
# * fixed remalltags for textquoted and rem2 (i.e., lines with "aaa//bbb",
#    "123456", //123 etc. should be colorized consistently 
# * comment/uncomment, indent/unindent applies to the current line 
#    if unselected

# Francois VOGEL, 16/04/2004
# * fixed silent clearing of selection in one buffer that occurred when another
#   buffer was modified
# * added Ctrl-F6 binding and proc switchbuffer to switch opened buffers
# * fixed bug that occurred when a copy-paste modification on a buffer was
#   undone once too much (a "lifo($id) pop error, empty" resulted)

#19/4/2004
# * added "Import matlab file..."
# * version --> 1.3
# * switchbuffer --> prevbuffer (F6) and nextbuffer (F7); bug corrected, wrong 
#   buffer when one or more previously open buffers were closed
# * added bindings Ctrl-+/- to increase/decrease fontsize

# Francois VOGEL, 21/04/2004
# * fixed size/position bug of sliders when switching buffers or opening a new file
# * corrected minor typos in french texts, added french text where was missing
# * restored cursor blinking as I could not make it disappear any more while
#   moving it with the arrows - was probably a tcl bug, seems to work with 8.4.6
# * many improvements in find/replace/replaceall:
#     - added forward and backward automatic wrap-around
#     - added message box in case of no match found
#     - added message box in case of find or replace with an empty string
#     - if text is selected, find or replace occurs inside that selection (for find,
#       if not found, then asks for extending the search to the entire buffer)
#     - starting from a saved file, doing stuff and then undoing all changes does
#       not report the file as modified any more
# * added Ctrl-F6 (prevbuffer) and Ctrl-F7 (nextbuffer) bindings

# Francois VOGEL, 13/05/04
# * breakpoint and debug tools added - see below in this file for detailed
#   explanations (perform a search for "breakpoint stuff" for quick access)
#   Note: matsys.f as provided under bug #718 correction should be used
# * proc whichfun now can search in a buffer different from the current one (used
#   for breakpoint stuff)
# * Find/Replace now can use a regexp mode
# * cosmetic: replaced tab indents by 4 spaces in the code (e.g. in proc findtext)
#   Please use spaces instead of tabs for further updates of the code!
# * cosmetic: harmonised accelerator names through menus (e.g. ctrl-x changed to
#   ctrl+x)
# * rearranged bindings in find/replace dialogs - were not always consistent with
#   the underlines, especially for the french dialog
# * cursor blinks only if tk version is >= 8.4
# * message at the bottom of the Scipad window is cleared when colorization ends
# * bugzilla bug #723 corrected - an error poped when trying to write text in
#   Scipad when a selection was active in another tcl/tk application such as the
#   Scilab help
# * version --> 2.0

# default global values
#global .

if { [info exists pad] } { 
    raise $pad
    update
} else {
    set pad .scipad

toplevel $pad
#global pad
set fileName " "
set textareacur $pad.textarea
set saveTextMsg 0
set winTitle "SciPad"
set version "Version 2.0"
set wordWrap none
##ES: default options which can be overridden
if { ![info exists BGCOLOR] } {set BGCOLOR "snow1"}
if { ![info exists FGCOLOR] } {set FGCOLOR "black"}
if { ![info exists CURCOLOR] } {set CURCOLOR "red"}
if { ![info exists PARCOLOR] } {set PARCOLOR "magenta3"}
if { ![info exists BRAKCOLOR] } {set BRAKCOLOR "DarkGoldenrod4"}
if { ![info exists BRACCOLOR] } {set BRACCOLOR "red"}
if { ![info exists PUNCOLOR] } {set PUNCOLOR "turquoise4"}
if { ![info exists KEYWCOLOR] } {set KEYWCOLOR "blue2"}
if { ![info exists OPCOLOR] } {set OPCOLOR "blue4"}
if { ![info exists TXTCOLOR] } {set TXTCOLOR $FGCOLOR}
if { ![info exists QTXTCOLOR] } {set QTXTCOLOR "darkred"}
if { ![info exists REMCOLOR] } {set REMCOLOR "green4"}
if { ![info exists XMLCOLOR] } {set XMLCOLOR "orange"}
if { ![info exists NUMCOLOR] } {set NUMCOLOR "yellow4"}
if { ![info exists SELCOLOR] } {set SELCOLOR "PaleGreen"}
# FV 13/05/04
if { ![info exists BREAKPOINTCOLOR] } {set BREAKPOINTCOLOR "pink"}
##ES: remember fontsize, pad geometry
if { ![info exists FontSize] } {set FontSize 12 }
if { ![info exists WMGEOMETRY] } { set WMGEOMETRY 32x36 }
if { ![info exists printCommand] } { set printCommand lpr }

#
#[string range $argv0 [expr [string last "/" $argv0] + 1] end]
#set argc 0
#
# added by Matthieu PHILIPPE
if { ![info exists lang] } { set lang "eng" }
if {[info exists SciPath]} { set env(SCIPATH) $SciPath };
set chset()                 {} 
set words()                 {}  
set listoffile("$pad.textarea",filename)   "[pwd]/Untitled.sce"
set listoffile("$pad.textarea",save) 0; 
# set flag to know if the editing file was change by user
set listoffile("$pad.textarea",new) 1; # is not an opened file from disk
set listoffile("$pad.textarea",thetime) 0; # set the time of the last modify
set listoftextarea $pad.textarea
set winopened 0
set radiobuttonvalue 0

# main window settings
eval destroy [winfo child $pad]
wm title $pad "$winTitle - $listoffile("$pad.textarea",filename)"
wm iconname $pad $winTitle
##ES 4/9/2003
##set taille [font measure $textFont " "]
##wm grid $pad 20 60 20 60 
#boh. "text $pad.textarea -height 1 -width 1" below
wm geometry $pad $WMGEOMETRY
wm minsize $pad 25 1 

#create main menu
menu $pad.filemenu -tearoff 0

# start by setting default font sizes
if [ expr [string compare $tcl_platform(platform) "unix"] ==0] {
	set textFont -Adobe-courier-medium-r-Normal-*-$FontSize-*
	set menuFont -adobe-helvetica-bold-r-normal--$FontSize-*-75-75-*-*-*-*
} else {
	set textFont -Adobe-Courier-medium-R-Normal-*-$FontSize-*
	#set menuFont -adobe-helvetica-bold-r-normal--12-*-75-75-*-*-*-*
	set menuFont [$pad.filemenu cget -font]
}

$pad.filemenu configure -font $menuFont

# added by Matthieu PHILIPPE 18/12/2001
# to have the possibility to launch multiple
# browser from scilab with independant menu
# a little uggly !!!

#ES: instead of several setfontscipadN{}
proc setfontscipad {FontSize} {
    global textFont menuFont pad
    global listoftextarea
    set textFont -Adobe-courier-medium-R-Normal-*-$FontSize-*
    set menuFont -adobe-helvetica-bold-r-normal--$FontSize-*
    $pad.filemenu configure -font $menuFont
    foreach textarea $listoftextarea {$textarea configure -font $textFont}
}


# create frames for widget layout
# this is for the text widget and the y scroll bar
frame $pad.bottomTopMenu
pack $pad.bottomTopMenu  -side top -expand 1 -fill both
# where the text widget is packed
frame $pad.bottomleftmenu
pack $pad.bottomleftmenu -in $pad.bottomTopMenu  -side left -expand 1 \
    -fill both
# where the y scrollbar is packed
frame $pad.bottomrightmenu 
pack  $pad.bottomrightmenu -in $pad.bottomTopMenu  -side right -expand 0 \
    -fill both 
# this is for the x scroll bar at the bottom of the window
frame $pad.bottombottommenu
pack $pad.bottombottommenu -side bottom -expand 0 -fill both


#file menu
menu $pad.filemenu.files -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu  add cascade -label "File" -underline 0 \
	-menu $pad.filemenu.files
    $pad.filemenu.files add command -label "New" -underline 0 \
                   -command "filesetasnewmat" -accelerator Ctrl+n
    $pad.filemenu.files add command -label "Open" -underline 0 \
                   -command "filetoopen $textareacur" -accelerator Ctrl+o
    $pad.filemenu.files add command -label "Save" -underline 0 \
                   -command "filetosavecur" -accelerator Ctrl+s
    $pad.filemenu.files add command -label "Save As" -underline 5 \
                   -command "filesaveascur" -accelerator Ctrl+S
    $pad.filemenu.files add separator
    $pad.filemenu.files add command -label "Import Matlab file..." \
	-underline 7 -command "importmatlab" -accelerator F4
    $pad.filemenu.files add separator
    if {"$tcl_platform(platform)" == "unix"} {
	$pad.filemenu.files add command -label "Print Setup" -underline 8 \
                   -command "printseupselection" -accelerator Ctrl+P
	$pad.filemenu.files add command -label "Print" -underline 0 \
                   -command "selectprint $textareacur" -accelerator Ctrl+p
	$pad.filemenu.files add separator
    }
    $pad.filemenu.files add command -label "Close" -underline 0 \
	-command "closecur" -accelerator Ctrl+w
    $pad.filemenu.files add command -label "Exit" -underline 1 \
	-command "exitapp" -accelerator Ctrl+q
} else {
    $pad.filemenu  add cascade -label "Fichier" -underline 0 \
	-menu $pad.filemenu.files
    $pad.filemenu.files add command -label "Nouveau" -underline 0 \
	-command "filesetasnewmat" -accelerator Ctrl+n
    $pad.filemenu.files add command -label "Ouvrir" -underline 0 \
	-command "filetoopen $textareacur" -accelerator Ctrl+o
    $pad.filemenu.files add command -label "Enregistrer" -underline 0 \
	-command "filetosavecur" -accelerator Ctrl+s
    $pad.filemenu.files add command -label "Enregistrer sous" -underline 2 \
	-command "filesaveascur" -accelerator Ctrl+S
    $pad.filemenu.files add separator
    $pad.filemenu.files add command -label "Importer fichier Matlab..." \
                   -underline 17 -command "importmatlab" -accelerator F4
    $pad.filemenu.files add separator
    if {"$tcl_platform(platform)" == "unix"} {
	$pad.filemenu.files add command -label "Mise en page" -underline 8 \
	    -command "printseupselection" -accelerator Ctrl+P
	$pad.filemenu.files add command -label "Imprimer" -underline 0 \
	    -command "selectprint $textareacur" -accelerator Ctrl+p
	$pad.filemenu.files add separator
    }
    $pad.filemenu.files add command -label "Fermer" -underline 0 \
	-command "closecur" -accelerator Ctrl+w
    $pad.filemenu.files add command -label "Quitter" -underline 0 \
	-command "exitapp" -accelerator Ctrl+q 
}

#edit menu
menu $pad.filemenu.edit -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Edit" -underline 0 \
	-menu $pad.filemenu.edit
    $pad.filemenu.edit add command -label "Undo" -underline 0 \
	-command " undo_menu_proc" -accelerator Ctrl+z
    $pad.filemenu.edit add command -label "Redo" -underline 0 \
	-command "redo_menu_proc" -accelerator Ctrl+Z
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Cut" -underline 2 \
	-command "cuttext" -accelerator Ctrl+x
    $pad.filemenu.edit add command -label "Copy" -underline 0 \
	-command "copytext" -accelerator Ctrl+c
    $pad.filemenu.edit add command -label "Paste" -underline 0 \
	-command "pastetext" -accelerator Ctrl+v
    $pad.filemenu.edit add command -label "Delete" -underline 0 \
	-command "deletetext" -accelerator Del
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Select All" -underline 7 \
	-command "selectall" -accelerator Ctrl+/
#    $pad.filemenu.edit add command -label "Time/Date" -underline 5 \
#	-command "printtime"
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Comment selection" -underline 3 \
	-command "CommentSel" -accelerator Ctrl+m
    $pad.filemenu.edit add command -label "Uncomment selection" -underline 1 \
	-command "UnCommentSel" -accelerator Ctrl+M
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Indent selection" -underline 0 \
	-command "IndentSel" -accelerator Ctrl+d
    $pad.filemenu.edit add command -label "Unindent selection" -underline 4 \
	-command "UnIndentSel" -accelerator Ctrl+D
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add check -label "Word Wrap" -underline 5 \
	-command "wraptext"
} else {
    $pad.filemenu add cascade -label "Edition" -underline 0 \
	-menu $pad.filemenu.edit
    $pad.filemenu.edit add command -label "Annuler" -underline 0 \
	-command " undo_menu_proc" -accelerator Ctrl+z
    $pad.filemenu.edit add command -label "Répéter" -underline 0 \
	-command "redo_menu_proc" -accelerator Ctrl+Z
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Couper" -underline 2 \
	-command "cuttext" -accelerator Ctrl+x
    $pad.filemenu.edit add command -label "Copier" -underline 0 \
	-command "copytext" -accelerator Ctrl+c
    $pad.filemenu.edit add command -label "Coller" -underline 2 \
	-command "pastetext" -accelerator Ctrl+v
    $pad.filemenu.edit add command -label "Effacer" -underline 0 \
	-command "deletetext" -accelerator Del
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Sélectionner tout" -underline 13 \
	-command "selectall" -accelerator Ctrl+/
#    $pad.filemenu.edit add command -label "Time/Date" -underline 5 \
#      -command "printtime"
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Commenter la sélection" -underline 3 \
	-command "CommentSel" -accelerator Ctrl+m
    $pad.filemenu.edit add command -label "Décommenter la sélection" -underline 0\
	-command "UnCommentSel" -accelerator Ctrl+M
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Indenter la sélection" -underline 0 \
	-command "IndentSel" -accelerator Ctrl+d
    $pad.filemenu.edit add command -label "Désindenter la sélection" -underline 1 \
	-command "UnIndentSel" -accelerator Ctrl+D
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add check -label "Retour à la ligne automatique" \
	-underline 12 -command "wraptext"
} 

#search menu
menu $pad.filemenu.search -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Search" -underline 0 \
	-menu $pad.filemenu.search 
    $pad.filemenu.search add command -label "Find" -underline 0 \
	-command "findtext find" -accelerator Ctrl+f
    $pad.filemenu.search add command -label "Find Next" -underline 5 \
	-command "findnext find" -accelerator F3
    $pad.filemenu.search add command -label "Replace" -underline 0 \
	-command "findtext replace" -accelerator Ctrl+r
# add new menu option include by Matthieu PHILIPPE from gotoline.pth 21/11/2001
    $pad.filemenu.search add command -label "Goto Line" -underline 0 \
	-command "gotoline" -accelerator Ctrl+g
} else {
    $pad.filemenu add cascade -label "Rechercher" -underline 0 \
	-menu $pad.filemenu.search 
    $pad.filemenu.search add command -label "Rechercher" \
	-underline 0 -command "findtext find" -accelerator Ctrl+f
    $pad.filemenu.search add command -label "Rechercher suivant" \
	-underline 11 -command "findnext find" -accelerator F3
    $pad.filemenu.search add command -label "Remplacer" -underline 3 \
	-command "findtext replace" -accelerator Ctrl+r
# add new menu option include by Matthieu PHILIPPE from gotoline.pth 21/11/2001
    $pad.filemenu.search add command -label "Atteindre" -underline 0 \
	-command "gotoline" -accelerator Ctrl+g
} 

# added by matthieu PHILIPPE dec 11th 2001
# window menu
if {$lang == "eng"} {
    menu $pad.filemenu.wind -tearoff 1 -title "Opened Files" -font $menuFont
    $pad.filemenu add cascade -label "Windows" -underline 0 \
	-menu $pad.filemenu.wind
} else {
    menu $pad.filemenu.wind -tearoff 1 -title "Fichiers ouverts" \
	-font $menuFont
    $pad.filemenu add cascade -label "Fenêtres" -underline 0 \
	-menu $pad.filemenu.wind
}
$pad.filemenu.wind add radiobutton \
    -label "$listoffile("$pad.textarea",filename)"\
    -value $winopened -variable radiobuttonvalue \
    -command "montretext $pad.textarea"
# options menu
if {$lang == "eng"} {
    menu $pad.filemenu.options -tearoff 1 -font $menuFont
    $pad.filemenu add cascade -label "Options" -underline 0 \
	-menu $pad.filemenu.options
    $pad.filemenu.options add command -label "font size" -foreground red 
    $pad.filemenu.options add radiobutton -label "micro" -value 10 \
	-variable FontSize -command "setfontscipad 10" -underline 0
    $pad.filemenu.options add radiobutton -label "small" -value 12 \
	-variable FontSize -command "setfontscipad 12" -underline 0
    $pad.filemenu.options add radiobutton -label "medium" -value 14 \
	-variable FontSize -command "setfontscipad 14" -underline 1
    $pad.filemenu.options add radiobutton -label "large" -value 18\
	-variable FontSize -command "setfontscipad 18" -underline 0
} else {
    menu $pad.filemenu.options -tearoff 1 -font $menuFont
    $pad.filemenu add cascade -label "Options" -underline 0 \
	-menu $pad.filemenu.options
    $pad.filemenu.options add command -label "taille de fontes" -foreground red
    $pad.filemenu.options add radiobutton -label "micro" -value 10 \
	-variable FontSize -command "setfontscipad 10" -underline 0
    $pad.filemenu.options add radiobutton -label "petit" -value 12 \
	-variable FontSize -command "setfontscipad 12" -underline 0
    $pad.filemenu.options add radiobutton -label "moyen" -value 14  \
	-variable FontSize -command "setfontscipad 14" -underline 1
    $pad.filemenu.options add radiobutton -label "grand" -value 18 \
	-variable FontSize -command "setfontscipad 18" -underline 0
}

# exec menu
menu $pad.filemenu.exec -tearoff 1 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Execute" -underline 1 \
	-menu $pad.filemenu.exec
    $pad.filemenu.exec add command -label "Load into Scilab" -underline 0\
	-command "execfile" -accelerator Ctrl+l
    $pad.filemenu.exec add command -label "Evaluate selection" -underline 0\
	-command "execselection" -accelerator Ctrl+y
} else {
    $pad.filemenu add cascade -label "Exécuter" -underline 1 \
	-menu $pad.filemenu.exec
    $pad.filemenu.exec add command -label "Charger dans Scilab" -underline 0\
	-command "execfile" -accelerator Ctrl+l
    $pad.filemenu.exec add command -label "Evaluer la sélection" -underline 0\
	-command "execselection" -accelerator Ctrl+y
}

proc execfile {} {
    global listoffile sciprompt lang

    set textarea [gettextareacur]

    set doexec 1
    if [ expr [string compare [getccount $textarea] 1] == 0 ] {
	if {$lang == "eng"} {
	    set answer [tk_messageBox -message "The contents of \
             $listoffile("$textarea",filename) may have changed, do you wish \
             to to save your changes?" \
	            -title "Save Confirm?" -type yesnocancel -icon question]
	} else {
	    set answer [tk_messageBox -message \
            "Voulez-vous enregistrer les modifications apportées à \
              $listoffile("$textarea",filename) ?" \
	     -title "Confirmer sauver ?" -type yesnocancel -icon question]
	}
	case $answer {
	    yes { filetosave $textarea; set doexec 1 }
	    no { set doexec 0 }
	    cancel { set doexec 0 }
	}
    }

    if $doexec {
	if [ expr [string compare $sciprompt -1] == 0 ] {
	    if {$lang == "eng"} {
		tk_messageBox -message \
                 "Scilab is working, wait for the prompt to load file \
                  $listoffile("$textarea",filename)" \
                    -title "Scilab working" -type ok -icon info
	    } else {
		tk_messageBox -message \
                "Scilab est occupé, attendez le prompt pour charger le fichier \
                 $listoffile("$textarea",filename)"\
                    -title "Scilab occupé" -type ok -icon info
	    }
	} else {
	    set f $listoffile("$textarea",filename)
	    ScilabEval "exec(\"$f\");\n"
	}
    }
}

proc execselection {} {
    global sciprompt lang textareacur
    if [ expr [string compare $sciprompt -1] == 0 ] {
	if {$lang == "eng"} {
	   tk_messageBox -message \
               "Scilab is working, wait for the prompt to execute the selection" \
	       -title "Scilab working" -type ok -icon info
	 } else {
	   tk_messageBox -message \
               "Scilab est occupé, attendez le prompt pour charger la sélection" \
	       -title "Scilab occupé" -type ok -icon info
	 }
     } else {
        set seltexts [selection own]
        if {$seltexts != "" } {
	  if [catch {selection get -selection PRIMARY} sel] {	    
	  } else {
     	    set f [selection get]
#SciEval does not digest multilines, nor comments. The following hacks are 
# not optimal - they can produce very long lines, and get confused about 
# quoted strings containing //.
#strip comments from // to \n (note - \n stays, as the interpreter allows 
#    "...//bla\n rest" ) (NOTE: this way strings like "...//..." are truncated 
#    -- FIXIT -- has to use tag textquoted information)
            regsub -all -line "//.*(\\n|\\Z)" $f "\n" f1
            unset f
# remove trailing white space
            regsub -all -line "^\\s*" $f1 " " f2
            unset f1
#join continued lines
            regsub -all -line "\\.{2,} *\\n" $f2 "" f3
            unset f2
#join multilines with ","
            regsub -all -line "\\n" $f3 "," comm
            unset f3
# last hack - add a final endfunction if there is an unterminated
# function in the selection: TODO (try to involve proc whichfun)
# Things are complicated because the selection may either include
#  the originating "function" or not
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
#TODO ES 9/10/03
#             if { $i2>$i1 } {
#                 set funselstart [lindex [whichfun $i1] 0]
#                 set funselend [lindex [whichfun $i2] 0]
#             } else {
#                 set funselstart [lindex [whichfun $i2] 0]
# 		set funselend [lindex [whichfun $i1] 0]
# 	     }
#             tk_messageBox -message $funselstart"--"$funselend
# 	      if { $funselend !={} && $funselstart == {}} {
# 		  append comm ",endfunction"}
# 	      if { $funselend !={} && $funselstart != $funselend} {
# 		  tk_messageBox -message \
# 		      "How do you pretend Scilab to evaluate the bottom of a function definition without its header?"
# 		  return
# 	    }

# Besides, I'd like to see screen output too.
            regsub -all -line "\"" $comm "\"\"" dispcomm
            regsub -all -line "'" $dispcomm "''" dispcomm1
            unset dispcomm
            ScilabEval "disp(\"$dispcomm1\")"
	    ScilabEval $comm
          }
        }
     }
}

proc importmatlab {} {
    global pad listoffile sciprompt lang
    if [ expr [string compare $sciprompt -1] == 0 ] {
	if {$lang == "eng"} {
	   tk_messageBox -message \
             "Scilab is working, wait for the prompt to convert a Matlab file" \
	       -title "Scilab working" -type ok -icon info
	 } else {
	   tk_messageBox -message \
             "Scilab est occupé, attendez le prompt pour importer un fichier Matlab"\
	       -title "Scilab occupé" -type ok -icon info
	 }
     } else {
       if {$lang == "eng"} {
	  set types {
	    {"Matlab files" {*.m }} 
	    {"All files" {* *.*}}
	    }
	  set dtitle "Matlab file to convert"
       } else {
	  set types {
	    {"Fichiers Matlab" {*.m }} 
	    {"Tous les fichiers" {*.* *}}
	    }
	    set dtitle "Fichier Matlab à importer"
       }
       set sourcefile [tk_getOpenFile -filetypes $types -parent $pad -title "$dtitle"]
       if {$sourcefile !=""} {
	 set sourcedir [file dirname $sourcefile]
         set destfile [file rootname $sourcefile].sci 
	 set convcomm "execstr(\"res=mfile2sci(\"\"$sourcefile\"\",\
                      \"\"$sourcedir\"\",%f,%f,1,%t)\",\"errcatch\",\"m\")"
	 set impcomm \
	      "if $convcomm==0 then \
                 TK_EvalStr(\"scipad eval {delinfo; openfile $destfile} \"); \
               else; \
                 TK_EvalStr(\"scipad eval {failmatlabimp} \");\
               end"
         if {$lang == "eng"} {
	     showinfo "Scilab is converting, please hold on..." }
         if {$lang == "fr"} {
	     showinfo "Scilab est en train de convertir, patientez SVP..." }
         ScilabEval $impcomm
       }
     }
}

proc failmatlabimp {} {
  global lang
  if {$lang == "eng"} {
     tk_messageBox -title "Matlab file import"  \
       -message "Conversion of the file failed, see the Scilab window\
                for details -- Perhaps report the error text and the\
                offending Matlab file to \
                http://scilabsoft.inria.fr/bugzilla_bug/index.cgi" \
                -icon error
  }
  if {$lang == "fr"} {
     tk_messageBox -title "Import du fichier Matlab"  \
       -message "La conversion du ficher a échoué, voyez dans la fenêtre\
                Scilab pour plus d'informations -- Veuillez rapporter le\
                message d'erreur et le fichier Matlab incriminé à\
                http://scilabsoft.inria.fr/bugzilla_bug/index.cgi" \
                -icon error
  }
}

#######################################################################
################## Beginning of breakpoint stuff ######################
#######################################################################
#            FV 13/05/04, breakpoint and debug stuff added            #
#######################################################################
#
# The way it works is basically the following:
# 
# 1. Set/remove bp anywhere you want in the buffer. This is just text tags
#    management (nice pink background, etc.).
# 
# 2. Configure execution: the user has to provide the function name to
#    execute, its variable names, and variable values. To ease this step, I
#    implemented a scan of the currently displayed buffer to look for defined
#    functions in it, scan their names and variable names (thanks to proc
#    whichfun!). All this is displayed to the user in a dialog for easy
#    selection. First, he has to select a function in a spinbox (sorry, but
#    comboboxes are not available natively in tk), then eventually one of
#    its variables (listbox). Once a variable is selected, the user can
#    set/change its value (another dialog pops). Variable values and names are
#    displayed in listboxes. The user can also add or remove a new variable. This
#    is in case the user changes the input variables of his function in the file,
#    and he does not want to scan the buffer again (which causes all the variable
#    values to reset to "").
# 
# 3. Launch execution with breakpoints. Scipad execs the _current _buffer to
#    Scilab, sends to Scilab the setbpt() instructions relative to the
#    _function_selected_ at step 2 (which may be different from the one he sees
#    in the displayed buffer), and executes the selected function with the
#    provided input variable values. Then the execution is automagically stopped
#    by Scilab at the first breakpoint.
# 
# 4. Now there are several possibilities for the user:
#   4.1. Continue up to next bp: just a resume to send to Scilab.
# 
#   4.2. Insert/Remove a new bp during the debug: Scipad sends a setbpt() (or
#        delbpt()), then a resume to Scilab.
# 
#   4.3. Step by step execution. This is what I'm currently fighting at (it is
#        disabled in the Debug menu for the time being).
#        See proc stepbystep_bp for further details about the problems I
#        encounter.
# 
#   4.4. Continue ignoring any breakpoint, and finish execution as if there
#        were no breakpoint: Scipad sends adequate delbpt(), then resume.
# 
#   4.5. Display call stack: Scipad sends a whereami() to Scilab. Sometimes
#        useful.
# 
#   4.6. Abort debug: Scipad sends adequate delbpt(), then sends abort to
#        Scilab.
# 
# 5. Besides, there is a command to remove all the bp in Scilab, in case
#    anything did not run as expected (could be removed later, but anyway).
#    Currently, this may be useful in case of loss of first characters sent by
#    ScilabEval, which still may occur, and which produce errors in the Scilab
#    shell.
# 
# Of course, after the end of the execution, the breakpoints are removed from
# Scilab (but kept in Scipad).
# 
# All this can be invoked through a nice debug menu, with adequate
# accelerators and shortcuts.
# 
# All what I have just described above works well with functions, i.e. .sci
# files. I thought about .sce files, but setbpt() essentially cannot be used
# with them. However, the .sce case is foreseen in all the procs of
# scipad.tcl. That means that a .sce file will not crash scipad, which is the
# very least, but more, once I figure out a solution it should be easy to add
# the functionality. Anyway, if actually a debug is required in a .sce, the
# user could first turn int into a .sci function and that's it. The absence of
# breakpoints management for .sce is therefore not such a major disadvantage.
# For the deff's, I really think that if a debug is required in such a simple
# thing, the user should also turn it into a [...]=function(...). A deff is
# almost always a very simple function that does not require internal debug.
# But you may disagree...
# 
#######################################################################
#
# Now a few words about the variables used:
#
# Highest level:
# --------------
#   funnameargs: Full command sent to Scilab (function name with arguments)
#                Generated by proc OKconf_bp
#                Used by execfile_bp
#
# One level down: ($conf level)
# ---------------
#   Elements of listboxinput
#   Elements of listboxinputval
#   Elements of spin
#   funname: function name shown by spin
#   funvars($funname): arguments list of function $funname, e.g. {arg1 arg2}
#   funvarsvals($funname,var): content of argument var from function $funname
#   funnames: list of all the different function names found by Obtainall_bp
#   funsinbuffer($textarea): same as funnames, but limited to $textarea
#
# One further level down: ($adda level)
# -----------------------
#   argname: name of currently selected function argument
#   argvalue: content of this argument
#
# Aside of these:
# ---------------
#   Breakpoints are set and managed using tags in the text widget that
#   displays the current buffer.
#   They are sent or removed in Scilab using setbpt and delbpt.
#
#######################################################################

menu $pad.filemenu.debug -tearoff 1 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Debug" -underline 0 \
	-menu $pad.filemenu.debug
    $pad.filemenu.debug add command -label "Insert/Remove breakpoint" \
      -underline 0 -command "insertremove_bp" -accelerator F9
    $pad.filemenu.debug add command -label "Remove all breakpoints" \
      -underline 7 -command "removeall_bp" -accelerator Ctrl+F9
    $pad.filemenu.debug add separator
    $pad.filemenu.debug add command -label "Configure execution" \
      -underline 0 -command "configurefoo_bp" -accelerator F10
    $pad.filemenu.debug add separator
    $pad.filemenu.debug add command -label "Start execution with breakpoints" \
      -underline 0 -command "execfile_bp" -accelerator Ctrl+F11
    $pad.filemenu.debug add command -label "Go on up to next breakpoint" \
      -underline 12 -command "resume_bp" -accelerator F11
    $pad.filemenu.debug add command -label "Insert/Remove breakpoint during debug" \
      -underline 18 -command "insertremovedebug_bp" -accelerator Shift+F11
    $pad.filemenu.debug add command -label "Step by step" \
      -underline 0 -command "stepbystep_bp" -accelerator F12 -state disabled
    $pad.filemenu.debug add command -label "Go on ignoring any breakpoint" \
      -underline 1 -command "goonwo_bp" -accelerator Shift+F12
    $pad.filemenu.debug add command -label "Display call stack" \
      -underline 6 -command "dispcallstack_bp"
    $pad.filemenu.debug add separator
    $pad.filemenu.debug add command -label "Cancel debug" \
      -underline 5 -command "canceldebug_bp"
    $pad.filemenu.debug add command -label "Remove all breakpoints in Scilab" \
      -underline 0 -command "removescilab_bp with_output"
} else {
    $pad.filemenu add cascade -label "Débug" -underline 0 \
	-menu $pad.filemenu.debug
    $pad.filemenu.debug add command -label "Insérer/Supprimer un point d'arrêt" \
      -underline 0 -command "insertremove_bp" -accelerator F9
    $pad.filemenu.debug add command -label "Supprimer tous les points d'arrêt" \
      -underline 10 -command "removeall_bp" -accelerator Ctrl+F9
    $pad.filemenu.debug add separator
    $pad.filemenu.debug add command -label "Configurer l'exécution" \
      -underline 3 -command "configurefoo_bp" -accelerator F10
    $pad.filemenu.debug add separator
    $pad.filemenu.debug add command -label "Démarrer l'exécution avec les points d'arrêt" \
      -underline 0 -command "execfile_bp" -accelerator Ctrl+F11
    $pad.filemenu.debug add command -label "Continuer jusqu'au prochain point d'arrêt" \
      -underline 10 -command "resume_bp" -accelerator F11
    $pad.filemenu.debug add command -label "Insérer/Supprimer un point d'arrêt pendant le debug" \
      -underline 40 -command "insertremovedebug_bp" -accelerator Shift+F11
    $pad.filemenu.debug add command -label "Pas à pas" \
      -underline 0 -command "stepbystep_bp" -accelerator F12 -state disabled
    $pad.filemenu.debug add command -label "Continuer sans aucun point d'arrêt" \
      -underline 0 -command "goonwo_bp" -accelerator Shift+F12
    $pad.filemenu.debug add command -label "Montrer la pile des appels" \
      -underline 0 -command "dispcallstack_bp"
    $pad.filemenu.debug add separator
    $pad.filemenu.debug add command -label "Annuler le débug" \
      -underline 0 -command "canceldebug_bp"
    $pad.filemenu.debug add command -label "Supprimer tous les points d'arrêt dans Scilab" \
      -underline 0 -command "removescilab_bp with_output"
}

proc insertremove_bp {} {
    set textareacur [gettextareacur]
    set i1 "insert linestart"
    set i2 "insert lineend"
    set activetags [$textareacur tag names $i1]
    if {[string first breakpoint $activetags] == -1} {
        $textareacur tag add breakpoint $i1 $i2
    } else {
        $textareacur tag remove breakpoint $i1 $i2
        $textareacur tag remove activebreakpoint $i1 $i2
    }
}

proc removeall_bp {} {
    set textareacur [gettextareacur]
    set saveinsert [$textareacur index insert]
    set tagranges [$textareacur tag ranges breakpoint]
    foreach {bp_start bp_stop} $tagranges {
        $textareacur mark set insert $bp_start
        insertremove_bp
    }
    $textareacur mark set insert $saveinsert
}

proc insertremovedebug_bp {} {
    set textareacur [gettextareacur]
    set i1 "insert linestart"
    set i2 "insert lineend"
    set activetags [$textareacur tag names $i1]
    if {[string first breakpoint $activetags] == -1} {
        $textareacur tag add breakpoint $i1 $i2
        set infun [whichfun [$textareacur index $i1]]
        if {$infun !={} } {
            set funname [lindex $infun 0]
            set lineinfun [expr [lindex $infun 1] - 1]
            set setbpcomm " setbpt(\"$funname\",$lineinfun);"
            ScilabEval $setbpcomm
        } else {
            # <TODO> .sce case
        }
    } else {
        # There was bug(s) in scilab routines/interf/matsys.f file
        # on this one: in -1-> mode, setbpt("foo",linenum) worked, but
        # delbpt("foo",linenum) did not (bp was removed from dispbpt
        # list but execution still stopped at linenum)
        # The correction has been submitted to Scilab team (see
        # bugzilla #718). If the patch is not installed, the following
        # will fail.
        $textareacur tag remove breakpoint $i1 $i2
        $textareacur tag remove activebreakpoint $i1 $i2
        set infun [whichfun [$textareacur index $i1]]
        if {$infun !={} } {
            set funname [lindex $infun 0]
            set lineinfun [expr [lindex $infun 1] - 1]
            set delbpcomm " delbpt(\"$funname\",$lineinfun);"
            ScilabEval $delbpcomm
        } else {
            # <TODO> .sce case
        }
    }
}

proc reshape_bp {} {
    set textareacur [gettextareacur]
    set tagranges [$textareacur tag ranges breakpoint]
    foreach {tstart tstop} $tagranges {
        $textareacur tag remove breakpoint $tstart $tstop
        $textareacur tag add breakpoint "$tstart linestart" "$tstart lineend"
    }
}

set funnameargs ""
set funnames ""

proc execfile_bp {} {
    global funnameargs funsinbuffer
    set removecomm [removescilab_bp "no_output"]
    set textareacur [gettextareacur]
    set tagranges [$textareacur tag ranges breakpoint]
    set setbpcomm ""
    set firstbp true
#    set nlins -1
 #   set nlins -2
    foreach {tstart tstop} $tagranges {
        set infun [whichfun [$textareacur index $tstart]]
        if {$infun !={} } {
            set funname [lindex $infun 0]
            set lineinfun [expr [lindex $infun 1] - 1]
            set setbpcomm [concat $setbpcomm "setbpt(\"$funname\",$lineinfun);"]
            if {$firstbp == true} {
                set firstbp false
                $textareacur tag remove activebreakpoint 1.0 end
                $textareacur tag add activebreakpoint "$tstart linestart" "$tstart lineend"
                $textareacur mark set insert "$tstart linestart"
                $textareacur see "$tstart linestart"
            }
       } else {
            # <TODO> .sce case: I thought about:
            # - inserting pause before each bp, or
            # - inserting mode(6) plus mode(0) before each bp
            # but none is satisfactory. Using mode() will fail in loops,
            # and pause is very limited (no way to add a new bp during debug,
            # or to remove all bp to finish execution ignoring them)
#            incr nlins 1
 #           incr nlins 2
#            $textareacur insert "$tstart +$nlins l linestart" "pause\n"
 #           $textareacur insert "$tstart +$nlins l linestart" "mode(6)\nmode(0)\n"
        }
    }
    if {$funnameargs != ""} {
        execfile
# Leading space and trailing ;\n here to avoid loss of first character
# However this does not work for long lines created by execfile (i.e.
# lines that take a long time to display or execute in Scilab window)
        if {$setbpcomm != ""} {
            ScilabEval " $setbpcomm; $funnameargs,$removecomm\n"
        } else {
            ScilabEval " $funnameargs\n"
        }
    } else {
        # <TODO> .sce case
##        execfile
    }
}

proc removescilab_bp {outp} {
    global funnames
    set textareacur [gettextareacur]
    set tagranges [$textareacur tag ranges breakpoint]
    set setbpcomm ""
    if {$funnames != ""} {
        foreach fun $funnames {
            set setbpcomm [concat $setbpcomm "delbpt(\"$fun\");"]
        }
        if {$outp != "no_output"} {
            ScilabEval " $setbpcomm\n"
        }
    } else {
        # <TODO> .sce case
    }
    return $setbpcomm
}

proc stepbystep_bp {} {
# <TODO>
# I have no satisfactory solution for the time being.
# The heart of the matter with step by step execution is that
# once the execution is stopped there is no way of knowing what is the next
# line of code to execute. Of course, it is usually the next code line in the
# sci file, but this is not necessarily true in for, while, if, and case
# structures. I do not foresee any other remedy than a complete code analysis
# performed in tcl (!), but this is a huge task I'm not prepared to go into.
# Moreover, all this analysis is already (and surely better) done by the
# Scilab interpreter, therefore the best way would probably be to add a new
# Scilab function that would return the line number of the next instruction to
# be executed. This should no be such a tricky thing to do drawing inspiration
# e.g. from setbpt. Then I would have to hack a way of returning a result to
# tcl from a ScilabEval. 
# Note: There is a similar issue with active bp tag in conditional structures.
# Currently the "next active bp" is the next one in the file
# but this becomes wrong if the bp is in a for, while, if or case
    tk_messageBox -message "Sorry, step execution not yet implemented!"
}

proc resume_bp {} {
# <TODO> correct wrong active bp tag when bp is in conditional structure
# Solution to this is similar to what is explained in proc stepbystep_bp 
 #   global funnameargs
    set textareacur [gettextareacur]
    set actbprange [$textareacur tag ranges activebreakpoint]
    if {$actbprange != {} } {
        set actstart [lindex $actbprange 0]
        set actstop [lindex $actbprange 1]
        $textareacur tag remove activebreakpoint $actstart $actstop
        set nextbprange [$textareacur tag nextrange breakpoint $actstop]
        if {$nextbprange != {} } {
            set newipos [lindex $nextbprange 0]
            $textareacur tag add activebreakpoint $newipos [lindex $nextbprange 1]
            $textareacur mark set insert $newipos
            $textareacur see $newipos
        }
    }
 #   if {$funnameargs != ""} {
        ScilabEval " resume\n"
 #   } else {
        # <TODO> .sce case
        # Sending \n is if mode(6) mode(0) is used. If pause, there is no
        # need to ditinguish between .sci and .sce (resume is sent for both)
 #       ScilabEval " \n"
 #   }
}

proc goonwo_bp {} {
    [gettextareacur] tag remove activebreakpoint 1.0 end
    removescilab_bp "with_output"
    ScilabEval " resume\n"
}

proc dispcallstack_bp {} {
    ScilabEval " whereami()\n"
}

proc canceldebug_bp {} {
    [gettextareacur] tag remove activebreakpoint 1.0 end
    ScilabEval " abort\n"
    removescilab_bp "with_output"
}

proc configurefoo_bp {} {
    global pad conf lang
    global listboxinput listboxinputval listboxscrolly spin buttonAdd
    global funnames funvars funvarsvals funnameargs
    set conf $pad.conf
    catch {destroy $conf}
    toplevel $conf
    if {$lang == "eng"} {
        wm title $conf "Configure execution"
    } else {
        wm title $conf "Configurer l'exécution"
    }
    setwingeom $conf
    frame $conf.f

    frame $conf.f.f1
    if {$lang == "eng"} {
        set tl "Function name:"
    } else {
        set tl "Nom de la fonction :"
    }
    label $conf.f.f1.label -text $tl -width 20
    set spin $conf.f.f1.spinbox
    spinbox $spin -width 30 -command "spinboxbuttoninvoke" \
                  -values $funnames -state readonly -takefocus 0
    set oppar [string first "\(" $funnameargs]
    set funname [string range $funnameargs 0 [expr $oppar-1]]
    if {$funname != "" } {
        $spin set $funname
    } else {
        $spin set [lindex $funnames 0]
        set funname [$spin get]
    }
    if {$lang == "eng"} {
        set bl "Obtain"
    } else {
        set bl "Obtenir"
    }
    button $conf.f.f1.buttonObtain -text $bl -command "Obtainall_bp"\
           -width 10 -underline 1
    pack $conf.f.f1.label $spin $conf.f.f1.buttonObtain \
         -side left -padx 4
    pack $conf.f.f1

    frame $conf.f.f2
    frame $conf.f.f2.f2l
    if {$lang == "eng"} {
        set tl "Input arguments:"
    } else {
        set tl "Arguments d'entrée :"
    }
    label $conf.f.f2.f2l.label -text $tl
    if {$lang == "eng"} {
        set bl "Add/Change"
    } else {
        set bl "Ajouter/Modifier"
    }
    set buttonAdd $conf.f.f2.f2l.buttonAdd
    button $buttonAdd -text $bl -command "Addarg_bp" -width 20 -underline 0
    if {$lang == "eng"} {
        set bl "Remove"
    } else {
        set bl "Supprimer"
    }
    button $conf.f.f2.f2l.buttonRemove -text $bl -command "Removearg_bp"\
           -width 20 -underline 0
    pack $conf.f.f2.f2l.label $buttonAdd $conf.f.f2.f2l.buttonRemove -pady 4
    frame $conf.f.f2.f2r
    set listboxinput $conf.f.f2.f2r.listboxinput
    set listboxscrolly $conf.f.f2.f2r.yscroll
    scrollbar $listboxscrolly -command "scrollyboth_bp"
    listbox $listboxinput -height 6 -yscrollcommand "scrollyrightandscrollbar_bp" \
                          -takefocus 0
    set listboxinputval $conf.f.f2.f2r.listboxinputval
    listbox $listboxinputval -height 6 -yscrollcommand "scrollyleftandscrollbar_bp" \
                          -takefocus 0
    if {[info exists funvars($funname)]} {
        foreach var $funvars($funname) {
            $listboxinput insert end $var
            $listboxinputval insert end $funvarsvals($funname,$var)
        }
        $listboxinput selection set 0
        $listboxinput see 0
    }
    pack $listboxinput $listboxscrolly $listboxinputval -side left \
            -expand 1 -fill both -padx 2
    pack $conf.f.f2.f2l $conf.f.f2.f2r -side left -padx 10
    pack $conf.f.f2 -pady 4

    frame $conf.f.f9
    button $conf.f.f9.buttonOK -text "OK" -command "OKconf_bp $conf"\
           -width 10 -height 1 -underline 0
    if {$lang == "eng"} {
        set bl "Cancel"
    } else {
        set bl "Annuler"
    }
#    button $conf.f.f9.buttonCancel -text $bl -command "Cancelconf_bp $conf"\
#           -width 10 -underline 0
#    pack $conf.f.f9.buttonOK $conf.f.f9.buttonCancel -side left -padx 10
    pack $conf.f.f9.buttonOK
    pack $conf.f.f9 -pady 4

    pack $conf.f
#    bind $conf <Return> "OKconf_bp $conf"
    bind $conf <Return> "Addarg_bp"
#    bind $conf <Escape> "Cancelconf_bp $conf"
    bind $conf <Escape> "OKconf_bp $conf"
    bind $conf <BackSpace> "Removearg_bp"
    bind $conf <Delete> "Removearg_bp"
    bind $listboxinputval <<ListboxSelect>> {selectininputval_bp}
    bind $listboxinput <ButtonPress-3> {set itemindex [dragitem_bp %y]}
    bind $listboxinput <ButtonRelease-3> {dropitem_bp $itemindex %y}
    bind $conf <Up> {scrollarrows_bp up}
    bind $conf <Down> {scrollarrows_bp down}
    bind $conf <MouseWheel> {if {%D<0} {scrollarrows_bp down}\
                                       {scrollarrows_bp up}}
    focus $buttonAdd
    grab $conf
    if {$funnames == ""} {Obtainall_bp}
    if {$funnames == ""} {OKconf_bp $conf}
}

proc scrollyboth_bp {args} {
    global listboxinput listboxinputval
    eval "$listboxinput yview $args"
    eval "$listboxinputval yview $args"
}
proc scrollyleftandscrollbar_bp {x y} {
    global listboxscrolly listboxinput listboxinputval
    eval "$listboxscrolly set $x $y"
    set firstvisible [expr round([lindex [$listboxinputval yview] 0] \
                               * [$listboxinputval size])]
    eval "$listboxinput yview $firstvisible"
}
proc scrollyrightandscrollbar_bp {x y} {
    global listboxscrolly listboxinput listboxinputval
    eval "$listboxscrolly set $x $y"
    set firstvisible [expr round([lindex [$listboxinput yview] 0] \
                               * [$listboxinput size])]
    eval "$listboxinputval yview $firstvisible"
}
proc selectininputval_bp {} {
    global listboxinput listboxinputval
    set selecteditem [$listboxinputval curselection]
    if {$selecteditem != ""} {
        $listboxinputval selection clear $selecteditem
        $listboxinput selection set $selecteditem
    }
}
proc spinboxbuttoninvoke {} {
    global spin listboxinput listboxinputval funvars funvarsvals
    $listboxinput delete 0 [$listboxinput size]
    $listboxinputval delete 0 [$listboxinputval size]
    set funname [$spin get]
    if {[info exists funvars($funname)]} {
        foreach var $funvars($funname) {
            $listboxinput insert end $var
            $listboxinputval insert end $funvarsvals($funname,$var)
        }
    $listboxinput selection set 0
    $listboxinput see 0
    }
}

proc scrollarrows_bp {dir} {
    global listboxinput
    set ind [$listboxinput curselection]
    if {$ind != ""} {
        if {$dir == "down"} {
            if {$ind < [expr [$listboxinput size] - 1]} {
                $listboxinput selection clear $ind
                $listboxinput selection set [expr $ind + 1]
                $listboxinput see [expr $ind + 1]
            }
        } else {
            if {$ind > 0} {
                $listboxinput selection clear $ind
                $listboxinput selection set [expr $ind - 1]
                $listboxinput see [expr $ind - 1]
            }
        }
    }
}

set curdropind 0

proc dragitem_bp {dragpos} {
    global listboxinput curdropind
    set se [$listboxinput curselection]
    if {$se != "" } {
        $listboxinput selection clear $se
        $listboxinput selection set @0,$dragpos
        set curdropind [$listboxinput curselection]
        bind $listboxinput <Motion> {set curdropind [showdroppos_bp $curdropind %y]}
    }
    return [$listboxinput curselection]
}

proc dropitem_bp {dragind droppos} {
    global listboxinput listboxinputval spin funvars curdropind
    if {$dragind != "" } {
        set funname [$spin get]
        set dropind [$listboxinput index @0,$droppos]
        set dragitem [$listboxinput get $dragind]
        if {$dragind != $dropind} {
            $listboxinput insert $dropind $dragitem
            $listboxinputval insert $dropind [$listboxinputval get $dragind]
            set funvars($funname) [linsert $funvars($funname) $dropind $dragitem]
            if {$dropind < $dragind} {
                incr dragind
                incr curdropind
            } else {
                incr dropind -1
            }
            $listboxinput delete $dragind
            $listboxinput selection set $dropind
            $listboxinputval delete $dragind
            set funvars($funname) [lreplace $funvars($funname) $dragind $dragind]
        }
        $listboxinput itemconfigure $curdropind -background white
        bind $listboxinput <Motion> {}
    }
}

proc showdroppos_bp {oldcurdropind droppos} {
    global listboxinput
    if {$droppos < 1} {
        if {$oldcurdropind > 0} {
            $listboxinput see [expr $oldcurdropind - 1]
        }
    }
# $maxi computation is unperfect, but should work in most cases
# The best: to get $listboxinput height in pixels - how?
    set maxi [$listboxinput bbox $oldcurdropind]
    set maxi [expr [lindex $maxi 3] + 1]
    set maxi [expr $maxi * [$listboxinput cget -height] + 4]
    if {$droppos > $maxi} {
        if {$oldcurdropind < [expr [$listboxinput size] - 1]} {
            $listboxinput see [expr $oldcurdropind + 1]
        }
    }
    set dropind [$listboxinput index @0,$droppos]
    if {$oldcurdropind != $dropind} {
        $listboxinput itemconfigure $oldcurdropind -background white
        $listboxinput itemconfigure $dropind -background lightblue
    }
    return $dropind
}

proc OKconf_bp {w} {
    global listboxinput listboxinputval spin
    global funnameargs
    set funname [$spin get]
    if {$funname != ""} {
        set orderOK [checkarglist $funname]
        if {$orderOK == true} {
            set strargs ""
            for {set i 0} {$i < [$listboxinput size]} {incr i} {
                set argvalue [$listboxinputval get $i]
                set strargs "$strargs,$argvalue"
            }
            set strargs [string range $strargs 1 end]
            set funnameargs "$funname\($strargs\)"
        }
    } else {
        set funnameargs ""
    }
    destroy $w
}

proc checkarglist {funname} {
# Because the user could add input variables (in the buffer text) to the
# currently selected function, checking the argument list cannot just
# rely on the latest Obtainall_bp
    global listoftextarea funvars lang
    set pat "\\mfunction\\M.*\\m$funname\\M"
    set orderOK false
    foreach textarea $listoftextarea {
        set ex [$textarea search -regexp $pat 0.0 end]
        if {$ex != "" } {
            while {[lsearch [$textarea tag names $ex] "textquoted"] != -1 || \
                   [lsearch [$textarea tag names $ex] "rem2"] != -1 } {
                set ex [$textarea search -regexp $pat "$ex+8c" end]
                if {$ex == ""} break
            }
            if {$ex != "" } {
                set infun [whichfun [$textarea index "$ex +1l"] $textarea]
                set funline [lindex $infun 2]
                set oppar [string first "\(" $funline]
                set clpar [string first "\)" $funline]
                set listvars [string range $funline [expr $oppar+1] [expr $clpar-1]]
                set listvars [string map {, " "} $listvars]
                set orderOK true
                set i 0
                foreach var $funvars($funname) {
                    if {$var != [lindex $listvars $i]} {
                        set orderOK false
                        break
                    } else {
                        incr i
                    }
                }
            }
        }
    }
    if {$orderOK != true } {
        if {$lang == "eng" } {
            set mes "Function name or input arguments do not match definition\
                     of the function $funname in the file!\n\nCheck function\
                     name and arguments (names, order) in the configuration dialog.\
                     \nArguments order can be changed using drap and drop with\
                     right mouse button in the arguments listbox."
            set tit "Error in selected function name or arguments"
        } else {
            set mes "Le nom de la fonction ou ses arguments ne correspondent pas\
                     à la définition de la fonction $funname dans le fichier!\n\n\
                     Vérifier le nom de la fonction et ses arguments (nom, ordre\
                     d'apparition) dans la boîte de dialogue de configuration.\n\
                     L'ordre des arguments peut être modifié par glisser/déposer\
                     avec le bouton droit de la souris."
            set tit "Erreur sur la fonction sélectionnée ou ses arguments"
        }
        tk_messageBox -message $mes -icon warning -title $tit
    }
    return $orderOK
}

#proc Cancelconf_bp {w} {
# Better always close the window with OK button. Saves variables management.
#    destroy $w
#}

proc Obtainall_bp {} {
    global spin listboxinput listboxinputval funnames funvars funvarsvals
    global funsinbuffer
    set textarea [gettextareacur]
    set funsinbuffer($textarea) ""
    set nextfun [$textarea search -exact -forwards -regexp\
                 "\\mfunction\\M" 0.0 end ]
    set firstfuninbuffer true
    while {$nextfun != ""} {
        while {[lsearch [$textarea tag names $nextfun] "textquoted"] != -1 || \
               [lsearch [$textarea tag names $nextfun] "rem2"] != -1 } {
            set nextfun [$textarea search -exact -forwards -regexp\
                         "\\mfunction\\M" "$nextfun +8c" end]
            if {$nextfun == ""} break
        }
        if {$nextfun != ""} {
            set infun [whichfun [$textarea index "$nextfun +1l"]]
        } else {
            set infun {}
        }
        if {$infun != {} } {
            $spin configure -state normal
            $spin delete 0 end
            set funname [lindex $infun 0]
            set precval [$spin cget -values]
            if {[lsearch $precval $funname] == -1} {
                $spin configure -values "$precval $funname"
            }
            $spin configure -state readonly
            if {$firstfuninbuffer == true} {
                set firstfuninbuffer false
                set funtoset $funname
            }
            set funline [lindex $infun 2]
            $listboxinput delete 0 [$listboxinput size]
            $listboxinputval delete 0 [$listboxinputval size]
            set oppar [string first "\(" $funline]
            set clpar [string first "\)" $funline]
            set listvars [string range $funline [expr $oppar+1] [expr $clpar-1]]
            set listvars [string map {, " "} $listvars]
            foreach var $listvars {
                $listboxinput insert end $var
                set funvarsvals($funname,$var) ""
                $listboxinputval insert end $funvarsvals($funname,$var)
            }
            $listboxinput selection set 0
            $listboxinput see 0
            set funvars($funname) $listvars
            set funsinbuffer($textarea) "$funsinbuffer($textarea) $funname"
            set nextfun [$textarea search -exact -forwards -regexp\
                         "\\mfunction\\M" "$nextfun +8c" end]
        }
    }
    set funnames [$spin cget -values]
    if {[info exists funtoset]} {
        $spin set $funtoset
    } else {
        $spin set [lindex $funnames 0]
    }
    spinboxbuttoninvoke
    $listboxinput see 0
}

proc Addarg_bp {} {
    global conf lang
    global argname argvalue listboxinput listboxinputval spin
    global buttonAdd
    set pos [$listboxinput curselection]
    if {[$spin get] != ""} {
        set adda $conf.adda
        toplevel $adda
        if {$lang == "eng"} {
            wm title $adda "Add argument"
        } else {
            wm title $adda "Ajouter un argument"
        }
        setwingeom $adda
        set selecteditem [$listboxinput curselection]
        if {$selecteditem != ""} {
            set argname [$listboxinput get $selecteditem]
            set argvalue [$listboxinputval get $selecteditem]
        }
        frame $adda.f
        frame $adda.f.f1
        if {$lang == "eng"} {
            set tl "Argument:"
        } else {
            set tl "Argument:"
        }
        label $adda.f.f1.label -text $tl -width 10
        entry $adda.f.f1.entry  -textvariable argname -width 20 
        pack $adda.f.f1.label $adda.f.f1.entry -side left
        $adda.f.f1.entry selection range 0 end
        pack $adda.f.f1
        frame $adda.f.f2
        if {$lang == "eng"} {
            set tl "Value:"
        } else {
            set tl "Valeur:"
        }
        label $adda.f.f2.label -text $tl -width 10
        entry $adda.f.f2.entry  -textvariable argvalue -width 20 
        pack $adda.f.f2.label $adda.f.f2.entry -side left
        $adda.f.f2.entry selection range 0 end
        pack $adda.f.f2
        frame $adda.f.f9
        button $adda.f.f9.buttonOK -text "OK" -command "OKadda_bp $adda $pos"\
               -width 10 -height 1 -underline 0
        if {$lang == "eng"} {
            set bl "Cancel"
        } else {
            set bl "Annuler"
        }
        button $adda.f.f9.buttonCancel -text $bl -command "Canceladda_bp $adda $pos"\
               -width 10 -underline 0
        pack $adda.f.f9.buttonOK $adda.f.f9.buttonCancel -side left -padx 10
        pack $adda.f.f9 -pady 4
        pack $adda.f
        bind $adda <Return> "OKadda_bp $adda $pos"
        bind $adda <Escape> "Canceladda_bp $adda $pos"
        focus $adda.f.f2.entry
        grab $adda
    }
    focus $buttonAdd
}

proc OKadda_bp {w pos} {
    global argname argvalue listboxinput listboxinputval
    global spin funvars funvarsvals
    if {$argname!= ""} {
        set listboxinputelts [$listboxinput get 0 end]
        set alreadyexists false
        set eltindex 0
        foreach elt $listboxinputelts {
            if {$argname == $elt} {
                set alreadyexists true
                break
            } else {
                incr eltindex
            }
        }
        set funname [$spin get]
        if {$alreadyexists == false} {
            set pos [expr $pos + 1]
            $listboxinput insert $pos $argname
            $listboxinputval insert $pos $argvalue

            $listboxinput selection set $pos
            $listboxinput see $pos
            set funvars($funname) [linsert $funvars($funname) $pos $argname]
       } else {
            set nextone [expr $eltindex + 1]
            if {$nextone >= [$listboxinput size]} {
                set nextone 0
            }
            $listboxinput selection set $nextone
            $listboxinput see $nextone
            $listboxinputval delete $eltindex
            $listboxinputval insert $eltindex $argvalue
        }
        set funvarsvals($funname,$argname) $argvalue
    }
    destroy $w
}

proc Canceladda_bp {w pos} {
    global listboxinput
    destroy $w
    $listboxinput selection set $pos
}

proc Removearg_bp {} {
    global listboxinput listboxinputval spin
    global funvars funvarsvals
    set selecteditem [$listboxinput curselection]
    if {$selecteditem != ""} {
        set funname [$spin get]
        set argname [$listboxinput get $selecteditem]
        unset funvarsvals($funname,$argname)
        set funvars($funname) [lreplace $funvars($funname) \
                                        $selecteditem $selecteditem]
        $listboxinput delete $selecteditem
        $listboxinput see $selecteditem
        $listboxinputval delete $selecteditem
        if {$selecteditem < [expr [$listboxinput size] ]} {
            $listboxinput selection set $selecteditem
        } else {
            $listboxinput selection set [expr [$listboxinput size] - 1]
        }
    }
}

proc removefuns_bp {textarea} {
    global funsinbuffer funvars funvarsvals funnames funnameargs
    if {[info exists funsinbuffer($textarea)]} {
        foreach fun $funsinbuffer($textarea) {
            if {[info exists funvars($fun)]} {
                foreach arg $funvars($fun) {
                    unset funvarsvals($fun,$arg)
                }
                unset funvars($fun)
            }
            set pos [lsearch $funnames $fun]
            set funnames [lreplace $funnames $pos $pos]
            set oppar [expr [string first "\(" $funnameargs] - 1]
            set curfunname [string range $funnameargs 0 $oppar]
            if {$curfunname == $fun} {
                set funnameargs "[lindex $funnames 0]("
            }
        }
        if {$funnameargs != "("} {
            set funname [string range $funnameargs 0 [expr [string length $funnameargs] - 2]]
            set strargs ""
            if {[info exists funvars($funname)]} {
                foreach var $funvars($funname) {
                    set argvalue $funvarsvals($funname,$var)
                    set strargs "$strargs,$argvalue"
                }
                set strargs [string range $strargs 1 end]
            }
            set funnameargs "$funname\($strargs\)"
        } else {
            set funnameargs ""
        }
    }
}

#######################################################################
##################### End of breakpoint stuff #########################
#######################################################################

# help menu
# FV 13/05/04, -accelerator Shift+F1 uncommented
menu $pad.filemenu.help -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Help" -underline 0 \
	-menu $pad.filemenu.help
    $pad.filemenu.help add command -label "About" -underline 0 \
	-command "aboutme" -accelerator Shift+F1
    $pad.filemenu.help add command -label "Help" -underline 0 \
        -command "helpme" -accelerator F1
    $pad.filemenu.help add command -label "What's?" -underline 0 \
	-command "helpword" -accelerator Ctrl+F1
} else {
    $pad.filemenu add cascade -label "Aide" -underline 0 \
	-menu $pad.filemenu.help
    $pad.filemenu.help add command -label "A propos" -underline 1 \
	-command "aboutme" -accelerator Shift+F1
    $pad.filemenu.help add command -label "Aide" -underline 0 \
        -command "helpme" -accelerator F1
    $pad.filemenu.help add command -label "Qu'est-ce ?" -underline 0 \
	-command "helpword" -accelerator Ctrl+F1
}

# now make the menu visible
$pad configure -menu $pad.filemenu 

#create textarea
set taille [expr [font measure $textFont " "] *3]

# creates the default textarea 
##ES was here: added insertofftime 0 and exportselection
## Francois Vogel, 21/04/04: changed insertofftime to 500, and added insertontime 500
## FV 13/05/04: added test with tk_version on ES request
text $pad.textarea -relief sunken -bd 2 -xscrollcommand "$pad.xscroll set" \
	-yscrollcommand "$pad.yscroll set" -wrap $wordWrap -width 1 -height 1 \
        -fg $FGCOLOR -bg $BGCOLOR  -setgrid 1 -font $textFont -tabs $taille \
        -insertwidth 3 -insertborderwidth 2 \
        -insertbackground $CURCOLOR -selectbackground $SELCOLOR -exportselection 1
set textareacur $pad.textarea  
if {[expr $tk_version] >= 8.4} {
    $textareacur configure -insertofftime 500 -insertontime 500
} else {
    $textareacur configure -insertofftime 0
}
####
##ES: remember fontsize
setfontscipad $FontSize

scrollbar $pad.yscroll -command "$textareacur yview"
scrollbar $pad.xscroll -command "$textareacur xview" -orient horizontal
pack $textareacur  -in  $pad.bottomleftmenu -side left -expand 1 -fill both
pack $pad.yscroll -in $pad.bottomrightmenu -side right -expand 1 -fill both
pack $pad.xscroll -in $pad.bottombottommenu -expand 1 -fill x 
focus $textareacur

##ES: this comes from a more recent version of Tknotepad: 0.7.8
proc showpopup2 {} {
    global pad
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    tk_popup $pad.filemenu.edit $numx $numy
}
proc showpopup3 {} {
    global pad
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    tk_popup $pad.filemenu.exec $numx $numy
}
proc showpopupfont {} {
    global pad
    set numx [winfo pointerx .]
    set numy [winfo pointery .]
    tk_popup $pad.filemenu.options $numx $numy
}


# added by matthieu PHILIPPE 21/11/2001
proc gettextareacur {} {
    global textareacur
    return $textareacur
}
proc settextareacur {val} {
    global textareacur
    set textareacur $val
}

# nondefOpts - return a list of options with values 
#              that are different than the default value.
#
# ARGUMENTS:
#	widget	the widget whose options are to be constructed
#
# RETURN VALUE:
#	a list with option/value pairs
#
# NOTES
#	The configure message to a widget returns either an option
#	description:
#		{argvName dbName dbClass defValue curValue}
#	or a synonym description:
#		{argvName dbName}
#
proc nondefOpts {widget} {
	set options {}
	foreach i [$widget configure] {
		if {[llength $i] != 2} {
			set optname [lindex $i 0]
			set defval [lindex $i 3]
			set curval [lindex $i 4]
			#if [string compare $defval $curval] {
				lappend options $optname $curval
			#}
		}
	}
	return $options
}

#
#
# dupWidget - make a copy of a widget.
#
# ARGUMENTS:
#	widget	the widget to be duplicated
#	name	the name for the new widget
#
# RETURN VALUE:
#	a new widget
#
proc dupWidgetOption {widget name} {
	return [eval  \
           "[string tolower [winfo class $widget]] $name [nondefOpts $widget]"]
}

#
#
#matthieu PHLIPPE 12/12/2001
proc dupWidgetText {widget name} {
	$name insert end [$widget get 1.0 end]
}
##

###ES: who uses this, besides TextStyles which sets it? 
# set the indent spacing (in cm) for lists
# TK uses a "weird" tabbing model that causes \t to insert a single
# space if the current line position is past the tab setting
proc scipadindent {textarea cm} {
    set tabs [expr $cm / 2.0]
    $textarea configure -tabs ${tabs}c
    set tab [expr 1 * $cm]
    $textarea tag configure indentation -lmargin1 ${tab}c -lmargin2 ${tab}c \
	    -tabs "[expr $tab + $tabs]c [expr $tab + 2*$tabs]c"
}

proc TextStyles { t } {
    global FGCOLOR PARCOLOR BRAKCOLOR BRACCOLOR PUNCOLOR KEYWCOLOR OPCOLOR
    global TXTCOLOR QTXTCOLOR REMCOLOR XMLCOLOR NUMCOLOR
    global BREAKPOINTCOLOR

    $t tag configure parenthesis -foreground $PARCOLOR
    $t tag configure bracket -foreground $BRAKCOLOR
    $t tag configure brace -foreground $BRACCOLOR
    $t tag configure punct -foreground $PUNCOLOR
    $t tag configure keywords -foreground $KEYWCOLOR
    $t tag configure operator -foreground $OPCOLOR
    $t tag configure text -foreground $FGCOLOR
    $t tag configure textquoted -foreground $QTXTCOLOR
    $t tag configure rem2 -foreground $REMCOLOR
    $t tag configure xmltag -foreground $XMLCOLOR
    $t tag configure number -foreground $NUMCOLOR
    scipadindent $t .8
# FV 13/05/04
    $t tag configure breakpoint -background $BREAKPOINTCOLOR
    $t tag configure activebreakpoint -underline true
}
TextStyles $textareacur
######################

# added by Matthieu PHILIPPE 07/12/2001
# this addes an entry widget to display information !
proc delinfo {} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes delete 0 end 
    $pad.statusmes configure -state disabled
}

proc showinfo {message} {
    global pad
    $pad.statusmes configure -state normal
    $pad.statusmes delete 0 end 
    $pad.statusmes insert 0 "$message"
    $pad.statusmes configure -state disabled
    after 5000 catch delinfo
}

proc blinkbrace {w pos brace} {
	global	bracefind fno lang

	switch $brace {
		\{	{ set findbs {[{}]}; set bs "\}";\
                              set dir {-forwards}; set bn brace }
		\}	{ set findbs {[{}]}; set bs "\{"; \
                              set dir {-backwards}; set bn brace }
		\[	{ set findbs {[][]}; set bs "\]"; \
                              set dir {-forwards}; set bn bracket }
		\]	{ set findbs {[][]}; set bs "\["; \
			      set dir {-backwards}; set bn bracket }
		\(	{ set findbs {[()]}; set bs "\)"; set dir {-forwards};\
			      set bn parenthesis }
		\)	{ set findbs {[()]}; set bs "\("; \
                          set dir {-backwards}; set bn parenthesis }
	}
	set p [set i [$w index $pos-1c]]
	set d 1
	while {$d > 0} {
		if {$dir == "-backwards"} {
			set p [$w search $dir -regexp $findbs $p 1.0]
		} else {
			set p [$w search $dir -regexp $findbs $p+1c end]
		}
		if {$p == {}} {
			set d -1
		} else {
			if {"[$w get $p $p+1c]" == "$bs"} {
				incr d -1
			} else {
				incr d 1
			}
		}
	}
	if {$d == 0} {
		if {$dir == "-backwards"} {
			$w tag add sel $p $pos
		} else {
			$w tag add sel $pos-1c $p+1c
		}
		$w see $p
		update idletasks
		after 300
		$w tag remove sel 1.0 end
		selection clear
		$w see insert
	} else {
            if {$lang == "eng"} {
                showinfo "No corresponding <$bs> found!"
            } else {
                showinfo "Aucun <$bs> correspondant trouvé !"
            }
	}
}

proc insblinkbrace {w brace} {
    puttext $w $brace
    blinkbrace $w insert $brace 
}

proc insquote {w quote} {
    $w insert insert $quote {textquoted}
}

########


# Added by Matthieu PHILIPPE 07/12/2001
# code coms from EDDI.tcl
proc insertnewline {w} {
    set n {}
    $w mark set p1 {insert linestart}
    set c [$w get p1 {p1+1c}]
    while {$c == { } || $c == "\t"} {
	$w mark set p1 {p1+1c}
	set c [$w get p1 {p1+1c}]
    }
    set n [$w get {insert linestart} p1]
    $w mark unset p1
    puttext $w "\n$n"
    set c [$w get insert "insert+1c"]
    while {$c == " " || $c == "\t"} {
	$w delete insert "insert+1c"
	set c [$w get insert "insert+1c"]
    }
}
### 

#############################################
#to get the line number
#included by Matthieu PHILIPPE 21/11/2001 from linenum.pth
set colormen [$pad.filemenu cget -background]
entry $pad.statusind -relief groove -state disabled -background $colormen
# added by Matthieu PHILIPPE 07/12/2001
# this addes an entry widget to dsplay information !
entry $pad.statusmes -relief groove -state disabled -background $colormen \
    -foreground blue
pack $pad.statusind -in $pad.bottombottommenu -side right -expand 0
pack $pad.statusmes -in $pad.bottombottommenu -side bottom -expand 0 -fill x

# this proc gets the posn and sets the statusbar
proc keyposn {textarea} {
    global pad lang
    $pad.statusind configure -state normal
    set indexin [$textarea index insert]
    $pad.statusind delete 0 end 
    # changement Matthieu PHILIPPE 30/11/2001
    scan $indexin "%d.%d" ypos xpos
    if {$lang == "eng"} {
        $pad.statusind insert 0 "Line: $ypos   Column: $xpos"
    } else {
        $pad.statusind insert 0 "Ligne: $ypos   Colonne: $xpos"
    }
    $pad.statusind configure -state disabled
}

###############


# this proc just sets the title to what it is passed
proc settitle {WinTitleName} {
    global winTitle fileName 
    global pad
    wm title $pad "$winTitle - $WinTitleName"
    set fileName $WinTitleName
}

# proc to open files or read a pipe
proc openoninit {textarea thefile} {
    global listoftextarea
    global lang

    if {$lang == "eng"} {
	set msgWait "Wait seconds while loading and colorizing file"
    } else {
	set msgWait \
         "Patientez un instant le temps du chargement et de la colorisation"
    }
    showinfo $msgWait
    lappend listoftextarea $textarea
    if [string match " " $thefile] {  
        fconfigure stdin -blocking 0
        set incoming [read stdin 1]
        if [expr [string length $incoming] == 0] {
            fconfigure stdin -blocking 1
        } else {
            fconfigure stdin -blocking 1
            $textarea insert end $incoming
            while {![eof stdin]} {
                $textarea insert end [read -nonewline stdin]
            }
        }
    } else {
        if [ file exists $thefile ] {
            set newnamefile [open $thefile r]
        } else {
            set newnamefile [open $thefile a+]
        }
        while {![eof $newnamefile]} {
	       $textarea insert end [read -nonewline $newnamefile ] 
        }
        close $newnamefile
        settitle $thefile
    }
    #colorize $textarea 1.0 end
}
	    
#added by Matthieu PHILIPPE 21/11/2001 from linenum.pth
$textareacur mark set insert "1.0"
#tkTextSetCursor $textareacur "1.0"
keyposn $textareacur
##geometry in what units? The width is more than 65 columns, though it's 
## resized proportionally
#wm geometry $pad 65x24 




# help menu
proc helpme {} {
    global lang
    global textFont
#    if {$lang == "eng"} {
#	tk_messageBox -title "Basic Help" -type ok \
#            -message "This is a simple editor for Scilab."
#    } else {
#	tk_messageBox -title "Aide de base" -type ok \
#            -message "C'est un simple éditeur pour Scilab."
#    }
##ES:
    ScilabEval "help scipad"
}

# about menu
proc aboutme {} {
    global winTitle version lang
    if {$lang == "eng"} {
	tk_messageBox -title "About" -type ok \
	    -message "$winTitle $version\n\
            Originated by Joseph Acosta, joeja@mindspring.com.\n\
            Modified by Scilab Consortium.\n\
            Revised by Enrico Segre 2003,2004.\n\
            Miscellaneous improvements, particularly debug tools, by François Vogel 2004."
    } else {
	tk_messageBox -title "A propos" -type ok \
	    -message "$winTitle $version\n\
            Créé par Joseph Acosta, joeja@mindspring.com.\n\
            Modifié par le Consortium Scilab.\n\
            Amélioré par Enrico Segre 2003,2004.\n\
            Améliorations diverses, dont outils de débug, par François Vogel 2004."
    }
}

#ES:
proc helpword {} {
    global textareacur
    set seltexts [selection own]
    if {[catch {selection get -selection PRIMARY} sel] ||$seltexts != $textareacur} {
#if there is no selection in the current textarea, select the word at the cursor position
           set i1 [$textareacur index insert]
           $textareacur tag add sel [$textareacur index "$i1 wordstart"] \
                                    [$textareacur index "$i1 wordend"]
           set curterm [selection get]
	} else {
           set cursel [selection get]
# get only the first word of the selection (or a symbol)
           regexp "(\\A\\w*\\M|\\A\\W)" $cursel curterm
	}
        if {[info exists curterm]} { ScilabEval "help $curterm" }	
}


# generic case switcher for message box
proc switchcase {yesfn argyesfn nofn argnofn} {
    global saveTextMsg lang
    if [ expr [string compare [getccount [gettextareacur]] 1] ==0 ] {
	if {$lang == "eng"} {
	    set answer [tk_messageBox -message \
             "The contents of this file may have changed, do you wish\
              to save your changes?"\
		    -title "Save Confirm?" -type yesnocancel -icon question]
	} else {
	    set answer [tk_messageBox -message \
                "Voulez-vous enregistrer les modifications apportées à\
                  ce fichier ?" \
		  -title "Confirmer sauver ?" -type yesnocancel -icon question]
	}
	case $answer {
	     yes { if {[eval $yesfn $argyesfn] == 1} { $nofn $argnofn} }
             no {$nofn  $argnofn}
	     cancel {}
	}
    } else {
   	$nofn $argnofn
    }
}

# new file
# Matthieu PHILIPPE 14/12/2001
proc filesetasnewmat {} {
    global winopened
    global listoffile
    global tcl_platform
    global fileName
    global listundo_id
    global listoftextarea
    global pad
    global radiobuttonvalue
    global lang

    incr winopened
    dupWidgetOption [gettextareacur] $pad.new$winopened
    set listoffile("$pad.new$winopened",filename) [pwd]/Untitled$winopened.sce
    set listoffile("$pad.new$winopened",save) 0
    set listoffile("$pad.new$winopened",new) 1
    set listoffile("$pad.new$winopened",thetime) 0
    set listundo_id("$pad.new$winopened") [new textUndoer $pad.new$winopened]
    lappend listoftextarea $pad.new$winopened
    bind $pad.new$winopened <KeyRelease> {keyposn %W}
    bind $pad.new$winopened <ButtonRelease> {keyposn %W}
    TextStyles $pad.new$winopened
    $pad.filemenu.wind add radiobutton -label "[pwd]/Untitled$winopened.sce" \
	-value $winopened -variable radiobuttonvalue \
	-command "montretext $pad.new$winopened"
    set radiobuttonvalue $winopened
    if {$lang == "eng"} {
        showinfo "New Script"
    } else {
        showinfo "Nouveau script"
    }
    montretext $pad.new$winopened
    set fileName " "
    #inccount $pad.new$winopened
    selection clear
}

# new file
proc filesetasnew {textarea} {
	switchcase filetosave $textarea setTextTitleAsNew $textarea
}

proc setTextTitleAsNew {textarea} {
	global winTitle fileName pad
	#$textarea delete 0.0 end
	set fileName " "
	wm title $pad $winTitle
	outccount $textarea
}

# kill main window
proc killwin {widget} {
	destroy $widget
}

# close app
# added by matthieu PHILIPPE 16/12/2001
proc closecur {} {
    removefuns_bp [gettextareacur]
    closefile [gettextareacur]
}

proc closefile {textarea} {
    global listoftextarea
    global listoffile
    global pad
    global lang

    proc byebye {textarea} {
	global listoftextarea
	global listoffile
	global pad winopened radiobuttonvalue wm WMGEOMETRY
	if {  [ llength $listoftextarea ] > 1 } {
	    # delete the textarea entry in the listoftextarea
	    set listoftextarea [lreplace $listoftextarea [lsearch \
		   $listoftextarea $textarea] [lsearch $listoftextarea \
						   $textarea]]
	    # delte the menu windows entry
	    $pad.filemenu.wind delete [$pad.filemenu.wind index \
					   $listoffile("$textarea",filename)]
	    # delete the textarea entry in listoffile
	    unset listoffile("$textarea",filename) 
	    unset listoffile("$textarea",save) 
	    unset listoffile("$textarea",new) 
	    unset listoffile("$textarea",thetime) 
	    # delete the textarea entry in Undoerr
##ES: the text widget was opened for $pad.textarea. If that is destroyed, the 
## next change font, new file or open file command causes the window to shrink.
## On the other side, the only side
## effect of not destroying it which I see is that font changes do not resize
## the window (so what? -- the resizing was not exact anyway)
#	    destroy $textarea
	    # place as current textarea the last 
	    montretext [lindex $listoftextarea end]
##ES 30/9/03 
            set lastwin [$pad.filemenu.wind entrycget end -value]
            set radiobuttonvalue $lastwin
##
	} else {
#ES 18/10/2003: save the geometry for the next time
            set WMGEOMETRY [eval {wm geometry $pad}] 
	    killwin $pad 
            unset pad
	}   
    }
    #### end of bye bye

    if  [ expr [string compare [getccount $textarea] 1] == 0 ] {
	if {$lang == "eng"} {
	    set answer [tk_messageBox -message "The contents of \
               $listoffile("$textarea",filename) may have changed, do you wish\
                   to to save your changes?"\
		       -title "Save Confirm?" -type yesnocancel -icon question]
	} else {
	    set answer [tk_messageBox -message "Voulez-vous enregistrer les \
               modifications apportées à $listoffile("$textarea",filename) ?" \
		 -title "Confirmer sauver ?" -type yesnocancel -icon question]
	}
	case $answer {
	    yes { filetosave $textarea; byebye $textarea }
	    no {byebye $textarea}
	    cancel {}
	}
    } else {
	byebye $textarea
    }
}

# exit app
proc exitapp {} {
    global listoffile
    global listoftextarea
    global pad

    foreach textarea $listoftextarea {
	#inccount $textarea
	closefile $textarea
    }
#    unset pad  ## not needed if closefile unsets it
}

proc montretext {textarea} {
#    global listoffile
    global pad
# Next line added by Francois VOGEL 16/04/04, to fix the silent clearing of 
# selection
    selection clear
    pack forget [gettextareacur]
    settextareacur $textarea
    $pad.yscroll configure -command "[gettextareacur] yview"
    $pad.xscroll configure -command "[gettextareacur] xview"
# Next two lines added by Francois VOGEL, 21/04/04, to fix size/position bug of sliders
    $pad.xscroll set [lindex [[gettextareacur] xview] 0] [lindex [[gettextareacur] xview] 1]
    $pad.yscroll set [lindex [[gettextareacur] yview] 0] [lindex [[gettextareacur] yview] 1]
#    settitle $listoffile("$textarea",filename)
    modifiedtitle $textarea
    pack  $textarea -in  $pad.bottomleftmenu -side left -expand 1 -fill both
    focus $textarea
    keyposn $textarea
}

# Francois VOGEL, 16/04/04, corrected and expanded by ES
proc nextbuffer {} {
    global pad listoftextarea listoffile radiobuttonvalue
    set textarea [gettextareacur]
    set curbuf [$pad.filemenu.wind index $listoffile("$textarea",filename)]    
    set curbuf [expr $curbuf+1]
    set nbuf [llength $listoftextarea]

    if {$curbuf>$nbuf} {
        set curbuf 1
    }
#    showinfo $curbuf
    set radiobuttonvalue [$pad.filemenu.wind entrycget $curbuf -value]
    montretext [lindex $listoftextarea [expr $curbuf-1]]
}

proc prevbuffer {} {
   global pad listoftextarea listoffile radiobuttonvalue
    set textarea [gettextareacur]
    set curbuf [$pad.filemenu.wind index $listoffile("$textarea",filename)]    
    set curbuf [expr $curbuf-1]
    set nbuf [llength $listoftextarea]

    if {$curbuf<1} {
        set curbuf $nbuf
    }
#    showinfo $curbuf
    set radiobuttonvalue [$pad.filemenu.wind entrycget $curbuf -value]
    montretext [lindex $listoftextarea [expr $curbuf-1]]
}


# bring up open win
# modified by Matthieu PHILIPPE 16/12/2001
# added Open multifiles
# added show an opened file
proc showopenwin {textarea} {
    global listoffile
    global FGCOLOR BGCOLOR textFont taille wordWrap 
    global pad winopened textareacur listoftextarea
    global tcl_platform listundo_id
    global radiobuttonvalue lang
    if {$lang == "eng"} {
	set types {
	    {"Scilab files" {*.sce *.sci }} 
	    {"XML files" {*.xml }} 
	    {"All files" {*.* *}}
	}
	#showinfo "Open file"
    } else {
	set types {
	    {"Fichiers Scilab" {*.sce *.sci }} 
	    {"Fichiers XML" {*.xml }} 
	    {"Tous les fichiers" {*.* *}}
	}
	#showinfo "Ouvrir le fichier"
    }
    set file [tk_getOpenFile -filetypes $types -parent $pad]
    #####
    if [string compare $file ""] {
	# search for a opened existing file
	if { [catch {$pad.filemenu.wind index $file} res]} {
	    # not opened file
	    incr winopened
	    dupWidgetOption [gettextareacur] $pad.new$winopened
	    set listoffile("$pad.new$winopened",filename) $file
	    set listoffile("$pad.new$winopened",new) 0
	    set listoffile("$pad.new$winopened",thetime) [file mtime $file]
#ES: the following line has to be brought here for modifiedtitle to work - what
# afterwards opening fails?
	    $pad.filemenu.wind add radiobutton -label "$file" \
		-value $winopened -variable radiobuttonvalue \
		-command "montretext $pad.new$winopened"
	    #####
	    setTextTitleAsNew $pad.new$winopened
	    montretext $pad.new$winopened
	    openoninit $pad.new$winopened $file
	    outccount $pad.new$winopened
	    update
	    colorize $pad.new$winopened 1.0 end
# FV 13/05/04
          reshape_bp
          showinfo " "
	    set listundo_id("$pad.new$winopened") \
		[new textUndoer $pad.new$winopened]
	    bind $pad.new$winopened <KeyRelease> {keyposn %W}
	    bind $pad.new$winopened <ButtonRelease> {keyposn %W}
	    TextStyles $pad.new$winopened
	    set radiobuttonvalue $winopened
	} else {
	    # file is already opened
            if {$lang == "eng"} {
  	       tk_messageBox -type ok -title "Open file" -message \
                   "This file is already opened! Save the current\
                    opened file to an another name and reopen\
                    it from disk!"
            } else {
  	       tk_messageBox -type ok -title "Ouvrir fichier" -message \
                  "Ce fichier est déjà ouvert ! Sauvez-le sous un\
                   autre nom et rouvrez-le à partir du disque !"
            }
	    $pad.filemenu.wind invoke $res
	}
	selection clear
    }
}
proc openfile {file} {
    global listoffile
    global FGCOLOR BGCOLOR textFont taille wordWrap 
    global pad winopened textareacur listoftextarea
    global tcl_platform listundo_id
    global radiobuttonvalue lang
    if [string compare $file ""] {
	# search for a opened existing file
	if { [catch {$pad.filemenu.wind index $file} res]} {
	    # not opened file
	    incr winopened
	    dupWidgetOption [gettextareacur] $pad.new$winopened
	    set listoffile("$pad.new$winopened",filename) $file
	    set listoffile("$pad.new$winopened",new) 0
#ES: the following line has to be brought here for modifiedtitle to work
	    $pad.filemenu.wind add radiobutton -label "$file" \
		-value $winopened -variable radiobuttonvalue \
		-command "montretext $pad.new$winopened"
	    if [ file exists $file ] {
		set listoffile("$pad.new$winopened",thetime) [file mtime $file]
		setTextTitleAsNew $pad.new$winopened
		montretext $pad.new$winopened
		openoninit $pad.new$winopened $file
		outccount $pad.new$winopened
		update
		colorize $pad.new$winopened 1.0 end
	    } else {
		set listoffile("$pad.new$winopened",thetime) 0
		setTextTitleAsNew $pad.new$winopened
##added ES 4/9/2003
                lappend listoftextarea $pad.new$winopened
		montretext $pad.new$winopened
#
		update
	    }
##added ES 11/9/2003
#            tkTextSetCursor $pad.new$winopened "1.0"
	    $pad.new$winopened mark set insert "1.0"
            keyposn $pad.new$winopened
#
	    set listundo_id("$pad.new$winopened") \
		[new textUndoer $pad.new$winopened]
	    bind $pad.new$winopened <KeyRelease> {keyposn %W}
	    bind $pad.new$winopened <ButtonRelease> {keyposn %W}
	    TextStyles $pad.new$winopened
	    set radiobuttonvalue $winopened
	} else {
	    # file is already opened
            if {$lang == "eng"} {
  	       tk_messageBox -type ok -title "Open file" -message \
                   "This file is already opened! Save the current opened\
                    file to an another name and reopen it from disk!"
            } else {
  	       tk_messageBox -type ok -title "Ouvrir fichier" -message \
               "Ce fichier est déjà ouvert! Sauvez-le sous un\
                autre nom et rouvrez-le à partir du disque !"
            }
	}
	selection clear
    }
}


#open an existing file
proc filetoopen {textarea} {
    #switchcase filetosave $textarea showopenwin $textarea
    showopenwin $textarea
##added ES 11/9/2003
#    tkTextSetCursor [gettextareacur] "1.0"
    [gettextareacur] mark set insert "1.0"
    keyposn [gettextareacur]
#
}

# generic save function
proc writesave {textarea nametosave} {
    global lang
    set FileNameToSave [open $nametosave w+]
    puts -nonewline $FileNameToSave [$textarea get 0.0 end]
    close $FileNameToSave
    outccount $textarea
    if {$lang == "eng"} {
	set msgWait "File $nametosave saved"
    } else {
	set msgWait \
         "Fichier $nametosave sauvegardé"
    }
    showinfo $msgWait
}

#save a file
#
#added by Matthieu PHILIPPE 14/12/2001
proc filetosavecur {} {
    filetosave [gettextareacur]
}

proc filetosave {textarea} {
    global listoffile
    global lang

    if {$lang == "eng"} {
	set msgChanged "The contents of $listoffile("$textarea",filename) \
            has changed on Disk, Save it anyway?"
	set msgTitle "File has changed!"
    } else {
	set msgChanged "Le contenu de $listoffile("$textarea",filename) a\
            changé sur le disque, êtes-vous sûr de vouloir le sauvegarder ?"
	set msgTitle "Le fichier a changé"
    }

    # save the opened file from disk, if not, user has to get a file name.
    # we would verify if the file has not been modify by another application
    if { [file exists $listoffile("$textarea",filename)] && \
	     $listoffile("$textarea",new) == 0  } {
	if { $listoffile("$textarea",thetime) != [file mtime \
		    $listoffile("$textarea",filename)]} {
	    set answer [tk_messageBox -message $msgChanged -title $msgTitle \
			    -type yesnocancel -icon question]
	    case $answer {
		yes { writesave $textarea $listoffile("$textarea",filename);
                       set listoffile("$textarea",thetime) \
                             [file mtime $listoffile("$textarea",filename)]}
		no {}
		cancel {}
	    }
	} else {  
	    writesave $textarea $listoffile("$textarea",filename)
	    set listoffile("$textarea",thetime) \
		[file mtime $listoffile("$textarea",filename)] 
	}
	return 1
    } else {
	return [eval filesaveas $textarea]
    }
}

#save a file as
#
#added by Matthieu PHILIPPE 14/12/2001
proc filesaveascur {} {
    filesaveas [gettextareacur]
}

proc filesaveas {textarea} {
    global listoffile
    global pad
    global radiobuttonvalue
    global winopened
    global lang

    if {$lang == "eng"} {
	set types {
	    {"Scilab files" {*.sce *.sci }} 
	    {"XML files" {*.xml }} 
	    {"All files" {*.* *}}
	}
	showinfo "Save As"
    } else {
	set types {
	    {"Fichiers Scilab" {*.sce *.sci }} 
	    {"Fichiers XML" {*.xml }} 
	    {"Tous les fichiers" {*.* *}}
	}
	showinfo "Enregistrer sous"
    }
    set myfile [tk_getSaveFile -filetypes $types -parent $pad \
		    -initialfile $listoffile("$textarea",filename)]
    if { [expr [string compare $myfile ""]] != 0} {
       $pad.filemenu.wind delete "$listoffile("$textarea",filename)"
	set listoffile("$textarea",filename) $myfile
	set listoffile("$textarea",new) 0
	set listoffile("$textarea",save) 0
##ES: brought up for modifiedtitle to work
	$pad.filemenu.wind add radiobutton -label "$myfile" \
	   -value $radiobuttonvalue -variable radiobuttonvalue \
	   -command "montretext $textarea"
	writesave  $textarea $myfile
	set listoffile("$textarea",thetime) \
	   [file mtime $listoffile("$textarea",filename)] 
	settitle $myfile
        return 1
    }
    return 0
}

# proc to set child window position
proc setwingeom {wintoset} {
    global pad
    wm resizable $wintoset 0 0
    set myx [expr (([winfo screenwidth $pad]/2) - \
		       ([winfo reqwidth $wintoset]))]
    set myy [expr (([winfo screenheight $pad]/2) - \
		       ([winfo reqheight $wintoset]/2))]
    wm geometry $wintoset +$myx+$myy
}

# procedure to setup the printer
proc printseupselection {} {
    global printCommand
    global pad
    global lang
    
    set print $pad.print
    catch {destroy $print}
    toplevel $print
    if {$lang == "eng"} {
	wm title $print "Print Setup"
    } else {
	wm title $print "Mise en page"
    }
    setwingeom $print
    frame $print.top 
    frame $print.bottom
    if {$lang == "eng"} {
	label $print.top.label -text "Print Command: "
    } else {
	label $print.top.label -text "Commande d'impression : "
    }
    entry $print.top.print -textvariable printsetupnew -width 40
    $print.top.print delete 0 end
    set printvar $printCommand 
    $print.top.print insert 0 $printvar
    if {$lang == "eng"} {
	button $print.bottom.ok -text "OK" -command "addtoprint $print"
	button $print.bottom.cancel -text "Cancel" -command "destroy $print"
    } else {
	button $print.bottom.ok -text "OK" -command "addtoprint $print"
	button $print.bottom.cancel -text "Annuler" -command "destroy $print"
    }
    pack $print.top -side top -expand 0 
    pack $print.bottom -side bottom -expand 0 
    pack $print.top.label $print.top.print -in $print.top -side left -fill x \
	-fill y
    pack $print.bottom.ok $print.bottom.cancel -in $print.bottom -side left \
	-fill x -fill y
    bind $print <Return> "addtoprint $print"
    bind $print <Escape> "destroy $print"
    
    proc addtoprint {prnt} {
	global printCommand
	set printCommand [$prnt.top.print get]
	destroy $prnt
    }
}

# procedure to print
proc selectprint {textarea} {
    set TempPrintFile [open /tmp/tkpadtmpfile w]
    puts -nonewline $TempPrintFile [$textarea get 0.0 end]
    close $TempPrintFile
    global printCommand
    set prncmd $printCommand	
    eval exec $prncmd /tmp/tkpadtmpfile
    eval exec rm -f /tmp/tkpadtmpfile
}

#cut text procedure
proc deletetext {} {
    global textareacur

    set cuttexts [selection own]
    if {$cuttexts != "" } {
	if [catch {selection get -selection PRIMARY} sel] {   	
	    [gettextareacur] delete "insert" "insert +1c"
	} else {
	    $cuttexts delete sel.first sel.last
	    selection clear
	}
    } else {
        [gettextareacur] delete "insert" "insert +1c"
    }
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i1 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] \
	[[gettextareacur] index "$i1 wordend"]
# FV 13/05/04
    reshape_bp
}

#cut text procedure
proc backspacetext {} {
    global textareacur
    set cuttexts [selection own]
    if {$cuttexts != "" } {
	if [catch {selection get -selection PRIMARY} sel] {   	
	    [gettextareacur] delete "insert-1c" "insert"
	} else {
	    $cuttexts delete sel.first sel.last
	    selection clear
	}
    } else {
        [gettextareacur] delete "insert-1c" "insert"
    }
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i1 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] \
	[[gettextareacur] index "$i1 wordend"]
# FV 13/05/04
    reshape_bp
}

#cut text procedure
proc cuttext {} {
    global textareacur
    tk_textCut [gettextareacur]
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i1 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 linestart"] \
	[[gettextareacur] index "$i1 lineend"]
    selection clear
# FV 13/05/04
    reshape_bp
}

#copy text procedure
proc copytext {} {
    global textareacur
    tk_textCopy  [gettextareacur]
#ES: why? just copying does not alter the buffer
#    inccount [gettextareacur]
#ES: isn't it nicer if it stays selected?
#    selection clear
}

#paste text procedure
proc pastetext {} {
    global textareacur
    global tcl_platform
    if {"$tcl_platform(platform)" == "unix"} {
	catch {
	    [gettextareacur] delete sel.first sel.last
	}
    } else {
	catch {
	    [gettextareacur] delete sel.first sel.last
	}
    }
    set i1  [[gettextareacur] index insert]
    tk_textPaste [gettextareacur] 
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i2 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] \
	[[gettextareacur] index "$i2 wordend"]
#added by ES (but might also be unhandy)
#    $textareacur tag add sel $i1 $i2
# FV 13/05/04
    reshape_bp
}

proc FindIt {w} {
# FV 13/05/04, regexp mode added
    global SearchString SearchPos SearchDir findcase regexpcase
    global textareacur pad
    global lang SearchEnd SearchPosI
#    [gettextareacur] tag configure sel -background green
# Francois VOGEL, 21/04/04
    if {[winfo exists $w]} {
        set pw $w
    } else {
        set pw $pad
    }
    if {$SearchString!=""} {
        if {$findcase=="1"} {
            set caset "-exact"
        } else {
            set caset "-nocase"
        }
# Francois VOGEL, 21/04/04, removed $limit to allow for wrap-around (both directions)
# and find in selected text only
#	if {$SearchDir == "forwards"} {
#	    set limit end
#	} else {
#	    set limit 1.0
#	}
#	set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
#			    -- $SearchString $SearchPos $limit]
        if {$regexpcase != "1"} {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -- $SearchString $SearchPos]
            } else {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -- $SearchString $SearchPos $SearchEnd]
            }
        } else {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos]
            } else {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos $SearchEnd]
            }
        }
        set len [string length $SearchString]
        if {$SearchPos != ""} {
	      [gettextareacur] see $SearchPos
#	    tkTextSetCursor [gettextareacur] $SearchPos
            [gettextareacur] mark set insert $SearchPos
            [gettextareacur] tag remove sel 0.0 end
	      [gettextareacur] tag add sel $SearchPos  "$SearchPos + $len char"
	      if {$SearchDir == "forwards"} {
                set SearchPos "$SearchPos + $len char"
            }         
	} else {
# Francois VOGEL, 21/04/04, added message box
#	    set SearchPos "0.0"
            if {$SearchEnd == "No_end"} {
                if {$lang == "eng"} {
                    tk_messageBox -message "No match found for $SearchString" -parent $pw -title "Find"
                } else {
                    tk_messageBox -message "La chaîne $SearchString n'a pu être trouvée" -parent $pw -title "Rechercher"
                }
                set SearchPos insert
            } else {
                if {$lang == "eng"} {
                    set answer [tk_messageBox -message "No match found in the selection for $SearchString\nWould you like to look for it in the entire text?" \
                                  -parent $pw -title "Find" -type yesno -icon question]
                } else {
                    set answer [tk_messageBox -message "La chaîne $SearchString n'a pu être trouvée dans la sélection.\nVoulez-vous rechercher dans la totalité du texte ?" \
                                  -parent $pw -title "Rechercher" -type yesno -icon question]
                }
                if {![string compare $answer "yes"]} {
                    if {$SearchDir == "forwards"} {
                        set SearchPos "insert + $len char"
                    } else {
                        set SearchPos insert
                    }
                    set SearchEnd "No_end"
                } else {
                    set SearchPos $SearchPosI
                }
            }
        }
    } else {
# Francois VOGEL, 21/04/04
        if {$lang == "eng"} {
            tk_messageBox -message "You are searching for an empty string!" -parent $pw -title "Find"
        } else {
            tk_messageBox -message "La chaîne à rechercher est vide!" -parent $pw -title "Rechercher"
        }
    }
    focus [gettextareacur]
}

proc ReplaceIt {once_or_all} {
# FV 13/05/04, regexp mode added
    global SearchString SearchDir ReplaceString SearchPos findcase regexpcase
    global textareacur
    global find lang SearchEnd
# Francois VOGEL, 21/04/04
    if {$SearchString != ""} {
        if {$findcase=="1"} {
            set caset "-exact"
        } else {
            set caset "-nocase"
        }
# Francois VOGEL, 21/04/04, removed $limit to allow for wrap-around (both directions)
# and replace in selected text only
#	if {$SearchDir == "forwards"} {
#	    set limit end
#	} else {
#	    set limit 1.0
#	}
#	set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
#			    -- $SearchString $SearchPos $limit]
        if {$regexpcase != "1"} {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -- $SearchString $SearchPos]
            } else {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -- $SearchString $SearchPos $SearchEnd]
            }
        } else {
            if {$SearchEnd == "No_end"} {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos]
            } else {
                set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir \
                                -regexp -- $SearchString $SearchPos $SearchEnd]
            }
        }
        set len [string length $SearchString]
        if {$SearchPos != ""} {
            [gettextareacur] see $SearchPos
            [gettextareacur] delete $SearchPos "$SearchPos+$len char"
            [gettextareacur] insert $SearchPos $ReplaceString
            colorize [gettextareacur] \
              [[gettextareacur] index "$SearchPos linestart"] \
              [[gettextareacur] index "$SearchPos lineend"]
            [gettextareacur] mark set insert $SearchPos
            [gettextareacur] tag remove sel 0.0 end
            set lenR [string length $ReplaceString]
            [gettextareacur] tag add sel $SearchPos  "$SearchPos + $lenR char"
            if {$SearchDir == "forwards"} {
                set SearchPos "$SearchPos+$lenR char"
# Francois VOGEL, 21/04/04, $SearchEnd must be adjusted for the search to occur in the new selection
                if {$SearchEnd != "No_end" } {
                    if {int([[gettextareacur] index $SearchEnd])==int([[gettextareacur] index $SearchPos]) } {
                        set SearchEnd "$SearchEnd+[expr $lenR - $len] char"
                    }
                }
            }         
            inccount [gettextareacur]
# FV 13/05/04
            reshape_bp
            focus [gettextareacur]
            return "Done"
        } else {
# Francois VOGEL, 21/04/04, added message box
#	    set SearchPos "0.0"
            set SearchPos insert
            if {$once_or_all == "once"} {
                if {$lang == "eng"} {
                    tk_messageBox -message "No match found for $SearchString" -parent $find -title "Replace"
                } else {
                    tk_messageBox -message "La chaîne $SearchString n'a pu être trouvée" -parent $find -title "Remplacer"
                }
            }
            return "No_match"
        }
    } else {
# Francois VOGEL, 21/04/04
        if {$lang == "eng"} {
            tk_messageBox -message "You are searching for an empty string!" -parent $find -title "Find"
        } else {
            tk_messageBox -message "La chaîne à rechercher est vide!" -parent $find -title "Rechercher"
        }
    }
#    inccount [gettextareacur]
}

proc ReplaceAll {} {
      global SearchPos SearchString lang find
# Francois VOGEL, 21/04/04
#      if {$SearchString != ""} {
#          ReplaceIt
#          while {$SearchPos!="0.0"} {
#              ReplaceIt
#          }
#      }
    if {$SearchString != ""} {
        set anotherone [ReplaceIt once]
        while {$anotherone != "No_match"} {
            set anotherone [ReplaceIt all]
        }
    } else {
        if {$lang == "eng"} {
            tk_messageBox -message "You are searching for an empty string!" -parent $find -title "Find"
        } else {
            tk_messageBox -message "La chaîne à rechercher est vide!" -parent $find -title "Rechercher"
        }
    }
}

proc CancelFind {w} {
    global textareacur pad
    [gettextareacur] tag delete tg1
    bind $pad <Expose> {};
#ajout pour mettre a la fenetre Search devant le scipad !
    destroy $w
}

proc ResetFind {} {
#    global SearchPos
#    set SearchPos insert
# Francois VOGEL, 21/04/04, added find in current selection
    global SearchPos SearchEnd SearchPosI SearchDir
    catch {[[gettextareacur] get sel.first sel.last]} sel
    if {$sel == "text doesn't contain any characters tagged with \"sel\""} {
        set SearchPos insert
        set SearchEnd "No_end"
    } else {
        if {$SearchDir=="forwards"} {
            set SearchPos [[gettextareacur] index sel.first]
            set SearchEnd [[gettextareacur] index sel.last]
        } else {
            set SearchPos [[gettextareacur] index sel.last]
            set SearchEnd [[gettextareacur] index sel.first]
        }
        set SearchPosI $SearchPos
    }
}

# procedure to find text
proc findtext {typ} {
# FV 13/05/04, regexp mode added
    global SearchString SearchDir ReplaceString findcase c find pad lang regexpcase
    set find $pad.find
    catch {destroy $find}
    toplevel $find
    if {$lang == "eng"} {
        wm title $find "Find"
    } else {
        wm title $find "Rechercher"
    }
    setwingeom $find
# Francois VOGEL, 21/04/04, this is already done by invoking down radiobutton below
# as I added -command on this radiobutton to take care of the case where find shall
# look backwards in selection
#    ResetFind
    frame $find.l
    frame $find.l.f1
    if {$lang == "eng"} {
        label $find.l.f1.label -text "Find what:" -width 11
    } else {
        label $find.l.f1.label -text "Rechercher :" -width 11
    }
    entry $find.l.f1.entry  -textvariable SearchString -width 30 
    pack $find.l.f1.label $find.l.f1.entry -side left
    $find.l.f1.entry selection range 0 end
#ES 
#        bind $find.l.f1.entry <Control-c> {tk_textCopy $find.l.f1.entry}
#        bind $find.l.f1.entry <Control-v> {tk_textPaste $find.l.f1.entry}
# this doesn't work?
#        bind $find.l.f1.entry <Control-x> {tk_textCut $find.l.f1.entry}
#
    if {$typ=="replace"} {
        frame $find.l.f2
        if {$lang == "eng"} {
            label $find.l.f2.label2 -text "Replace with:" -width 11
        } else {
            label $find.l.f2.label2 -text "Remplacer par :" -width 11
        }
        entry $find.l.f2.entry2  -textvariable ReplaceString -width 30 
        pack $find.l.f2.label2 $find.l.f2.entry2 -side left
        pack $find.l.f1 $find.l.f2 -side top -pady 2
#ES 
#            bind $find.l.f2.entry2 <Control-c> {tk_textCopy $find.l.f2.entry2}
#            bind $find.l.f2.entry2 <Control-v> {tk_textPaste $find.l.f2.entry2}
# this doesn't work?
#            bind $find.l.f2.entry2 <Control-x> {tk_textCut $find.l.f2.entry2}
#
    } else {
        pack $find.l.f1 -pady 4
    }
    frame $find.f2
    if {$lang == "eng"} {
        button $find.f2.button1 -text "Find Next" -command "FindIt $find" \
            -width 10 -height 1 -underline 5 
        button $find.f2.button2 -text "Cancel" -command "CancelFind $find"\
            -width 10 -underline 5
    } else {
        button $find.f2.button1 -text "Rechercher suivant" -command \
            "FindIt $find" -width 15 -height 1 -underline 16 
        button $find.f2.button2 -text "Annuler" \
            -command "CancelFind $find" -width 15 -underline 4	    
    }
    if {$typ=="replace"} {
        if {$lang == "eng"} {
            button $find.f2.button3 -text "Replace" -command "ReplaceIt once"\
                -width 10 -height 1 -underline 2
            button $find.f2.button4 -text "Replace All" \
                -command ReplaceAll -width 10 -height 1 -underline 8
        } else {
            button $find.f2.button3 -text "Remplacer" -command "ReplaceIt once" \
                -width 15 -height 1 -underline 3
            button $find.f2.button4 -text "Remplacer tout" \
                -command ReplaceAll -width 15 -height 1 -underline 10
        }
        pack $find.f2.button3 $find.f2.button4 $find.f2.button2  -pady 4
    } else {
        pack $find.f2.button1 $find.f2.button2  -pady 4
    }
    frame $find.l.f4
    frame $find.l.f4.f3 -borderwidth 2 -relief groove
# Francois VOGEL 21/04/04, added -command on these two radiobuttons
    if {$lang == "eng"} {
        radiobutton $find.l.f4.f3.up -text "Up" -underline 0 \
            -variable SearchDir -value "backwards" -command "ResetFind"
        radiobutton $find.l.f4.f3.down -text "Down"  -underline 0 \
            -variable SearchDir -value "forwards" -command "ResetFind"
    } else {
        radiobutton $find.l.f4.f3.up -text "Vers le haut" -underline 10 \
            -variable SearchDir -value "backwards" -command "ResetFind"
        radiobutton $find.l.f4.f3.down -text "Vers le bas"  -underline 9 \
            -variable SearchDir -value "forwards" -command "ResetFind"
    } 
    $find.l.f4.f3.down invoke
    pack $find.l.f4.f3.up $find.l.f4.f3.down -side left
    frame $find.l.f4.f5
    if {$lang == "eng"} {
        checkbutton $find.l.f4.f5.cbox1 -text "Match case" \
            -variable findcase -underline 0
    } else {
        checkbutton $find.l.f4.f5.cbox1 -text "Respecter la casse" \
            -variable findcase -underline 0
    }
    if {$lang == "eng"} {
        checkbutton $find.l.f4.f5.cbox2 -text "Regular expression" \
            -variable regexpcase -underline 0
    } else {
        checkbutton $find.l.f4.f5.cbox2 -text "Expression régulière" \
            -variable regexpcase -underline 13
    }
    pack $find.l.f4.f5.cbox1 $find.l.f4.f5.cbox2 -anchor sw
    pack $find.l.f4.f5 $find.l.f4.f3 -side left -padx 10
    pack $find.l.f4 -pady 11
    pack $find.l $find.f2 -side left -padx 1
#	bind $find <Escape> "destroy $find" ##this hangs scipad
    bind $find <Escape> "CancelFind $find"

     # each widget must be bound to the events of the other widgets
    proc bindevnt {widgetnm types find} {
        global lang
        if {$lang == "eng"} {
            if {$types=="replace"} {
                bind $widgetnm <Return> "ReplaceIt once"
                bind $widgetnm <Control-p> "ReplaceIt once"
                bind $widgetnm <Control-a> "ReplaceAll"
            } else {
                bind $widgetnm <Return> "FindIt $find"
                bind $widgetnm <Control-n> "FindIt $find"
            }
            bind $widgetnm <Control-m> { $find.l.f4.f5.cbox1 invoke }
            bind $widgetnm <Control-r> { $find.l.f4.f5.cbox2 invoke }
            bind $widgetnm <Control-u> { $find.l.f4.f3.up invoke }
            bind $widgetnm <Control-d> { $find.l.f4.f3.down invoke }
        } else {
            if {$types=="replace"} {
                bind $widgetnm <Return> "ReplaceIt once"
                bind $widgetnm <Control-p> "ReplaceIt once"
                bind $widgetnm <Control-t> "ReplaceAll"
            } else {
                bind $widgetnm <Return> "FindIt $find"
                bind $widgetnm <Control-n> "FindIt $find"
            }
            bind $widgetnm <Control-r> { $find.l.f4.f5.cbox1 invoke }
            bind $widgetnm <Control-g> { $find.l.f4.f5.cbox2 invoke }
            bind $widgetnm <Control-u> { $find.l.f4.f3.up invoke }
            bind $widgetnm <Control-a> { $find.l.f4.f3.down invoke }
        }
    }
    if {$typ == "replace"} {
        bindevnt $find.f2.button3 $typ $find
        bindevnt $find.f2.button4 $typ $find
    } else {
        bindevnt $find.f2.button1 $typ $find
        bindevnt $find.f2.button2 $typ $find
    }
    bindevnt $find.l.f4.f3.up  $typ $find
    bindevnt $find.l.f4.f3.down $typ $find
    bindevnt $find.l.f4.f5.cbox1 $typ $find
    bindevnt $find.l.f4.f5.cbox2 $typ $find
    bindevnt $find.l.f1.entry $typ $find	
#    bind $find <Control-c> "destroy $find";
#    bind $find <Control-c> "CancelFind $find";
    bind $find <Control-l> "CancelFind $find"
    bind $find <Visibility> {raise $find $pad};
#ajout pour mettre a la fenetre Search devant le scipad !
    bind $pad <Expose> {raise $find $pad};
#ajout pour mettre a la fenetre Search devant le scipad !
    focus $find.l.f1.entry
    grab $find
}

# proc for find next
proc findnext {typof} {
	global SearchString SearchDir ReplaceString findcase c find
	if [catch {expr [string compare $SearchString "" ] }] {
		findtext $typof
	} else {
	 	FindIt $find
	}
}
# add gotoline option included by Matthieu PHILIPPE 21/11/2001 from gotoline.pth
proc gotoline {} {
	global textareacur pad lang
	set gotln $pad.gotln
	catch {destroy $gotln}
	toplevel $gotln
	if {$lang == "eng"} {
	    wm title $gotln "Goto Line?"
	} else {
	    wm title $gotln "Aller à la ligne"
	}
	setwingeom $gotln
	frame $gotln.top 
	frame $gotln.bottom
	if {$lang == "eng"} {
	    label $gotln.top.label -text "Goto Line: "
	} else {
	    label $gotln.top.label -text "Aller à la ligne : "
	}
	entry $gotln.top.gotln -textvariable gotlnsetupnew -width 10
	$gotln.top.gotln delete 0 end 
	button $gotln.bottom.ok -text "OK" -command "addtogotln $gotln"
	if {$lang == "eng"} {
	    button $gotln.bottom.cancel -text "Cancel" \
		-command "destroy $gotln"
	} else {
	    button $gotln.bottom.cancel -text "Annuler" \
	         -command "destroy $gotln"
	}
	focus $gotln.top.gotln
	pack $gotln.top -side top -expand 0 
	pack $gotln.bottom -side bottom -expand 0 
	pack $gotln.top.label $gotln.top.gotln -in $gotln.top -side left \
	    -fill x -fill y
	pack $gotln.bottom.ok $gotln.bottom.cancel -in $gotln.bottom \
	    -side left -fill x -fill y
	bind $gotln <Return> "addtogotln $gotln"
	bind $gotln <Escape> "destroy $gotln"

    proc addtogotln {prnt} {
	global textareacur
	global gotlnCommand
	set gotlnCommand [$prnt.top.gotln get]
#	tkTextSetCursor [gettextareacur] "$gotlnCommand.0"
	[gettextareacur] mark set insert "$gotlnCommand.0"
	catch {keyposn [gettextareacur]}
        [gettextareacur] see insert
	destroy $prnt
    }
}

#procedure to set the time change %R to %I:%M for 12 hour time display
proc printtime {} {
    global textareacur
    [gettextareacur] insert insert [clock format [clock seconds] \
					-format "%R %p %D"]
    inccount [gettextareacur]
}

# binding for wordwrap
proc wraptext {} {
    global wordWrap
    global textareacur
    if [expr [string compare $wordWrap word] == 0] {
	set wordWrap none	
    } else {
	set wordWrap word
    }
    [gettextareacur] configure -wrap $wordWrap
}

# this sets saveTextMsg to 1 for message boxes
proc getccount {textarea} {
    global listoffile
    return $listoffile("$textarea",save)
}

proc inccount {textarea} {
    global listoffile
    global saveTextMsg

    set saveTextMsg 1 
    set listoffile("$textarea",save) 1
##ES 23/9/2003
    modifiedtitle $textarea
}
# this resets saveTextMsg to 0
proc outccount {textarea} {
    global listoffile
    global saveTextMsg
    set saveTextMsg 0 
    set listoffile("$textarea",save) 0
##ES 23/9/2003
    modifiedtitle $textarea
}

# catch the kill of the windowmanager
wm protocol $pad WM_DELETE_WINDOW exitapp

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
bind Text <Button-2> {copytext; selection clear; 
                      %W mark set insert "@%x,%y";
                      pastetext }

bind $textareacur <KeyRelease> {keyposn %W}
bind $textareacur <ButtonRelease> {keyposn %W}

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

# FV 13/05/04
bind $pad <F9> {insertremove_bp}
bind $pad <Control-F9> {removeall_bp}
bind all <F10> {}
bind $pad <F10> {configurefoo_bp}
bind $pad <Control-F11> {execfile_bp}
bind $pad <F11> {resume_bp}
bind $pad <Shift-F11> {insertremovedebug_bp}
bind $pad <Shift-F12> {goonwo_bp}
bind $pad <F12> {stepbystep_bp}

###################################################################
#set zed_dir [file dirname [info script]] 
# here is where the undo stuff begins
if {![info exists classNewId]} {
    # work around object creation between multiple include of this file problem
    set classNewId 0
}

proc new {className args} {
    # calls the constructor for the class with optional arguments
    # and returns a unique object identifier independent of the class name

    global classNewId
    # use local variable for id for new can be called recursively
    set id [incr classNewId]
    if {[llength [info procs ${className}:$className]]>0} {
        # avoid catch to track errors
        eval ${className}:$className $id $args
    }
    return $id
}

proc delete {className id} {
    # calls the destructor for the class and delete all the object data members

    if {[llength [info procs ${className}:~$className]]>0} {
        # avoid catch to track errors
        ${className}:~$className $id
    }
    global $className
    # and delete all this object array members if any (assume that they were 
    # stored as $className($id,memberName))
    foreach name [array names $className "$id,*"] {
        unset ${className}($name)
    }
}

proc lifo:lifo {id {size 2147483647}} {
    global lifo
    set lifo($id,maximumSize) $size
    lifo:empty $id
}

proc lifo:push {id data} {
    global lifo saveTextMsg
    set saveTextMsg 1

    inccount [gettextareacur]
    lifo:tidyUp $id
    if {$lifo($id,size)>=$lifo($id,maximumSize)} {
        unset lifo($id,data,$lifo($id,first))
        incr lifo($id,first)
        incr lifo($id,size) -1
    }
    set lifo($id,data,[incr lifo($id,last)]) $data
    incr lifo($id,size)
}

proc lifo:pop {id} {
    global lifo saveTextMsg
    set saveTextMsg 1
    inccount [gettextareacur]
    lifo:tidyUp $id
    if {$lifo($id,last)<$lifo($id,first)} {
        error "lifo($id) pop error, empty"
    }
   # delay unsetting popped data to improve performance by avoiding a data copy
    set lifo($id,unset) $lifo($id,last)
    incr lifo($id,last) -1
    incr lifo($id,size) -1
    return $lifo($id,data,$lifo($id,unset))
}

proc lifo:tidyUp {id} {
    global lifo
    if {[info exists lifo($id,unset)]} {
        unset lifo($id,data,$lifo($id,unset))
        unset lifo($id,unset)
    }
}

proc lifo:empty {id} {
    global lifo
    lifo:tidyUp $id
    foreach name [array names lifo $id,data,*] {
        unset lifo($name)
    }
    set lifo($id,size) 0
    set lifo($id,first) 0
    set lifo($id,last) -1
}

proc textUndoer:textUndoer {id widget {depth 2147483647}} {
    global textUndoer

    if {[string compare [winfo class $widget] Text]!=0} {
        error "textUndoer error: widget $widget is not a text widget"
    }
    set textUndoer($id,widget) $widget
    set textUndoer($id,originalBindingTags) [bindtags $widget]
    bindtags $widget [concat $textUndoer($id,originalBindingTags) \
			  UndoBindings($id)]

    bind UndoBindings($id) <Control-u> "textUndoer:undo $id"

    # self destruct automatically when text widget is gone
    bind UndoBindings($id) <Destroy> "delete textUndoer $id"

    # rename widget command
    rename $widget [set textUndoer($id,originalCommand) \
			textUndoer:original$widget]
    # and intercept modifying instructions before calling original command
    proc $widget {args} "textUndoer:checkpoint $id \$args; 
		global search_count;
		eval $textUndoer($id,originalCommand) \$args"

    set textUndoer($id,commandStack) [new lifo $depth]
    set textUndoer($id,cursorStack) [new lifo $depth]
    #lee 
    textRedoer:textRedoer $id $widget $depth 
}

proc textUndoer:~textUndoer {id} {
    global textUndoer
    bindtags $textUndoer($id,widget) $textUndoer($id,originalBindingTags)
    rename $textUndoer($id,widget) "" 
# this bellow command doesn't work right and I can understand why ?
#    rename $textUndoer($id,originalCommand) $textUndoer($id,widget)
    delete lifo $textUndoer($id,commandStack)
    delete lifo $textUndoer($id,cursorStack)
    #lee
    textRedoer:~textRedoer $id
}

proc textUndoer:checkpoint {id arguments} {
    global textUndoer textRedoer

    # do nothing if non modifying command
    if {[string compare [lindex $arguments 0] insert]==0} {
        textUndoer:processInsertion $id [lrange $arguments 1 end]
        if {$textRedoer($id,redo) == 0} {
           textRedoer:reset $id
        }
    }
    if {[string compare [lindex $arguments 0] delete]==0} {
        textUndoer:processDeletion $id [lrange $arguments 1 end]
        if {$textRedoer($id,redo) == 0} {
           textRedoer:reset $id
        }
    }
}

proc textUndoer:processInsertion {id arguments} {
    global textUndoer

    set number [llength $arguments]
    set length 0
    # calculate total insertion length while skipping tags in arguments
    for {set index 1} {$index<$number} {incr index 2} {
        incr length [string length [lindex $arguments $index]]
    }
    if {$length>0} {
        set index [$textUndoer($id,originalCommand) index \
		       [lindex $arguments 0]]
        lifo:push $textUndoer($id,commandStack) \
	    "delete $index $index+${length}c"
        lifo:push $textUndoer($id,cursorStack) \
	    [$textUndoer($id,originalCommand) index insert]
    }
}

proc textUndoer:processDeletion {id arguments} {
    global textUndoer

    set command $textUndoer($id,originalCommand)
    lifo:push $textUndoer($id,cursorStack) [$command index insert]

    set start [$command index [lindex $arguments 0]]
    if {[llength $arguments]>1} {
        lifo:push $textUndoer($id,commandStack) \
	    "insert $start [list [$command get $start [lindex $arguments 1]]]"
    } else {
        lifo:push $textUndoer($id,commandStack) \
	    "insert $start [list [$command get $start]]"
    }
}

proc textUndoer:undo {id} {
    global textUndoer

    if {[catch {set cursor [lifo:pop $textUndoer($id,cursorStack)]}]} {
        return "Undo_lifo_empty"
    }
# Francois VOGEL, 16/04/04, added catch to fix the bug that happens if undo
# is performed once too much
    if {[catch {set popArgs [lifo:pop $textUndoer($id,commandStack)]}]} {
        return "Undo_lifo_empty"
    } 
    textRedoer:checkpoint $id $popArgs
    
    eval $textUndoer($id,originalCommand) $popArgs
    # now restore cursor position
    $textUndoer($id,originalCommand) mark set insert $cursor
    # make sure insertion point can be seen
    $textUndoer($id,originalCommand) see insert
# Francois VOGEL, 21/04/04, added test to check empty lifo
    global lifo
    if {$lifo($textUndoer($id,commandStack),last)<$lifo($textUndoer($id,commandStack),first)} {
        return "Undo_lifo_empty"
    }
}


proc textUndoer:reset {id} {
    global textUndoer
    lifo:empty $textUndoer($id,commandStack)
    lifo:empty $textUndoer($id,cursorStack)
}

#########################################################################
proc textRedoer:textRedoer {id widget {depth 2147483647}} {
    global textRedoer
    if {[string compare [winfo class $widget] Text]!=0} {
        error "textRedoer error: widget $widget is not a text widget"
    }
    set textRedoer($id,commandStack) [new lifo $depth]
    set textRedoer($id,cursorStack) [new lifo $depth]
    set textRedoer($id,redo) 0
}

proc textRedoer:~textRedoer {id} {
    global textRedoer
    delete lifo $textRedoer($id,commandStack)
    delete lifo $textRedoer($id,cursorStack)
}


proc textRedoer:checkpoint {id arguments} {
    global textUndoer textRedoer
    # do nothing if non modifying command
    if {[string compare [lindex $arguments 0] insert]==0} {
        textRedoer:processInsertion $id [lrange $arguments 1 end]
    }
    if {[string compare [lindex $arguments 0] delete]==0} {
        textRedoer:processDeletion $id [lrange $arguments 1 end]
    }
}

proc textRedoer:processInsertion {id arguments} {
    global textUndoer textRedoer
    set number [llength $arguments]
    set length 0
    # calculate total insertion length while skipping tags in arguments
    for {set index 1} {$index<$number} {incr index 2} {
        incr length [string length [lindex $arguments $index]]
    }
    if {$length>0} {
        set index [$textUndoer($id,originalCommand) index \
		       [lindex $arguments 0]]
        lifo:push $textRedoer($id,commandStack) \
	    "delete $index $index+${length}c"
        lifo:push $textRedoer($id,cursorStack) \
	    [$textUndoer($id,originalCommand) index insert]
    }
}

proc textRedoer:processDeletion {id arguments} {
    global textUndoer textRedoer
    set command $textUndoer($id,originalCommand)
    lifo:push $textRedoer($id,cursorStack) [$command index insert]

    set start [$command index [lindex $arguments 0]]
    if {[llength $arguments]>1} {
        lifo:push $textRedoer($id,commandStack) \
	    "insert $start [list [$command get $start [lindex $arguments 1]]]"
    } else {
        lifo:push $textRedoer($id,commandStack) \
	    "insert $start [list [$command get $start]]"
    }
}
proc textRedoer:redo {id} {
    global textUndoer textRedoer
    if {[catch {set cursor [lifo:pop $textRedoer($id,cursorStack)]}]} {
        return
    }
    set textRedoer($id,redo) 1
    set popArgs [lifo:pop $textRedoer($id,commandStack)]     
    textUndoer:checkpoint $id $popArgs
    eval $textUndoer($id,originalCommand) $popArgs
    set textRedoer($id,redo) 0
    # now restore cursor position
    $textUndoer($id,originalCommand) mark set insert $cursor
    # make sure insertion point can be seen
    $textUndoer($id,originalCommand) see insert
}


proc textRedoer:reset {id} {
    global textRedoer
    lifo:empty $textRedoer($id,commandStack)
    lifo:empty $textRedoer($id,cursorStack)
}

# end of where youd source in undo.tcl

set undo_id [new textUndoer $textareacur]
#
# added by Matthieu PHILIPPE 14/12/2001
set listundo_id("$textareacur") $undo_id
#### 
proc undo_menu_proc {} {
    global textareacur
    global undo_id
    global listundo_id

    #textUndoer:undo $undo_id
    set i1 [[gettextareacur] index insert]
    set isempty [textUndoer:undo $listundo_id("[gettextareacur]")]
    set i2 [[gettextareacur] index insert]
    colorize [gettextareacur] $i1 [[gettextareacur] index "$i2+1l linestart"]
# Francois VOGEL, 21/04/04, added test and outccount
    if {$isempty != "Undo_lifo_empty"} {
        inccount [gettextareacur]
    } else {
        outccount [gettextareacur]
    }
# FV 13/05/04
    reshape_bp
}

proc redo_menu_proc {} {
    global undo_id
    global listundo_id

    #textRedoer:redo $undo_id
    set i1 [[gettextareacur] index insert]
    textRedoer:redo $listundo_id("[gettextareacur]")
    set i2 [[gettextareacur] index insert]
    colorize [gettextareacur] $i1 [[gettextareacur] index "$i2+1l linestart"]
    inccount [gettextareacur]
# FV 13/05/04
    reshape_bp
}

#######################################
# added by Matthieu PHILIPPE 21/11/2001
#
proc load_words {} {
	global	words chset env

	set ownpath "$env(SCIPATH)/tcl"
        set type {}
	set col {}
	set f [open $ownpath/words r]
	while {[gets $f line] > 0} {
		if {[lindex $line 0] == {#MODE}} {
			set type [lindex $line 1]
		} elseif {[lindex $line 0] == {#TAG}} {
			set col [lindex $line 1]
			set chset($type.$col) {}
		} else {
			set ch [string range $line 0 0]
			append chset($type.$col) $ch
			set words($type.$col.$ch) $line
		}
	}
}

#ES 2/10/2003
proc remalltags {w begin ende} {
	$w tag remove parenthesis $begin $ende
	$w tag remove bracket $begin $ende
	$w tag remove brace $begin $ende
	$w tag remove punct $begin $ende
	$w tag remove operator $begin $ende
	$w tag remove number $begin $ende
	$w tag remove keywords $begin $ende
	$w tag remove text $begin $ende
	$w tag remove rem2 $begin $ende
        $w tag remove xmltag $begin $ende
	$w tag remove textquoted $begin $ende  
        $w tag remove indentation $begin $ende
	    }


proc colorize {w cpos iend} {
	global	words chset
        global listoffile 
        set num 0
## ES: xml colorization only for xml files
        set textarea [gettextareacur]
        set xmlcolor [string first "\.xml" $listoffile("$textarea",filename)]
###
	$w mark set begin "$cpos linestart"
	$w mark set ende "$iend+1l linestart"
        remalltags $w begin ende
# TAGS:
# order matters here - for instance textquoted has to be after operator, so 
# operators are not colorized within strings
#
# Scilab specials
	$w mark set last begin
	while {[set ind [$w search -regexp {[;,]} last ende]] != {}} {
		if {[$w compare $ind >= last]} {
			$w mark set last $ind+1c
			$w tag add punct $ind last
		} else break
        }
	$w mark set last begin
	while {[set ind [$w search -regexp {[()]} last ende]] != {}} {
		if {[$w compare $ind >= last]} {
			$w mark set last $ind+1c
			$w tag add parenthesis $ind last
		} else break
	}
	$w mark set last begin
	while {[set ind [$w search -regexp {[\[\]]} last ende]] != {}} {
		if {[$w compare $ind >= last]} {
			$w mark set last $ind+1c
			$w tag add bracket $ind last
		} else break
	}
#ES: why at all call ":" a "brace? 
# "{}" are anyway parsed above for matching pairs
	$w mark set last begin
	while {[set ind [$w search -regexp {[\{\}:]} last ende]] != {}} {
		if {[$w compare $ind >= last]} {
			$w mark set last $ind+1c
			$w tag add brace $ind last
		} else break
	}
##ES: numbers (the regexp can be perfectioned -- it matches e.g. single e6)
# number can contain +-. so follows operator (?)
	$w mark set last begin
	while {[set ind [$w search -regexp {\m\d*\.?\d*[deDE]?\-?\d{1,3}\M} \
			     last ende]] != {}} {
		if {[$w compare $ind >= last]} {
			$w mark set last $ind+1c
			$w tag add number $ind last
		} else break
	}
	$w mark set last begin
	while {[set ind [$w search -regexp {['\.+\-*\/\\\^=\~$|&<>]} \
			     last ende]] != {}} {
		if {[$w compare $ind >= last]} {
			$w mark set last $ind+1c
			$w tag add operator $ind last
		} else break
	}
# Scilab
        set sciChset "(\[^A-Za-z0-9_\]|^)\[$chset(scilab.col1)\]"
 	$w mark set last begin
 	while {[set ind [$w search -count num -regexp $sciChset last ende]]\
            != {}} {
 	    if {[$w compare $ind >= last]} {
 		set res ""
 		regexp $sciChset [$w get $ind "$ind wordend+1c"] res
 		set num [string length $res]
 		$w mark set last "$ind + $num c"
 		$w mark set next {last wordend}
 		set word [$w get last-1c next]
 		if {[lsearch -exact $words(scilab.col1.[string range \
 					       $word 0 0]) $word] != -1} {
 		    $w tag add keywords last-1c next
 		}
 		$w mark set last next-1c
 	    } else break
 	  }
# XML (#ES this is a problem as <> are also operators)
        if {$xmlcolor > 0} {
	$w mark set last begin
	  while {[set ind [$w search  -regexp "<" last ende]] != {}} {
	      if {[$w compare $ind >= last]} {
	   	set res ""
		regexp ">" [$w get $ind end] res
		set num [string length $res]
		$w mark set last "$ind + $num c"
		$w mark set next {last+1c wordend}
		$w tag add xmltag last-1c next+1c
#		$w tag add xmltag $ind last
		$w mark set last next+1c
	      } else break
	    }
          }
# Text
	  $w mark set last begin
	  while { [set ind [$w search -count num -regexp \
                          {"[^"]*("|$)} last ende]] != {}} {
	      if {[$w compare $ind >= last]} {
		  set res ""
		  $w mark set endetext "$ind lineend"
		  regexp {"[^"]*"} [$w get last endetext] res
		  set num [string length $res]
		  if {$num <= 0} {
		      $w mark set last "$ind + 1c"
		  } else {
		      $w mark set last "$ind + $num c"
                 # textquoted deletes any other tag
                      remalltags $w $ind last
		      $w tag add textquoted $ind last
		  }	  
	      } else break
	      }
# remark
	      $w mark set last begin
	      while {[set ind [$w search -exact {//} last ende]] != {}} {
		  if {[$w compare $ind >= last]} {
#ES
		      $w mark set last "$ind+1l linestart"
# tags as rem2 only if not already textquoted (does not tag as comment "..//..")
                      if {[lsearch [$w tag names $ind] "textquoted"] ==-1 } {
                 # rem2 deletes any other tag
		           remalltags $w $ind "$ind lineend" 
			   $w tag add rem2 $ind "$ind lineend" }
		  } else break
		  }
# anyway, commented textquoted are displayed as rem2
              $w tag raise rem2 textquoted
}

proc selectall {} {
[gettextareacur] tag add sel 1.0 end
}

proc puttext {w text} {
    global winTitle pad
    set rem 0
    set cuttexts [selection own]
# FV 13/05/04, next line corrected (see bug #723)
    if {[string range $cuttexts 0 [expr [string length $pad]-1]] == $pad} {
	if [catch {selection get -selection PRIMARY} sel] {
	    
	} else {
	    $cuttexts delete sel.first sel.last
	    selection clear
	}
    }	
    set i1 [$w index insert]
    $w insert insert $text
    set i2 [$w index insert]
    if {$i1 != $i2 || $rem} {
	colorize $w $i1 [$w index "$i2+1l linestart"]
    }
# FV 13/05/04
    reshape_bp
    $w see insert
}

proc selectline {} {
# ancillary for un/comment, un/indent
    global textareacur
    set i1 [$textareacur index "insert linestart"]
    set i2 [$textareacur index "insert lineend"]
    selection clear
    $textareacur tag add sel $i1 $i2
    set seltext ""
    catch {set seltext [selection get]}
    return $seltext
}

proc CommentSel {} {
    global textareacur
    set seltexts [selection own]
    if {$seltexts != "" } {
	if [catch {selection get -selection PRIMARY} sel] {	    
	    if {[selectline] != ""} CommentSel
	} else {
            set uctext [selection get]	    
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
#I thought this shouldn't match a newline als last character of the string...
            regsub -all -line "(?!.\Z)^" $uctext  "//" ctext
#glitches: are they because of my misunderstanding or bugs in regsub? 
#this fixes the trailing "\n//"
            if {[string range $ctext end-1 end]=="//"} {
                   set ctext [string range $ctext 0 end-2]
                   set i2 $i2-2c }
#this fixes another glitch - missing "//" at the second line if the first is
#  empty
            if {[string range $ctext 0 2]=="//\n"} {
                   set ctext1 [string range $ctext 3 end]
                   set ctext "//\n//"
                   append ctext $ctext1
               }
            puttext $textareacur $ctext
            $textareacur tag add sel $i1 $i2+2c
         }
    } else {
       if {[selectline] != ""} CommentSel
    }
}

proc UnCommentSel {} {
    global textareacur
    set seltexts [selection own]
    if {$seltexts != "" } {
	if [catch {selection get -selection PRIMARY} sel] {	    
           if {[selectline] != ""} {UnCommentSel}
	} else {
            set ctext [selection get]	    
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            if { $i2<$i1 } {
		set i3 $i2; set i2 $i1; set i1 $i3
	    } 
            regsub -all -line "^\\s*//" $ctext  "" uctext
            puttext $textareacur $uctext
# after many deglitches, this only sometimes loses the selection for <one line
            $textareacur tag add sel $i1 [$textareacur index insert] 
	}
    } else {
       if {[selectline] != ""} UnCommentSel
    }
}

proc IndentSel {} {
# just copied from CommentSel
    global textareacur
    set seltexts [selection own]
    if {$seltexts != "" } {
	if [catch {selection get -selection PRIMARY} sel] {	    
          if {[selectline] != ""} {IndentSel}
	} else {
            set uctext [selection get]	    
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
#I thought this shouldn't match a newline as last character of the string...
            regsub -all -line "(?!.\Z)^" $uctext "  " ctext
#glitches: are they because of my misunderstanding or bugs in regsub? 
#this fixes the trailing "\n  "
            if {[string range $ctext end-1 end]=="  "} {
                   set ctext [string range $ctext 0 end-2]
                   set i2 $i2-2c }
#this fixes another glitch - missing "  " at the second line if the first is
#  empty
              if {[string range $ctext 0 2]=="  \n"} {
                     set ctext1 [string range $ctext 3 end]
  	             set ctext "  \n  "
                     append ctext $ctext1
                 }
#clean up lines which contain only spaces
            regsub -all -line "\ *\n" $ctext "\n" ctext1
            puttext $textareacur $ctext1
            $textareacur tag add sel $i1 $i2+2c
         }
    } else {
       if {[selectline] != ""} IndentSel
    }
}

proc UnIndentSel {} {
# just copied from UncommentSel
    global textareacur
    set seltexts [selection own]
    if {$seltexts != ""} {
	if [catch {selection get -selection PRIMARY} sel] {	    
          if {[selectline] != ""} {UnIndentSel}
	} else {
            set ctext [selection get]	    
            set i1 [$textareacur index sel.first]
            set i2 [$textareacur index sel.last]
            if { $i2<$i1 } {
 		set i3 $i2; set i2 $i1; set i1 $i3
	    }
            regsub -all -line "^  " $ctext  "" uctext
            puttext $textareacur $uctext
 # as above in UnCommentSel
            $textareacur tag add sel $i1 [$textareacur index insert]
        }
    } else {
       if {[selectline] != ""} UnIndentSel
    }
}

###added by ES 24/9/2003
proc modifiedtitle {textarea} {
    global pad listoffile lang 
    set fname $listoffile("$textarea",filename)
    if {$listoffile("$textarea",save) ==1} { 
       if {$lang == "eng"} {
          settitle "$fname - (modified)"
       } else {
          settitle "$fname - (modifié)"
       }
       $pad.filemenu.wind  entryconfigure [$pad.filemenu.wind index $fname] \
                          -background Salmon -activebackground LightSalmon
       $pad.statusind configure -background Salmon
    } else {  
       settitle "$fname"
       $pad.filemenu.wind  entryconfigure [$pad.filemenu.wind index $fname]\
                          -background "" -activebackground ""
       $pad.statusind configure -background [$pad.filemenu cget -background]
     }
}

proc whichfun {indexin {buf "current"}} {
    global lang
#it is implicitely meant that indexin refers to a position in textareacur
# FV 13/05/04, added capability for looking in a buffer which is not the current one
    if {$buf == "current"} {
        set textarea [gettextareacur]
    } else {
        set textarea $buf
    }
    scan $indexin "%d.%d" ypos xpos
# search for the previous "function" which is not in a comment nor
# in a string
    set precfun [$textarea search -count len -exact -backwards -regexp\
		     "\\mfunction\\M" $indexin 0.0]
    if {$precfun!=""} {
# FV 13/05/04, changed ==1 to !=-1 (twice) to take breakpoint tag into account
        while {[lsearch [$textarea tag names $precfun] "textquoted"] !=-1 | \
	       [lsearch  [$textarea tag names $precfun] "rem2"] !=-1} {
          set precfun [$textarea search -count len -exact -backwards -regexp\
		     "\\mfunction\\M" $precfun 0.0]
 	  if {$precfun==""} break
	}
    }
# search for the previous "endfunction" which is not in a comment nor
# in a string
    set precendfun [$textarea search -count len -exact -backwards -regexp\
		     "\\mendfunction\\M" $indexin 0.0]
    if {$precendfun!=""} {
# FV 13/05/04, changed ==1 to !=-1 (twice) to take breakpoint tag into account
        while {[lsearch [$textarea tag names $precendfun] "textquoted"] !=-1 | \
	       [lsearch  [$textarea tag names $precendfun] "rem2"] !=-1} {
          set precendfun [$textarea search -count len -exact -backwards -regexp\
		     "\\mendfunction\\M" $precendfun 0.0]
 	  if {$precendfun==""} break
	}
    }
    set insidefun 1
    if {$precfun == "" | $precendfun > $precfun} {
      set insidefun 0
    } else { 
# find the function name, excluding too pathological cases
      set i1 [$textarea index "$precfun+8c"]
      set i2 [$textarea index "$precfun lineend"]
      set funline [$textarea get $i1 $i2]
      set funname ""
      set funpat  "\[\%\#\]*\\m\[\\w%\#\]*\\M\[%\#\]*"
      if {[set i3 [string first "=" $funline]] !={}} {
	  regexp -start [expr $i3+1] $funpat $funline funname  
      } else {
	  regexp  $funpat $funline funname  
      }
      if {$funname==""} {set insidefun 0}
    }
    if {$insidefun == 0} {
#      tk_messageBox -message \
#	  "The cursor is not currently inside a function body"
	return {}
    } else {
# check how many continued (...) lines between $indexin and $precfun,
#  and derive the current line within the function definition
        set last $precfun
        set contlines 0
        while {[set ind [$textarea search -regexp "\\.{2,} *(//.*)?\\Z"\
			     $last $indexin]] != {}} {
            	if {[$textarea compare $ind >= $last]} {
		    set last $ind+1l
		    set contlines [expr $contlines+1]
		} else break
	}
        
        scan $precfun "%d." beginfunline 
	set lineinfun [expr $ypos-$beginfunline-$contlines+1]
# FV 13/05/04, message box commented
#      if {$lang == "eng"} {
#        tk_messageBox -message \
#	   "Being at line $ypos, function $funname begins at $precfun, and there are $contlines continued lines, i.e. we are at line $lineinfun of $funname"
#      } else {
#        tk_messageBox -message \
#	   "Etant à la ligne $ypos, la fonction $funname débute à $precfun, et il y a $contlines lignes multiples, i.e. nous sommes à la ligne $lineinfun de $funname"
#      }
        return [list $funname $lineinfun $funline $precfun] 
    }
}

proc showwhichfun {} {
    global lang
    set textarea [gettextareacur]
    set infun [whichfun [$textarea index insert]]
    if {$infun !={} } {
	set funname [lindex $infun 0]
	set lineinfun [lindex $infun 1]
      if {$lang == "eng"} {
        tk_messageBox -message "Function $funname line $lineinfun"
      } else {
        tk_messageBox -message "Fonction $funname ligne $lineinfun"
      }
    } else {
      if {$lang == "eng"} {
        tk_messageBox -message \
	    "The cursor is not currently inside a function body"
      } else {
        tk_messageBox -message \
	    "Le curseur ne se trouve pas à l'intérieur d'une fonction"
      }
    }
}

proc bindtext {textarea} {
    bind $textarea <Control-v> {pastetext}
    bind $textarea <Control-o> {}
    bind $textarea <Control-f> {}
    bind $textarea <KeyPress> { if {{%A} != {{}}} {puttext %W %A}}
    bind $textarea <Delete> { deletetext}
    bind $textarea <BackSpace> { backspacetext}
    bind $textarea <parenright> { if {{%A} != {{}}} {insblinkbrace %W %A}}
    bind $textarea <bracketright> { if {{%A} != {{}}} {insblinkbrace %W %A}} 
    bind $textarea <braceright>  { if {{%A} != {{}}} {insblinkbrace %W %A}}
}

load_words
update idletasks
#################
} 

