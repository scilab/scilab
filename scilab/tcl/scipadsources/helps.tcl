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
	    set cursel [string trim [selection get]]
# get only the first word of the selection (or a symbol)
           regexp "(\\A\\w*\\M|\\A\\W)" $cursel curterm
	}
    if {[info exists curterm]} {
          set curterm [string trim $curterm]
          if {$curterm!=""} { ScilabEval "help \"$curterm\"" }
    }
}

