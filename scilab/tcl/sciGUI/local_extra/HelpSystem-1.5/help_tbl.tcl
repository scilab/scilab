#===============================================================================
# HelpSystem     :  Extension 
#                :  Experemental support of HTML tables
# Developed by   :  Andrei A. Gratchev      <grand@deversys.com>
#===============================================================================
# Functions      :  help::tag_*
# Description    :  Processing of appropriated table tags
# Parameters     :  params      - parameters of tags
# Return         :  Nothing
#-------------------------------------------------------------------------------

if {[namespace current]!="::help"} {
 help::add_ext [info script]
} else {

proc tag_table {params} {
 variable w
 variable attr
 variable tables
 variable _istext 0
 variable settings
 variable attr

 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }

 if {[set p_bgcolor [lindex $attr(bgcolors) 0]]==""} {
  set p_bgcolor $settings(back)
 }
 
 set p_width ""
 set p_border 1
 set p_cellspacing 0
 set p_cellpadding 0 ;#2
 set p_align left
 foreach {param value} $params {
  set p_$param $value
 }
 if {$p_border==""} {set p_border 1}

 incr tables(level)
 incr tables(count)
# puts "$tables(count) -> $tables(level)"
 set tables($tables(level),container) $w.text.table_$tables(count)
 frame $tables($tables(level),container) -bd $p_border -relief raised \
       -background $p_bgcolor
 tableBind $tables($tables(level),container)
 lappend tables(tables) $w $tables($tables(level),container) $p_width $p_border $attr(margin)
# grid propagate $tables($tables(level),container) 0
 $w.text window create end -window $tables($tables(level),container) -padx 0 -pady 0
 $w.text tag add [gettags] $tables($tables(level),container)
 set tables($tables(level),row) -1
 set tables($tables(level),col) -1
 set tables($tables(level),spanned) ""
 set tables($tables(level),tb_bg) $p_bgcolor
 set tables($tables(level),border) $p_border
 set tables($tables(level),padding) $p_cellpadding
 set tables($tables(level),spacing) $p_cellspacing
 set tables($tables(level),margin) $attr(margin)
 set attr(margin) 0
 set tables($tables(level),table_align) $attr(justify)
 set attr(justify) $p_align
 set tables($tables(level),tag) "tb"
}

proc tag_/table {params} {
 variable w
 variable attr
 variable tables
 variable _istext 1
 variable attr

 switch $tables($tables(level),tag) {
  "tr" {tag_/tr ""}
  "td" {tag_/td ""; tag_/tr ""}
 }
 set attr(margin) $tables($tables(level),margin)
 set attr(justify) $tables($tables(level),table_align)
 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {$tables(level)>0} {
  if {![incr tables(level) -1]} {
   tablesCalc
   set tables(tables) ""
  }
 }
}

proc tag_tr {params} {
 variable w
 variable tables
 variable attr
 variable _istext 0

 switch $tables($tables(level),tag) {
  "tr" {tag_/tr ""}
  "td" {tag_/td ""; tag_/tr ""}
 }
 set tables($tables(level),tag) "tr"
 incr tables($tables(level),row)

 set p_bgcolor $tables($tables(level),tb_bg)
 set p_height ""
 set p_align $attr(justify)
 foreach {param value} $params {
  set p_$param $value
 }
 if {$p_height!=""} {
  if {[string match "*%" $p_height]} {
   catch {grid rowconfig $tables($tables(level),container) $tables($tables(level),row) -weight [string trim $p_height "%"]}
  } else {
   catch {grid rowconfig $tables($tables(level),container) $tables($tables(level),row) -weight 0 -minsize [expr {$p_height + ($tables($tables(level),border)*2)}]}
  }
 }
 set tables($tables(level),tr_align) $attr(justify)
 set tables($tables(level),tr_bg) $p_bgcolor
 set attr(justify) $p_align
}

proc tag_/tr {params} {
 variable w
 variable tables
 variable attr
 variable _istext 0

 switch $tables($tables(level),tag) {
  "td" {tag_/td ""}
  "tb" {tag_tr ""}
 }
 set tables($tables(level),tag) "tb"
 set attr(justify) $tables($tables(level),tr_align)
 set tables($tables(level),col) -1
}

