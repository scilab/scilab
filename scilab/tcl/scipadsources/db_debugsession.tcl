proc checkscilabbusy {{mb "message"}} {
    global sciprompt lang
    if [ expr [string compare $sciprompt -1] == 0 ] {
        if {$mb != "nomessage"} {
            if {$lang == "eng"} {
                set mes "Scilab is working, wait for the prompt to issue a\
                     debugger command."
                set tit "Scilab working"
            } else {
                set mes "Scilab est occupé, attendez le prompt pour\
                     effectuer des commandes de débug."
                set tit "Scilab occupé"
            }
            tk_messageBox -message $mes -title $tit -type ok -icon info
        }
        return "busy"
    } else {
        return "OK"
    }
}

proc execfile_bp {} {
    global funnameargs funsinbuffer
    if {[checkscilabbusy] == "OK"} {
        set removecomm [removescilab_bp "no_output"]
        set textareacur [gettextareacur]
        set tagranges [$textareacur tag ranges breakpoint]
        set setbpcomm ""
        set firstbp "true"
#        set nlins -1
 #       set nlins -2
        foreach {tstart tstop} $tagranges {
            set infun [whichfun [$textareacur index $tstart]]
            if {$infun !={} } {
                set funname [lindex $infun 0]
                set lineinfun [expr [lindex $infun 1] - 1]
                set setbpcomm [concat $setbpcomm "setbpt(\"$funname\",$lineinfun);"]
                if {$firstbp == "true"} {
                    set firstbp "false"
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
#                incr nlins 1
 #               incr nlins 2
#                $textareacur insert "$tstart +$nlins l linestart" "pause\n"
 #               $textareacur insert "$tstart +$nlins l linestart" "mode(6)\nmode(0)\n"
            }
        }
        if {$funnameargs != ""} {
            execfile
# Leading spaces here to avoid possible loss of first characters
# However this does not work for long lines created by execfile (i.e.
# lines that take a long time to display or execute in Scilab window)
            if {$setbpcomm != ""} {
                set commnvars [createsetinscishellcomm]
                set watchsetcomm [lindex $commnvars 0]
                if {$watchsetcomm != ""} {
                    ScilabEval "     $watchsetcomm"
                }
                while {[checkscilabbusy "nomessage"] == "busy"} {}
                ScilabEval "     $setbpcomm; $funnameargs,$removecomm"
                set filename [creategetfromshellcomm]
                ScilabEval "     exec(\"$filename\");"
            } else {
                ScilabEval "     $funnameargs"
            }
        } else {
            # <TODO> .sce case
##            execfile
        }
    }
}

proc removescilab_bp {outp} {
    global funnames
    if {[checkscilabbusy] == "OK"} {
        set textareacur [gettextareacur]
        set tagranges [$textareacur tag ranges breakpoint]
        set setbpcomm ""
        if {$funnames != ""} {
            foreach fun $funnames {
                set setbpcomm [concat $setbpcomm "delbpt(\"$fun\");"]
            }
            if {$outp != "no_output"} {
                ScilabEval " $setbpcomm"
            }
        } else {
            # <TODO> .sce case
        }
        return $setbpcomm
    }
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
    global funnameargs
    if {[checkscilabbusy] == "OK"} {
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
        if {$funnameargs != ""} {
            set commnvars [createsetinscishellcomm]
            set watchsetcomm [lindex $commnvars 0]
            if {$watchsetcomm != ""} {
                ScilabEval "     $watchsetcomm"
                set returnwithvars [lindex $commnvars 1]
                while {[checkscilabbusy "nomessage"] == "busy"} {}
    # <TODO> : [..]=resume(..) is bugged! -> replaced by a simple resume with no argument
    # but then the changes in the watch window are obviously not reflected in the calling
    # workspace in Scilab
    #            ScilabEval "     $returnwithvars"
                ScilabEval "     resume"
            } else {
                while {[checkscilabbusy "nomessage"] == "busy"} {}
                ScilabEval "     resume"
            }
            set filename [creategetfromshellcomm]
            ScilabEval "     exec(\"$filename\");"
        } else {
            # <TODO> .sce case
            # Sending \n is if mode(6) mode(0) is used. If pause, there is no
            # need to distinguish between .sci and .sce (resume is sent for both)
 #           ScilabEval " "
        }
    }
}

proc goonwo_bp {} {
    global funnameargs
    if {[checkscilabbusy] == "OK"} {
        if {$funnameargs != ""} {
            [gettextareacur] tag remove activebreakpoint 1.0 end
            removescilab_bp "with_output"
            ScilabEval " resume"
            set filename [creategetfromshellcomm]
            ScilabEval "     exec(\"$filename\");"
        }
    }
}

proc dispcallstack_bp {} {
    if {[checkscilabbusy] == "OK"} {
        ScilabEval " whereami()"
    }
}

proc canceldebug_bp {} {
    global funnameargs
    if {[checkscilabbusy] == "OK"} {
        if {$funnameargs != ""} {
            [gettextareacur] tag remove activebreakpoint 1.0 end
            removescilab_bp "with_output"
            ScilabEval " abort"
            set filename [creategetfromshellcomm]
            ScilabEval "     exec(\"$filename\");"
        }
    }
}
