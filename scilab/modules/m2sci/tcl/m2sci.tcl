# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
# 
# This file is distributed under the same license as the Scilab package.

# GUI for M2SCI tools

#!/bin/sh
#exec wish "$0" "$1" & exit 0 

# Scilab specification
toplevel .m2sci

ScilabEval "TCL_SetVar(\"localizedtitle\",gettext(\"Matlab to Scilab conversion tool\"))" "sync"
wm title .m2sci $localizedtitle


bind .m2sci <Destroy> {cancelConversionCross}
######################################################################
# Gets the directory to convert, and initialize the eTranslatePath
######################################################################
proc setPathToConvert {} { 
    global rFlagWhatToConvert
    set ePathToConvert [ tk_chooseDirectory ]
    
    # Refresh the entry text
    .m2sci.mainframe.eDirName delete 0 end 
    .m2sci.mainframe.eDirName insert 1 $ePathToConvert
    set rFlagWhatToConvert 0
} 


######################################################################
# Gets the file to convert
######################################################################
proc setFileToConvert {} { 
    global rFlagWhatToConvert
    set eFileToConvert [ tk_getOpenFile ]
    
    # Refresh the entry text
    .m2sci.mainframe.eFileName delete 0 end 
    .m2sci.mainframe.eFileName insert 1 $eFileToConvert
    set rFlagWhatToConvert 1
} 


######################################################################
# Gets the result directory
######################################################################
proc setResDirName {} { 
    set eResDirName [ tk_chooseDirectory ]

    # Refresh the entry text
    .m2sci.mainframe.eResDirName delete 0 end 
    .m2sci.mainframe.eResDirName insert 1 $eResDirName
} 


######################################################################
# Close the window when conversion is launched
######################################################################
proc launchConversion {} {
    global rFlagWhatToConvert
    global eDirName
    global eFileName
    global eResDirName
    global rRecMode
    global rOnlyDouble
    global rVerbMode
    global rPrettyPrint
    global rToDo
    global rWindowKilled

    set rWindowKilled 0
    set rToDo 1
    
    destroy .m2sci
}

######################################################################
# Close the window when cancel button clicked
######################################################################
proc cancelConversion {} {
    global rToDo
    global rWindowKilled
 
    set rWindowKilled 0
    set rToDo -1
    
    destroy .m2sci
}

######################################################################
# Close the window when cross clicked
######################################################################
proc cancelConversionCross {} {
 global rWindowKilled
 set rWindowKilled 1

 destroy .m2sci
}

######################################################################
# Window creation
######################################################################
global rFlagWhatToConvert

frame .m2sci.mainframe

# FILES SELECTION
ScilabEval "TCL_SetVar(\"localizedLabelFiles\",gettext(\"FILES SELECTION\"))" "sync"
label .m2sci.mainframe.lFileSelect -textvariable localizedLabelFiles -background lightblue

grid .m2sci.mainframe.lFileSelect -column 0 -columnspan 4 -row 1 -sticky news

# Whole directory
radiobutton .m2sci.mainframe.rDir -variable rFlagWhatToConvert -value 0 
ScilabEval "TCL_SetVar(\"localizedlDir\",gettext(\"Convert a whole directory\"))" "sync"
label .m2sci.mainframe.lDir -textvariable localizedlDir

grid .m2sci.mainframe.rDir -column 0 -row 2
grid .m2sci.mainframe.lDir -column 1 -columnspan 3 -row 2 -sticky w

ScilabEval "TCL_SetVar(\"localizedlDirName\",gettext(\"Directory name: \"))" "sync"
label .m2sci.mainframe.lDirName -textvariable localizedlDirName
entry .m2sci.mainframe.eDirName -textvariable ePathToConvert -width 50
bind .m2sci.mainframe.eDirName <FocusIn> {set rFlagWhatToConvert 0}
ScilabEval "TCL_SetVar(\"localizedBrowse\",gettext(\"Browse\"))" "sync"
button .m2sci.mainframe.bBrowseDir -textvariable localizedBrowse -command {setPathToConvert}

grid .m2sci.mainframe.lDirName -column 1 -row 3
grid .m2sci.mainframe.eDirName -column 2 -row 3
grid .m2sci.mainframe.bBrowseDir -column 3 -row 3

