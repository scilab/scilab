# help menu
proc helpme {} {
    ScilabEval "help scipad"
}

# about menu
proc aboutme {} {
    global winTitle version
    set mess [mc "\n Originated by Joseph Acosta, joeja@mindspring.com.\n\
                  Modified by Scilab Consortium.\n\
                  Revised by Enrico Segre 2003-2005.\n\
                  Miscellaneous improvements, particularly debug tools, by Francois Vogel 2004-2005."]
    tk_messageBox -title [mc "About"] -type ok -message " $winTitle $version $mess"
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
	    set cursel [string trim [selection get]]
# get only the first word of the selection (or a symbol)
           regexp "(\\A\\w*\\M|\\A\\W)" $cursel curterm
	}
    if {[info exists curterm]} {
          set curterm [string trim $curterm]
          if {$curterm!=""} { ScilabEval "help \"$curterm\"" }
    }
}