proc tag_td {params} {
 variable w
 variable w
 variable tables
 variable attr
 variable _istext 1
 variable fontsize
 variable settings

 switch $tables($tables(level),tag) {
  "tb" {tag_tr ""}
  "td" {tag_/td ""}
 }
 set tables($tables(level),tag) "td"
 incr tables($tables(level),col)

 set p_align $attr(justify)
 while {[lsearch -exact $tables($tables(level),spanned) "$tables($tables(level),row) $tables($tables(level),col)"]!=-1} {
  incr tables($tables(level),col)
 }
 set tables(stack) [linsert $tables(stack) 0 $w]

 set p_colspan 1
 set p_rowspan 1
 set p_width ""
 set p_align $attr(justify)
 set p_bgcolor $tables($tables(level),tr_bg)
 foreach {param value} $params {
  set p_$param $value
 }
 set tables($tables(level),td_align) $attr(justify)
 set attr(justify) $p_align
 if {$p_width!=""} {
  if {[string match "*%" $p_width]} {
   catch {grid columnconfig $tables($tables(level),container) $tables($tables(level),col) -weight [string trim $p_width "%"]}
  } else {
   catch {grid columnconfig $tables($tables(level),container) $tables($tables(level),col) -weight 0 -minsize [expr {$p_width + ($tables($tables(level),border)*2)}]}
  }
 }
 frame [set w $tables($tables(level),container).r$tables($tables(level),row)_c$tables($tables(level),col)] \
         -bd 0 
 grid $w -padx $tables($tables(level),spacing) -pady $tables($tables(level),spacing) \
         -ipadx 0 -ipady 0 \
         -row $tables($tables(level),row) -column $tables($tables(level),col) \
         -rowspan $p_rowspan -columnspan $p_colspan -sticky news
 text $w.text -padx $tables($tables(level),padding) -pady $tables($tables(level),padding) \
              -wrap none -cursor arrow -bd $tables($tables(level),border) \
              -insertwidth 0 -insertontime 0 -exportselection 1\
              -highlightthickness 0 -takefocus 0 -insertborderwidth 0\
              -height 1 -width 1 -font "-size $fontsize"
 catch {$w.text configure -foreground $settings(fore)}
 catch {$w.text configure -background $p_bgcolor}
 tableBind $w.text
 pack $w.text -expand 1 -fill both
 for {set r 0} {$r<$p_rowspan} {incr r} {
  for {set c 0} {$c<$p_colspan} {incr c} {
   lappend tables($tables(level),spanned) "[expr {$tables($tables(level),row) + $r}] [expr {$tables($tables(level),col) + $c}]"
  }
 }
 if {$p_bgcolor!=""} {
  set attr(bgcolors) [linsert $attr(bgcolors) 0 [correctcolor [string tolower $p_bgcolor]]]
  set attr(tables) [linsert $attr(tables) 0 "bg"]
 }
}

proc tag_/td {params} {
 variable w
 variable tables
 variable attr
 variable _istext 0

 switch $tables($tables(level),tag) {
  "tb" {tag_tr ""; tag_td ""}
  "tr" {tag_td ""}
 }
 set tables($tables(level),tag) "tr"
 set attr(justify) $tables($tables(level),td_align)
 if {[set pos [lsearch $attr(tables) "bg"]]!=-1} {
  set attr(tables) [lreplace $attr(tables) $pos $pos]
  catch {set attr(bgcolors) [lreplace $attr(bgcolors) 0 0]}
 }
 if {$tables(stack)!=""} {
  $w.text delete "end -1c"
  $w.text configure -state disabled
  set w [lindex $tables(stack) 0]
  set tables(stack) [lreplace $tables(stack) 0 0]
 }
}