# Single file
radiobutton .m2sci.mainframe.rFile -variable rFlagWhatToConvert -value 1
ScilabEval "TCL_SetVar(\"localizedlFile\",gettext(\"Convert a single file\"))" "sync"
label .m2sci.mainframe.lFile -textvariable localizedlFile  

grid .m2sci.mainframe.rFile -column 0 -row 4
grid .m2sci.mainframe.lFile -column 1 -columnspan 3 -row 4 -sticky w

ScilabEval "TCL_SetVar(\"localizedlFileName\",gettext(\"File name: \"))" "sync"
label .m2sci.mainframe.lFileName -textvariable localizedlFileName
entry .m2sci.mainframe.eFileName -textvariable eFileToConvert -width 50
bind .m2sci.mainframe.eFileName <FocusIn> {set rFlagWhatToConvert 1}
button .m2sci.mainframe.bBrowseFile -textvariable localizedBrowse -command {setFileToConvert} 

grid .m2sci.mainframe.lFileName -column 1 -row 5
grid .m2sci.mainframe.eFileName -column 2 -row 5
grid .m2sci.mainframe.bBrowseFile -column 3 -row 5

# RESULTING FILES DIRECTORY
ScilabEval "TCL_SetVar(\"localizedlResDirSelect\",gettext(\"OUTPUT FILES DIRECTORY\"))" "sync"
label .m2sci.mainframe.lResDirSelect -textvariable localizedlResDirSelect -background lightblue

grid .m2sci.mainframe.lResDirSelect -column 0 -columnspan 4 -row 6 -sticky news

ScilabEval "TCL_SetVar(\"localizedlResDirName\",gettext(\"Directory name:\"))" "sync"
label .m2sci.mainframe.lResDirName -textvariable localizedlResDirName
entry .m2sci.mainframe.eResDirName -textvariable eResDirName -width 50
button .m2sci.mainframe.bBrowseResDir -textvariable localizedBrowse -command {setResDirName}

grid .m2sci.mainframe.lResDirName -column 1 -row 7
grid .m2sci.mainframe.eResDirName -column 2 -row 7
grid .m2sci.mainframe.bBrowseResDir -column 3 -row 7

# OPTIONS
ScilabEval "TCL_SetVar(\"localizedlOptions\",gettext(\"CONVERSION OPTIONS\"))" "sync"
label .m2sci.mainframe.lOptions -textvariable localizedlOptions -background lightblue

grid .m2sci.mainframe.lOptions -column 0 -columnspan 4 -row 8 -sticky news

# RecMode
ScilabEval "TCL_SetVar(\"localizedlRecMode\",gettext(\"Recursive conversion:\"))" "sync"
label .m2sci.mainframe.lRecMode -textvariable localizedlRecMode
frame .m2sci.mainframe.fRecMode
ScilabEval "TCL_SetVar(\"localizedYes\",gettext(\"Yes\"))" "sync"
ScilabEval "TCL_SetVar(\"localizedNo\",gettext(\"No\"))" "sync"
ScilabEval "TCL_SetVar(\"localizedYES\",gettext(\"YES\"))" "sync"
ScilabEval "TCL_SetVar(\"localizedNO\",gettext(\"NO\"))" "sync"
radiobutton .m2sci.mainframe.fRecMode.rRecModeYES -textvariable localizedYes -variable rRecMode -value $localizedYES
radiobutton .m2sci.mainframe.fRecMode.rRecModeNO -textvariable localizedNo -variable rRecMode -value $localizedNO 
		   
pack configure  .m2sci.mainframe.fRecMode.rRecModeYES \
		.m2sci.mainframe.fRecMode.rRecModeNO  \
		-side left

grid .m2sci.mainframe.lRecMode -column 0 -columnspan 2 -row 9 
grid .m2sci.mainframe.fRecMode -column 1 -columnspan 2 -row 9 

