#!/bin/sh
# the next line restarts using wish \
exec `which wish` "$0" "$@"

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
set version "Version 1.1"
set wordWrap none
set printCommand lpr
set BGCOLOR "white"
set FGCOLOR "black"
set BASENAME scipad5.tcl;#[string range $argv0 [expr [string last "/" $argv0] + 1] end]
set argc 0
#
# added by Matthieu PHILIPPE
if { ![info exists lang] } { set lang "eng" }
if {[info exists SciPath]} { set env(SCIPATH) $SciPath };
set chset()                 {} 
set words()                 {}  
set listoffile("$pad.textarea",filename)   "[pwd]/Untitled.sce"
set listoffile("$pad.textarea",save) 0; # set flag to know if the editing file was change by user
set listoffile("$pad.textarea",new) 1; # is not an opened file from disk
set listoffile("$pad.textarea",thetime) 0; # set the time of the last modify
set listoftextarea $pad.textarea
set winopened 0
set radiobuttonvalue 0
###
# main window settings
eval destroy [winfo child $pad]
wm title $pad "$winTitle - $listoffile("$pad.textarea",filename)"
wm iconname $pad $winTitle
wm geometry $pad 80x25
wm minsize $pad 25 1 

#create main menu
menu $pad.filemenu -tearoff 0 

# start by setting default font sizes 
if [ expr [string compare $tcl_platform(platform) "unix"] ==0] {
	set textFont -Adobe-Helvetica-*-R-Normal-*-14-*
	set menuFont -adobe-helvetica-bold-r-normal--12-*-75-75-*-*-*-*
} else {
	set textFont -Adobe-Courier-*-R-Normal-*-14-*
	#set menuFont -adobe-helvetica-bold-r-normal--12-*-75-75-*-*-*-*
	set menuFont [$pad.filemenu cget -font]
}
$pad.filemenu configure -font $menuFont

# added by Matthieu PHILIPPE 18/12/2001
# to have the possibility to launch multiple
# browser from scilab with independant menu
# a little uggly !!!
set Size 0
proc setfontscipad0 {} {
    global textFont
    global listoftextarea

    set textFont -Adobe-Helvetica-*-R-Normal-*-12-*
    foreach textarea $listoftextarea {
	$textarea configure -font $textFont
    }
}

proc setfontscipad4 {} {
    global textFont
    global listoftextarea

    set textFont -Adobe-Helvetica-*-R-Normal-*-14-*
    foreach textarea $listoftextarea {
	$textarea configure -font $textFont
    }
}

proc setfontscipad12 {} {
    global textFont
    global listoftextarea

    set textFont -Adobe-Helvetica-*-R-Normal-*-18-*
    foreach textarea $listoftextarea {
	$textarea configure -font $textFont
    }
}

# create frames for widget layout
# this is for the text widget and the y scroll bar
frame $pad.bottomTopMenu
pack $pad.bottomTopMenu  -side top -expand 1 -fill both
# where the text widget is packed
frame $pad.bottomleftmenu
pack $pad.bottomleftmenu -in $pad.bottomTopMenu  -side left -expand 1 -fill both
# where the y scrollbar is packed
frame $pad.bottomrightmenu 
pack  $pad.bottomrightmenu -in $pad.bottomTopMenu  -side right -expand 0 -fill both 
# this is for the x scroll bar at the bottom of the window
frame $pad.bottombottommenu
pack $pad.bottombottommenu -side bottom -expand 0 -fill both

#file menu
menu $pad.filemenu.files -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu  add cascade -label "File" -underline 0 -menu $pad.filemenu.files
    $pad.filemenu.files add command -label "New" -underline 0 -command "filesetasnewmat"
    $pad.filemenu.files add command -label "Open" -underline 0 -command "filetoopen $textareacur" -accelerator Ctrl+o
    $pad.filemenu.files add command -label "Save" -underline 0 -command "filetosavecur" -accelerator Ctrl+s
    $pad.filemenu.files add command -label "Save As" -underline 5 -command "filesaveascur"
    $pad.filemenu.files add separator
    if {"$tcl_platform(platform)" == "unix"} {
	$pad.filemenu.files add command -label "Print Setup" -underline 8 -command "printseupselection"
	$pad.filemenu.files add command -label "Print" -underline 0 -command "selectprint $textareacur"
	$pad.filemenu.files add separator
    }
    $pad.filemenu.files add command -label "Close" -underline 0 -command "closecur"
    $pad.filemenu.files add command -label "Exit" -underline 1 -command "exitapp"
} else {
    $pad.filemenu  add cascade -label "Fichier" -underline 0 -menu $pad.filemenu.files
    $pad.filemenu.files add command -label "Nouveau" -underline 0 -command "filesetasnewmat"
    $pad.filemenu.files add command -label "Ouvrir" -underline 0 -command "filetoopen $textareacur" -accelerator Ctrl+o
    $pad.filemenu.files add command -label "Enregistrer" -underline 0 -command "filetosavecur" -accelerator Ctrl+s
    $pad.filemenu.files add command -label "Enregistrer sous" -underline 5 -command "filesaveascur"
    $pad.filemenu.files add separator
    if {"$tcl_platform(platform)" == "unix"} {
	$pad.filemenu.files add command -label "Mise en page" -underline 8 -command "printseupselection"
	$pad.filemenu.files add command -label "Imprimer" -underline 0 -command "selectprint $textareacur"
	$pad.filemenu.files add separator
    }
    $pad.filemenu.files add command -label "Fermer" -underline 0 -command "closecur"
    $pad.filemenu.files add command -label "Quitter" -underline 1 -command "exitapp"   
}