proc tablesCalc {} {
 variable tables
 variable lmargin

# update idletasks
 set backtables ""
 set lmarg $lmargin
 foreach {wp container p_width border marg} $tables(tables) {
  set backtables [linsert $backtables 0 $container]
  set cols [lindex [grid size $container] 0]
  set cell_w [expr {100/$cols}]
  set width [expr {[winfo width $wp.text] - $marg - $lmarg}]
  set lmarg 0
  if {$p_width!=""} {
   if {[string match "*%" $p_width]} {
    set width [expr {$width * [string trim $p_width "%"] / 100}]
   } else {
    set width $p_width
   }
  }
#  puts "METRICS: cols=$cols width=$width"
  for {set c 0} {$c<$cols} {incr c} {
   set weight [grid columnconfigure $container $c -weight]
   set minsize [grid columnconfigure $container $c -minsize]
   if {!$minsize && !$weight} {
    grid columnconfigure $container $c -weight $cell_w \
         -minsize [expr {($width - ($border*4)) * $cell_w / 100}]
   }
  }
  update idletasks
 }

 foreach {container} $backtables {
  set cols [lindex [grid size $container] 0]
  set rows [lindex [grid size $container] 1]
  for {set r 0} {$r<$rows} {incr r} {
   for {set c 0} {$c<$cols} {incr c} {
    set win [grid slaves $container -row $r -column $c]
    if {$win!=""} {
     set lines [lindex [split [$win.text index "end -1c"] .] 0]
#     puts "[$win.text cget -height] - [$win.text yview]"
     catch {set lines [expr {round(ceil([$win.text cget -height] / [lindex [$win.text yview] 1]))}]}
     $win.text configure -height $lines
     update idletasks
#     puts "[$win.text cget -height] - [$win.text yview]"
     catch {set lines [expr {round(ceil([$win.text cget -height] / [lindex [$win.text yview] 1]))}]}
     $win.text configure -height $lines
     set minsize [grid rowconfigure $container $r -minsize]
#     puts "WIN: $win , $lines - [$win.text dlineinfo "$lines.0"] | $minsize"
#     while {[set dli [$win.text dlineinfo "$lines.0"]]=="" && 0} {
#      grid rowconfigure $container $r -minsize [incr minsize 300]
#      puts "$minsize , $lines - [$win.text dlineinfo "$lines.0"] - [winfo height $wp.text]"
#      update idletasks; update
#     }
#     if {[set dli [$win.text dlineinfo "$lines.0"]]!=""} {
#      if {!$minsize && [set nms [expr {1 + [lindex $dli 1] + [lindex $dli 3]}]]>$minsize} {
#       puts "WIN: $win , $lines - [$win.text dlineinfo "$lines.0"] | $nms > $minsize"
#       grid rowconfigure $container $r -minsize [set minsize $nms]
#       update idletasks
#      }
#     }
    }
   }
  }
  update idletasks
 }
# for {set r 0} {$r<$rows} {incr r} {
#  for {set c 0} {$c<$cols} {incr c} {
#   set win [grid slaves $tables($tables(level),container) -row $r -column $c]
 #  set weight [grid columnconfigure $tables($tables(level),container) $c -weight]
 #  set minsize [grid columnconfigure $tables($tables(level),container) $c -minsize]
#   puts "WINDOW: $r.$c - $win"

#   set lines [lindex [split [$win.text index "end -1c"] .] 0]
#   set max 0
#   for {set l 1} {$l<=$lines} {incr l} {
#    set sl [$win.text dlineinfo "$l.0"]
#    if {$sl!="" && [set sl [expr {[lindex $sl 0] + [lindex $sl 2]}]]>$max} {set max $sl}
#   }
#   puts "LINES: $lines, max=$max"
#  }
# }

}

proc tableBind {w} {
 variable gw

  bind $w <Key> "break"
  bind $w <KeyPress> "break"
  bind $w <KeyRelease> "break"
  bind $w <BackSpace> "event generate $gw.text <BackSpace>;break"
  bind $w <Alt-F5> "event generate $gw.text <Alt-F5>;break"
  bind $w <Alt-Left> "event generate $gw.text <Alt-Left>;break"
  bind $w <Alt-Right> "event generate $gw.text <Alt-Right>;break"
  bind $w <Double-1> "break"
  bind $w <1> "event generate $gw.text <1>"
  bind $w <Escape> "event generate $gw.text <Escape>"
  bind $w <Control-R> "event generate $gw.text <Control-R>"
  bind $w <Control-r> "event generate $gw.text <Control-r>"
  bind $w <Control-L> "event generate $gw.text <Control-L>"
  bind $w <Control-l> "event generate $gw.text <Control-l>"
  bind $w <Control-bracketleft>  "event generate $gw.text <Control-bracketleft>"
  bind $w <Control-bracketright> "event generate $gw.text <Control-bracketright>"
  bind $w <Control-c> "event generate $gw.text <<Copy>>"     ;#??
  bind $w <Control-C> "event generate $gw.text <Control-C>"  ;#??
  bind $w <Control-Insert> "event generate $w.text <Control-c>" ;#??
  bind $w <Control-o> "event generate $gw.text <Control-o>"
  bind $w <Control-O> "event generate $gw.text <Control-O>"
  bind $w <Right> "event generate $gw.text <Right>; break"
  bind $w <Left>  "event generate $gw.text <Left>; break"
  bind $w <Down>  "event generate $gw.text <Down>; break"
  bind $w <Up>    "event generate $gw.text <Up>; break"
  bind $w <Next>  "event generate $gw.text <Next>; break"
  bind $w <Prior> "event generate $gw.text <Prior>; break"
  bind $w <Home>  "event generate $gw.text <Home>; break"
  bind $w <End>   "event generate $gw.text <End>; break"
  bind $w <Control-f> "event generate $gw.text <Control-f>; break"
  bind $w <Control-F> "event generate $gw.text <Control-F>"
  bind $w <Control-s> "event generate $gw.text <Control-s>; break"
  bind $w <Control-S> "event generate $gw.text <Control-S>"
  bind $w <3> "help::rightclick %X %Y"
}

}; #source
