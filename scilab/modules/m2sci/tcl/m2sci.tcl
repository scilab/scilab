# GUI for M2SCI tools
# Copyright INRIA
# Scilab Project - V. Couvert

#!/bin/sh
#exec wish "$0" "$1" & exit 0 

# Scilab specification
toplevel .m2sci

wm title .m2sci {Matlab to Scilab conversion tool}
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
label .m2sci.mainframe.lFileSelect -text "FILES SELECTION" -background lightblue

grid .m2sci.mainframe.lFileSelect -column 0 -columnspan 4 -row 1 -sticky news

# Whole directory
radiobutton .m2sci.mainframe.rDir -variable rFlagWhatToConvert -value 0 
label .m2sci.mainframe.lDir -text "Convert a whole directory"

grid .m2sci.mainframe.rDir -column 0 -row 2
grid .m2sci.mainframe.lDir -column 1 -columnspan 3 -row 2 -sticky w

label .m2sci.mainframe.lDirName -text "Directory name: "
entry .m2sci.mainframe.eDirName -textvariable ePathToConvert -width 50
bind .m2sci.mainframe.eDirName <FocusIn> {set rFlagWhatToConvert 0}
button .m2sci.mainframe.bBrowseDir -text "Browse" -command {setPathToConvert}

grid .m2sci.mainframe.lDirName -column 1 -row 3
grid .m2sci.mainframe.eDirName -column 2 -row 3
grid .m2sci.mainframe.bBrowseDir -column 3 -row 3

# Single file
radiobutton .m2sci.mainframe.rFile -variable rFlagWhatToConvert -value 1
label .m2sci.mainframe.lFile -text "Convert a single file"

grid .m2sci.mainframe.rFile -column 0 -row 4
grid .m2sci.mainframe.lFile -column 1 -columnspan 3 -row 4 -sticky w

label .m2sci.mainframe.lFileName -text "File name:"
entry .m2sci.mainframe.eFileName -textvariable eFileToConvert -width 50
bind .m2sci.mainframe.eFileName <FocusIn> {set rFlagWhatToConvert 1}
button .m2sci.mainframe.bBrowseFile -text "Browse" -command {setFileToConvert} 

grid .m2sci.mainframe.lFileName -column 1 -row 5
grid .m2sci.mainframe.eFileName -column 2 -row 5
grid .m2sci.mainframe.bBrowseFile -column 3 -row 5

# RESULTING FILES DIRECTORY
label .m2sci.mainframe.lResDirSelect -text "OUTPUT FILES DIRECTORY" -background lightblue

grid .m2sci.mainframe.lResDirSelect -column 0 -columnspan 4 -row 6 -sticky news

label .m2sci.mainframe.lResDirName -text "Directory name:"
entry .m2sci.mainframe.eResDirName -textvariable eResDirName -width 50
button .m2sci.mainframe.bBrowseResDir -text "Browse" -command {setResDirName}

grid .m2sci.mainframe.lResDirName -column 1 -row 7
grid .m2sci.mainframe.eResDirName -column 2 -row 7
grid .m2sci.mainframe.bBrowseResDir -column 3 -row 7

# OPTIONS
label .m2sci.mainframe.lOptions -text "CONVERSION OPTIONS" -background lightblue

grid .m2sci.mainframe.lOptions -column 0 -columnspan 4 -row 8 -sticky news

# RecMode
label .m2sci.mainframe.lRecMode -text "Recursive conversion:"
frame .m2sci.mainframe.fRecMode
radiobutton .m2sci.mainframe.fRecMode.rRecModeYES -text "Yes" -variable rRecMode -value "YES"  
radiobutton .m2sci.mainframe.fRecMode.rRecModeNO -text "No" -variable rRecMode -value "NO" 
		   
pack configure  .m2sci.mainframe.fRecMode.rRecModeYES \
		.m2sci.mainframe.fRecMode.rRecModeNO  \
		-side left

grid .m2sci.mainframe.lRecMode -column 0 -columnspan 2 -row 9 
grid .m2sci.mainframe.fRecMode -column 1 -columnspan 2 -row 9 

# OnlyDouble
label .m2sci.mainframe.lOnlyDouble -text "Only double values used:"
frame .m2sci.mainframe.fOnlyDouble
radiobutton .m2sci.mainframe.fOnlyDouble.rOnlyDoubleYES -text "Yes" -variable rOnlyDouble -value "YES"  
radiobutton .m2sci.mainframe.fOnlyDouble.rOnlyDoubleNO -text "No" -variable rOnlyDouble -value "NO" 
		   
pack configure  .m2sci.mainframe.fOnlyDouble.rOnlyDoubleYES \
		.m2sci.mainframe.fOnlyDouble.rOnlyDoubleNO  \
		-side left

grid .m2sci.mainframe.lOnlyDouble -column 0 -columnspan 2 -row 10
grid .m2sci.mainframe.fOnlyDouble -column 1 -columnspan 2 -row 10

# VerbMode
label .m2sci.mainframe.lVerbMode -text "Verbose Mode:"
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
label .m2sci.mainframe.lPrettyPrint -text "Generate pretty printed code:"
frame .m2sci.mainframe.fPrettyPrint
radiobutton .m2sci.mainframe.fPrettyPrint.rPrettyPrintYES -text "Yes" -variable rPrettyPrint -value "YES"  
radiobutton .m2sci.mainframe.fPrettyPrint.rPrettyPrintNO -text "No" -variable rPrettyPrint -value "NO" 
		   
pack configure  .m2sci.mainframe.fPrettyPrint.rPrettyPrintYES \
		.m2sci.mainframe.fPrettyPrint.rPrettyPrintNO  \
		-side left

grid .m2sci.mainframe.lPrettyPrint -column 0 -columnspan 2 -row 12
grid .m2sci.mainframe.fPrettyPrint -column 1 -columnspan 2 -row 12

# VALIDATION
label .m2sci.mainframe.lButtons -text "VALIDATION" -background lightblue

grid .m2sci.mainframe.lButtons -column 0 -columnspan 4 -row 13 -sticky news

frame .m2sci.mainframe.fButtons
button .m2sci.mainframe.fButtons.bConvert -text "Convert" -command {launchConversion} 
button .m2sci.mainframe.fButtons.bCancel -text "Cancel" -command {cancelConversion} 

pack configure  .m2sci.mainframe.fButtons.bCancel  \
		.m2sci.mainframe.fButtons.bConvert \
		-side left

grid .m2sci.mainframe.fButtons -column 0 -columnspan 4 -row 14

pack .m2sci.mainframe