#Copyright INRIA
#2004
#Author: F.Leray
#freely inspired by lemontree at http://wiki.tcl.tk/12751


#global env(SCIPATH)
global ged_handle_list_size OBJECTSARRAY LEVELS
global ged_listofpref
global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION msdos
global ww


package require BWidget
package provide lemontree

namespace eval LemonTree {variable uniqueID 0}

#if 0 {The Tree widget allows a -data item for each node, which I use for a dict-like list that 
# contains the node's type and "real name" (as opposed to the "display name" - for instance, 
# a dir node would display only its [file tail], but the real name is the full path). 
# This routine adds a node to the LemonTree: }

proc LemonTree::add {w parent type name {text ""}} {
    global OBJECTSARRAY
    variable uniqueID; variable icon
    
    if {$text eq ""} {
#	puts "text vaut rien pour $w $parent $name FIN"
	set text $name}
    set id n[incr uniqueID]
    
    set data [list type $type name $name]

    set fill [expr {[string match (* $text)]? "blue": "black"}]
    set drawcross [expr {[info proc ::LemonTree::kids($type)] eq ""?
			 "never": "allways"}]
    $w insert end $parent $id -text $text -data $data -font {Arial -15}\
	-drawcross $drawcross -fill $fill
        
     if { $type eq "variable" } {
	 global OBJECTSARRAY
	 set name [dict'get [$w itemcget $id -data] name]
	 set arr [dict'get [$w itemcget [$w parent $id] -data] name]
	 #	 list ${arr}($name) = [set ${arr}($name)]
	 
	 set text [set ${arr}($name)]
	 
	 #update the text in element array
	 $w itemconfigure $id -text $text
     }
    
    
    if [info exists icon($type)] {
	$w itemconfigure $id -image $icon($type)
    }

    $w selection add $id
    
    set ii2 [$w selection get]

#    $w selection remove
}

# if 0 {For speed, a Tree isn't drawn fully expanded at the beginning. 
# Instead, nodes are opened on demand, when the user clicks on the [+] icon.
# I use the -drawcross "allways" mode (shudder - should be fixed to "always", 
# but then older code might break) to indicate that the node hasn't been opened 
# before - after the first opening, the mode is set to "auto", meaning to draw a cross 
# only if the node has children. }

proc LemonTree::open {w node} {
    if {[$w itemcget $node -drawcross] eq "allways"} {
	set data [$w itemcget $node -data]
	set type [dict'get $data type]
	
	set aa kids($type)
	
	foreach {ktype kids} [kids($type) $w $node] {
	    foreach kid $kids {add $w $node $ktype $kid}
	}
	$w itemconfigure $node -drawcross auto
    }
}

# if 0 {So far for the generic LemonTree - the rest is already customization 
# for specific item types. The kids($type) call above looks like an array element 
# - in fact it's a way of dispatching the generic operation of providing the list 
# of children of an entity of given type, which of course depends on the type. For instance, 
# the children of a directory are its subdirectories, and then its files (with special-casing 
# for Windows, so that drive letters are the children of "/"): }

proc LemonTree::kids(dir) {w node} {
    set name [dict'get [$w itemcget $node -data] name]
    if {$::tcl_platform(platform) eq "windows" && $name eq "/"} {
	return [list dir [file volumes]]
    }
    set dirs  [lsort [glob -nocomplain -type d $name/*]]
    set files [lsort [glob -nocomplain -type f $name/*]]
    list dir $dirs file $files
}

# if 0 {Namespaces have a hierarchy, but contain collections of commands and variables as well. 
# So I introduced an intermediate layer (parens around the display name make these "meta-children" 
# come displayed in blue):}

proc LemonTree::kids(namespace) {w node} {
    list ns-commands (Commands) ns-vars (Variables) ns-children (Children)
}
proc LemonTree::kids(ns-children) {w node} {
    set ns [dict'get [$w itemcget [$w parent $node] -data] name]
    list namespace [lsort [namespace children $ns]]
}
proc LemonTree::kids(ns-commands) {w node} {
    set ns [dict'get [$w itemcget [$w parent $node] -data] name]
    list command [lsort [info commands ${ns}::*]]
}
proc LemonTree::kids(ns-vars) {w node} {
    set ns [dict'get [$w itemcget [$w parent $node] -data] name]
    set res ""
    foreach var [lsort [info vars ${ns}::*]] {
	lappend res [expr {[array exists $var]? "array": "variable"}] $var
    }
    set res
}

proc LemonTree::kids(FIGURE) {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size

    set axes ""

    for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
	if { [string match "Axes*" $OBJECTSARRAY($i)] } {
	    lappend axes $OBJECTSARRAY($i)
	    
	}
    }
    
    set toto [list variable [array names axes]]

    list AXES $axes
}

proc LemonTree::kids(AXES) {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size
    global axes

    set lalist [LemonTree::build_list $w $node]

    set lalist
}

proc LemonTree::build_list {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size
    global LEVELS

    set axeschildren ""
    set nom_axe [$w itemcget $node -text]
    
    for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
	if { [string match $nom_axe $OBJECTSARRAY($i)] } {
	    break
	}
    }
    
    set levelparent $LEVELS($i)

    incr i

    if { $i > $ged_handle_list_size } {
	return ""
    }
    
    while { $i <= $ged_handle_list_size } {
	if { $LEVELS($i) < [expr $levelparent+1] } break
	
	if { $LEVELS($i) == [expr $levelparent+1] } {
	    if { [string match "Compound*" $OBJECTSARRAY($i)] } {
		lappend axeschildren COMPOUND  $OBJECTSARRAY($i) 
	    } else {
		lappend axeschildren LEAF $OBJECTSARRAY($i) 
	    }
	}
	incr i
    }

    return $axeschildren
}


proc LemonTree::kids(COMPOUND) {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size


    set lalist [LemonTree::build_list $w $node]

    set lalist
}


proc LemonTree::kids(LEAF) {w node} {
    global OBJECTSARRAY
    set name [dict'get [$w itemcget $node -data] name]

    set toto [list variable [lsort [array names $name]]]

    return $toto
}



#-- Arrays can also be seen as a one-level subtree:

proc LemonTree::kids(array) {w node} {
    global OBJECTSARRAY
    set name [dict'get [$w itemcget $node -data] name]

    set toto [list variable [lsort [array names $name]]]

    return $toto;
}

proc LemonTree::kids(widget) {w node} {
    set name [dict'get [$w itemcget $node -data] name]
    list widget [winfo children $name]
}

# if 0 {A Tree looks prettier if nodes have icons, so I'm using some of those that 
# BWidget comes with:}

#  set path $BWIDGET::LIBRARY/images
set path "$env(SCIPATH)/tcl/ged/images"
#set path "/home/leray/scilab/tcl/ged/images"
# foreach {type name} {dir folder file file array copy} {
#     set LemonTree::icon($type) [image create photo -file $path/$name.gif]
# }

# Some more icons come from adavis's Icons package:

#set LemonTree::icon(widget) [image create photo -file $path/folder.gif]

set LemonTree::icon(COMPOUND) [image create photo -file $path/compound.gif]
set LemonTree::icon(AXES) [image create photo -file $path/axes.gif]

set LemonTree::icon(LEAF) [image create photo -file $path/leaf.gif]

set LemonTree::icon(FIGURE) [image create photo -file $path/figure.gif]


# CllKVBnT5UiaGkFKHPiRY0aIF3/yDPqQJ8mEAQEAOw==
#     R0lGODlhEAAQAIUAAFxaXGRmZFRWVGQmhFwmfFxeXOTm5MTCxLyWzLySzKyC
#     vKSCvJxyrJRmrIxipIxWpNze3AQCBMTGxJRWtJRatIxOrIRCpHw+pHw6nHQ2
#     lGwulOzu7Pz+/Pz+9Ozu5Pz+7NzexPz+5Pz+hPz+3NzevPz+BMTCBNzetMTC
#     DPz+xNze1NzezPwCBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
#     AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAACwALAAAAAAQABAAAAaV
#     QEBAMCAUj0aCYFkwHBAJhWLBYDQcD8ghIjhIJhRKxXLBZDQaiYQLABDe8PiS
#     u+HY7/dOh+PhQvB4eh8fIH6Adh2DHyGFAn+BiQAiISMkhnmSIQAlI5KXHIkf
#     AiUmpCUnhoKLISgpIikmAlwqtCArkiUlIhwiuSKyEcHCESausMEsycrJEaal
#     y9ARIizN1NPQ0dfJfkEAIf5oQ3JlYXRlZCBieSBCTVBUb0dJRiBQcm8gdmVy
#     c2lvbiAyLjUNCqkgRGV2ZWxDb3IgMTk5NywxOTk4LiBBbGwgcmlnaHRzIHJl
#     c2VydmVkLg0KaHR0cDovL3d3dy5kZXZlbGNvci5jb20AOw==}]
set LemonTree::icon(namespace) [image create photo -data {
    R0lGODlhEAAQAIIAAPwCBAQCBPz+xERCBMTCBISCBDQyNAAAACH5BAEAAAAA
    LAAAAAAQABAAAANPCLoR+7AJ0SALYkxd79za12FgOTlAQBDhRxUFqrKEG8Py
    OqwEfMeKwGDI8zVGul0vFsAFdaxB43ecKZfUKm1lZD6ERZgBZWn0OpYvGeJP
    AAAh/mhDcmVhdGVkIGJ5IEJNUFRvR0lGIFBybyB2ZXJzaW9uIDIuNQ0KqSBE
    ZXZlbENvciAxOTk3LDE5OTguIEFsbCByaWdodHMgcmVzZXJ2ZWQuDQpodHRw
    Oi8vd3d3LmRldmVsY29yLmNvbQA7}]
set LemonTree::icon(command) [image create photo -data {
    R0lGODlhEAAQAIIAAPwCBAQCBISChMTCxDQyNFxaXKSipPz+/CH5BAEAAAAA
    LAAAAAAQABAAAANdCLobwbAFMciLwBFSihBEFHSG8QnmpQQEBX6loI5G5QTl
    cMgrZSmEmsGxKqRWNV3hMrFlBtDoA1eTEaKHJdMYhR6+gxkF++UMGbiDzvDV
    ioyHAJSHcchuGLQq4k8AACH+aENyZWF0ZWQgYnkgQk1QVG9HSUYgUHJvIHZl
    cnNpb24gMi41DQqpIERldmVsQ29yIDE5OTcsMTk5OC4gQWxsIHJpZ2h0cyBy
    ZXNlcnZlZC4NCmh0dHA6Ly93d3cuZGV2ZWxjb3IuY29tADs=}]
set LemonTree::icon(variable) [image create photo -data {
    R0lGODlhEAAQAIYAAPwCBFxaVMR+RPzKjNze3AQCBMR6RPzGjPyODPz+/MzO
    zPyKDPyKBPz29OTWzPyGDPyGBOx6BOza1OR2BKROBNSOXKRKBBwOBOzu7PTW
    xPzizOySZPyCDFxaXOy2lNRyRMxmJCQOBPTm1OzStPTKrMR+XIRWLFxGNCQS
    BDQyNIRSNDQuJERGRLyqlNzSvIx6ZKRuVEw6LLSyrLymhKSShBwaFFROTJyW
    jMS+vNzW1OTazNzKrHRqXOzezOTOpPTq3OzWvOTStLyedMS+rLy2pMSynMSu
    lAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAAAA
    LAAAAAAQABAAAAewgAAAAYSFhoQCA4IBBI2OjgUGBwiLBAmXlpcKkgsMlZcJ
    BA0JDpIPEBGVjwkSBgOnExSfmBIVBxAMExYXswkYGRobHLq8gh2PHhoeHyAW
    IYKzIiMkJSYnKCnQg5YNHtQqKywtK9qMBC4vMDEBMjIz2dCMDTQ1Njc4OToz
    5PEEOzw3ZPToMcLHO23HfogQ0QMIkCA+hPBbhAPHECJFjMyYIUQIvEUpUqwQ
    OXKkSEF+AgEAIf5oQ3JlYXRlZCBieSBCTVBUb0dJRiBQcm8gdmVyc2lvbiAy
    LjUNCqkgRGV2ZWxDb3IgMTk5NywxOTk4LiBBbGwgcmlnaHRzIHJlc2VydmVk
    Lg0KaHR0cDovL3d3dy5kZXZlbGNvci5jb20AOw==}]
 set LemonTree::icon(list) [image create photo -data {
     R0lGODlhEAAQAIUAAFxaXGRmZFRWVGQmhFwmfFxeXOTm5MTCxLyWzLySzKyC
     vKSCvJxyrJRmrIxipIxWpNze3AQCBMTGxJRWtJRatIxOrIRCpHw+pHw6nHQ2
     lGwulOzu7Pz+/Pz+9Ozu5Pz+7NzexPz+5Pz+hPz+3NzevPz+BMTCBNzetMTC
     DPz+xNze1NzezPwCBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
     AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACH5BAEAACwALAAAAAAQABAAAAaV
     QEBAMCAUj0aCYFkwHBAJhWLBYDQcD8ghIjhIJhRKxXLBZDQaiYQLABDe8PiS
     u+HY7/dOh+PhQvB4eh8fIH6Adh2DHyGFAn+BiQAiISMkhnmSIQAlI5KXHIkf
     AiUmpCUnhoKLISgpIikmAlwqtCArkiUlIhwiuSKyEcHCESausMEsycrJEaal
     y9ARIizN1NPQ0dfJfkEAIf5oQ3JlYXRlZCBieSBCTVBUb0dJRiBQcm8gdmVy
     c2lvbiAyLjUNCqkgRGV2ZWxDb3IgMTk5NywxOTk4LiBBbGwgcmlnaHRzIHJl
     c2VydmVkLg0KaHR0cDovL3d3dy5kZXZlbGNvci5jb20AOw==}]


proc SavePreferences { } {
    global ged_listofpref
    global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION
    global ww msdos
    
    ScilabEval "DestroyGlobals()" "seq"
        
    if { $msdos == "F" } {
#unix mandrake (at least) needs this offset
#test other unix distribution to see (red hat, suse...)
	set xoffset -5
	set yoffset -26
    } else {
	set xoffset 0
	set yoffset 0
    }
    
    set x [eval {winfo x $ww}]
    set y [eval {winfo y $ww}]
    set MAIN_WINDOW_POSITION "+[expr $x+$xoffset]+[expr $y+$yoffset]"
    
    #save preferences (position...)
    set preffilename [file join $envSCIHOME .GedPreferences.tcl]
    catch {
 	set preffile [open $preffilename w]
	foreach opt $ged_listofpref {
	    global $opt
	    puts $preffile [concat "set $opt " [set $opt]]
	    # 	    puts [concat "set $opt" [set $opt]]
	}
	close $preffile
    }
}

# if 0 {This thing is more useful if you can get more information about an item by 
# clicking on it - for a file, its size and date; for a variable, its value; for a proc, 
# its full specification, etc. As a small first shot, I selected a "balloon" for that purpose. }

proc LemonTree::Info {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size

    set type [dict'get [$w itemcget $node -data] type]
    #     if {[info proc ::LemonTree::info($type)] ne ""} {
    #     balloon $w [info($type) $w $node]
    #     }
    
    set indexinlist [info($type) $w $node]
    
    #    puts "indexinlist vaut: $indexinlist"
    
    set nom_correspondant [$w itemcget $node -text]
    
    #    puts "nom_correspondant vaut: $nom_correspondant"
     
    for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
	if { [string match $nom_correspondant $OBJECTSARRAY($i)] } {
	    break
	}
    }

    SavePreferences
   
    ScilabEval "Get_handle_from_index($i);"

    return $indexinlist
}

#-- type-specific info providers:

proc LemonTree::info(FIGURE) {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size
    
    set nom_axe [$w itemcget $node -text]
        
    for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
	if { [string match $nom_axe $OBJECTSARRAY($i)] } {
	    break
	}
    }
    
    $w selection set $node
    
    return $i
}

proc LemonTree::info(AXES) {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size
    
    set nom_axe [$w itemcget $node -text]
        
    for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
	if { [string match $nom_axe $OBJECTSARRAY($i)] } {
	    break
	}
    }
    
    $w selection set $node
    
    return $i
}

proc LemonTree::info(COMPOUND) {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size
    
    set nom_axe [$w itemcget $node -text]
        
    for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
	if { [string match $nom_axe $OBJECTSARRAY($i)] } {
	    break
	}
    }
    
    $w selection set $node
    
    return $i
}

proc LemonTree::info(LEAF) {w node} {
    global OBJECTSARRAY
    global ged_handle_list_size
    
    set nom_axe [$w itemcget $node -text]
        
    for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
	if { [string match $nom_axe $OBJECTSARRAY($i)] } {
	    break
	}
    }
    
    $w selection set $node
    
    return $i
}

proc LemonTree::info(array) {w node} {
    global OBJECTSARRAY
    set name [dict'get [$w itemcget $node -data] name]
    return "$name: array, [array size $name] elements"
}
proc LemonTree::info(command) {w node} {
    set name [dict'get [$w itemcget $node -data] name]
    if {[info procs $name] ne ""} {
	return [procinfo $name]
    } else {return "$name: compiled command"}
}
proc LemonTree::info(dir) {w node} {
    set name [dict'get [$w itemcget $node -data] name]
    set mtime [clock format [file mtime $name] -format %y-%m-%d,%H:%M:%S]
    set nfiles [llength [glob -nocomplain $name/*]]
    return "$name\n$nfiles files\nModified: $mtime"
}
proc LemonTree::info(file) {w node} {
    set name [dict'get [$w itemcget $node -data] name]
    set mtime [clock format [file mtime $name] -format %y-%m-%d,%H:%M:%S]
    return "$name\n[file size $name] bytes\nModified: $mtime"
}
proc LemonTree::info(namespace) {w node} {
    set ns [dict'get [$w itemcget $node -data] name]
    return "namespace $ns\n[llength [info commands ${ns}::*]] commands,\
        [llength [info vars ${ns}::*]] variables,\
        [llength [namespace children $ns]] child(ren)"
}
proc LemonTree::info(variable) {w node} {
    global OBJECTSARRAY
    set name [dict'get [$w itemcget $node -data] name]
    if [info exists $name] {
	list $name = [set $name]
    } else { #-- array element
	set aa "$w parent $node"
	set bb "[$w itemcget [$w parent $node] -data]"

	set arr [dict'get [$w itemcget [$w parent $node] -data] name]

	list ${arr}($name) = [set ${arr}($name)]
    }
}
proc LemonTree::info(widget) {w node} {
    set name [dict'get [$w itemcget $node -data] name]
    return "[winfo class $name] $name [winfo geometry $name]"
}

#-- A simple ballon, modified from Bag of Tk algorithms:

# proc balloon {w text} {
#     set top .balloon
#     catch {destroy $top}
#     toplevel $top -bd 1
#     pack [message $top.txt -aspect 10000 -bg lightyellow \
# 	      -borderwidth 0 -text $text -font {Helvetica 9}]
#     wm overrideredirect $top 1
#     wm geometry $top +[winfo pointerx $w]+[winfo pointery $w]
#     bind  $top <1> [list destroy $top]
#     raise $top
# }

# if 0 {From Tcl 8.5, one would use a real dict, but it's easy to make a replacement 
# that works roughly the same in 8.4 (it returns "" for non- existing keys instead of 
# throwing an error), and might be slower, but I won't notice on dicts with two elements ;-}

proc dict'get {dict key} {
    foreach {k value} $dict {if {$k eq $key} {return $value}}
}

#-- reconstruct a proc's definition as a string:

proc procinfo name {
    set args ""
    foreach arg [info args $name] {
	if [info default $name $arg def] {lappend arg $def}
	lappend args $arg
    }
    return "proc $name {$args} {[info body $name]}"
}


proc LemonTree::finddata {w node data} {

    foreach knode $node  {
	set nom [$w itemcget $knode -text]
	if { [string match $nom $data] } {
	    break
	}
    }

    set thenode $knode

    set node2display $knode

    while { $knode != "" } {
	set knode [$w parent $knode]
	lappend node2display $knode
    }

    $w selection remove


    set i 0
    foreach node $node2display  {
	incr i
	set aa [expr [llength $node2display]-1]

	if { $i == $aa } break
	$w itemconfigure $node -open 1

    }

    $w selection set $thenode

}

# #-- Now to demonstrate and test the whole thing:

# Tree .t -background white -opencmd {LemonTree::open .t} \
#     -width 40 -height 30 -yscrollcommand {.y set} \
#     -selectbackground blue -selectforeground white
# .t bindText  <1> {LemonTree::Info .t}
# .t bindImage <1> {LemonTree::Info .t}
# LemonTree::add .t root dir       /  "(Files /)"
# LemonTree::add .t root namespace :: "(Namespace ::)"
# LemonTree::add .t root widget    .  "(Widget .)"
# LemonTree::add .t root array     a  "tablo"
# LemonTree::add .t root array     b  "tablo"
# LemonTree::add .t root FIGURE    currentfigure  "Figure(1)"

# .t opentree n6

# #.t itemconfigure n6 -open 1

# set ii [.t selection get]

# puts "ii VAUT: $ii"
# .t closetree n6
# LemonTree::finddata .t $ii "Compound(2)"


# #"(Array)"

# pack [scrollbar .y -command {.t yview}] -side right -fill y
# pack .t -fill both -expand 1 -side left

# #-- Little development helpers:

# bind . <Escape> {exec wish $argv0 &; exit}
# bind . <F1> {console show}