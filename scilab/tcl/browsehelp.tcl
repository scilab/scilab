#!/bin/sh
# here is a sample html viewer to demonstrate the library usage
# Copyright (c) 1995 by Sun Microsystems
#
# See the file "license.terms" for information on usage and redistribution
# of this file, and for a DISCLAIMER OF ALL WARRANTIES.
#
# This REQUIRES Tk4.0 -- make sure "wish" on the next line is a 4.0 version
# The next line is a TK comment, but a shell command \
  exec wish "$0" "$@" & exit 0


### modified by Matthieu 16/11/2001
if { ![info exists sciw] } {
    set sciw .foo
}
if { ![info exists lang] } {
    set lang "eng"
}
if { ![info exists manpath] } {
    set manpath "/usr/local/lib/scilab/man"
}

catch {destroy $sciw}
toplevel $sciw
wm title $sciw "Scilab Help"

set helpfile($sciw) $manpath

##
if {$tk_version < 4.0 || [regexp {b[123]} $tk_patchLevel] } {
	puts stderr "This library requires TK4.0, this is only $tk_version, \
			patchlevel $tk_patchLevel"
	exit 1
}
if {[catch {array get env *}]} {
	puts stderr "This library requires tcl7.4, this version is too old!"
	exit 1
}

if {[info exists SciPath]} { set env(SCI) $SciPath };
source "$env(SCI)/tcl/html_library.tcl"

# construct a simple user interface
proc quitapp {widget} {
    global helpfile
    global hreflist
    global hrefindex
    global Url

    unset helpfile($widget)
    unset hreflist($widget)
    unset hrefindex($widget)
    unset Url($widget)
    destroy $widget
}
# added by Matthieu PHILIPPE 18/12/2001
# to have the possibility to launch multiple
# browser from scilab with independant menu
# a little uggly !!!
proc setfont0 {sciwwidget} {
        global sciw
        global Size

        set sciw $sciwwidget
        set Size 0
        HMset_state $sciw.text -size $Size;
        HMreload $sciw;#render $sciw $Url
}
proc setfont4 {sciwwidget} {
        global sciw
        global Size

        set sciw $sciwwidget
        set Size 4
        HMset_state $sciw.text -size $Size;
        HMreload $sciw;#render $sciw $Url
}
proc setfont12 {sciwwidget} {
        global sciw
        global Size

        set sciw $sciwwidget
        set Size 12
        HMset_state $sciw.text -size $Size;
        HMreload $sciw;#render $sciw $Url
}

proc setindent06 {sciwwidget} {
        global sciw
        global Indent
    
        set sciw $sciwwidget
        set Indent 0.6
        HMset_indent $sciw.text $Indent
        HMreload $sciw;#render $sciw $Url
}
proc setindent12 {sciwwidget} {
        global sciw
        global Indent

        set sciw $sciwwidget
        set Indent 1.2
        HMset_indent $sciw.text $Indent
        HMreload $sciw;#render $sciw $Url
}
proc setindent24 {sciwwidget} {
        global sciw
        global Indent

        set sciw $sciwwidget
        set Indent 2.4
        HMset_indent $sciw.text $Indent
        HMreload $sciw;#render $sciw $Url
}
#####