#edit menu
menu $pad.filemenu.edit -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Edit" -underline 0 -menu $pad.filemenu.edit
    $pad.filemenu.edit add command -label "Undo" -underline 0 -command " undo_menu_proc" -accelerator Ctrl+z
    $pad.filemenu.edit add command -label "Redo" -underline 0 -command "redo_menu_proc" -accelerator Ctrl+y
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Cut" -underline 2 -command "cuttext" -accelerator Ctrl+x
    $pad.filemenu.edit add command -label "Copy" -underline 0 -command "copytext" -accelerator Ctrl+c
    $pad.filemenu.edit add command -label "Paste" -underline 0 -command "pastetext" -accelerator Ctrl+v
    $pad.filemenu.edit add command -label "Delete" -underline 2 -command "deletetext" -accelerator Del
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Select All" -underline 7 -command "selectall" -accelerator Ctrl+/
#    $pad.filemenu.edit add command -label "Time/Date" -underline 5 -command "printtime"
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add check -label "Word Wrap" -underline 5 -command "wraptext"
} else {
    $pad.filemenu add cascade -label "Edition" -underline 0 -menu $pad.filemenu.edit
    $pad.filemenu.edit add command -label "Annuler" -underline 0 -command " undo_menu_proc" -accelerator Ctrl+z
    $pad.filemenu.edit add command -label "Répéter" -underline 0 -command "redo_menu_proc" -accelerator Ctrl+y
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Couper" -underline 2 -command "cuttext" -accelerator Ctrl+x
    $pad.filemenu.edit add command -label "Copier" -underline 0 -command "copytext" -accelerator Ctrl+c
    $pad.filemenu.edit add command -label "Coller" -underline 0 -command "pastetext" -accelerator Ctrl+v
    $pad.filemenu.edit add command -label "Effacer" -underline 2 -command "deletetext" -accelerator Del
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add command -label "Selectionner tout" -underline 7 -command "selectall" -accelerator Ctrl+/
#    $pad.filemenu.edit add command -label "Time/Date" -underline 5 -command "printtime"
    $pad.filemenu.edit add separator
    $pad.filemenu.edit add check -label "Retour à la ligne automatique" -underline 5 -command "wraptext"
} 

#search menu
menu $pad.filemenu.search -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Search" -underline 0 -menu $pad.filemenu.search 
    $pad.filemenu.search add command -label "Find" -underline 0 -command "findtext find" -accelerator Ctrl+f
    $pad.filemenu.search add command -label "Find Next" -underline 1 -command "findnext find" -accelerator F3
    $pad.filemenu.search add command -label "Replace" -underline 0 -command "findtext replace" -accelerator Ctrl+r
    # add new menu option include by Matthieu PHILIPPE from gotoline.pth 21/11/2001
    $pad.filemenu.search add command -label "Goto Line" -underline 0 -command "gotoline"
} else {
    $pad.filemenu add cascade -label "Rechercher" -underline 0 -menu $pad.filemenu.search 
    $pad.filemenu.search add command -label "Rechercher" -underline 0 -command "findtext find" -accelerator Ctrl+f
    $pad.filemenu.search add command -label "Rechercher suivant" -underline 1 -command "findnext find" -accelerator F3
    $pad.filemenu.search add command -label "Remplacer" -underline 0 -command "findtext replace" -accelerator Ctrl+r
    # add new menu option include by Matthieu PHILIPPE from gotoline.pth 21/11/2001
    $pad.filemenu.search add command -label "Atteindre" -underline 0 -command "gotoline"
} 

# added by matthieu PHILIPPE dec 11th 2001
# window menu
if {$lang == "eng"} {
    menu $pad.filemenu.wind -tearoff 1 -title "Opened Files" -font $menuFont
    $pad.filemenu add cascade -label "Windows" -underline 0 -menu $pad.filemenu.wind
    #$pad.filemenu.wind add command -label "$listoffile("$pad.textarea",filename)" -command "montretext $pad.textarea"
} else {
    menu $pad.filemenu.wind -tearoff 1 -title "Fichiers ouverts" -font $menuFont
    $pad.filemenu add cascade -label "Fenêtres" -underline 0 -menu $pad.filemenu.wind
    #$pad.filemenu.wind add command -label "$listoffile("$pad.textarea",filename)" -command "montretext $pad.textarea"
}
$pad.filemenu.wind add radiobutton -label "$listoffile("$pad.textarea",filename)"  -value 0 -variable radiobuttonvalue -command "montretext $pad.textarea"
# options menu
if {$lang == "eng"} {
    menu $pad.filemenu.options -tearoff 1 -font $menuFont
    $pad.filemenu add cascade -label "Options" -underline 0 -menu $pad.filemenu.options
    $pad.filemenu.options add command -label "font size" -foreground red 
    $pad.filemenu.options add radiobutton -label "small" -value 0  -variable $Size -command "setfontscipad0"
    $pad.filemenu.options add radiobutton -label "medium" -value 4  -variable $Size -command "setfontscipad4"
    $pad.filemenu.options add radiobutton -label "large" -value 12  -variable $Size -command "setfontscipad12"
} else {
    menu $pad.filemenu.options -tearoff 1 -font $menuFont
    $pad.filemenu add cascade -label "Options" -underline 0 -menu $pad.filemenu.options
    $pad.filemenu.options add command -label "taille de fontes" -foreground red 
    $pad.filemenu.options add radiobutton -label "petit" -value 0  -variable $Size -command "setfontscipad0"
    $pad.filemenu.options add radiobutton -label "moyen" -value 4  -variable $Size -command "setfontscipad4"
    $pad.filemenu.options add radiobutton -label "grand" -value 12  -variable $Size -command "setfontscipad12"
}
set Size 2
# exec menu
if {$lang == "eng"} {
    $pad.filemenu add command -label "Load into Scilab" -command "execfile"
} else {
    $pad.filemenu add command -label "Charger dans Scilab" -command "execfile"
}