# OnlyDouble
ScilabEval "TCL_SetVar(\"localizedlOnlyDouble\",gettext(\"Only double values used:\"))" "sync"
label .m2sci.mainframe.lOnlyDouble -textvariable localizedlOnlyDouble
frame .m2sci.mainframe.fOnlyDouble
radiobutton .m2sci.mainframe.fOnlyDouble.rOnlyDoubleYES -textvariable localizedYes -variable rOnlyDouble -value $localizedYES  
radiobutton .m2sci.mainframe.fOnlyDouble.rOnlyDoubleNO -textvariable localizedNo -variable rOnlyDouble -value $localizedNO
		   
pack configure  .m2sci.mainframe.fOnlyDouble.rOnlyDoubleYES \
		.m2sci.mainframe.fOnlyDouble.rOnlyDoubleNO  \
		-side left

grid .m2sci.mainframe.lOnlyDouble -column 0 -columnspan 2 -row 10
grid .m2sci.mainframe.fOnlyDouble -column 1 -columnspan 2 -row 10

# VerbMode
ScilabEval "TCL_SetVar(\"localizedlVerbMode\",gettext(\"Verbose Mode:\"))" "sync"
label .m2sci.mainframe.lVerbMode -textvariable localizedlVerbMode
frame .m2sci.mainframe.fVerbMode
radiobutton .m2sci.mainframe.fVerbMode.rVerbMode0 -text "0" -variable rVerbMode -value 0  
radiobutton .m2sci.mainframe.fVerbMode.rVerbMode1 -text "1" -variable rVerbMode -value 1 
radiobutton .m2sci.mainframe.fVerbMode.rVerbMode2 -text "2" -variable rVerbMode -value 2 
radiobutton .m2sci.mainframe.fVerbMode.rVerbMode3 -text "3" -variable rVerbMode -value 3 
		   
pack configure  .m2sci.mainframe.fVerbMode.rVerbMode0 \
		.m2sci.mainframe.fVerbMode.rVerbMode1 \
		.m2sci.mainframe.fVerbMode.rVerbMode2 \
		.m2sci.mainframe.fVerbMode.rVerbMode3 \
		-side left

grid .m2sci.mainframe.lVerbMode -column 0 -columnspan 2 -row 11
grid .m2sci.mainframe.fVerbMode -column 1 -columnspan 2 -row 11

# PrettyPrint
ScilabEval "TCL_SetVar(\"localizedlPrettyPrint\",gettext(\"Generate pretty printed code:\"))" "sync"
label .m2sci.mainframe.lPrettyPrint -textvariable localizedlPrettyPrint
frame .m2sci.mainframe.fPrettyPrint
radiobutton .m2sci.mainframe.fPrettyPrint.rPrettyPrintYES -textvariable localizedYes -variable rPrettyPrint -value $localizedYES  
radiobutton .m2sci.mainframe.fPrettyPrint.rPrettyPrintNO -textvariable localizedNo -variable rPrettyPrint -value $localizedNO
		   
pack configure  .m2sci.mainframe.fPrettyPrint.rPrettyPrintYES \
		.m2sci.mainframe.fPrettyPrint.rPrettyPrintNO  \
		-side left

grid .m2sci.mainframe.lPrettyPrint -column 0 -columnspan 2 -row 12
grid .m2sci.mainframe.fPrettyPrint -column 1 -columnspan 2 -row 12

# VALIDATION
ScilabEval "TCL_SetVar(\"localizedlButtons\",gettext(\"VALIDATION\"))" "sync"
label .m2sci.mainframe.lButtons -textvariable localizedlButtons -background lightblue

grid .m2sci.mainframe.lButtons -column 0 -columnspan 4 -row 13 -sticky news

frame .m2sci.mainframe.fButtons
ScilabEval "TCL_SetVar(\"localizedbConvert\",gettext(\"Convert\"))" "sync"
button .m2sci.mainframe.fButtons.bConvert -textvariable localizedbConvert -command {launchConversion} 
ScilabEval "TCL_SetVar(\"localizedbCancel\",gettext(\"Cancel\"))" "sync"
button .m2sci.mainframe.fButtons.bCancel -textvariable localizedbCancel -command {cancelConversion} 

pack configure  .m2sci.mainframe.fButtons.bCancel  \
		.m2sci.mainframe.fButtons.bConvert \
		-side left

grid .m2sci.mainframe.fButtons -column 0 -columnspan 4 -row 14

pack .m2sci.mainframe