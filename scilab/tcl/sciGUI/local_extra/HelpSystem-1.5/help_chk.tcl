#===============================================================================
# HelpSystem     :  Extension 
#                :  Checks references in help files
# Developed by   :  Andrei A. Gratchev      <grand@deversys.com>
#===============================================================================
# Function       :  help::check_refs
# Description    :  Checks references (tag <a href="..">) to topics of help file
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------

if {[namespace current]!="::help"} {
 help::add_ext [info script]
} else {

proc check_refs {} {
 variable gw
 variable index
 variable data
 variable abort 0
 variable stat
 variable insearch

 if {$insearch} {
  set abort 1
  after 250 help::check_errors
  return
 }

 set insearch 1
 
 set found 0
 set data(__@CheckResult@__,title) "Check results"
 set data(__@CheckResult@__,text) "<body text=black bgcolor=#ffffd2 link=#006800><ul>"
 updatehead "__stop"
 if {$index!=""} {
  if {[set pos [lsearch -exact $index "__index"]]!=-1} {
   set topics [lreplace $index $pos $pos]
  } else {
   set topics $index
  }
  foreach topic $topics {
   set names($topic) ""
   set tocheck($topic) ""
   set _all_text $data($topic,text)
   set totallen [string length $_all_text]
   set _text_pos 0
   set stat "Analysing $data($topic,title)..."
   update
   while {!$abort && $_text_pos!=$totallen} {
    set pos [string first "<" [string range $_all_text [expr {$_text_pos + 1024}] end]]
    if {$pos==-1} {
     set _text [string range $_all_text $_text_pos end]
     set _text_pos $totallen
    } else {
     set _text [string range $_all_text $_text_pos [expr {$_text_pos + 1024 + $pos - 1}]]
     set _text_pos [expr {$_text_pos + 1024 + $pos}]
    } 
    while {!$abort && [set pos [string first "<" $_text]]!=-1} {
     set _text [string range $_text $pos end]
     if {[set pos [string first ">" $_text]]!=-1} {
      set tag [string trim [string range $_text 0 $pos] "< >\t\n"]
      set _text [string range $_text [incr pos] end]
     } else {
      set tag [string trim $_text "< >\t\n"]
      set _text ""
     }
     regsub -all -- "\[\r\t\n \]+" $tag " " tag
     if {[set pos [string first " " $tag]]!=-1} {
      set tagparams [parsetag [string range $tag $pos end]]
      set tag [string range $tag 0 [expr $pos - 1]]
     } else {
      set tagparams ""
     } 
     set tag [string tolower $tag]
     if {$tag=="a"} {
      set p_href ""
      set p_name ""
      foreach {param value} $tagparams {
       set p_$param $value
      }
      if {$p_name!="" && [lsearch $names($topic) [string tolower $p_name]]==-1} {
       lappend names($topic) [string tolower $p_name]
      }
      if {[string first ":" $p_href]>=0} {
       switch -glob -- [string tolower $p_href] {
        file://* {set p_href [string range $p_href 7 end]}
        default {continue}
       }
      }
      if {$p_href!=""} {
       set p_href_neu [string tolower $p_href]
       set filename ""
       if {[set pos [string first "?" $topic]]!=-1} {
        set filename [string tolower [string range $topic 0 [expr $pos - 1]]]
        if {[string first "#" $p_href_neu]==0} {
         set p_href_neu $filename?[string range $p_href_neu 1 end]
        } elseif {[set pos [string first "#" $p_href_neu]]>0} {
         set p_href_neu [string replace $p_href_neu $pos $pos "?"]
        } 
       }
       if {[lsearch $index $p_href_neu]>=0} {
        set p_href $p_href_neu
       } else {
        if {[string first "#" $p_href]==0} {
         set p_href $topic$p_href
        } else {
        }
       }
       if {[lsearch $tocheck($topic) $p_href]==-1} {
        lappend tocheck($topic) $p_href
       }
      }
     }
    }
   }
  }
  if {!$abort} {
   set stat "Checking..."
   update idletasks
   foreach topic $topics {
    set outtext ""
    foreach href $tocheck($topic) {
     set mark ""
     if {[set pos [string first "#" $href]]!=-1} {
      set mark [string tolower [string range $href [expr $pos + 1] end]]
      set p_href [string range $href 0 [expr $pos - 1]]
     } else {
      set p_href $href
     }
     if {($mark=="" && $p_href!="" && [lsearch $topics $p_href]==-1) ||
         ($mark!="" && (($p_href=="" && [lsearch $names($topic) $mark]==-1) || 
         ($p_href!="" && ([lsearch $topics $p_href]==-1 || [lsearch $names($p_href) $mark]==-1)))) } {
      incr found
      append outtext "Topic referenced by \"<font color=#c00000><b>[string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} "$href"]</b></font>\" not found<br>"
     }
    }
    if {$outtext!=""} {
     append data(__@CheckResult@__,text) "<li>Topic <a href=\"$topic\">[string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} $data($topic,title)]</a><br><font size=-1>$outtext<font><br>"
    }
   }
  }
 }
 if {$abort} {
  append data(__@CheckResult@__,text) "</ul><font color=#c00000>Check aborted.</font></body>"
 } elseif {!$found} {
  append data(__@CheckResult@__,text) "</ul><font color=#00a000>No errors found.</font></body>"
 } else {
  append data(__@CheckResult@__,text) "</ul></body>"
 }
 set data(__@CheckResult@__,title) "Check results ($found found)"
 set abort 0
 $gw.text configure -cursor arrow
 set stat "Done."
 set insearch 0
# updatehead "__stop"
 set help::curtopic ""
 show "__@CheckResult@__"
}

# Initialization
rename "userinit" "help_chk:userinit"
proc userinit {w} {
 $w.menuX add command -label "Check references" -command "help::check_refs"
 help_chk:userinit $w
}
rename "rightclickX" "help_chk:rightclickX"
proc rightclickX {x y} {
 variable gw
 variable index
 $gw.menuX entryconfigure "Check references" -state [expr [llength $index]?"normal":"disabled"]
 help_chk:rightclickX $x $y
}

}; #source