proc setup {} {
        global sciw
        global lang
        # added $sciw to have multiple browser from scilab's launch
        # little uggly !!
	frame $sciw.frame
        
      if {$lang == "eng"} {
	  menubutton $sciw.menu -relief raised -bd 2 -text Options... -menu $sciw.menu.m
	  button $sciw.quit  -text Quit  -command "quitapp $sciw"        
	  button $sciw.prev -text Previous -command "HMprev $sciw"
	  button $sciw.next -text Next -command "HMnext $sciw"
	  button $sciw.home -text Chapters -command "HMhome $sciw"
	  button $sciw.reload -text Reload -command "HMreload $sciw"
	  button $sciw.copy -text Copy -command "HMcopy $sciw"
      } else {
	  menubutton $sciw.menu -relief raised -bd 2 -text Options... -menu $sciw.menu.m
	  button $sciw.quit  -text Quitter  -command "quitapp $sciw"
          button $sciw.prev -text Précédent -command "HMprev $sciw"
          button $sciw.next -text Suivant -command "HMnext $sciw"
          button $sciw.home -text Chapitres -command "HMhome $sciw"
          button $sciw.reload -text Recharger -command "HMreload $sciw"
	  button $sciw.copy -text Copier -command "HMcopy $sciw"
      }

	#entry $sciw.entry  -textvariable Url -width 35
	#label $sciw.file  -text file:
	#label $sciw.status -textvariable Running -width 6 -relief ridge \
	#		-bd 2 -padx 9 -pady 3
	label $sciw.msg -textvariable message
	scrollbar $sciw.scrollbar  -command "$sciw.text yview"  -orient v
	option add *Text.height 40 startup
	option add *Text.width 80 startup
	text $sciw.text  -yscrollcommand "$sciw.scrollbar set" -padx 3 -pady 3 -takefocus 0 -relief ridge

	pack $sciw.frame $sciw.msg -side top
	pack $sciw.scrollbar -side left -expand 0 -fill y
	pack $sciw.text -side left -fill both -expand 1
	#pack $sciw.file $sciw.entry $sciw.status $sciw.menu $sciw.quit -in $sciw.frame -side left
        pack $sciw.prev $sciw.next $sciw.home $sciw.reload $sciw.menu $sciw.copy $sciw.quit -in $sciw.frame -side left
	# set up some sample keyboard bindings for the text widget
	#bind $sciw.entry <Return> {render $sciw $Url}
	bind all <End> {$sciw.text yview end}
	bind all <Home> {$sciw.text yview 0.0}
	bind all <Next> {$sciw.text yview scroll 1 page}
	bind all <Prior> {$sciw.text yview scroll -1 page}

	## added by Matthieu PHILIPPE 20/11/2001
	bind Text <Key-BackSpace> {}
	bind Text <Key> {}
	bind Text <Key-Return> {}
        bind Text <ButtonPress-2> {}
    bind Text <ButtonRelease-2> {set tototo 1}; # BEURK qui fera mieux pour inhiber le coller!!!
	bind Text <Key-Tab> {}
	bind Text <Key-Delete> {}
        bind Text <<Cut>> {}
        bind Text <<Paste>> {}
	###

	# I'm constantly being criticized for never using menus.
	# so here's a menu.  So there.
	menu $sciw.menu.m
#	$sciw.menu.m add command -label "option menu"
#	$sciw.menu.m add separator
	if {$lang == "eng"} {
	    $sciw.menu.m add command -label "font size" -foreground red 
	    $sciw.menu.m add radiobutton -label small -value 0   -variable Size$sciw \
		    -command "setfont0 $sciw"
	    $sciw.menu.m add radiobutton -label medium -value 4  -variable Size$sciw \
		    -command "setfont4 $sciw"
	    $sciw.menu.m add radiobutton -label large -value 12  -variable Size$sciw \
		    -command  "setfont12 $sciw"
	    $sciw.menu.m add separator
	    $sciw.menu.m add command -label "indent level" -foreground red
	    $sciw.menu.m add radiobutton -label small -value 0.6 -variable Indent$sciw \
		    -command "setindent06  $sciw"
	    $sciw.menu.m add radiobutton -label medium -value 1.2 -variable Indent$sciw \
		    -command "setindent12  $sciw"
	    $sciw.menu.m add radiobutton -label large -value 2.4 -variable Indent$sciw \
		    -command "setindent24  $sciw"
	} else {
	    $sciw.menu.m add command -label "taille de fontes" -foreground red 
	    $sciw.menu.m add radiobutton -label petit -value 0   -variable Size$sciw \
		    -command "setfont0 $sciw"
	    $sciw.menu.m add radiobutton -label moyen -value 4  -variable Size$sciw \
		    -command "setfont4 $sciw"
	    $sciw.menu.m add radiobutton -label grand -value 12  -variable Size$sciw \
		    -command  "setfont12 $sciw"
	    $sciw.menu.m add separator
	    $sciw.menu.m add command -label "indentations" -foreground red
	    $sciw.menu.m add radiobutton -label petites -value 0.6 -variable Indent$sciw \
		    -command "setindent06  $sciw"
	    $sciw.menu.m add radiobutton -label moyennes -value 1.2 -variable Indent$sciw \
		    -command "setindent12  $sciw"
	    $sciw.menu.m add radiobutton -label grandes -value 2.4 -variable Indent$sciw \
		    -command "setindent24  $sciw"
	}
}

# Go render a page.  We have to make sure we don't render one page while
# still rendering the previous one.  If we get here from a recursive 
# invocation of the event loop, cancel whatever we were rendering when
# we were called.
# If we have a fragment name, try to go there.

