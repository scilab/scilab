set winTitle "SciPad"
set version "Version 3.14"

# all one needs in order to add a new retriavable preference is:
#  -add the variable name to $listofpref below
#  -use the variable when needed in the program, such that 
#    it is visible at main level (i.e. globalize it)
#  -if needed, assign an initial fallback value here

#ES 11/10/04: those are the preferences which are going to be saved
set listofpref {wordWrap BGCOLOR FGCOLOR CURCOLOR PARCOLOR BRAKCOLOR \
       BRACCOLOR PUNCOLOR KEYWCOLOR OPCOLOR TXTCOLOR QTXTCOLOR \
       REMCOLOR XMLCOLOR NUMCOLOR SELCOLOR BREAKPOINTCOLOR FontSize \
	  WMGEOMETRY printCommand actbptextFont}


##ES: default options which can be overriden
set wordWrap "none"
set BGCOLOR "snow1"
set FGCOLOR "black"
set CURCOLOR "red"
set PARCOLOR "magenta3"
set BRAKCOLOR "DarkGoldenrod4"
set BRACCOLOR "red"
set PUNCOLOR "turquoise4"
set KEYWCOLOR "blue2"
set OPCOLOR "blue4"
set TXTCOLOR $FGCOLOR
set QTXTCOLOR "darkred"
set REMCOLOR "green4"
set XMLCOLOR "orange"
set NUMCOLOR "yellow4"
set SELCOLOR "PaleGreen"
set BREAKPOINTCOLOR "pink"
set FontSize 12 
set WMGEOMETRY 600x480
set printCommand lpr
set actbptextFont "-Adobe-courier-bold-R-Normal-*-[expr $FontSize + 2]-*"

#other non-pref initial settings
if { ![info exists lang] } { set lang "eng" }
set Scheme scilab

#ES 12/11/04: source the user preferences file if any
set preffilename $env(HOME)/.SciPadPreferences.tcl
catch {source $preffilename}