proc execfile {} {
    global listoffile sciprompt lang

    set textarea [gettextareacur]

    set doexec 1
    if [ expr [string compare [getccount $textarea] 1] == 0 ] {
	if {$lang == "eng"} {
	    set answer [tk_messageBox -message "The contents of $listoffile("$textarea",filename) may have changed, do you wish to to save your changes?" -title "Save Confirm?" -type yesnocancel -icon question]
	} else {
	    set answer [tk_messageBox -message "Voulez-vous enregistrer les modifications apportées à $listoffile("$textarea",filename) ?" -title "Confirmer sauver ?" -type yesnocancel -icon question]
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
		tk_messageBox -message "Scilab is working, wait for the prompt to load file $listoffile("$textarea",filename)" -title "Scilab working" -type ok -icon info
	    } else {
		tk_messageBox -message "Scilab est occupé, attendez le prompt pour charger le fichier $listoffile("$textarea",filename)" -title "Scilab occupé" -type ok -icon info
	    }
	} else {
	    set f $listoffile("$textarea",filename)
	    ScilabEval "exec(\"$f\");"
	}
    }
}

# help menu
menu $pad.filemenu.help -tearoff 0 -font $menuFont
if {$lang == "eng"} {
    $pad.filemenu add cascade -label "Help" -underline 0 -menu $pad.filemenu.help
    $pad.filemenu.help add command -label "Help" -underline 0 -command "helpme"
    $pad.filemenu.help add command -label "About" -underline 0 -command "aboutme"
} else {
    $pad.filemenu add cascade -label "Aide" -underline 0 -menu $pad.filemenu.help
    $pad.filemenu.help add command -label "Aide" -underline 0 -command "helpme"
    $pad.filemenu.help add command -label "Apropos" -underline 0 -command "aboutme"
}

# now make the menu visible
$pad configure -menu $pad.filemenu 

#create textarea
set taille [expr [font measure $textFont " "] *3]

# creates the default textarea 
text $pad.textarea -relief sunken -bd 2 -xscrollcommand "$pad.xscroll set" \
	-yscrollcommand "$pad.yscroll set" -wrap $wordWrap -width 1 -height 1 \
        -fg $FGCOLOR -bg $BGCOLOR -font $textFont -setgrid 1 -tabs $taille -insertwidth 3 -insertborderwidth 8 
set textareacur $pad.textarea
####

scrollbar $pad.yscroll -command "$textareacur yview"
scrollbar $pad.xscroll -command "$textareacur xview" -orient horizontal
pack $textareacur  -in  $pad.bottomleftmenu -side left -expand 1 -fill both
pack $pad.yscroll -in $pad.bottomrightmenu -side right -expand 1 -fill both
pack $pad.xscroll -in $pad.bottombottommenu -expand 1 -fill x 
focus $textareacur

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
	return [eval  "[string tolower [winfo class $widget]] $name [nondefOpts $widget]"]
}

#
#
#matthieu PHLIPPE 12/12/2001
proc dupWidgetText {widget name} {
	$name insert end [$widget get 1.0 end]
}
##

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
    global FGCOLOR

    $t tag configure parenthesis -foreground red
    $t tag configure bracket -foreground red
    $t tag configure brace -foreground red
    $t tag configure keywords -foreground blue
    $t tag configure text -foreground $FGCOLOR
    $t tag configure textquoted -foreground green
    $t tag configure rem2 -foreground gray
    scipadindent $t .8
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
    after 5000 delinfo
}

