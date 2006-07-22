#===============================================================================
# HelpSystem     :  Extension 
#                :  Splits help file(s) to set of HTML files
# Developed by   :  Andrei A. Gratchev      <grand@deversys.com>
#===============================================================================
# Function       :  help::help2html
# Description    :  Converts help file(s) to sepatated HTML files
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------

if {[namespace current]!="::help"} {
 help::add_ext [info script]
} else {

proc help2html:calcimg {name} {
 variable data
 variable curfilename

 set parfnameX [set parfname $name]
 if {![string match "*.gif" $parfname]} {
  append parfname ".gif"
 }
 if {![info exists data($parfnameX,image)] && ![info exists data($parfname,image)]} {
  catch {set data($parfname,image) [image create photo -file [file join [file dirname $curfilename] $parfname]]}
 }
 return $parfname
}

proc help2html:decode_base64 {data} {
 set i -1
 foreach char {A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z 0 1 2 3 4 5 6 7 8 9 + /} {
  set decode_table($char) [incr i]
 }
 set result ""
 set state 0
 foreach c [split $data ""] {
  if {[catch {set x $decode_table($c)}]} {
   continue
  }
  switch [incr state] {
   1 {set r [expr {($x <<2) & 0xFC}]}
   2 {append result [format %c [expr {($x >>4) & 0x03 | $r}]]
      set r [expr {($x <<4) & 0xF0}]
     }
   3 {append result [format %c [expr {($x >>2) & 0x0F | $r}]]
      set r [expr {($x <<6) & 0xC0}]
     }
   4 {append result [format %c [expr {$x & 0x3F | $r}]]
      set state 0
     }
  }
 }
 return $result
}

proc help2html {} {
 variable gw
 variable data
 variable index
 variable font
 variable fontsize
 variable linkcolor
 variable alinkcolor
 variable backcolor
 variable forecolor
 variable stat
 variable curfilename

 set tk_strictMotif 0

 set dir [file dirname $curfilename]
 if {[set dir [file dirname [tk_getSaveFile -parent $gw -initialdir $dir\
               -title "Select directory to Save HTML files" \
               -initialfile "Filename will be ignored"]]]=="."} {
  return
 }

 set count 0
 set total [llength $index]
 set stat "Generating HTMLs...."
 update 
 foreach topic $index {
  incr count
  regsub -all -- {[\?\*\:\>\<\&]} $topic "_" topicfname
  if {![string match "*.htm" $topic] && ![string match "*.html" $topic]} {
   append topicfname ".htm"
  } 
  if {[catch {set fout [::open [file join $dir $topicfname] "w"]}]!=0} {
   error "Can't open output file:\n[file join $dir $topicfname]"
   continue
  }
  set stat "Generating HTMLs...  $count of $total  \"$topicfname\""
  update idletasks
  catch {fconfigure $fout -eofchar "" -buffersize 32768 -buffering full}
  puts $fout "<html>\n<head>\n<meta name=\"Generator\" content=\"Help System v.1.5\">"
  puts $fout "<title>$data($topic,title)</title>\n<basefont name=\"$font\">\n</head>"
  puts $fout "<body bgcolor=\"$backcolor\" text=\"$forecolor\" link=\"$linkcolor\" alink=\"$alinkcolor\" style=\"font-family: '$font';\">"

  set _text $data($topic,text)
  set text ""
  while {[set pos [string first "<" $_text]]!=-1} {
   append text [string range $_text 0 [expr $pos - 1]]
   set _text [string range $_text $pos end]
   if {[set pos [string first ">" $_text]]!=-1} {
    set tag [string trim [string range $_text 0 $pos] "< >\t\n"]
    set _text [string range $_text [incr pos] end]
   } else {
    set tag [string trim [string range $_text 0 end] "< >\t\n"]
    set _text ""
   }
   regsub -all -- "\[\r\t\n \]+" $tag " " tag
   if {[set pos [string first " " $tag]]!=-1} {
    set tagparams [parsetag [string range $tag $pos end]]
    set tagname [string range $tag 0 [expr $pos - 1]]
   } else {
    set tagparams ""
    set tagname $tag
   }
   set tagname [string tolower $tagname]
   switch -exact -- $tagname {
    "a" {
      if {[set parpos [lsearch -exact $tagparams "href"]]!=-1} {
       incr parpos
       set parfname [lindex $tagparams $parpos]
       if {![regexp -nocase "^tcl(script)?:" $parfname]} {
        if {[set markpos [string first "#" $parfname]]!=-1} {
         set mark [string range $parfname $markpos end]
         set parfname [string range $parfname 0 [incr markpos -1]]
        } else {
         set mark ""
        }
        if {$mark!="" && [set lpos [lsearch -glob $index "*\?[string range $mark 1 end]"]]!=-1} {
         set parfname [lindex $index $lpos]
         set mark ""
        }
        regsub -all -- {[\?\*\:\>\<\&]} $parfname "_" parfname
        if {$parfname!="" && ![string match "*.htm" $parfname] && ![string match "*.html" $parfname]} {
         append parfname ".htm"
        } 
        append parfname $mark
       } else {
        set parfname "#"
       }
       set tagparams [lreplace $tagparams $parpos $parpos $parfname]
       set tag $tagname
       foreach {tagparname tagparvalue} $tagparams {
        append tag " $tagparname=\"$tagparvalue\""
       }
      }
    }
    "img" {
      if {[set parpos [lsearch -exact $tagparams "src"]]!=-1} {
       incr parpos
       set parfname [help2html:calcimg [lindex $tagparams $parpos]]
       set tagparams [lreplace $tagparams $parpos $parpos $parfname]
      }
      if {[set parpos [lsearch -exact $tagparams "border"]]!=-1} {
       lappend tagparams "style" "margin: [lindex $tagparams [incr parpos]]px"
       set tagparams [lreplace $tagparams [expr {$parpos - 1}] $parpos]
      }
      lappend tagparams "border" "0"
      set tag $tagname
      foreach {tagparname tagparvalue} $tagparams {
       append tag " $tagparname=\"$tagparvalue\""
      }
    }
    "li" {
      if {[set parpos [lsearch -exact $tagparams "type"]]!=-1} {
#       set _text "<img src=\"[lindex $tagparams [incr parpos]]\">$_text"
       if {[lindex $tagparams $parpos]!="disc" && \
           [lindex $tagparams $parpos]!="circle" && \
           [lindex $tagparams $parpos]!="square" } {
        lappend tagparams "style" "list-style: url([help2html:calcimg [lindex $tagparams [incr parpos]]]) disc"
        set tagparams [lreplace $tagparams [expr {$parpos - 1}] $parpos]
        set tag $tagname
        foreach {tagparname tagparvalue} $tagparams {
         append tag " $tagparname=\"$tagparvalue\""
        }
       }
      }
    }
    "ol" -
    "ul" {
      set f_style ""
      if {[set parpos [lsearch -exact $tagparams "type"]]!=-1} {
       if {[lindex $tagparams $parpos]!="disc" && \
           [lindex $tagparams $parpos]!="circle" && \
           [lindex $tagparams $parpos]!="square" } {
        append f_style "list-style: url([help2html:calcimg [lindex $tagparams [incr parpos]]]) disc"
        set tagparams [lreplace $tagparams [expr {$parpos - 1}] $parpos]
       }
      }
      if {[set parpos [lsearch -exact $tagparams "indent"]]!=-1} {
       if {$f_style!=""} {append f_style "; "}
       append f_style "margin-left: [string trim [lindex $tagparams [incr parpos]]]px"
       set tagparams [lreplace $tagparams [expr {$parpos - 1}] $parpos]
      }
      if {$f_style!=""} {
       lappend tagparams "style" $f_style
       set tag $tagname
       foreach {tagparname tagparvalue} $tagparams {
        append tag " $tagparname=\"$tagparvalue\""
       }
      }
    }
    "\$" {
      if {[info exists data(subst,[lindex $tagparams 0],begin)]} {
       set _text "$data(subst,[lindex $tagparams 0],begin)$_text"
      }
      continue
    }
    "/\$" {
      if {[info exists data(subst,[lindex $tagparams 0],end)]} {
       set _text "$data(subst,[lindex $tagparams 0],end)$_text"
      }
      continue
    }
    "font" {
      set f_style ""
      if {[set parpos [lsearch -exact $tagparams "bgcolor"]]!=-1} {
       append f_style "background-color: [lindex $tagparams [incr parpos]]"
       set tagparams [lreplace $tagparams [expr {$parpos - 1}] $parpos]
      }
      if {[set parpos [lsearch -exact $tagparams "point-size"]]!=-1} {
       set f_size [string trim [lindex $tagparams [incr parpos]]]
       if {[string index $f_size 0]=="+" || [string index $f_size 0]=="-"} {
        set f_size [expr "$fontsize $f_size"]
       }
       if {$f_style!=""} {append f_style "; "}
       append f_style "font-size: ${f_size}pt"
       set tagparams [lreplace $tagparams [expr {$parpos - 1}] $parpos]
      }
      if {$f_style!=""} {
       lappend tagparams "style" $f_style
       set tag $tagname
       foreach {tagparname tagparvalue} $tagparams {
        append tag " $tagparname=\"$tagparvalue\""
       }
      }
    }
   }
   append text "<$tag>"
  }
  append text $_text
  
  set ttl $data($topic,title)
  puts $fout "<h2 align=center>[string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} $ttl]</h2>\n$text\n</body>\n</html>"
  ::close $fout
 }
 
 set count 0
 set total [llength [array names data "*,image"]]
 set stat "Generating images..."
 update idletasks
 foreach im [array names data "*,image"] {
  if {$im=="__@LoGo@__,image"} {continue}
  incr count
  set parfname [string range $im 0 [expr {[string length $im] - 7}]]
  if {![string match "*.gif" $parfname]} {
   append parfname ".gif"
  }
  set stat "Generating images...  $count of $total  \"$parfname\""
  update idletasks
  if {[catch {$data($im) write [file join $dir $parfname] -format GIF}]} {
   if {[set dt [$data($im) cget -file]]!=""} {
    file copy -force -- $dt [file join $dir $parfname]
   } elseif {[set dt [$data($im) cget -data]]!=""} {
    if {[catch {set fout [::open [file join $dir $parfname] "w"]}]!=0} {
     error "Can't open output file:\n[file join $dir $parfname]"
     continue
    }
    catch {fconfigure $fout -eofchar "" -buffersize 32768 -buffering full -translation binary}
    puts -nonewline $fout [help2html:decode_base64 $dt]
    ::close $fout
   }
  }
 }
 set stat "Done."
}

# Initialization
rename "userinit" "help_2htm:userinit"
proc userinit {w} {
 $w.menuX add command -label "Save as HTML files..." -command "help::help2html"
 help_2htm:userinit $w
}
rename "rightclickX" "help_2htm:rightclickX"
proc rightclickX {x y} {
 variable gw
 variable index
 $gw.menuX entryconfigure "Save as HTML files..." -state [expr [llength $index]?"normal":"disabled"]
 help_2htm:rightclickX $x $y
}

}; #source