proc render {sciw file} {
	global HM.text Url
	global Running message
        #global sciw
	set fragment ""
	regexp {([^#]*)#(.+)} $file dummy file fragment
	if {$file == "" && $fragment != ""} {
		HMgoto $sciw.text $fragment
		return
	}
	HMreset_win $sciw.text
	# added by Matthieu PHILIPPE 06/12/2001
	# to preserve any click on mouse button
	# which disturbs the rendering.
	set Running busy
	$sciw.scrollbar configure  -command ""
	$sciw.prev configure  -command ""
	$sciw.next configure  -command ""
	$sciw.home configure  -command ""
	$sciw.reload configure  -command ""
	set message "Displaying $file"
	######
	update idletasks
	if {$fragment != ""} {
		HMgoto $sciw.text $fragment
	}
	set Url($sciw) $file
	HMparse_html [get_html $file] {HMrender $sciw.text}
	set Running ready
	# added by Matthieu PHILIPPE 06/12/2001
	# to preserve any click on mouse button
	# which disturbs the rendering.
	$sciw.scrollbar configure  -command "$sciw.text yview"
	$sciw.prev configure  -command "HMprev $sciw"
	$sciw.next configure  -command "HMnext $sciw"
	$sciw.home configure  -command "HMhome $sciw"
	$sciw.reload configure  -command "HMreload $sciw"
	####
	HMset_state $sciw.text -stop 1	;# stop rendering previous page if busy
	set message ""
}
#################################
# given a file name, return its html, or invent some html if the file can't
# be opened.

proc get_html {file} {
	global Home
        global sciw

	if {[catch {set fd [open $file]} msg]} {
		return "
			<title>Bad file $file</title>
			<h1>Error reading $file</h1><p>
			$msg<hr>
			<a href=$Home>Go home</a>
		"
	}
	set result [read $fd]
	close $fd
	return $result
}

# Override the library link-callback routine for the sample app.
# It only handles the simple cases.

proc HMlink_callback {win href} {
	global Url
        global sciw
        global helpfile
        global hreflist
        global hrefindex
        global Running
        global sciw
    set sciw [winfo parent $win]
    # added by Matthieu PHILIPPE 07/12/2001
    # to prevserve any hit on link or everything else
    # while the rendering is busy I have added and IF
    # condition. The rest is the original code.
    if {$Running != "busy"} {
	if {[string match #* $href]} {
		render $sciw $href
		return
	}
	if {[string match /* $href] || [string match *:* $href]} {
		set Url($sciw) $href
	} else {
		set Url($sciw) [file dirname $Url($sciw)]/$href
	}
	update
	set hreflist($sciw)  [HMget_sublist $hreflist($sciw) 0 $hrefindex($sciw)]
	incr hrefindex($sciw)
	set hreflist($sciw) [linsert $hreflist($sciw) $hrefindex($sciw) $Url($sciw)]
	render $sciw $Url($sciw)
    }
}

# Supply an image callback function
# Read in an image if we don't already have one
# callback to library for display

proc HMset_image {win handle src} {
	global Url message
        global sciw

	if {[string match /* $src] || [string match *:* $src]} {
		set image $src
	} else {
		set image [file dirname $Url($sciw)]/$src
	}
	set message "fetching image $image"
	update
	if {[string first " $image " " [image names] "] >= 0} {
		HMgot_image $handle $image
	} else {
		set type photo
		if {[file extension $image] == ".bmp"} {set type bitmap}
		catch {image create $type $image -file $image} image
		HMgot_image $handle $image
	}
}

# Handle base tags.  This breaks if more than 1 base tag is in the document

proc HMtag_base {win param text} {
	global Url
        global sciw

	upvar #0 HM$win var
	HMextract_param $param href Url($sciw)
}

# downloading fonts can take a long time.  We'll override the default
# font-setting routine to permit better user feedback on fonts.  We'll
# keep our own list of installed fonts on the side, to guess when delays
# are likely

proc HMset_font {win tag font} {
	global message Fonts
        global sciw

	if {![info exists Fonts($font)]} {
		set Fonts($font) 1
		$sciw.msg configure -fg blue
		set message "downloading font $font"
		update
	}
	$sciw.msg configure -fg black
	set message ""
	catch {$win tag configure $tag -font $font} message
}

# Lets invent a new HTML tag, just for fun.
# Change the color of the text. Use html tags of the form:
# <color value=blue> ... </color>
# We can invent a new tag for the display stack.  If it starts with "T"
# it will automatically get mapped directly to a text widget tag.

proc HMtag_fontc {win param text} {
        global sciw

	upvar #0 HM$win var
	set value bad_color
	HMextract_param $param color
	HMextract_param $param size
	$win tag configure $color -foreground $color
	HMstack $win "" "Tcolor $color"
}

proc HMtag_/fontc {win param text} {
        global sciw

	upvar #0 HM$win var
	HMstack $win / "Tcolor {}"
}

# Add a font size manipulation primitive, so we can use this sample program
# for on-line presentations.  sizes prefixed with + or - are relative.
#  <font size=[+-]3>  ..... </font>.  Note that this is not the same as
# Netscape's <font> tag.

proc HMtag_font {win param text} {
        global sciw

	upvar #0 HM$win var
	set size 0; set sign ""
	HMextract_param $param size
	regexp {([+-])? *([0-9]+)} $size dummy sign size
	if {$sign != ""} {
		set size [expr [lindex $var(size) end] $sign $size]
	}
	HMstack $win {} "size $size"
}

# This version is closer to what Netscape does
# modified by Matthieu PHILIPE
# to includ colr management !
proc HMtag_font {win param text} {
        global sciw

	upvar #0 HM$win var
	set size 0; set sign ""
	set color bad_color
    if {[HMextract_param $param size]} then {
	regexp {([+-])? *([0-9]+)} $size dummy sign size
	if {$sign != ""} {
	    set size [expr [lindex $var(size) end] $sign  $size*2]
	    HMstack $win {} "Tfont $size"
	} else {
	    HMstack $win {} "Tfont [expr 10 + 2 * $size]"
	}
    }
    if {[HMextract_param $param color]} then {
	$win tag configure $color -foreground $color
    }
    HMstack $win "" "Tfont $color"
}

proc HMtag_/font {win param text} {
        global sciw

	upvar #0 HM$win var
	HMstack $win / "Tfont {}"
}
### ajout Matthieu le 16/11/2001
# reurn to the next page memorized
proc HMnext {sciwwidget} {
        global hreflist
        global hrefindex
        global sciw

        set sciw $sciwwidget
        if { $hrefindex($sciw) < [llength $hreflist($sciw)] - 1 } {
	    incr hrefindex($sciw)
	}
	set next [lindex $hreflist($sciw) $hrefindex($sciw)]
	render $sciw $next
}

# return to the previous page memorized
proc HMprev {sciwwidget} {
        global hreflist
        global hrefindex
        global sciw

        set sciw $sciwwidget
        if { $hrefindex($sciw) > 0 } {
	    incr hrefindex($sciw) -1
	}
	set prev [lindex $hreflist($sciw) $hrefindex($sciw)]
	render $sciw $prev
}
### ajout Claude le 2/12/2001
# return to Home
proc HMhome {sciwwidget} {
        global hreflist
        global hrefindex
        global Home
        global sciw

        set sciw $sciwwidget

	set hreflist($sciw)  [HMget_sublist $hreflist($sciw) 0 $hrefindex($sciw)]
        incr hrefindex($sciw)
        set hreflist($sciw) [linsert $hreflist($sciw) $hrefindex($sciw) $Home]
        render $sciw $Home
    }

proc HMreload {sciwwidget} {
        global hreflist
        global hrefindex
        global sciw

        set sciw $sciwwidget

        set file [lindex $hreflist($sciw) $hrefindex($sciw)]
	render $sciw $file
}
###

#### Added by Matthieu PHILIPPE 17/11/2001
#    this function returns a sublist of thelist. The index begin at 0 as llindex
#    So we have these syntaxe MHget_sublist $a 0 3
#    and if llength $a == 6, then to have the three ended elements
#    MHget_sublist $a 3 5
#    or  MHget_sublist $a [expr ([llength $a] - 1) expr ([length $a] -1)
proc HMget_sublist {thelist deb fin} {
    set sublist [list [lindex $thelist $deb]]
    for {set i [expr ($deb + 1)]} {$i <= $fin} {incr i} {
	set sublist [linsert $sublist $i [lindex $thelist $i] ]
    }
    return $sublist
}
# added by Matthieu to bond Copy to clipboard function
proc HMcopy {sciwidget} {
    clipboard clear
    clipboard append [selection get -selection PRIMARY]
}

# set initial values
if { ![info exists Size] } {
    set Size 0
}		                        ;# font size adjustment
if { ![info exists Indent] } {
set Indent 1.2				;# tab spacing (cm)
}
### ajout Claude le 2/12/2001
if { ![info exists Home] } {
    set Home [pwd]/html/help.html	;# home document
}
set Url($sciw) $Home			;# current file
set Running busy			;# page status
set message ""				;# message line


# make the interface and render the home page
# the catch lets us re-source this file
set Size$sciw $Size
set Indent$sciw $Indent
catch setup
HMinit_win $sciw.text
HMset_state $sciw.text -size $Size
HMset_indent $sciw.text $Indent


### ajout Matthieu le 16/11/2001
if { [info exists helpfile($sciw)] } {
    if { [file exists $helpfile($sciw)] } {
	set next $helpfile($sciw)
	set prev $helpfile($sciw)
	set hrefindex($sciw) 0
	set hreflist($sciw) [list $helpfile($sciw)]
	render $sciw $helpfile($sciw)
    }  else {
	set next $Home
	set prev $Home
	set hrefindex($sciw) 0
	set hreflist($sciw) [list $Home]
	render $sciw $Home
    }  
} else {
    set next $Home
    set prev $Home
    set hrefindex($sciw) 0
    set hreflist($sciw) [list $Home]
    render $sciw $Home
}
###