proc blinkbrace {w pos brace} {
	global	bracefind fno

	switch $brace {
		\{	{ set findbs {[{}]}; set bs "\}"; set dir {-forwards}; set bn brace }
		\}	{ set findbs {[{}]}; set bs "\{"; set dir {-backwards}; set bn brace }
		\[	{ set findbs {[][]}; set bs "\]"; set dir {-forwards}; set bn bracket }
		\]	{ set findbs {[][]}; set bs "\["; set dir {-backwards}; set bn bracket }
		(	{ set findbs {[()]}; set bs ")"; set dir {-forwards}; set bn parencesis }
		)	{ set findbs {[()]}; set bs "("; set dir {-backwards}; set bn parencesis }
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
		showinfo "No corresponding <$bs> found!"
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
    #puttext $w "\n$n"
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
entry $pad.statusmes -relief groove -state disabled -background $colormen -foreground blue
pack $pad.statusind -in $pad.bottombottommenu -side right -expand 0
pack $pad.statusmes -in $pad.bottombottommenu -side bottom -expand 0 -fill x

# this proc gets the posn and sets the statusbar
proc keyposn {textarea} {
    global pad
    $pad.statusind configure -state normal
    set indexin [$textarea index insert]
    $pad.statusind delete 0 end 
    # changement Matthieu PHILIPPE 30/11/2001
    scan $indexin "%d.%d" ypos xpos
    $pad.statusind insert 0 "Line: $ypos   Column: $xpos"
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
	set msgWait "Patientez un instant le temps du chargement et de la colorisation"
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
tkTextSetCursor $textareacur "1.0"
keyposn $textareacur
wm geometry $pad 65x24




# help menu
proc helpme {} {
    global lang
    if {$lang == "eng"} {
	tk_messageBox -title "Basic Help" -type ok -message "This is a simple editor for Scilab."
    } else {
	tk_messageBox -title "Aide de base" -type ok -message "C'est un simple éditeur pour Scilab."
    }
}

# about menu
proc aboutme {} {
    global winTitle version lang
    if {$lang == "eng"} {
	tk_messageBox -title "About" -type ok -message "$winTitle $version\n originated by Joseph Acosta,\n\
	    joeja@mindspring.com.\n\
            Modified by Scilab Group"
    } else {
	tk_messageBox -title "Apropos" -type ok -message "$winTitle $version\n créé par Joseph Acosta,\n\
	    joeja@mindspring.com.\n\
            Modifié par le Groupe Scilab"
    }
}

# generic case switcher for message box
proc switchcase {yesfn argyesfn nofn argnofn} {
    global saveTextMsg lang
    if [ expr [string compare [getccount [gettextareacur]] 1] ==0 ] {
	if {$lang == "eng"} {
	    set answer [tk_messageBox -message "The contents of this file may have changed, do you wish to save your changes?" \
		    -title "Save Confirm?" -type yesnocancel -icon question]
	} else {
	    set answer [tk_messageBox -message "Voulez-vous enregistrer les modifications apportées à ce fichier ?" \
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

    incr winopened
    dupWidgetOption [gettextareacur] $pad.new$winopened
    set listoffile("$pad.new$winopened",filename) [pwd]/Untitled$winopened.sce
    set listoffile("$pad.new$winopened",save) 0
    set listoffile("$pad.new$winopened",new) 1
    set listoffile("$pad.new$winopened",thetime) 0
    set listundo_id("$pad.new$winopened") [new textUndoer $pad.new$winopened]
    lappend listoftextarea $pad.new$winopened
    if [ expr [string compare $tcl_platform(platform) "unix"] ==0] {
	# more bindings
	bind Text <Control-v> {}
	bind $pad.new$winopened <Control-v> {pastetext}
    } else {
	bind Text <Control-v> {}
	bind $pad.new$winopened <Control-v> {pastetext}
    }
    bind $pad.new$winopened <KeyRelease> {keyposn %W}
    bind $pad.new$winopened <ButtonRelease> {keyposn %W}
    TextStyles $pad.new$winopened
    #$pad.filemenu.wind add command -label "[pwd]/Untitled$winopened.sce" -command "montretext $pad.new$winopened"
    $pad.filemenu.wind add radiobutton -label "[pwd]/Untitled$winopened.sce" -value $winopened -variable radiobuttonvalue -command "montretext $pad.new$winopened"
    set radiobuttonvalue $winopened
    showinfo "New Script"
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
	global pad
	if {  [ llength $listoftextarea ] > 1 } {
	    # delete the textarea entry in the listoftextarea
	    set listoftextarea [lreplace $listoftextarea [lsearch $listoftextarea $textarea] [lsearch $listoftextarea $textarea]]
	    # delte the menu windows entry
	    $pad.filemenu.wind delete [$pad.filemenu.wind index $listoffile("$textarea",filename)]
	    # delete the textarea entry in listoffile
	    unset listoffile("$textarea",filename) 
	    unset listoffile("$textarea",save) 
	    unset listoffile("$textarea",new) 
	    unset listoffile("$textarea",thetime) 
	    # delete the textarea entry in Undoerr
	    destroy $textarea
	    # place as current textarea the last 
	    montretext [lindex $listoftextarea end]
	} else {
	    killwin $pad 
	}   
    }
    #### end of bye bye

    if  [ expr [string compare [getccount $textarea] 1] == 0 ] {
	if {$lang == "eng"} {
	    set answer [tk_messageBox -message "The contents of $listoffile("$textarea",filename) may have changed, do you wish to to save your changes?" -title "Save Confirm?" -type yesnocancel -icon question]
	} else {
	    set answer [tk_messageBox -message "Voulez-vous enregistrer les modifications apportées à $listoffile("$textarea",filename) ?" -title "Confirmer sauver ?" -type yesnocancel -icon question]
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
    unset pad
}

proc montretext {textarea} {
    global listoffile
    global pad
    pack forget [gettextareacur]
    settextareacur $textarea
    $pad.yscroll configure -command "[gettextareacur] yview"
    $pad.xscroll configure -command "[gettextareacur] xview"
    settitle $listoffile("$textarea",filename)
    pack  $textarea -in  $pad.bottomleftmenu -side left -expand 1 -fill both
    focus $textarea
}


# bring up open win
# modified by Matthieu PHILIPPE 16/12/2001
# added Open multifiles
# added show an opened file
proc showopenwin {textarea} {
    global listoffile
    global FGCOLOR 
    global BGCOLOR 
    global textFont 
    global taille 
    global wordWrap 
    global winopened
    global textareacur
    global listoftextarea
    global tcl_platform
    global listundo_id
    global pad
    global radiobuttonvalue
    global lang
    if {$lang == "eng"} {
	set types {
	    {"Scilab files" {*.sce *.sci }} 
	    {"All files" *.*}
	}
	#showinfo "Open file"
    } else {
	set types {
	    {"Fichiers Scilab" {*.sce *.sci }} 
	    {"Tous les fichiers" *.*}
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
	    #####
	    setTextTitleAsNew $pad.new$winopened
	    montretext $pad.new$winopened
	    openoninit $pad.new$winopened $file
	    outccount $pad.new$winopened
	    update
	    colorize $pad.new$winopened 1.0 end
	    set listundo_id("$pad.new$winopened") [new textUndoer $pad.new$winopened]
	    if [ expr [string compare $tcl_platform(platform) "unix"] ==0] {
		# more bindings
		bind Text <Control-v> {}
		bind $pad.new$winopened <Control-v> {pastetext}
	    } else {
		bind Text <Control-v> {}
		bind $pad.new$winopened <Control-v> {pastetext}
	    }
	    bind $pad.new$winopened <KeyRelease> {keyposn %W}
	    bind $pad.new$winopened <ButtonRelease> {keyposn %W}
	    TextStyles $pad.new$winopened
	    #$pad.filemenu.wind add command -label "$file" -command "montretext $pad.new$winopened"
	    $pad.filemenu.wind add radiobutton -label "$file" -value $winopened -variable radiobuttonvalue  -command "montretext $pad.new$winopened"
	    set radiobuttonvalue $winopened
	} else {
	    # file is already opened
	    tk_messageBox -type ok -title "Open file" -message "This file is already opened ! save the current opened file to an another name and reopen it from disk"
	    $pad.filemenu.wind invoke $res
	}
	selection clear
    }
}

#open an existing file
proc filetoopen {textarea} {
    #switchcase filetosave $textarea showopenwin $textarea
    showopenwin $textarea
}

# generic save function
proc writesave {textarea nametosave} {
    set FileNameToSave [open $nametosave w+]
    puts -nonewline $FileNameToSave [$textarea get 0.0 end]
    close $FileNameToSave
    outccount $textarea
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
	set msgChanged "The contents of $listoffile("$textarea",filename) has changed on Disk, Save it anyway ?"
	set msgTitle "File has changed !"
    } else {
	set msgChanged "Le contenu de $listoffile("$textarea",filename) a changè sur le disque, étes-vous sur de vouloir le sauvegarder ?"
	set msgTitle "Le fichie a changè"
    }

    # save the opened file from disk, if not, user has to get a file name.
    # we would verify if the file has not been modify by antother application
    if { [file exists $listoffile("$textarea",filename)] && $listoffile("$textarea",new) == 0  } {
	if { $listoffile("$textarea",thetime) != [file mtime $listoffile("$textarea",filename)]} {
	    set answer [tk_messageBox -message $msgChanged -title $msgTitle -type yesnocancel -icon question]
	    case $answer {
		yes { writesave $textarea $listoffile("$textarea",filename); set listoffile("$textarea",thetime) [file mtime $listoffile("$textarea",filename)]}
		no {}
		cancel {}
	    }
	} else {  
	    writesave $textarea $listoffile("$textarea",filename)
	    set listoffile("$textarea",thetime) [file mtime $listoffile("$textarea",filename)] 
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
	    {"All files" *.*}
	}
	showinfo "Save As"
    } else {
	set types {
	    {"Fichiers Scilab" {*.sce *.sci }} 
	    {"Tous les fichiers" *.*}
	}
	showinfo "Enregistrer sous"
    }
    set myfile [tk_getSaveFile -filetypes $types -parent $pad -initialfile $listoffile("$textarea",filename)]
    if { [expr [string compare $myfile ""]] != 0} {
       $pad.filemenu.wind delete "$listoffile("$textarea",filename)"
	set listoffile("$textarea",filename) $myfile
	set listoffile("$textarea",new) 0
	set listoffile("$textarea",save) 0
	writesave  $textarea $myfile
	set listoffile("$textarea",thetime) [file mtime $listoffile("$textarea",filename)] 
	#$pad.filemenu.wind add command -label "$myfile" -command "montretext $textarea"
	$pad.filemenu.wind add radiobutton -label "$myfile" -value $radiobuttonvalue -variable radiobuttonvalue -command "montretext $textarea"
	#set radiobuttonvalue $winopened
	settitle $myfile
        return 1
    }
    return 0
}

# proc to set child window position
proc setwingeom {wintoset} {
    global pad
    wm resizable $wintoset 0 0
    set myx [expr (([winfo screenwidth $pad]/2) - ([winfo reqwidth $wintoset]))]
    set myy [expr (([winfo screenheight $pad]/2) - ([winfo reqheight $wintoset]/2))]
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
    pack $print.top.label $print.top.print -in $print.top -side left -fill x -fill y
    pack $print.bottom.ok $print.bottom.cancel -in $print.bottom -side left -fill x -fill y
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
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] [[gettextareacur] index "$i1 wordend"]
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
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] [[gettextareacur] index "$i1 wordend"]
}

#cut text procedure
proc cuttext {} {
    global textareacur
    tk_textCut [gettextareacur]
    inccount [gettextareacur]
    # Added by Matthieu PHILIPPE
    set  i1 [[gettextareacur] index insert]
    colorize [gettextareacur] [[gettextareacur] index "$i1 linestart"] [[gettextareacur] index "$i1 lineend"]
    selection clear
}

#copy text procedure
proc copytext {} {
    global textareacur
    tk_textCopy  [gettextareacur]
    inccount [gettextareacur]
    selection clear
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
    colorize [gettextareacur] [[gettextareacur] index "$i1 wordstart"] [[gettextareacur] index "$i2 wordend"]
}

proc FindIt {w} {
    global SearchString SearchPos SearchDir findcase 
    global textareacur pad
    [gettextareacur] tag configure sel -background green
    if {$SearchString!=""} {
	if {$findcase=="1"} {
	    set caset "-exact"
	} else {
	    set caset "-nocase"
	}
	if {$SearchDir == "forwards"} {
	    set limit end
	} else {
	    set limit 1.0
	}
	set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir $SearchString $SearchPos $limit]
	set len [string length $SearchString]
	if {$SearchPos != ""} {
	    [gettextareacur] see $SearchPos
	    tkTextSetCursor [gettextareacur] $SearchPos
	    [gettextareacur] tag add sel $SearchPos  "$SearchPos + $len char"
	    
	    if {$SearchDir == "forwards"} {
		set SearchPos "$SearchPos + $len char"
	    }         
	} else {
	    set SearchPos "0.0"
	}
    }
    focus [gettextareacur]
}

proc ReplaceIt {} {
    global SearchString SearchDir ReplaceString SearchPos findcase
    global textareacur
    if {$SearchString != ""} {
	if {$findcase=="1"} {
	    set caset "-exact"
	} else {
	    set caset "-nocase"
	}
	if {$SearchDir == "forwards"} {
	    set limit end
	} else {
	    set limit 1.0
	}
	set SearchPos [ [gettextareacur] search -count len $caset -$SearchDir $SearchString $SearchPos $limit]
	set len [string length $SearchString]
	if {$SearchPos != ""} {
	    [gettextareacur] see $SearchPos
	    [gettextareacur] delete $SearchPos "$SearchPos+$len char"
	    [gettextareacur] insert $SearchPos $ReplaceString
	    colorize [gettextareacur] [[gettextareacur] index "$SearchPos linestart"] [[gettextareacur] index "$SearchPos lineend"]
	    if {$SearchDir == "forwards"} {
		set SearchPos "$SearchPos+$len char"
	    }         
	} else {
	    set SearchPos "0.0"
	}
    }
    inccount [gettextareacur]
}

proc ReplaceAll {} {
      global SearchPos SearchString
       if {$SearchString != ""} {
                ReplaceIt
	while {$SearchPos!="0.0"} {
		ReplaceIt
	}
       }
}

proc CancelFind {w} {
    global textareacur pad
    [gettextareacur] tag delete tg1
    bind $pad <Expose> {};#ajout pour mettre a la fenetre Search devant le scipad !
    destroy $w
}

proc ResetFind {} {
    global SearchPos
    set SearchPos insert
}

# procedure to find text
proc findtext {typ} {
	global SearchString SearchDir ReplaceString findcase c find pad lang
	set find $pad.find
	catch {destroy $find}
	toplevel $find
	if {$lang == "eng"} {
	    wm title $find "Find"
	} else {
	    wm title $find "Rechercher"
	}
	setwingeom $find
	ResetFind
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
	if {$typ=="replace"} {
	    frame $find.l.f2
	    if {$lang == "eng"} {
		label $find.l.f2.label2 -text "Replace with:" -width 11
	    } else {
		label $find.l.f2.label2 -text "Remplacer par :" -width 11
	    }
	    entry $find.l.f2.entry2  -textvariable ReplaceString -width 30 
	    pack $find.l.f2.label2 $find.l.f2.entry2 -side left
	    pack $find.l.f1 $find.l.f2 -side top
	} else {
	    pack $find.l.f1
	}
	frame $find.f2
	if {$lang == "eng"} {
	    button $find.f2.button1 -text "Find Next" -command "FindIt $find" -width 10 -height 1 -underline 5 
	    button $find.f2.button2 -text "Cancel" -command "CancelFind $find" -width 10 -underline 0
	} else {
	    button $find.f2.button1 -text "Rechercher suivant" -command "FindIt $find" -width 15 -height 1 -underline 5 
	    button $find.f2.button2 -text "Annuler" -command "CancelFind $find" -width 10 -underline 0	    
	}
	if {$typ=="replace"} {
	    if {$lang == "eng"} {
		button $find.f2.button3 -text "Replace" -command ReplaceIt -width 10 -height 1 -underline 0
		button $find.f2.button4 -text "Replace All" -command ReplaceAll -width 10 -height 1 -underline 8		
	    } else {
		button $find.f2.button3 -text "Remplacer" -command ReplaceIt -width 10 -height 1 -underline 0
		button $find.f2.button4 -text "Remplacer tout" -command ReplaceAll -width 12 -height 1 -underline 8		
	    }
	    pack $find.f2.button3 $find.f2.button4 $find.f2.button2  -pady 4
	} else {
	    pack $find.f2.button1 $find.f2.button2  -pady 4
	}
	frame $find.l.f4
	frame $find.l.f4.f3 -borderwidth 2 -relief groove
	if {$lang == "eng"} {
	    radiobutton $find.l.f4.f3.up -text "Up" -underline 0 -variable SearchDir -value "backwards" 
	    radiobutton $find.l.f4.f3.down -text "Down"  -underline 0 -variable SearchDir -value "forwards"
	} else {
	    radiobutton $find.l.f4.f3.up -text "Vers le haut" -underline 0 -variable SearchDir -value "backwards" 
	    radiobutton $find.l.f4.f3.down -text "Vers le bas"  -underline 0 -variable SearchDir -value "forwards"
	} 
	$find.l.f4.f3.down invoke
	pack $find.l.f4.f3.up $find.l.f4.f3.down -side left
	if {$lang == "eng"} {
	    checkbutton $find.l.f4.cbox1 -text "Match case" -variable findcase -underline 0
	} else {
	    checkbutton $find.l.f4.cbox1 -text "Respecter la casse" -variable findcase -underline 0
	}
	pack $find.l.f4.cbox1 $find.l.f4.f3 -side left -padx 10
	pack $find.l.f4 -pady 11
	pack $find.l $find.f2 -side left -padx 1
	bind $find <Escape> "destroy $find"

     # each widget must be bound to th eevents of the other widgets
     proc bindevnt {widgetnm types find} {
	if {$types=="replace"} {
		bind $widgetnm <Return> "ReplaceIt"
		bind $widgetnm <Control-r> "ReplaceIt"
		bind $widgetnm <Control-a> "ReplaceAll"
	} else {
		bind $widgetnm <Return> "FindIt $find"
		bind $widgetnm <Control-n> "FindIt $find"
	}
	bind $widgetnm <Control-m> { $find.l.f4.cbox1 invoke }
	bind $widgetnm <Control-u> { $find.l.f4.f3.up invoke }
	bind $widgetnm <Control-d> { $find.l.f4.f3.down invoke }
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
        bindevnt $find.l.f4.cbox1 $typ $find
	bindevnt $find.l.f1.entry $typ $find	
    bind $find <Control-c> "destroy $find";
    bind $find <Visibility> {raise $find $pad};#ajout pour mettre a la fenetre Search devant le scipad !
    bind $pad <Expose> {raise $find $pad};#ajout pour mettre a la fenetre Search devant le scipad !
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
	    button $gotln.bottom.cancel -text "Cancel" -command "destroy $gotln"
	} else {
	    button $gotln.bottom.cancel -text "Annuler" -command "destroy $gotln"
	}
	focus $gotln.top.gotln
	pack $gotln.top -side top -expand 0 
	pack $gotln.bottom -side bottom -expand 0 
	pack $gotln.top.label $gotln.top.gotln -in $gotln.top -side left -fill x -fill y
	pack $gotln.bottom.ok $gotln.bottom.cancel -in $gotln.bottom -side left -fill x -fill y
	bind $gotln <Return> "addtogotln $gotln"
	bind $gotln <Escape> "destroy $gotln"

    proc addtogotln {prnt} {
	global textareacur
	global gotlnCommand
	set gotlnCommand [$prnt.top.gotln get]
	tkTextSetCursor [gettextareacur] "$gotlnCommand.0"
	catch {keyposn [gettextareacur]}
	destroy $prnt
    }
}

#procedure to set the time change %R to %I:%M for 12 hour time display
proc printtime {} {
    global textareacur
    [gettextareacur] insert insert [clock format [clock seconds] -format "%R %p %D"]
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
}
# this resets saveTextMsg to 0
proc outccount {textarea} {
    global listoffile
    global saveTextMsg
    set saveTextMsg 0 
    set listoffile("$textarea",save) 0
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
bind $pad <Control-y> {redo_menu_proc}
bind $pad <Control-f> {findtext find}
bind $pad <Control-r> {findtext replace}
bind $pad <Button> {selection clear}
# because windows is 'different' and mac is unknown
if [ expr [string compare $tcl_platform(platform) "unix"] ==0] {
	#events
	set tk_strictMotif 0
	event delete <<Cut>> <Control-x>
	event delete <<Paste>> <Control-v>
        event delete <<Paste>> <Control-Key-y>
	# more bindings
	bind Text <Control-v> {}
	bind $textareacur <Control-v> {pastetext}
    } else {
	#events
	set tk_strictMotif 0
	event delete <<Cut>> <Control-x>
	event delete <<Paste>> <Control-v>
        event delete <<Paste>> <Control-Key-y>
	# more bindings
	bind Text <Control-v> {}
	bind $textareacur <Control-v> {pastetext}
    }

bind $textareacur <KeyRelease> {keyposn %W}
bind $textareacur <ButtonRelease> {keyposn %W}

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
    # and delete all this object array members if any (assume that they were stored as $className($id,memberName))
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
    bindtags $widget [concat $textUndoer($id,originalBindingTags) UndoBindings($id)]

    bind UndoBindings($id) <Control-u> "textUndoer:undo $id"

    # self destruct automatically when text widget is gone
    bind UndoBindings($id) <Destroy> "delete textUndoer $id"

    # rename widget command
    rename $widget [set textUndoer($id,originalCommand) textUndoer:original$widget]
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
        set index [$textUndoer($id,originalCommand) index [lindex $arguments 0]]
        lifo:push $textUndoer($id,commandStack) "delete $index $index+${length}c"
        lifo:push $textUndoer($id,cursorStack) [$textUndoer($id,originalCommand) index insert]
    }
}

proc textUndoer:processDeletion {id arguments} {
    global textUndoer

    set command $textUndoer($id,originalCommand)
    lifo:push $textUndoer($id,cursorStack) [$command index insert]

    set start [$command index [lindex $arguments 0]]
    if {[llength $arguments]>1} {
        lifo:push $textUndoer($id,commandStack) "insert $start [list [$command get $start [lindex $arguments 1]]]"
    } else {
        lifo:push $textUndoer($id,commandStack) "insert $start [list [$command get $start]]"
    }
}

proc textUndoer:undo {id} {
    global textUndoer

    if {[catch {set cursor [lifo:pop $textUndoer($id,cursorStack)]}]} {
        return
    }
    
    set popArgs [lifo:pop $textUndoer($id,commandStack)] 
    textRedoer:checkpoint $id $popArgs
    
    eval $textUndoer($id,originalCommand) $popArgs
    # now restore cursor position
    $textUndoer($id,originalCommand) mark set insert $cursor
    # make sure insertion point can be seen
    $textUndoer($id,originalCommand) see insert
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
        set index [$textUndoer($id,originalCommand) index [lindex $arguments 0]]
        lifo:push $textRedoer($id,commandStack) "delete $index $index+${length}c"
        lifo:push $textRedoer($id,cursorStack) [$textUndoer($id,originalCommand) index insert]
    }
}

proc textRedoer:processDeletion {id arguments} {
    global textUndoer textRedoer
    set command $textUndoer($id,originalCommand)
    lifo:push $textRedoer($id,cursorStack) [$command index insert]

    set start [$command index [lindex $arguments 0]]
    if {[llength $arguments]>1} {
        lifo:push $textRedoer($id,commandStack) "insert $start [list [$command get $start [lindex $arguments 1]]]"
    } else {
        lifo:push $textRedoer($id,commandStack) "insert $start [list [$command get $start]]"
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
    textUndoer:undo $listundo_id("[gettextareacur]")
    set i2 [[gettextareacur] index insert]
    colorize [gettextareacur] $i1 [[gettextareacur] index "$i2+1l linestart"]
    inccount [gettextareacur]
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

proc colorize {w cpos iend} {
	global	words chset
        set num 0
	$w mark set begin "$cpos linestart"
	$w mark set ende "$iend+1l linestart"
	$w tag remove parenthesis begin ende
	$w tag remove bracket begin ende
	$w tag remove brace begin ende
	$w tag remove text begin ende
	$w tag remove textquoted begin ende
	$w tag remove keywords begin ende
	$w tag remove rem2 begin ende
        $w tag remove indentation begin ende
# Scilab specials
	$w mark set last begin
	while {[set ind [$w search -regexp {[][;:]} last ende]] != {}} {
		if {[$w compare $ind >= last]} {
			$w mark set last $ind+1c
			$w tag add brace $ind last
		} else break
	}
# Scilab
	$w mark set last begin
	while {[set ind [$w search -count num -regexp "(\[^A-Za-z0-9_\]|^)\[$chset(scilab.col1)\]" last ende]] != {}} {
	    if {[$w compare $ind >= last]} {
		set res ""
		regexp "(\[^A-Za-z0-9_\]|^)\[$chset(scilab.col1)\]" [$w get $ind end] res
		set num [string length $res]
		$w mark set last "$ind + $num c"
		$w mark set next {last wordend}
		set word [$w get last-1c next]
		if {[lsearch -exact $words(scilab.col1.[string range $word 0 0]) $word] != -1} {
		    $w tag add keywords last-1c next
		}
		$w mark set last next-1c
	    } else break
	  }
# Text
	  $w mark set last begin
	  while { [set ind [$w search -count num -regexp {"[^"]*("|$)} last ende]] != {}} {
	      if {[$w compare $ind >= last]} {
		  set res ""
		  $w mark set endetext "$ind lineend"
		  regexp {"[^"]*"} [$w get last endetext] res
		  set num [string length $res]
		  if {$num <= 0} {
		      $w mark set last "$ind + 1c"
		  } else {
		      $w mark set last "$ind + $num c"
		      $w tag add textquoted $ind last
		  }	  
	      } else break
	      }
# remark
	      $w mark set last begin
	      while {[set ind [$w search -exact {//} last ende]] != {}} {
		  if {[$w compare $ind >= last]} {
		      $w tag add rem2 $ind "$ind lineend"
		      $w mark set last "$ind+1l linestart"
		  } else break
		  }
	      }

proc selectall {} {
[gettextareacur] tag add sel 1.0 end
}

proc puttext {w text} {
    set rem 0
    set cuttexts [selection own]
    if {$cuttexts != "" } {
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
    $w see insert
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
