################################################################################
#      
#  Project       :  Help System
#
#  Developed by  :  Andrei A. Gratchev      <grand@deversys.com>
#
#  Filename      :  help.tcl
#  Version       :  1.5
#  Description   :  Help system based on HTML & HTML viewer
#
################################################################################
                                                                               
package provide help 1.5

namespace eval help {
 variable w
 variable gw
 variable abort         0
 variable showing       0
 variable stat          ""
 variable curfilename   ""
 variable data
 variable history       ""
 variable hpos          0
 variable index         ""
 variable attr
 variable perc
 variable font          "Helvetica"
 variable fontfixed     "Courier"
 variable fontsize      10
 variable backcolor     ""
 variable forecolor     black
 variable linkcolor     "#006800"
 variable alinkcolor    "#c00000"
 variable lmargin       6
 variable rmargin       6
 variable settings
 variable curtopic      ""
 variable _count
 variable _tags
 variable _images
 array set _images "broken  [image create photo -data "R0lGODlhIAAgALP/AP///8DAwP8A/7+/v39/f38AfwD//wB/AAAA/wAAfwAAAAAAAAAAAAAAAAAAAAAAACH5BAEAAAEALAAAAAAgACAAAAT/kMhJq50h60yA/2AoAtKmdWMqEgNhBuggz3Rdd60Z35NNx66Th3c4EBJI1u2T4wxnEhLyk/yBmjsZwZgYJZStUC7bOnRTX5lizV7Dntrj54iSQiuKN+B3BiAVUx5pcRN5ZAN9CQoSiWAzHoZwMkiUjHWDNJB6NhVfc5iPAJF7PpYgioWhoy0+np8ECAaoMppACjZ9UosGCKgstUMdtzR9R7sIvYu/opsow5NUx8i+A8B7HQKiM5RJvMjJEtXMQAAC2cPcX9/TyuKr2AIF2o3p1NZh5gXnjW1s4ffY9MUT0OjCv3HBABRYyDCdwYPv4j0M54MWwj39Mj6r6G5Ti4mOGHzcI/SQY8c6KlKOGKWyJYg8GmPK1BggAgA7"]
                    disc    [image create photo -data "R0lGODlhEAAFAID/AMDAwAAAACH5BAEAAAAALAAAAAAQAAUAAAINhA+hyKff2FvRzYogKwA7"]
                    circle  [image create photo -data "R0lGODlhEAAHAID/AMDAwAAAACH5BAEAAAAALAAAAAAQAAcAAAIShA+hyBfe0JPRTFivrbvztDAFADs="]
                    square  [image create photo -data "R0lGODlhEAAFAID/AMDAwAAAACH5BAEAAAAALAAAAAAQAAUAAAINhB+Zx6zb2gtRvuouKwA7"]
                    ugol    [image create photo -data "R0lGODlhDAAMAJH/AP///8DAwH9/fwAAACH5BAEAAAEALAAAAAAMAAwAAAIbjI8BmSB83IMSKvFWw3dnHnFV+GVGhZZXmaoFADs="]
                    d1      [image create photo -data "R0lGODlhEAAQAPf/AP///////P//+///7///6f//5f//5P//4f//3P//2f//0v//z///zv//zf//zP//y///yv//yf//x///xv//xf//xP//wv/+5v/+1f/82//82v/81//81v/81f7/8/783v373Pv9x/v9vfr9zPn5+vn5+fn57/n57ff7yfX4v/T68uv0w+Dvrdvtr9TU19TU0dTT19PptNLS1dLS0tLS0dLSz8znoczMzszMzMvLy8nkxsnIksjjw8fHx8fHwMbGxsLCwsHBwcDAwL+/v76+vri4uLbiyLGxsbCwsK+vr6ampqWlpaGhoaCgoJaWmJaWlpOTk5KSko+Pj4yMjIvFa4mJiX7HsHq+YXJycmy2WmmzRmSyV2Cfe16uN1udkVqsM1RUVE2XhExJTkpIPkhFSURBRkI/R0I/RkGgFD2dEzo4OSWSCiKQAB0cHRyAaxcWFwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACwAAAAAEAAQAAAI0QDhCBxIsOAQOEESKly4EAAcIEeiPJn4xIkTKEp+EBniEEgTHCVCkjhhwgWYKTmIdGQyA4BLDxUY+LiBpcqPlS1dKuiiYscYMW+KrJRhgICOLWy+KHgQgEySlTUcpHBjJA0PBREwnEECdcEKLgVYJAghYcPWlTQAHBABocUHLw0ynH3IhKiFEVS0WAkTdy4QJjUeOIixBk0WFGXnBmHyogGFBTauDJggQStXOD2WwLiQgQMCEBo2dBDg1KEQKWrKnDFj5sxqM2TaCOXosrbt2wEBADs="]
                    d2      [image create photo -data "R0lGODlhEAAQAPf/AP///////f///P//+///9///8v//5v//5P//4///4f//3///3f//2///2f//2P//1P//0P//zv//y///yv//yf//yP//x///xf//w///wP/+5f/+2f/+1f/82//81//81v/81f7+v/v96vv9zPn7yPn5+vn5+fn57/n57fj8uPb7xvT7xvH3/vH2wNrvldTpndTU19TU0dTT19LpxtLS1dLS0tLS0dLSz8/pgc/MkMzMzszMzMvLy8nmrsnfwsfHx8fHwcbGxsLCwsHiq8HBwcDAwL+/v76+vr3hpby8vLvdrri4uLe4t7XadLKysrGxsbCwsK+wr6+vr6ampqWlpaPGwKLYlaGhoaCgoJaWmJaWlpOTk5KSko+Pj43FdIyMjImJiX/FiXmtp3PAW3Jycm2jp2y2RGu2T2ayRlafmFRUVFBLRkxITkmNdUhFSURBRkI/R0I/RkCfLT6eHzo4OTeCgSGQCB0cHRcWFxOJAAGAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACwAAAAAEAAQAAAI2gD3CBxIsOCOPUQSKlyo0AiAPUKecNFCUUuWLFumBDly5KEQLDtMiCyB4gQMNV94BPF4pQaAlwQyTACigwyYJSxdAihQwYCDHGvY4JHCksYBBiTEpBnTAwcAN1BY3pBwwUcbFmfyIFkQJyrEKzciQHCQ4MUcO0MUdGVpI8AMJU1SqBARosParzYEoNHjpUELDBbsehUCdoEZOWHK1BlRwcNdIldiPHCxwkqVFRQscIhDdM8PKjI0INigwACDDyAGuMG5p0gXOm/iwIETRzYcN3cc7njJu7dvAAEBADs="]
                    d3      [image create photo -data "R0lGODlhEAAQAPf/AP///////P//+v//9f//7///6///5f//4f//4P//2///2P//0P//z///zf//zP//y///yv//yf//yP//x///xv//xf//w//+5v/+3f/+2v/+2f/+1f/91//82//81//81vz+zPz9y/v98vn5+vn5+fn57/n57fj7w/j7wvj7ufb62O33tOn2oefzv+PxntTU19TU0dTT19LS1dLS0tLS0dLSz9DjxczMzszMzMvLy8nfxsjIjcfHx8fHwcbiocbGxsLCwsHBwcDZ0sDAwL+/v77htr6+vrvfrbrijLncsri4uLbTtbXbirLYhrGxsbCwsK+vr6jUeqampqWlpaHM0aGhoaCgoJjJx5aWmJaWlpOTk5KSko+Pj4yMjImJiXS5WnJycmq0UWSxOla0aFRUVFGsSExJTkukM0pIPkhFSUeRhERBRkKQlkI/R0I/Rj2eGzo4OSSRAB0cHRcWFxKIABFragB/AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACwAAAAAEAAQAAAI1wDvCBxIsCCRO0ESKlyokAiAO0CcbMlCMQsWLFqk/DBi5CEQKzhIiBxhosQLMl1y/PBYZQaAlwMsQOhxA4wXJSxdvjxQYcEONGbmQGEpg0CUMF9ATFgAIUCaJyxrMPBBJ44IFDYebHADFWKVGgrGFBGT4kQdHRq4sqQBgI0QBy5UULlyQK1XGQaWqEnQ5EwZJAjsAvn6IEIIBUnsvGnBwW6QKjAaUJAQYcURJiwUuBl6h8eUGBc6ePiAocCBDALS4LwzhAucNW7atHETu00aOQ4dvtzNu3dAADs="]

                    back    [image create photo -data "R0lGODlhDwANAKL/AM///8DAwJD//y/I/y+X/y9n/wAAAAAAACH5BAEAAAEALAAAAAAPAA0AAAM0GLq2/qE0+AqYVFmB6eZFKEoRIAyCaaYCYWxDLM9uYBAxoe/7dA8ug3AoZOg6mRsyuUxmEgA7"]
                    forward [image create photo -data "R0lGODlhDwANAKL/AM///8DAwJD//y/I/y+X/y9n/wAAAAAAACH5BAEAAAEALAAAAAAPAA0AAAM3GLpa/K8YSMuYlBVwV/kgCAhdsAFoig7ktA1wLA9SQdw4DkuB4f8/Ag2TMRB4GYUBmewRm09FAgA7"]
                    stop    [image create photo -data "R0lGODlhDQANALP/AP///1Lq81I5Of+EhCEAAHsAAMYAAP+UQv9zCHuMjP8AMf8AKf+MnK1CSv8QIQAAACH5BAEAAAEALAAAAAANAA0AAARWMMjUTC1J6ubOQYdiCBuIIMuiiCT1OWu6Ys05AMPC4ItBGB8dYMdI+RoHR4qY6v1CwlvRcEQ4brndwFAgJAwIRdPIzVTEYiqXJBEU1FQCW5Mg2O0ZSQQAOw=="]
                    refresh [image create photo -data "R0lGODlhDAANALP/AP///zk5OVJSUoSEhKWlpcDAwP//1v//xr3erZTOezGcEFKtSimce3NzezkxOQAAACH5BAEAAAUALAAAAAAMAA0AAARRcJBJyRilEMC5AcjQaB1wHMYkCFuXLKDQONsBLIuynEBAGAcJAnYy0AyGBOLENPg4qGUISTMdEIoEg4A6ohK6BND4YyqBqCdyve453vB44BEBADs="]

                    inc     [image create photo -data "R0lGODlhDAAMAJECAICAgAAAAP///wAAACH5BAEAAAIALAAAAAAMAAwAAAIglH+BphxtQADi2fTWkY9qdS2S42HWtECVOmJleGqyUQAAOw=="]
                    dec     [image create photo -data "R0lGODlhDAAMAJECAICAgAAAAP///wAAACH5BAEAAAIALAAAAAAMAAwAAAIalC+hirjdApyUygoCqG/mtEEL03yGFjHdUQAAOw=="]
                    cont    [image create photo -data "R0lGODlhDAAMALMAADg4ONDQ0AEBAZqamv///4aGhrCwsGdnZ2wCAgCYmIaenoaGnj8/BJiYAAQ/P56GhiH5BAAAAAAALAAAAAAMAAwAAARBkAwhACGADqJJOEcgUcAwAGVxKQiSeoKBGqQ5C2JxvkF8VwPVSLRoMEq74GUCVPU2hEfCAaABTxTSBYbbZrJgcAQAOw=="]
                    index   [image create photo -data "R0lGODlhDAAMAKIEAACAAP//AP///wAAAP///wAAAAAAAAAAACH5BAEAAAQALAAAAAAMAAwAAAMpSLM89G+IMMWCimrpogWAxURaRXUcqAZoWaKfCrCkWcGDPLf2GTVAQgIAOw=="]
                    find    [image create photo -data "R0lGODlhDAAMAJEDAMDAwP///wAAAP///yH5BAEAAAMALAAAAAAMAAwAAAIgnI8oaysa3hDxuMtadFtegE3UAnYYh3XiuGzQqTjJPBQAOw=="]
                    search  [image create photo -data "R0lGODlhDAAMALMPADExMP//j2hoaGFhLgEBAYWFhe3t7a2tqv39aNPT0xcXFqGhH1BQXCoqHQgICv///yH5BAEAAA8ALAAAAAAMAAwAAARV8LR2kk3m6RZCGeAgaA/QIV0HkByKBgiQlafSLIECHA8XHAoBY6Eo8BggR4HAXMoMlgaAIJAyExoB4UAFJJiOLIMJUAKmWYNDdii4CdmGY/QwKBSOCAA7"]
                    open    [image create photo -data "R0lGODlhDAAMALMPAOvr605OOfv7iDk5FbGxsZ2dNM3NzY2NjcrKXYaGAImJABoaCIuLABkZAGFhYf///yH5BAEAAA8ALAAAAAAMAAwAAARJ8EkJyBhmanAeDwTXaY/TEE5ROCxrHAcQCAJiIw4xzXSfhAAZrdAoFheNwawwYCieDEZAhhgkrlcF5rGwMrCMLbexKJt1pPQkAgA7"]
                    reload  [image create photo -data "R0lGODlhDAAMAKIHAO3tYefnnkVFQTExgzc3J29vf8DAXv///yH5BAEAAAcALAAAAAAMAAwAAAM7eEJ8/sqAYAR0ZE7GyQqTURSGQTFTMYwDGHzGoI3gAtDTsL4EJYMqiS2gKrgmCwmloDl5BNBF1HOpHhIAOw=="]
                    prev    [image create photo -data "R0lGODlhDAAMAKIEAAAAgP//AP///wAAAP///wAAAAAAAAAAACH5BAEAAAQALAAAAAAMAAwAAAMpSLM89G+IMMWCimrpYqUAE2kCAFCdBJpTupqo97ZjBVuyTWdflTZAQgIAOw=="]
                    up      [image create photo -data "R0lGODlhDAAMAKIEAAAAgP//AP///wAAAP///wAAAAAAAAAAACH5BAEAAAQALAAAAAAMAAwAAAMnSLM89G+IMMWCimrpYt1XVgFVJ1FAOploqpqCK7Nf6WkfjVNm4xMJADs="]
                    next    [image create photo -data "R0lGODlhDAAMAKIEAAAAgP//////AAAAAP///wAAAAAAAAAAACH5BAEAAAQALAAAAAAMAAwAAAMrSLM89G8EMcOCimrp4gTblQUAWHWSUJoCSq0m+pVb9LKWp2qTW/GohpCQAAA7"]
 "
 variable btninfo
#Controls/buttons:  link_name     text/hint  vis_cond  activ_cond  button_name
 array set btninfo "__prev     {[list "Back"     1 {$help::hpos>0} "bback"]}
                    contents   {[list "Contents" 1 {[lsearch $help::index "contents"]!=-1} "bcont"]}
                    __dec      {[list "Decrease" 1 {$help::fontsize>6} "bdec"]}
                    __find     {[list "Find"     1 1 "bfind"]}
                    __next     {[list "Forward"  1 {$help::hpos<[expr [llength $help::history] - 1]} "bforward"]}
                    __inc      {[list "Increase" 1 {$help::fontsize<30} "binc"]}
                    __index    {[list "Index"    1 {$help::index!=""} "bindex"]}
                    __Next     {[list "Next"     {$help::h4contents!=""} {[lsearch $help::sequence $help::curtopic]<[expr [llength $help::sequence] - 1] && [lsearch $help::sequence $help::curtopic]>-1} "bnext"]}
                    __open     {[list "Open"     1 1 "bopen"]}
                    __Prev     {[list "Previous" {$help::h4contents!=""} {[lsearch $help::sequence $help::curtopic]>0} "bprev"]}
                    __refresh  {[list "Refresh"  1 1 "brefresh"]}
                    __reload   {[list "Reload"   1 1 "breload"]}
                    __search   {[list "Search"   1 {$help::index!=""} "bsearch"]}
                    __stop     {[list "Stop"     1 {$help::showing || $help::insearch} "bstop"]}
                    __up1level {[list "UpLevel"  {$help::h4contents!=""} {[array names help::up1level $help::curtopic]!=""} "bup"]}
 "
 variable _istitle      0
 variable _isscript     0
 variable _istext       1
 variable _t_title
 variable _dnum         1
 variable _focus_w      ""
 variable _need_tag
#sch
 variable up1level 
 variable sequence      ""
 variable hcontents     ""
 variable h2ignore      ""
 variable h4topics      ""
 variable h4contents    ""
#/sch
 variable h_appendlist  1
 array set settings     "back {$backcolor} fore {$forecolor} link {$linkcolor} alink {$alinkcolor}"
 variable fnd_str       ""
 variable fnd_len
 variable fnd_case      "-nocase"
 variable fnd_regexp    ""
 variable fnd_direction "-forwards"
 variable searchedtext  ""
 variable insearch      0
 variable controls
 variable buttons
 variable interp
 variable tables
 array set tables       "level 0 stack {} count 0 tables {}"
 variable ext_loaded    ""
}

#===============================================================================

#-------------------------------------------------------------------------------
# Functions      :  help::tag_*
# Description    :  Process corresponding open/close tag
# Parameters     :  params      - arguments of tag
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::tag_\$ {params} {
 variable data
 upvar _text _text

 foreach {param value} $params {
  if {[info exists data(subst,$param,begin)]} {
   set _text "$data(subst,$param,begin)$_text"
  }
  break
 }
}

proc help::tag_/\$ {params} {
 variable data
 upvar _text _text

 foreach {param value} $params {
  if {[info exists data(subst,$param,end)]} {
   set _text "$data(subst,$param,end)$_text"
  }
  break
 }
}

proc help::tag_center {params} {
 variable w
 variable attr

 set attr(justify,global) [linsert $attr(justify,global) 0 "center"]
 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
}

proc help::tag_/center {params} {
 variable w
 variable attr

 catch {set attr(justify,global) [lreplace $attr(justify,global) 0 0]}
 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
}

proc help::tag_b {params} {
 variable attr

 set attr(style) [linsert $attr(style) 0 "b"]
}

proc help::tag_/b {params} {
 variable attr

 if {[set pos [lsearch $attr(style) "b"]]!=-1} {
  set attr(style) [lreplace $attr(style) $pos $pos]
 }
}

proc help::tag_i {params} {
 variable attr

 set attr(style) [linsert $attr(style) 0 "i"]
}

proc help::tag_/i {params} {
 variable attr

 if {[set pos [lsearch $attr(style) "i"]]!=-1} {
  set attr(style) [lreplace $attr(style) $pos $pos]
 }
}

proc help::tag_u {params} {
 variable attr

 set attr(style) [linsert $attr(style) 0 "u"]
}

proc help::tag_/u {params} {
 variable attr

 if {[set pos [lsearch $attr(style) "u"]]!=-1} {
  set attr(style) [lreplace $attr(style) $pos $pos]
 }
}

proc help::tag_pre {params} {
 variable attr

 set attr(style) [linsert $attr(style) 0 "p"]
}

proc help::tag_/pre {params} {
 variable attr

 if {[set pos [lsearch $attr(style) "p"]]!=-1} {
  set attr(style) [lreplace $attr(style) $pos $pos]
 }
}

proc help::tag_nobr {params} {
 variable attr

 set attr(style) [linsert $attr(style) 0 "n"]
}

proc help::tag_/nobr {params} {
 variable attr

 if {[set pos [lsearch $attr(style) "n"]]!=-1} {
  set attr(style) [lreplace $attr(style) $pos $pos]
 }
}

proc help::tag_hr {params} {
 variable attr
 variable w
 variable _text

 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_height ""
 set p_color ""
 foreach {param value} $params {
  set p_$param $value
 }
 eval "$w.text insert end [gethr $p_height $p_color]"
 if {[string index $_text 0]=="\n"} {
  set _text [string range $_text 1 end]
 }
}

proc help::tag_br {params} {
 variable w
 $w.text insert end "\n" [gettags]
}

proc help::tag_ul {params} {
 variable w
 variable attr

 set attr(justify) ""
 set p_type "disc"
 set p_indent 48
 foreach {param value} $params {
  set p_$param $value
 }
 incr attr(margin) $p_indent
 set attr(ul_type) [linsert $attr(ul_type) 0 $p_type]
 set attr(indent) [linsert $attr(indent) 0 $p_indent]
# if {[$w.text get "end -3c"]!="\n"} {
#  $w.text insert end "\n" [gettags]
# }
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
}

proc help::tag_/ul {params} {
 variable w
 variable attr
 variable lmargin
 variable tables

 catch {incr attr(margin) [expr {-[lindex $attr(indent) 0]}]}
 if {$attr(margin)<$lmargin && $tables(stack)==""} {set attr(margin) $lmargin}
 if {[$w.text get "end-3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 catch {set attr(indent) [lreplace $attr(indent) 0 0]}
 catch {set attr(ul_type) [lreplace $attr(ul_type) 0 0]}
}

proc help::tag_li {params} {
 variable w
 variable attr
 variable data
 variable _text
 variable _count
 variable _images
 variable rmargin

 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_type [lindex $attr(ul_type) 0]
 foreach {param value} $params {
  set p_$param $value
 }
 if {![info exists _images($p_type)]} {
  if {[info exists data($p_type,image)]} {
   set im $data($p_type,image)
  } else {
   set im $_images(disc)
  }
 } else {
  set im $_images($p_type)
 }
 catch {set imind [$w.text image create end -image $im -align center -padx 3]}
 catch {$w.text tag add [gettags] $imind}
 catch {$w.text tag configure tag[incr _count] -lmargin1 [expr $attr(margin) - 22] -rmargin $rmargin}
 catch {$w.text tag add tag$_count $imind}
 set _text [string trimleft $_text " \t\n"]
}

proc help::tag_p {params} {
 variable w
 variable attr

 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_align ""
 foreach {param value} $params {
  set p_$param $value
 }
 
 if {[set p_align [string tolower $p_align]]!="left" && $p_align!="center" && $p_align!="right"} {set p_align ""}
 set attr(justify) $p_align
}

proc help::tag_/p {params} {
 variable attr

 set attr(justify) ""
}

proc help::tag_a {params} {
 variable w
 variable attr
 upvar mark mark

 set p_name ""
 set p_href ""
 set p_title ""
 foreach {param value} $params {
  set p_$param $value
 }
#sch p_href bearbeiten
# if p_href is a external reference (http: file: mailto: usw.), then ignore
 if {[string first ":" $p_href]>=0} {
  switch -glob -- [string tolower $p_href] {
   file://* {set p_href [string range $p_href 7 end]}
   tcl:* {set p_href "tcl:[string range $p_href 4 end]"}
   tclscript:* {set p_href "tclscript:[string range $p_href 10 end]"}
   default {set p_title $p_href
            set p_href "tcl:help::showurl {$p_href}"
           }
  }
 }
 if {$p_href!="" && ![regexp "^tcl(script)?:" $p_href]} {
  set p_href_neu [string tolower $p_href]
  variable curtopic
  set filename ""
  if {[set pos [string first "?" $curtopic]]!=-1} {
   set filename [string tolower [string range $curtopic 0 [expr $pos - 1]]]
   if {[string first "#" $p_href_neu]==0} {
    set p_href_neu $filename?[string range $p_href_neu 1 end]
   } elseif {[set pos [string first "#" $p_href_neu]]>0} {
    set p_href_neu [string replace $p_href_neu $pos $pos "?"]
   } 
  }
  variable index
  if {[lsearch $index $p_href_neu]>=0} {
   set p_href $p_href_neu
  } else {
   if {[string first "#" $p_href]==0} {
    set p_href $curtopic$p_href
   } else {
   }
  }
 }
#/sch
 set attr(links) [linsert $attr(links) 0 $p_href]

 if {$p_name!=""} {
  set p_name [string tolower $p_name]
  $w.text mark set "mark_$p_name" [$w.text index end-1c]
  $w.text mark gravity "mark_$p_name" left
  if {$p_name==$mark} {
   after 200 "catch {$w.text yview \"mark_$p_name\"}; catch {$w.text see \"mark_$p_name\"}"
  }
 }

 set attr(link_titles) [linsert $attr(link_titles) 0 $p_title]
}

proc help::tag_/a {params} {
 variable attr

 catch {set attr(links) [lreplace $attr(links) 0 0]}
 catch {set attr(link_titles) [lreplace $attr(link_titles) 0 0]}
}

proc help::tag_font {params} {
 variable attr
 variable fontsize

 set tag_font ""
 set p_color ""
 set p_bgcolor ""
 set p_size ""
 set p_face ""
 set {p_point-size} ""
 foreach {param value} $params {
  set p_$param $value
 }
 if {$p_color!=""} {
  set attr(colors) [linsert $attr(colors) 0 [correctcolor [string tolower $p_color]]]
  lappend tag_font "colors"
 }
 if {$p_bgcolor!=""} {
  set attr(bgcolors) [linsert $attr(bgcolors) 0 [correctcolor [string tolower $p_bgcolor]]]
  lappend tag_font "bgcolors"
 }
 if {$p_size!=""} {
  if {[set sign [string index $p_size 0]]=="+" || $sign=="-" || ($p_size=="0" && [set sign "+"]=="+")} {
   set attr(sizes) [linsert $attr(sizes) 0 "$sign[expr {abs($p_size) * 2}]"]
  } else {
   set attr(sizes) [linsert $attr(sizes) 0 [expr {$p_size * 2 + $fontsize - 6}]]
  }
  lappend tag_font "sizes"
 }
 if {${p_point-size}!=""} {
  set attr(sizes) [linsert $attr(sizes) 0 ${p_point-size}]
  lappend tag_font "sizes"
 }
 if {$p_face!=""} {
  set attr(face) [linsert $attr(face) 0 $p_face]
  lappend tag_font "face"
 }
 set attr(tag_fonts) [linsert $attr(tag_fonts) 0 $tag_font]
}

proc help::tag_/font {params} {
 variable attr

 foreach t [lindex $attr(tag_fonts) 0] {
  catch {set attr($t) [lreplace $attr($t) 0 0]}
 }
 catch {set attr(tag_fonts) [lreplace $attr(tag_fonts) 0 0]}
}

proc help::tag_img {params} {
 variable w
 variable attr
 variable data
 variable curfilename
 variable _images

 set p_src ""
 set p_align ""
 set p_border ""
 set p_id ""
 foreach {param value} $params {
  set p_$param $value
 }
 if {[info exists data($p_src,image)]} {
  catch {set imind [$w.text image create end -image $data($p_src,image)]}
 } else {
#sch i think you mean [catch {...}] == 0
#angra No, i mean that i mean: if i can't load image, the default image is drawing
 if {[catch {set data($p_src,image) [image create photo -file [file join [file dirname $curfilename] $p_src]]}]} {
#/sch
   catch {set imind [$w.text image create end -image $_images(broken)]}
 } else {
   catch {set imind [$w.text image create end -image $data($p_src,image)]}
  }
 }
 if {[set p_align [string tolower $p_align]]=="middle"} {
  set p_align "center"
 }
 catch {$w.text image configure $imind -align [string tolower $p_align]}
 catch {$w.text image configure $imind -padx $p_border -pady $p_border}
 catch {$w.text tag add [gettags] $imind}
 if {$p_id!=""} {
  set data($p_id,image,id) $imind
  set data($p_id,image,widget) $w
 }
}

proc help::tag_tt {params} {
 variable attr

 set attr(style) [linsert $attr(style) 0 "t"]
}

proc help::tag_/tt {params} {
 variable attr

 if {[set pos [lsearch $attr(style) "t"]]!=-1} {
  set attr(style) [lreplace $attr(style) $pos $pos]
 }
}

proc help::tag_ol {params} {
 tag_ul $params
}

proc help::tag_/ol {params} {
 tag_/ul $params
}

proc help::tag_em {params} {
 tag_i $params
}

proc help::tag_/em {params} {
 tag_/i $params
}

proc help::tag_strong {params} {
 tag_b $params
}

proc help::tag_/strong {params} {
 tag_/b $params
}

proc help::tag_code {params} {
 tag_tt {}
 tag_font {size +0}
}

proc help::tag_/code {params} {
 tag_/font {}
 tag_/tt {}
}

proc help::tag_samp {params} {
 tag_code {}
}

proc help::tag_/samp {params} {
 tag_/code {}
}

proc help::tag_kbd {params} {
 variable attr

 set attr(style) [linsert $attr(style) 0 "k"]
}

proc help::tag_/kbd {params} {
 variable attr

 if {[set pos [lsearch $attr(style) "k"]]!=-1} {
  set attr(style) [lreplace $attr(style) $pos $pos]
 }
}

proc help::tag_cite {params} {
 tag_i $params
}

proc help::tag_/cite {params} {
 tag_/i $params
}

proc help::tag_var {params} {
 tag_i $params
}

proc help::tag_/var {params} {
 tag_/i $params
}

proc help::tag_address {params} {
 tag_i $params
}

proc help::tag_/address {params} {
 tag_/i $params
}

proc help::tag_h1 {params} {
 variable w
 variable attr
 variable fontsize

 set attr(sizes) [linsert $attr(sizes) 0 [expr {$fontsize + 10}]]    ;#22
 set attr(style) [linsert $attr(style) 0 "b"]
#sch
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_align ""
 foreach {param value} $params {
  set p_$param $value
 }
 
 if {[set p_align [string tolower $p_align]]!="left" && $p_align!="center" && $p_align!="right"} {set p_align ""}
 set attr(justify) $p_align
#/sch
}

proc help::tag_/h1 {params} {
 variable w
 variable attr

 if {[set pos [lsearch $attr(style) "b"]]!=-1} {
  set attr(style) [lreplace $attr(style) $pos $pos]
 }
 catch {set attr(sizes) [lreplace $attr(sizes) 0 0]}
 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
#sch
 set attr(justify) ""
#/sch
}

proc help::tag_h2 {params} {
 variable w
 variable attr
 variable fontsize

 set attr(sizes) [linsert $attr(sizes) 0 [expr {$fontsize + 6}]]  ;#18
 set attr(style) [linsert $attr(style) 0 "b"]
#sch
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_align ""
 foreach {param value} $params {
  set p_$param $value
 }
 
 if {[set p_align [string tolower $p_align]]!="left" && $p_align!="center" && $p_align!="right"} {set p_align ""}
 set attr(justify) $p_align
#/sch
}

proc help::tag_/h2 {params} {
 tag_/h1 $params
}

proc help::tag_h3 {params} {
 variable w
 variable attr
 variable fontsize

 set attr(sizes) [linsert $attr(sizes) 0 [expr {$fontsize + 2}]]  ;#14
 set attr(style) [linsert $attr(style) 0 "b"]
#sch
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_align ""
 foreach {param value} $params {
  set p_$param $value
 }
 
 if {[set p_align [string tolower $p_align]]!="left" && $p_align!="center" && $p_align!="right"} {set p_align ""}
 set attr(justify) $p_align
#/sch
}

proc help::tag_/h3 {params} {
 tag_/h1 $params
}

proc help::tag_h4 {params} {
 variable w
 variable attr
 variable fontsize

 set attr(sizes) [linsert $attr(sizes) 0 $fontsize]  ;#12
 set attr(style) [linsert $attr(style) 0 "b"]
#sch
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_align ""
 foreach {param value} $params {
  set p_$param $value
 }
 
 if {[set p_align [string tolower $p_align]]!="left" && $p_align!="center" && $p_align!="right"} {set p_align ""}
 set attr(justify) $p_align
#/sch
}

proc help::tag_/h4 {params} {
 tag_/h1 $params
}

proc help::tag_h5 {params} {
 variable w
 variable attr
 variable fontsize

 set attr(sizes) [linsert $attr(sizes) 0 [expr {$fontsize - 2}]]  ;#10
 set attr(style) [linsert $attr(style) 0 "b"]
#sch
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_align ""
 foreach {param value} $params {
  set p_$param $value
 }
 
 if {[set p_align [string tolower $p_align]]!="left" && $p_align!="center" && $p_align!="right"} {set p_align ""}
 set attr(justify) $p_align
#/sch
}

proc help::tag_/h5 {params} {
 tag_/h1 $params
}

proc help::tag_h6 {params} {
 variable w
 variable attr
 variable fontsize

 set attr(sizes) [linsert $attr(sizes) 0 [expr {$fontsize - 4}]]  ;#8
 set attr(style) [linsert $attr(style) 0 "b"]
#sch
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {[$w.text get "end -3c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 set p_align ""
 foreach {param value} $params {
  set p_$param $value
 }
 
 if {[set p_align [string tolower $p_align]]!="left" && $p_align!="center" && $p_align!="right"} {set p_align ""}
 set attr(justify) $p_align
#/sch
}

proc help::tag_/h6 {params} {
 tag_/h1 $params
}

proc help::tag_blockquote {params} {
 tag_ul {}
}

proc help::tag_/blockquote {params} {
 tag_/ul {}
}

proc help::tag_menu {params} {
 tag_ul {}
}

proc help::tag_/menu {params} {
 tag_/ul {}
}

proc help::tag_dir {params} {
 tag_ul {}
}

proc help::tag_/dir {params} {
 tag_/ul {}
}

proc help::tag_div {params} {
 tag_p $params
}

proc help::tag_/div {params} {
 tag_/p $params
}

proc help::tag_body {params} {
 variable w
 variable settings
 variable _t_title

 tag_/head {}
 foreach {v1 v2} {text fore bgcolor back link link alink alink} {
  set p_$v1 $settings($v2)
 }
 foreach {param value} $params {
  set p_$param $value
 }
 foreach {v1 v2} {text fore bgcolor back link link alink alink} {
  set settings($v2) [correctcolor [string tolower [set p_$v1]]]
 }
 catch {$w.text configure -foreground $settings(fore)}
 catch {$w.text configure -background $settings(back)}
 catch {$w.title configure -foreground $settings(fore)}
 catch {$w.title configure -background $settings(back)}
 foreach {name} {title line __index contents __next __prev __Next __Prev __up1level 
                 __dec __find __inc __open __refresh __reload __stop __search} {
  catch {
   if {[$w.title bind $_t_title($name) <1>]==""} {
    catch {$w.title itemconfigure $_t_title($name) -fill $settings(fore)}
   } else {
    catch {$w.title itemconfigure $_t_title($name) -fill $settings(link)}
   }
  }
 }
}

proc help::tag_title {params} {
 variable _istitle 1
}

proc help::tag_/title {params} {
 variable w
 variable _istitle 0
 variable _t_title
 variable data

 if {[info exists data(title)] && $data(title)!=""} {
  regsub -all -- "\[\r\n \]+" $data(title) " " data(title)
 } else {
  set data(title) "Untitled"
 }

 if {[string match {.tophelpwindow*} $w]} {
  wm title .tophelpwindow "Help - [specialentities $data(title)]"
 }
 $w.title itemconfigure $_t_title(title) -text [specialentities $data(title)]
}

proc help::tag_/head {params} {
 variable _istitle

 if {$_istitle} {
  tag_/title {}
 }
}

proc help::tag_/tr {params} {
 variable w

 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
}

proc help::tag_/td {params} {
 variable w

 if {[$w.text get "end -2c"]!=" "} {
  $w.text insert end " " [gettags]
 }
}

proc help::tag_table {params} {
 variable w
 variable attr

 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }

 set p_bgcolor ""
 foreach {param value} $params {
  set p_$param $value
 }
 if {$p_bgcolor!=""} {
  set attr(bgcolors) [linsert $attr(bgcolors) 0 [correctcolor [string tolower $p_bgcolor]]]
  set attr(tables) [linsert $attr(tables) 0 "bg"]
 }
}

proc help::tag_/table {params} {
 variable w
 variable attr

 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert end "\n" [gettags]
 }
 if {[set pos [lsearch $attr(tables) "bg"]]!=-1} {
  set attr(tables) [lreplace $attr(tables) $pos $pos]
  catch {set attr(bgcolors) [lreplace $attr(bgcolors) 0 0]}
 }
}

proc help::tag_dd {params} {
 variable w
 variable _need_tag

 if {[set pos [lsearch $_need_tag "/dt"]]!=-1} {
  tag_/dt {}
 }
 if {[$w.text get "end -2c"]!="\n"} {
  $w.text insert "end -1c" "\n" [gettags]
 }
 set _need_tag [linsert $_need_tag 0 "/dd"]
}

proc help::tag_/dd {params} {
 variable _need_tag

 if {[set pos [lsearch $_need_tag "/dd"]]!=-1} {
  set _need_tag [lreplace $_need_tag $pos $pos]
 }
}

proc help::tag_dl {params} {
 tag_ul {}
}

proc help::tag_/dl {params} {
 tag_/ul {}
}

proc help::tag_dt {params} {
 variable w
 variable _need_tag

 if {[set pos [lsearch $_need_tag "/dt"]]!=0} {
  set _need_tag [linsert $_need_tag 0 "/dt"]
  tag_/ul {}
 } else {
  if {[$w.text get "end -2c"]!="\n"} {
   $w.text insert "end -1c" "\n" [gettags]
  }
 }
}

proc help::tag_/dt {params} {
 variable _need_tag

 if {[set pos [lsearch $_need_tag "/dt"]]!=-1} {
  set _need_tag [lreplace $_need_tag $pos $pos]
 }
 tag_ul {}
}

proc help::tag_frame {params} {
 variable w

 set p_name ""
 set p_src ""
 foreach {param value} $params {
  set p_$param $value
 }
 if {$p_name==""} {
  set p_name $p_src
 }
 if {$p_src!=""} {
  if {[$w.text get "end -2c"]!="\n"} {
   $w.text insert end "\n" [gettags]
  }
  $w.text insert end "Frame " [gettags]
  tag_a [list href $p_src]
  $w.text insert end "$p_name" [gettags]
  tag_/a {}
  tag_br {}
 }
}

proc help::tag_script {params} {
 variable _isscript
 variable _istext
 variable data

 set p_language ""
 foreach {param value} $params {
  set p_$param $value
 }
 if {[string tolower $p_language]=="tclscript"} {
  set _isscript 1
  set data(script) ""
 } else {
  set _istext 0
 }
}

proc help::tag_/script {params} {
 variable data
 variable interp
 variable _isscript
 variable _istext

 if {$_isscript && $data(script)!=""} {
  if {[catch {$interp eval $data(script)}]} {
   set data(__@Error@__,title) "Error in loaded script"
   set data(__@Error@__,text) "<font color=red><pre>[string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} $::errorInfo]</pre></font>"
   set _isscript 0
   show __@Error@__  
  }
 } elseif {!$_istext} {
  set _istext 1
 }
 set _isscript 0
}

proc help::tag_style {params} {
 variable _istext 0
}

proc help::tag_/style {params} {
 variable _istext 1
}

proc help::tag_span {params} {
 variable w
 variable attr
 variable data
 variable _count

 set p_id ""
 set p_hidden 0
 foreach {param value} $params {
  set p_$param $value
 }
 if {$p_id!=""} {
  incr _count
  set data($p_id,names,id) "tag$_count"
  set data($p_id,names,widget) $w
  set data($p_id,names,pos) [$w.text index "end-1c"]
  if {$p_hidden==""} {set p_hidden 1}
  catch {$w.text tag configure "tag$_count" -elide $p_hidden}
 }
 set attr(span) [linsert $attr(span) 0 $p_id]
}

proc help::tag_/span {params} {
 variable attr
 variable data

 if {[set p_id [lindex $attr(span) 0]]!=""} {
  catch {$data($p_id,names,widget).text tag add $data($p_id,names,id) $data($p_id,names,pos) "end-1c"}
 }
 set attr(span) [lreplace $attr(span) 0 0]
}

#===============================================================================

#-------------------------------------------------------------------------------
# Function       :  help::correctcolor
# Description    :  Converts color names to color values
# Parameters     :  color       - name of color
# Return         :  Color value
#-------------------------------------------------------------------------------
proc help::correctcolor {color} {
 return [string map {\
   green   "#008000" silver "#c0c0c0" lime   "#00ff00" gray "#808080" \
   olive   "#808000" navy   "#000080" purple "#800080" teal "#008080" \
   fuchsia "#ff00ff" aqua   "#00ffff" maroon "#800000"
 } $color]
}

#-------------------------------------------------------------------------------
# Function       :  help::_load
# Description    :  Load helpfile with includes
# Parameters     :  filename    - name of file with help
# Return         :  True or false
#-------------------------------------------------------------------------------
proc help::_load {filename} {
 variable stat
 variable data
 variable index 
 variable font 
 variable fontfixed 
 variable fontsize 
 variable backcolor 
 variable forecolor 
 variable linkcolor 
 variable alinkcolor
 variable lmargin
 variable rmargin
 variable curfilename
#sch
 variable hcontents
 variable h2ignore 
 variable h4topics
 variable h4contents
 set schAnkerNr 0
 set headerstr ""
 set headerlevel ""
 set headertxt ""
 set topicname ""
 set topictitle ""
#/sch
 variable h_appendlist
 variable searchedtext ""
 variable controls
 variable buttons

 set stat "Loading $filename..."
 update
 if {[catch {set fname [::open $filename r]}]!=0} {
  append data(__file_error,text)  "<p><font color=#c00000>Error while opening file:</font> <b>\"$filename\"</b><font color=#c00000>!</font></p>"
  set data(__file_error,title) "Error!"
  if {![info exists index] || [lsearch $index "__file_error"]==-1} {
   lappend index __file_error
  }
  return 0
 }
 set curtopic ""
 set filedata ""
 while {![eof $fname]} {
  append filedata [read $fname 100000]
 }
 ::close $fname
 if {$h4topics!="" || $h4contents!=""} {
  regsub -all -- {<!--.*?-->} $filedata "" filedata
 }
 set filedata [split $filedata "\n"]
 foreach str $filedata {
#sch test section headings and create topics, fill hcontents
  append headerstr $str
  if {![regexp -nocase {<h[1-6][^>]*>} $headerstr]} {
   set headerstr ""
  } elseif {[regexp -nocase \
  {<h([1-6])[^>]*>(.*?)</h[1-6]>} $headerstr Dummy headerlevel headertxt]} {
 
   if {$h2ignore!="" && [regexp -nocase $h2ignore $headerstr]} {
    set str $headerstr
    set headerstr ""
#    continue
   } else {
    if {$h4topics!="" && [regexp -nocase -- $h4topics $headerstr]} {
     regsub -all -nocase -- {<br>} $headertxt { } headertxt
     regsub -all -- {<[^>]*>} $headertxt {} topictitle
     if {![regexp -nocase {^.*<a +.*? *name *= *(" *([^\"]+) *"|([^\"]([^ >]*)))} \
     $headertxt Dummy topicname]} {
      set topicname internal_[incr schAnkerNr]
     }
     set curtopic [string tolower [file tail $filename]?[string trim $topicname "\t\" "]]
     lappend index $curtopic
     set data($curtopic,text) ""
     set data($curtopic,title) [string trim [specialentities $topictitle] "\t\" "]
    } else {
     set topicname ""
    }
  
    if {$h4contents!="" && [regexp -nocase -- $h4contents $headerstr]} {
     regsub -all -nocase -- {<br>} $headertxt { } headertxt
     regsub -all -- {<[^>]*>} $headertxt {} topictitle
     if {$topicname!=""} {
      lappend hcontents [list $headertxt $headerlevel [string trim $topictitle] $curtopic]
     } else {
      lappend hcontents [list $headertxt $headerlevel [string trim $topictitle] ""]
     }
    }
    set str $headerstr 
    set headerstr ""
   }
  } else {
   continue
  }
#/sch

#sch new control directives .h2ignore* .h4topics* .h4contents* added in switch
#/sch
  switch -glob -- $str {
   .h_ignore*   {set h2ignore [string trim [string range $str 9 end] "\t\" "]}
   .h_topics*   {set h4topics [string trim [string range $str 9 end] "\t\" "]}
   .h_contents* {set h4contents [string trim [string range $str 11 end] "\t\" "]}
   .h_appendlist* {set h_appendlist [regexp -- "(yes)|(1)|(true)" [string tolower [string trim [string range $str 11 end] "\t\" "]]]}
   .fontsize*   {catch {set fontsize [expr [string trim [string range $str 9 end] "\t\" "]]}}
   .fontfixed*  {set fontfixed [string trim [string range $str 10 end] "\t\" "]}
   .font*       {set font [string trim [string range $str 5 end] "\t\" "]}
   .forecolor*  {set forecolor [correctcolor [string tolower [string trim [string range $str 10 end] "\t\" "]]]}
   .backcolor*  {set backcolor [correctcolor [string tolower [string trim [string range $str 10 end] "\t\" "]]]}
   .linkcolor*  {set linkcolor [correctcolor [string tolower [string trim [string range $str 10 end] "\t\" "]]]}
   .alinkcolor* {set alinkcolor [correctcolor [string tolower [string trim [string range $str 11 end] "\t\" "]]]}
   .lmargin*    {catch {set lmargin [expr [string trim [string range $str 8 end] "\t\" "]]}}
   .rmargin*    {catch {set rmargin [expr [string trim [string range $str 8 end] "\t\" "]]}}
   .topic*      {set curtopic [string trim [string range $str 6 end] "\t\" "]
                 if {$curtopic!=""} {
                  lappend index $curtopic
                  set data($curtopic,title) "$curtopic"
                  set data($curtopic,text) ""
                 }
                }
   .title*      {if {$curtopic!=""} {
                  set data($curtopic,title) [string trim [string range $str 6 end] "\t\" "]
                 }
                }
   .image*      {set str [split $str]
                 set imname ""
                 set imtype ""
                 set imdata ""
                 set i 1
                 for {} {$i<[llength $str]} {incr i} {
                  if {[lindex $str $i]!=""} {
                   set imname [string trim [lindex $str $i]]
                   incr i
                   break
                  }
                 }
                 for {} {$i<[llength $str]} {incr i} {
                  if {[lindex $str $i]!=""} {
                   set imtype [string trim [lindex $str $i]]
                   incr i
                   break
                  }
                 }
                 set imdata [string trim [join [lrange $str $i end]]]
                 switch -- $imtype {
                  file {catch {set data($imname,image) [image create photo -file [file join [file dirname $filename] $imdata]]}}
                  data {catch {set data($imname,image) [image create photo -data $imdata]}}
                 }
                }
   .include*    {variable inclstr
                 set inclstr [string trim [join [lrange [split $str] 1 end]] "\t\" "]
                 catch "namespace eval :: {set [namespace current]::inclstr \"$inclstr\"}"
                 _load [file join [file dirname $filename] $inclstr]
                }
   .$*          {set str [split $str]
                 set substname ""
                 set substbegin ""
                 set substend ""
                 set i 1
                 for {} {$i<[llength $str]} {incr i} {
                  if {[lindex $str $i]!=""} {
                   set substname [string tolower [string trim [lindex $str $i]]]
                   incr i
                   break
                  }
                 }
                 set subst [string trim [join [lrange $str $i end]]]
                 catch {set substbegin [lindex $subst 0]}
                 catch {set substend   [lindex $subst 1]}
                 if {$substname!="" && ($substbegin!="" || $substend!="")} {
                  set data(subst,$substname,begin) $substbegin
                  set data(subst,$substname,end)   $substend
                 }
                }
    .controls*  {
                 set controls [string tolower [string trim [string range $str 9 end] "\t\" "]]
                } 
    .buttons*   {
                 set buttons [string tolower [string trim [string range $str 8 end] "\t\" "]]
                } 
    default     {if {$curtopic!=""} {
                  if {[string index $str 0]!={;}} {
                   append data($curtopic,text) "$str\n"
                  }
                 } else {
                  append data(text) "$str\n"
                 }
                }
  }
 }
 return 1
}


#-------------------------------------------------------------------------------
# Function       :  help::load
# Description    :  Load helpfile and make index
# Parameters     :  filename    - name of file with help
# Return         :  True or false
#-------------------------------------------------------------------------------
proc help::load {filename} {
 variable w
 variable stat
 variable data
 variable curfilename

 if {$filename==$curfilename} {
  return 1
 }

 variable settings
 variable history 
 variable index 
 variable hpos

 if {$index!=""} {
  set index ""
  set history ""
  set hpos 0
 }

 variable font "Helvetica"
 variable fontfixed "Courier"
 #CHANGED BY FRANCOIS VOGEL, 30/03/05 - Fixes Scilab bug 902
 #variable fontsize 12
 variable fontsize
 variable backcolor ""
 variable forecolor black
 variable linkcolor "#006800"
 variable alinkcolor "#c00000"
 variable lmargin 6
 variable rmargin 6
 variable curtopic ""
 variable attr
#sch declare vars
 variable up1level 
 variable sequence ""
 variable hcontents ""
 variable h2ignore ""
 variable h4topics ""
 variable h4contents ""
#/sch
 variable h_appendlist 1
 # CHANGED BY JAIME URZUA
 #variable controls "back forw cont index search prev up next"
 variable controls ""
 # CHANGED BY JAIME URZUA
 #variable buttons "back forw prev up next sep stop sep cont ind"
 variable buttons "back forw prev up next sep stop sep"
 array set settings "back {$backcolor} fore {$forecolor} link {$linkcolor} alink {$alinkcolor}"

 foreach imn [array names data *,image] {
  catch {image delete $data($imn)}
 }
 catch {unset data}
 foreach btn "back cont dec find forward inc index next open prev refresh reload search stop up" {
  catch {grid forget $w.b$btn}
 }
 for {set i 0} {$i<50} {incr i} {grid columnconfig $w $i -minsize 0}
 if {![_load $filename]} {
  set curfilename ""
  set stat "Error while loading $filename!"
  buttons
  return 0
 }
 buttons
 set stat "Done."
 update

#sch create Topic Contents from hcontents and up1level for __up1level
 if {$hcontents!=""} {
  set data(contents,title) "Contents"
  set data(contents,text) "<ul>"
  set links(contents) ""
  # set topicname_prev ""
  set level(0) [list Contents 0 Contents contents]
  for {set i 1} {$i<=6} {incr i} {  
   set level($i) ""
  }
  foreach str $hcontents {
   #set headertxt [lindex $str 0]
   set headerlevel  [lindex $str 1]
   set topictitle  [lindex $str 2]
   set topicname  [lindex $str 3]
   set prefix [string repeat "&nbsp;&nbsp;&nbsp;&nbsp;" [expr $headerlevel - 1]]
   set level($headerlevel) $str
   for {set i [expr $headerlevel + 1]} {$i<=6} {incr i} {   
    set level($i) ""
   }
   set links($topicname) ""
   if {$topicname!=""} {
    append data(contents,text) "$prefix<a href=\"$topicname\">$topictitle</a><br>"
   } else {
    append data(contents,text) "$prefix$topictitle<br>"
   }
   for {set i [expr $headerlevel - 1]} {$i>0&&$level($i)==""} {incr i -1} { 
   }
   set str_i $level($i)
   if {$str_i!=""} {
    #set headertxt_i [lindex $str_i 0]
    set headerlevel_i  [lindex $str_i 1]
    set topictitle_i  [lindex $str_i 2]
    set topicname_i [lindex $str_i 3]
    if {$topicname_i != ""} {
     set up1level($topicname) $topicname_i
     if {$topicname!=""} {
      append links($topicname_i) "<a href=\"$topicname\">$topictitle</a><br>"
     }
    }
   }
  }
  append data(contents,text) "</ul>"
  lappend index contents
  if {$h_appendlist} {
   set links(contents) ""
   foreach topicname_i [array names links] {
    if {$links($topicname_i)!=""} {
     append data($topicname_i,text) "<ul>" $links($topicname_i) "</ul>"
    }
   }
  }
 }
#/sch

#sch_05 create sequence for __Next, __up1level and __Prev
 if {[lsearch $index contents]>=0} {
  lappend sequence contents
 }
 foreach topicname $index {
  if {$topicname!="contents"} {
   lappend sequence $topicname
  }
 }
 lappend sequence __index
#/sch

 set data(title) ""
 if {$index!=""} {
  set iind ""
  foreach i $index {
   lappend iind [list $i $data($i,title)]
  }
  set iind [lsort -index 1 -dictionary $iind]
  set data(__index,title) "Index"
  set data(__index,text) "<br><ul>"
  foreach i $iind {
   set outtext [lindex $i 1]
   append data(__index,text) "<li><a href=\"[lindex $i 0]\">[string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} $outtext]</a> <br>"
  }
  append data(__index,text) "</ul>"
  lappend index __index
 }
 catch {$w.text configure -font "-family $font -size $fontsize"}
 catch {$w.text configure -foreground $forecolor}
 catch {$w.text configure -background $backcolor}
 catch {$w.title configure -background [$w.text cget -background]}
 set curfilename $filename
 array set settings "back {$backcolor} fore {$forecolor} link {$linkcolor} alink {$alinkcolor}"
 return 1
}

#-------------------------------------------------------------------------------
# Function       :  help::init
# Description    :  Create window, load file and show topic
# Parameters     :  filename    - name of file with help
#                :  topic       - topic name
#                :  parent      - parent widget
#                :  width       - initial width of help window
#                :  height      - initial height of help window
# Return         :  Nothing
#-------------------------------------------------------------------------------
#sch new parameters  {width 450} {height 400}
proc help::init {filename {topic contents} {parent {}} {width 450} {height 400} } {
#/sch
 variable w
 variable gw
 variable settings
 variable font
 variable fontsize
 variable history
 variable _images
 variable _dnum
 variable btninfo
 global   tcl_platform

 if {![info exists w] || ![winfo exists $w]} {
  set w $parent
  if {$parent==""} {
   set w ".tophelpwindow"
#sch 
   if {$width<450} {set width 450}
   if {$height<400} {set height 400}
   toplevel $w -width $width -height $height
#/sch
   wm title .tophelpwindow "Help"
   wm minsize .tophelpwindow 170 120
   raise .tophelpwindow
   focus -force .tophelpwindow
  }
  
# F.Leray modif. here 09.03.05
# to center the window
  set height [expr [winfo screenheight .]/2]
  set width  [expr [winfo screenwidth . ]/2]
  set ypos   [expr [winfo screenheight .]/4]
  set xpos   [expr [winfo screenwidth . ]/4]
  wm geometry [winfo toplevel $w] "${width}x${height}+${xpos}+${ypos}"
     
  if {[wm protocol [winfo toplevel $w] WM_DELETE_WINDOW]==""} {
   wm protocol [winfo toplevel $w] WM_DELETE_WINDOW "help::destroy 1"
  }

  if {$w!="."} {append w "."}
#sch
  frame ${w}f -width $width -height $height -bd 2 -relief sunken
#/sch1
  pack ${w}f -fill both -expand 1
  append w "f"
  set gw $w

  canvas $w.title -bd 0 -highlightthickness 0 -cursor arrow -height 0
  # CHANGED BY JAIME URZUA
  # ---------------->    <----
  text $w.text -wrap word -cursor arrow -state disabled -bd 0 \
               -insertwidth 0 -insertontime 0 -exportselection 1\
               -highlightthickness 0 -takefocus 1 -insertborderwidth 0 \
               -yscrollcommand "$w.vs set" -xscrollcommand "$w.hs set"
  foreach elem {text title} {
   catch {$w.$elem configure -font "-family $font"}
   catch {$w.$elem configure -font "-size $fontsize"}
   catch {$w.$elem configure -foreground $settings(fore)}
   catch {$w.$elem configure -background $settings(back)}
  }
  scrollbar $w.vs -command "$w.text yview"
  scrollbar $w.hs -command "$w.text xview" -orient horizontal

  button $w.bback    -image $_images(back)    -width 15 -height 13 -command "help::show __prev" -state disabled
  button $w.bforward -image $_images(forward) -width 15 -height 13 -command "help::show __next" -state disabled
  button $w.bstop    -image $_images(stop)    -width 15 -height 13 -command "set help::abort 1" -state disabled
  button $w.brefresh -image $_images(refresh) -width 15 -height 13 -command "event generate $w.text <Control-r>"

  button $w.bdec     -image $_images(dec)    -width 15 -height 13 -command "event generate $w.text <Control-bracketleft>"
  button $w.binc     -image $_images(inc)    -width 15 -height 13 -command "event generate $w.text <Control-bracketright>"
  button $w.bcont    -image $_images(cont)   -width 15 -height 13 -command "help::show contents"
  button $w.bindex   -image $_images(index)  -width 15 -height 13 -command "help::show __index"
  button $w.bfind    -image $_images(find)   -width 15 -height 13 -command "help::dlgfind:init"
  button $w.bsearch  -image $_images(search) -width 15 -height 13 -command "help::dlgfind:init 0"
  button $w.breload  -image $_images(reload) -width 15 -height 13 -command "event generate $w.text <Control-l>"
  button $w.bopen    -image $_images(open)   -width 15 -height 13 -command "event generate $w.text <Control-o>"
  button $w.bprev    -image $_images(prev)   -width 15 -height 13 -command "help::show __Prev"
  button $w.bup      -image $_images(up)     -width 15 -height 13 -command "help::show __up1level"
  button $w.bnext    -image $_images(next)   -width 15 -height 13 -command "help::show __Next"

  foreach {name} {__index contents __next __prev __Next __Prev __up1level
                 __dec __find __inc __open __refresh __reload __stop __search} {
   foreach {txt vis expr btn} $btninfo($name) {
    bind $w.$btn <Enter> "set help::stat \{$txt\}"
    bind $w.$btn <Leave> "set help::stat \{Done.\}"
   }
  }
  
  label $w.stat -relief sunken -bd 1 -textvariable help::stat -justify left -anchor w
  label $w.d -image $_images(d[set _dnum]) -bd 0 -anchor se
  label $w.ugol -image $_images(ugol) -bd 0 -anchor se

  grid rowconfig    $w 0 -weight 1    -minsize 0
  grid rowconfig    $w 1 -weight 1000 -minsize 50
#  grid rowconfig    $w 3 -minsize 20
  grid columnconfig $w 50 -weight 1000 -minsize 50

  grid $w.title -padx 0 -pady 0 \
               -row 0 -column 0 -rowspan 1 -columnspan 52 -sticky new
  grid $w.text -padx 0 -pady 0 \
               -row 1 -column 0 -rowspan 1 -columnspan 51 -sticky news
  grid $w.vs   -padx 1 -pady 0 \
               -row 1 -column 51 -rowspan 1 -columnspan 1 -sticky news
  grid $w.hs   -padx 0 -pady 1 \
               -row 2 -column 0 -rowspan 1 -columnspan 51 -sticky news
  grid $w.d    -padx 0 -pady 0 \
               -row 2 -column 51 -rowspan 1 -columnspan 1 -sticky news
  grid $w.stat -padx 0 -pady 0 \
               -row 3 -column 50 -rowspan 1 -columnspan 1 -sticky nwes
  grid $w.ugol -padx 0 -pady 0 \
               -row 3 -column 51 -rowspan 1 -columnspan 1 -sticky es

  grid propagate $w 0

  focus $w.text
  $w.text tag configure h -font "-size 1" -spacing3 0

  bind $w.text <Key> "break"
  bind $w.text <KeyPress> "break"
  bind $w.text <KeyRelease> "break"
  bind $w.text <BackSpace> "help::show __prev;break"
  bind $w.text <Alt-F5> "help::show __prev;break"
  bind $w.text <Alt-Left> "help::show __prev;break"
  bind $w.text <Alt-Right> "help::show __next;break"
  bind $w.text <Double-1> "break"
  bind $w.text <1> "focus $w.text; $w.text tag remove fnd 1.0 end"
  bind $w.text <Escape> "set help::abort 1"
  bind $w.text <Control-R> "event generate $w.text <Control-r>"
  bind $w.text <Control-L> "event generate $w.text <Control-l>"
  bind $w.text <Control-bracketleft>  "if {\$help::fontsize>6} {incr help::fontsize -2; event generate $w.text <Control-r>}"
  bind $w.text <Control-bracketright> "if {\$help::fontsize<30} {incr help::fontsize 2; event generate $w.text <Control-r>}"
  bind $w.text <Control-c> "event generate $w.text <<Copy>>"
  bind $w.text <Control-C> "event generate $w.text <Control-c>"
  bind $w.text <Control-Insert> "event generate $w.text <Control-c>"
  bind $w.text <Control-o> "help::open"
  bind $w.text <Control-O> "event generate $w.text <Control-o>"
  bind $w.text <Right> "$w.text xview scroll +1 units; break"
  bind $w.text <Left>  "$w.text xview scroll -1 units; break"
  bind $w.text <Down>  "$w.text yview scroll +1 units; break"
  bind $w.text <Up>    "$w.text yview scroll -1 units; break"
  bind $w.text <Next>  "$w.text yview scroll +1 pages; break"
  bind $w.text <Prior> "$w.text yview scroll -1 pages; break"
  bind $w.text <Home>  "$w.text yview moveto 0; break"
  bind $w.text <End>   "$w.text yview moveto 1; break"
  bind $w.d    <Double-1> "help::show_about"
  bind $w.text <Control-f> "help::dlgfind:init; break"
  bind $w.text <Control-F> "event generate $w.text <Control-f>"
  bind $w.text <Control-s> "help::dlgfind:init 0; break"
  bind $w.text <Control-S> "event generate $w.text <Control-s>"
  bind $w.text <Control-Shift-f> "event generate $w.text <Control-s>"
  bind $w.text <Control-Shift-F> "event generate $w.text <Control-s>"
  if {$parent==""} {
   bind .tophelpwindow <FocusIn>  "help::focusin %W %m"
   bind .tophelpwindow <FocusOut> "help::focusout %W %m"
   bind .tophelpwindow <Configure> "help::focusin %W %m"
   bind .tophelpwindow <Destroy> "help::focusout %W %m"
   bind .tophelpwindow <Expose> "help::focusin %W %m"
  }

  menu $w.menu  -tearoff 0
  $w.menu add command -label "Back" -command "help::show __prev" -accelerator "<--"
  $w.menu add command -label "Forward" -command "help::show __next"
  $w.menu add command -label "Stop" -command "set help::abort 1" -accelerator "Esc"
  $w.menu add sep
  $w.menu add command -label "Copy" -command "event generate $w.text <Control-c>" -accelerator "Ctrl+Ins"
  $w.menu add sep
  $w.menu add command -label "Find..." -command "event generate $w.text <Control-f>" -accelerator "Ctrl+F"
  $w.menu add command -label "Search in topics..." -command "event generate $w.text <Control-Shift-f>" -accelerator "Ctrl+S"
  $w.menu add sep
  $w.menu add command -label "Open file..." -command "event generate $w.text <Control-o>" -accelerator "Ctrl+O"
  $w.menu add sep
  $w.menu add command -label "Increase font size" -command "event generate $w.text <Control-bracketright>" -accelerator "Ctrl+\]"
  $w.menu add command -label "Decrease font size" -command "event generate $w.text <Control-bracketleft>" -accelerator "Ctrl+\["
  $w.menu add sep
  $w.menu add command -label "Refresh" -command "event generate $w.text <Control-r>" -accelerator "Ctrl+R"
  $w.menu add command -label "Reload" -command "event generate $w.text <Control-l>" -accelerator "Ctrl+L"
  bind $w.text <3> "help::rightclick %X %Y"

  menu $w.menuX  -tearoff 0
  $w.menuX add command -label "About" -command "help::show_about"
  bind $w.d <3> "help::rightclickX %X %Y"
  help::userinit $w
 }

 if {[info exists w] && [winfo exists $w]} {
  raise [winfo toplevel $w]
  focus -force [winfo toplevel $w]
  focus -force $w.text
 }

 bind $w.text <Control-l> "set help::curtopic {}; set help::curfilename {}; help::init \{$filename\} \[lindex \[lindex \$help::history \$help::hpos\] 0\]"

 if {$filename==""} {
  help::open
  return
 }

 if {[load $filename]} {
  show $topic
 } else {
  show __file_error
 }
}

proc help::userinit {w} {
}

#-------------------------------------------------------------------------------
# Function       :  help::buttons
# Description    :  Place buttons 
# Parameters     :  None
# Return         :  Tagname
#-------------------------------------------------------------------------------
proc help::buttons {} {
 variable gw
 variable buttons
 variable h4contents

 set bcount -1
 foreach btn $buttons {
  switch -glob -- $btn {
   b*   {set btn "back"}
   c*   {set btn "cont"}
   d*   {set btn "dec"}
   fi*  {set btn "find"}
   fo*  {set btn "forward"}
   inc* {set btn "inc"}
   ind* {set btn "index"}
   n*   {if {$h4contents==""} {continue}; set btn "next"}
   o*   {set btn "open"}
   p*   {if {$h4contents==""} {continue}; set btn "prev"}
   ref* {set btn "refresh"}
   rel* {set btn "reload"}
   sea* {set btn "search"}
   sep* {grid columnconfig $gw [incr bcount] -minsize 5; continue}
   st*  {set btn "stop"}
   u*   {if {$h4contents==""} {continue}; set btn "up"}
   default {continue}
  }
  grid $gw.b$btn  -padx 0 -pady 0 -row 3 -column [incr bcount] \
                 -rowspan 1 -columnspan 1 -sticky news
 }
 updatehead
}

#-------------------------------------------------------------------------------
# Function       :  help::gettags
# Description    :  Configure and get tag for current text with attr
# Parameters     :  None
# Return         :  Tagname
#-------------------------------------------------------------------------------
proc help::gettags {} {
 variable _count
 variable _tags
 variable w
 variable settings
 variable font 
 variable fontfixed
 variable fontsize 
 variable curtopic
 variable data
 variable attr
 variable rmargin
 variable index
 variable interp

 if {[set justify [lindex $attr(justify) 0]]==""} {
  set justify [lindex $attr(justify,global) 0]
 }

 if {[info exists _tags($w,$attr(style),$attr(links),$attr(bgcolors),$attr(colors),$attr(sizes),$justify,$attr(margin))]} {
  return $_tags($w,$attr(style),$attr(links),$attr(bgcolors),$attr(colors),$attr(sizes),$justify,$attr(margin))
 }

 incr _count
 set color ""
 if {[lsearch $attr(style) "p"]==-1 && [lsearch $attr(style) "t"]==-1 && [lsearch $attr(style) "k"]==-1} {
  if {$attr(face)!=""} {
   append ffont "-family \{[lindex $attr(face) 0]\} "
  } else {
   append ffont "-family \{$font\} "
  }
 } else {
  append ffont "-family \{$fontfixed\} "
 }
 if {[lsearch $attr(style) "b"]!=-1} {
  append ffont "-weight bold "
 }
 if {[lsearch $attr(style) "i"]!=-1} {
  append ffont "-slant italic "
 }
 if {[lsearch $attr(style) "u"]!=-1} {
  append ffont "-underline 1 "
 }
 if {[lsearch $attr(style) "n"]==-1 && [lsearch $attr(style) "t"]==-1 && [lsearch $attr(style) "p"]==-1} {
  $w.text tag configure tag$_count -wrap word
 }
 if {$attr(sizes)!=""} {
  if {[set ind [string index [set size [lindex $attr(sizes) 0]] 0]]=="+" || $ind=="-"} {
   catch {append ffont "-size [expr $fontsize $size]"}
  } else {
   catch {append ffont "-size [expr $size]"}
  }
 } else {
  append ffont "-size $fontsize"
 }
 if {$attr(colors)!=""} {
  set color "[lindex $attr(colors) 0]"
 }
 if {$attr(bgcolors)!=""} {
  catch {$w.text tag configure tag$_count -background [lindex $attr(bgcolors) 0]}
 }
 catch {$w.text tag configure tag$_count -font $ffont}
 catch {$w.text tag configure tag$_count -foreground $color}
 catch {$w.text tag configure tag$_count -justify $justify}
 catch {$w.text tag configure tag$_count -lmargin1 $attr(margin) -lmargin2 $attr(margin) -rmargin $rmargin}
 if {[set lnk [lindex $attr(links) 0]]!=""} {
  $w.text tag configure tag$_count -underline 1
  catch {$w.text tag configure tag$_count -foreground $settings(link)}
  $w.text tag bind tag$_count <Enter> "$w.text configure -cursor hand2; catch {$w.text tag configure tag$_count -foreground \$help::settings(alink)}"
  if {![regexp "^tcl(script)?:(.*)" $lnk x lnk_type lnk]} {
   $w.text tag bind tag$_count <1> "help::show \{$lnk\}"
   if {[lindex $attr(link_titles) 0]!=""} {
    $w.text tag bind tag$_count <Enter> "+ set help::stat \{[lindex $attr(link_titles) 0]\}"
   } elseif {[info exists data([set ltopic [lindex [split $lnk "#"] 0]],title)]} {
    $w.text tag bind tag$_count <Enter> "+ set help::stat \{$data($ltopic,title)\}"
   } elseif {$ltopic=="" && [info exists data($curtopic,title)]} {
    $w.text tag bind tag$_count <Enter> "+ set help::stat \{$data($curtopic,title)\}"
   } elseif {$index==""} {
    $w.text tag bind tag$_count <Enter> "+ set help::stat \{$lnk\}"
   }
  } else {
   if {$lnk_type==""} {
    $w.text tag bind tag$_count <1> "if {\[catch \{namespace eval :: \{$lnk\}\}\]} {set help::data(__@Error@__,title) Error; set help::data(__@Error@__,text) \"<font color=red><pre>\$::errorInfo</pre></font>\"; help::show __@Error@__}"
   } else {
    $w.text tag bind tag$_count <1> "if {\[catch \{$interp eval \{$lnk\}\}\]} {set help::data(__@Error@__,title) Error; set help::data(__@Error@__,text) \"<font color=red><pre>\$::errorInfo</pre></font>\"; help::show __@Error@__}"
   }
   if {[lindex $attr(link_titles) 0]!=""} {
    $w.text tag bind tag$_count <Enter> "+ set help::stat \{[lindex $attr(link_titles) 0]\}"
   }
  }
  $w.text tag bind tag$_count <Leave> "$w.text configure -cursor arrow; catch {$w.text tag configure tag$_count -foreground \$help::settings(link)}; set help::stat \{Done.\}"
 }
 set _tags($w,$attr(style),$attr(links),$attr(bgcolors),$attr(colors),$attr(sizes),$justify,$attr(margin)) tag$_count
 return tag$_count
}

#-------------------------------------------------------------------------------
# Function       :  help::gethr
# Description    :  Configure and get tag for horizontal line
# Parameters     :  width       - line width
# Return         :  Text with tags
#-------------------------------------------------------------------------------
proc help::gethr {height color} {
 variable _count
 variable w
 variable settings

 if {$color==""} {set color $settings(fore)}
 if {$height==""} {set height 1}
 incr _count
 $w.text tag configure tagh$_count -font "-size 1"
 catch {$w.text tag configure tagh$_count -background $settings(fore)}
 catch {$w.text tag configure tagh$_count -background $color}
 catch {$w.text tag configure tagh$_count -font "-size $height" -spacing3 0}
 return "\"\n\" h \"\n\" tagh$_count \"\n\" h"
}

#-------------------------------------------------------------------------------
# Function       :  help::show
# Description    :  Show topic
# Parameters     :  topic       - topic name
#                :  modhist     - modify or not history
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::show {{topic contents} {modhist 1} {textpos {}}} {
 variable w
 variable gw
 variable attr
 variable abort 
 variable showing 
 variable stat
 variable data
 variable index
 variable linkcolor
 variable alinkcolor
 variable history
 variable hpos
 variable backcolor
 variable forecolor
 variable settings
 variable lmargin
 variable font
 variable fontsize
 variable curtopic
 variable curfilename
 variable _count
 variable _tags
# variable _images
 variable _text
 variable perc
 variable _istitle 0
 variable _isscript 0
 variable _istext 1
 variable _t_title
#sch vars for __Prev, __up1level, __Next 
 variable sequence 
 variable up1level 
 variable h4topics
 variable h4contents
#/sch
 variable _need_tag ""
 variable controls
 variable insearch
 variable btninfo
 variable interp
 variable tables

 switch -exact -- $topic {
  __dec     {event generate $w.text <Control-bracketleft>; return}
  __find    {dlgfind:init; return}
  __inc     {event generate $w.text <Control-bracketright>; return}
  __open    {event generate $w.text <Control-o>; return}
  __refresh {event generate $w.text <Control-r>; return}
  __reload  {event generate $w.text <Control-l>; return}
  __stop    {set abort 1; return}
  __search  {dlgfind:init 0; return}
 }
 
 set mark ""
 if {[set pos [string first "#" $topic]]!=-1} {
  set mark  [string tolower [string range $topic [expr $pos + 1] end]]
  set topic [string range $topic 0 [expr $pos - 1]]
 }

 if {$topic==""} {
  set topic $curtopic
  if {$topic==""} {
   set topic "contents"
  }
 }

 if {$curtopic==$topic || ($index=="" && $curtopic==[file join [file dirname $curfilename] $topic])} {
  if {$mark!=""} {
   if {$modhist} {
    if {[llength $history] && $hpos==[expr [llength $history]-1]} {
     set history [lreplace $history $hpos $hpos [lreplace [lindex $history $hpos] 1 1 [$w.text index @0,0]]]
    }
    catch {set history [lrange $history 0 $hpos]}
    if {[lindex [lindex $history end] 0]!="$curtopic#$mark"} {
     lappend history [list "$curtopic#$mark" 0]
    }
    set hpos [expr [llength $history] - 1]
   }
   showmark "mark_$mark"
  } else {
   if {$textpos!=""} {
    showpos $textpos
   }
  }
  return
 }

 if {$showing} {
  set abort 1
  catch {after cancel "help::show $topic $modhist"}
  after 200 "help::show $topic $modhist"
  return
 }
 set abort 0
 set showing 1
 bind $w.text <Control-r> ""

#sch find topic for __up1level, __Prev and __Next
 if {$topic=="__up1level"} {
  set topic $up1level($curtopic)
 }
 if {$topic=="__Prev" || $topic=="__Next"} {
  set i [lsearch $sequence $curtopic]
  if {$topic=="__Prev"} {
   incr i -1
  } else {
   incr i +1
  }
  set topic [lindex $sequence $i]
 }
#/sch

 if {$topic=="__prev" || $topic=="__next"} {
  set showing 0
  if {[llength $history]} {
   set history [lreplace $history $hpos $hpos [lreplace [lindex $history $hpos] 1 1 [$w.text index @0,0]]]
  }
  [string range $topic 2 end]
  return
 }

 if {$modhist && [llength $history] && $hpos==[expr [llength $history]-1]} {
  set history [lreplace $history $hpos $hpos [lreplace [lindex $history $hpos] 1 1 [$w.text index @0,0]]]
 }

 set curtopic $topic

 if {$topic!="" && $index!=""} {
  if {[lsearch $index $topic]==-1} {
   if {$topic=="contents"} {
    set showing 0
    show __index
    return
   } else {
    if {[string match "__@*@__" $topic]} {
     if {[catch {set _text $data($topic,text); set title $data($topic,title)}]} {
      set showing 0
      show contents
      return
     }
    } else {
     set _text "<font color=#c00000>Topic</font> <b>\"$topic\"</b> <font color=#c00000>not found in</font> <b>\"[file join [pwd] $curfilename]\"</b><font color=#c00000>!</font>"
     set title "Topic not found!"
    }
   }
  } else {
   set _text  $data($topic,text)
   set title $data($topic,title)
   if {$index=="__file_error"} {
    set index ""
   }
  }
 } else {
   if {[string match "__@*@__" $topic]} {
    if {[catch {set _text $data($topic,text); set title $data($topic,title)}]} {
     set showing 0
     show contents
     return
    }
   } else {
    if {$topic=="contents"} {
     set topic $curfilename
    }
    set topic [file join [file dirname $curfilename] $topic]
    # make it an absolute path
    if {[file exists [file dirname $topic]]} {
     set oldpath [pwd]
     cd  [file dirname $topic]
     set absdir [pwd]
     cd  $oldpath
     unset oldpath
     set topic [file join $absdir [file tail $topic]]
    }
    if {![load $topic]} {
     set showing 0
     catch {set history [lrange $history 0 $hpos]}
     if {[lindex [lindex $history end] 0]!="$topic#$mark"} {
      lappend history [list "$topic#$mark" 0]
     }
     set hpos [expr [llength $history] - 1]
     show __file_error 0
     return
    }
    set _text  $data(text)
    regsub -all -- "\[\r\n \]+" $data(title) " " data(title)
    set title $data(title)
    set curtopic $topic
#    set curtopic $curfilename
#    set topic $curfilename
   }
 }
 
 if {$modhist} {
  catch {set history [lrange $history 0 $hpos]}
  if {[lindex [lindex $history end] 0]!="$topic#$mark"} {
   lappend history [list "$topic#$mark" 0]
  }
  set hpos [expr [llength $history] - 1]
 }

#sch get position for __Prev and __Next 
 set spos [lsearch $sequence $curtopic]
#/sch
 catch {unset _tags}
 catch {unset tables}
 array set tables "level 0 stack {} count 0 tables {}"
 catch {interp delete $interp}
 catch {set interp [interp create]}
 catch {$interp alias g_eval ::eval}
 catch {$interp alias img_change ::help::img_change}
 catch {$interp alias id_change  ::help::id_change }
 catch {$interp alias id_visible ::help::id_visible}
 catch {$interp eval [list set script_dir [file dirname $curfilename]]}


 bind $w.text <Control-r> "set help::curtopic {}; help::show $topic"

 set stat "Creating title..."
 update
 if {$title==""} {set title "Untitled"}
 $w.title delete all
 array set settings "back {$backcolor} fore {$forecolor} link {$linkcolor} alink {$alinkcolor}"
 catch {$w.title configure -foreground $settings(fore)}
 catch {$w.title configure -background $settings(back)}
 catch {$w.text configure -foreground $settings(fore)}
 catch {$w.text configure -background $settings(back)}
 set _t_title(title) [$w.title create text 4 4 -anchor nw -fill $settings(fore) -text $title \
                     -font "-family $font -size [expr $fontsize + 12] -weight bold"]

 set inc [font metrics "-family $font -size [expr $fontsize + 12] -weight bold" -displayof $w -ascent]
 set spc [font measure "-family $font -size [expr $fontsize - 2]" -displayof $w.title "    "]

 set x 4
 foreach control $controls {
  switch -glob -- $control {
   b*   {set link "__prev"}
   c*   {set link "contents"}
   d*   {set link "__dec"}
   fi*  {set link "__find"}
   fo*  {set link "__next"}
   inc* {set link "__inc"}
   ind* {set link "__index"}
   n*   {set link "__Next"}
   o*   {set link "__open"}
   p*   {set link "__Prev"}
   ref* {set link "__refresh"}
   rel* {set link "__reload"}
   sea* {set link "__search"}
   sep* {incr x $spc; continue}
   st*  {set link "__stop"}
   u*   {set link "__up1level"}
   default {continue}
  }
  foreach {txt vis expr btn} $btninfo($link) {
   if {[expr $vis]} {
    if {[expr $expr]} {
     set _t_title($link) [$w.title create text $x [expr $fontsize + $inc] -anchor nw -fill $settings(fore) \
                         -text $txt -font "-family $font -size [expr $fontsize - 2] -underline 1"]
     catch {$w.title itemconfigure $_t_title($link) -fill $settings(link)}
     $w.title bind $_t_title($link) <1> "help::show \{$link\}"
     $w.title bind $_t_title($link) <Enter> "$w.title configure -cursor hand2; catch {$w.title itemconfigure $_t_title($link) -fill \$help::settings(alink)}; set help::stat \{$txt\}"
     $w.title bind $_t_title($link) <Leave> "$w.title configure -cursor arrow; catch {$w.title itemconfigure $_t_title($link) -fill \$help::settings(link)}; set help::stat \{Done.\}"
    } else {
     set _t_title($link) [$w.title create text $x [expr $fontsize + $inc] -anchor nw -fill $settings(fore) \
                         -text $txt -font "-family $font -size [expr $fontsize - 2]"]
    }
    set x [expr [lindex [$w.title bbox $_t_title($link)] 2] + $spc]
   }
  }
 }
#/sch
 updatehead "__prev __next __stop __Prev __Next __up1level contents __index __search"

 if {$controls!=""} {
  incr inc [expr 4 + [font metrics "-family $font -size [expr $fontsize - 2]" -displayof $w -linespace]]
 }
 incr inc [font metrics "-family $font -size [expr $fontsize - 2]" -displayof $w -ascent]
 set _t_title(line) [$w.title create line 0 $inc 5000 $inc -width 2 -fill $settings(fore)]

 foreach {x1 y1 x2 y2} [$w.title bbox all] {break}
 $w.title configure -scrollregion "$x1 $y1 $x2 $y2" -height [expr $y2 - $y1]
 $w.title xview moveto 0 ; $w.title yview moveto 0

 if {[string match {.tophelpwindow*} $w]} {
  wm title .tophelpwindow "Help - $title"
 }

 set stat "Displaying..."
 set perc 0
 catch {after cancel "help::updatestate"}
 after 250 "help::updatestate"
 catch {after cancel "help::update_d"}
 after 100 "help::update_d"
 update
 $w.text configure -state normal
 $w.text delete 1.0 end
 eval "$w.text mark unset [$w.text mark names]"
 if {$textpos!=""} {
  showpos $textpos
 }
 set attr(span) ""
 set attr(style) ""
 set attr(ul_type) ""
 set attr(indent) ""
 set attr(tag_fonts) ""
 set attr(face) ""
 set attr(links) ""
 set attr(link_titles) ""
 set attr(colors) ""
 set attr(bgcolors) ""
 set attr(tables) ""
 set attr(sizes) ""
 set attr(justify) ""
 set attr(justify,global) ""
 set attr(margin) $lmargin
 set _count 0
 set data(title) ""
 set deltags [lrange [$w.text tag names] 2 end]
 catch {eval "$w.text tag delete $deltags"}
 catch {$w.text tag configure fnd -foreground [$w.text tag cget sel -foreground] \
                                  -background [$w.text tag cget sel -background]}
 if {$h4topics=="" && $h4contents==""} {
  regsub -all -- {<!--.*?-->} $_text "" _all_text
 } else {
  set _all_text $_text
 }
 set totallen [string length $_all_text]
 set _text_pos 0
#main cycle
 while {!$abort && $_text_pos!=$totallen} {
  set pos [string first "<" [string range $_all_text [expr {$_text_pos + 1024}] end]]
  if {$pos==-1} {
   set _text [string range $_all_text $_text_pos end]
   set _text_pos $totallen
  } else {
   set _text [string range $_all_text $_text_pos [expr {$_text_pos + 1024 + $pos - 1}]]
   set _text_pos [expr {$_text_pos + 1024 + $pos}]
  }
  set perc [format %2.1f [expr ($_text_pos - [string length $_text]) * 100 / $totallen.0]]
  update
  while {!$abort && [set pos [string first "<" $_text]]!=-1} {
   set outtext [string range $_text 0 [expr {$pos - 1}]]
   set _text [string range $_text $pos end]
   if {$_istitle} {
    append data(title) $outtext
   } elseif {$_isscript} {
    append data(script) $outtext
    if {![regexp -nocase "^\<\[\r\t\n \]*\/script" $_text]} {
     append data(script) "<"
     set _text [string range $_text 1 end]
     continue
    }
   } elseif {$_istext} {
    if {[lsearch $attr(style) "p"]==-1} {
     if {[$w.text get "end -2c"]=="\n"} {
      set outtext [string trimleft $outtext " \t\n"]
     }
     regsub -all -- "\[\r\t\n \]+" $outtext " " outtext
    }
#sch
    $w.text insert end [specialentities $outtext] [gettags]
#/sch
   }
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
   catch {tag_$tag $tagparams}
  }
  if {!$abort} {
   if {!$_istitle && !$_isscript && $_istext} {
    if {[lsearch $attr(style) "p"]==-1} {
     if {[$w.text get "end -2c"]=="\n"} {
      set _text [string trimleft $_text " \t\n"]
      if {$_text_pos==$totallen} {
       set _text [string trimright $_text " \t\n"]
      }
     }
     regsub -all -- "\[\r\t\n \]+" $_text " " _text
    }
#sch
    $w.text insert end [specialentities $_text] [gettags]
#/sch
   } elseif {$_isscript} {
    append data(script) "$_text"
   }
  }
 } 

 if {!$abort && $_isscript} {
  append data(script) "$_text"
  catch {tag_/script ""}
 }
 set abort 0
 set showing 0
 while {$tables(stack)!=""} {
  tag_/td {}
 }
 updatehead 
 $w.text configure -cursor arrow
 $w.text configure -state disabled
 $w.text tag raise fnd
 $w.text tag raise sel
 set stat "Done."
}

#-------------------------------------------------------------------------------
# Function       :  help::updatehead
# Description    :  
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::updatehead {{what "__prev __next __inc __dec __stop __Prev __Next __up1level"}} {
 variable w
 variable history
 variable hpos
 variable _t_title
 variable settings
 variable font
 variable fontsize
 variable showing
 variable insearch
 variable sequence
 variable curtopic
 variable up1level
 variable btninfo

 foreach link $what {
  foreach {txt vis expr btn } $btninfo($link) {
   if {[expr $expr]} {
    catch {$w.[set btn] configure -state normal} 
    catch {
     $w.title itemconfigure $_t_title($link) -font "-family $font -size [expr $fontsize - 2] -underline 1"
     catch {$w.title itemconfigure $_t_title($link) -fill $settings(link)}
     $w.title bind $_t_title($link) <1> "help::show \{$link\}"
     $w.title bind $_t_title($link) <Enter> "$w.title configure -cursor hand2; catch {$w.title itemconfigure $_t_title($link) -fill \$help::settings(alink)}; set help::stat \{$txt\}"
     $w.title bind $_t_title($link) <Leave> "$w.title configure -cursor arrow; catch {$w.title itemconfigure $_t_title($link) -fill \$help::settings(link)}; set help::stat \{Done.\}"
    }
   } else {
    catch {$w.[set btn] configure -state disabled}
    catch {
     $w.title itemconfigure $_t_title($link) -font "-family $font -size [expr $fontsize - 2] -underline 0"
     catch {$w.title itemconfigure $_t_title($link) -fill $settings(fore)}
     $w.title bind $_t_title($link) <1> ""
     $w.title bind $_t_title($link) <Enter> ""
     $w.title bind $_t_title($link) <Leave> ""
     $w.title configure -cursor arrow
    }
   }
  }
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::prev
# Description    :  Show previous topic
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::prev {} {
 variable w
 variable history
 variable hpos

 if {$hpos>0} {
  set tpos [lindex [lindex $history [incr hpos -1]] 1] 
  if {[set pos [string first "#" [set topic [lindex [lindex $history $hpos] 0]]]]!=-1} {
   set topic [string range $topic 0 [expr $pos - 1]]
  }
  show $topic 0 $tpos
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::next
# Description    :  Show next topic
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::next {} {
 variable w
 variable history
 variable hpos

 if {$hpos<[expr [llength $history] - 1]} {
  set tpos [lindex [lindex $history [incr hpos]] 1]
  if {[set pos [string first "#" [set topic [lindex [lindex $history $hpos] 0]]]]!=-1} {
   set topic [string range $topic 0 [expr $pos - 1]]
  }
  show $topic 0 $tpos
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::reg                                        
# Description    :  Assign help topic to widget
# Parameters     :  widget      - widget path
#                :  filename    - name of file with help
#                :  topic       - topic name
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::reg {widget filename topic {parent {}} {event {<F1>}} {width 450} {height 400}} {
 variable regs

 if {$filename=="" || $topic==""} {
  if {[info exists $regs($widget)]} {
   bind $widget $event ""
   unset regs($widget)
  }
 } else {
  bind $widget $event "help::init \{$filename\} \{$topic\} \{$parent\} $width $height; break"
  set regs($widget) 1
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::updatestate                                          
# Description    :  Update statusbar every 1/4 of seconds
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::updatestate {} {
 variable perc
 variable stat
 variable showing

 if {$showing} {
  set stat "Displaying... $perc%."
  after 250 "help::updatestate"
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::update_d                                              
# Description    :  Update animated image
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::update_d {} {
 variable gw
 variable _images
 variable _dnum
 variable showing

 if {$showing} {
  incr _dnum
  if {$_dnum>3} {set _dnum 1}
  $gw.d configure -image $_images(d[set _dnum])
  after 200 "help::update_d"
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::show_about                                           
# Description    :  Show about text
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::show_about {} {
 variable data

 set data(__@AbOuT@__,text) "<body text=black bgcolor=#ffffd2 link=navy>
     <p><img src=\"__@LoGo@__\"></p>
     <p>(c) Copyright by <b><font color=navy>Andrei A. Gratchev</font></b>,
     <font color=navy>Russia</font>, <font color=navy>Moscow</font>,
     <font color=navy>2000</font> - <font color=navy>2003</font><br>
     E-Mail: <font color=navy><u><tt><a href=mailto:grand@deversys.com>grand@deversys.com</a></tt></u></font>
     </p><br><p><font size=-1>Special thanks to:<ul><li><kbd>J.W.Schmitz-H&uuml;bsch
     </kbd><br> for implementations of additional features<li><kbd>Dave Clews</kbd><br>
     for help in translation of in-line documentation</ul></font></p>"
 set data(__@AbOuT@__,title) "About"
 if {![info exists data(__@LoGo@__,image)]} {
  set data(__@LoGo@__,image) [image create photo -data \
    "R0lGODlhowBFAPf/AAMDCQUFCgMDBgsLDfDw8efn6MnJyjM1ai8xYTc8eCksVzA3bz9IjiMoTB4hOQkKEUhWpgUHET1Nm0BQnzZEhBwjRCIuYik3cx4oUBMZMiczZSgyWzA5YhUZKnmFum10kRElbBEjZBUmYhkraRQiUhEdRhkpYQ0VMx0vbQcLGhwrXxsoWRomVCQ0bSc4dBolTC0/fgUHDhcgPzZKkiAsVxYdNiEoQRASGT5E
     WxUXHo2UrMrP4g4ndBYvexUrcxcrbhcrahovchktbhcpZRYoYBQkVxEfSxktaxUmWw0XOBAcQg8aPQsTLAkPIx4zdCA1eRssZQgNHh4vZyAyayQ3dhYiRyg8fgwSJSs9dSg4aklYijM7VlZhhE1TZmZrfA8rexgzfwMGDhw1fh04ghIhTiU+hQYKFS1FjCw9bT5VlDtPh0hemzVFclRooEJPc2Bzp0dVe1lolEFLaG19rC40RVNc
     dIyZvV1jdIaNol5haY6SnQMFCg0PFH2NtXeEppikwjo+SKewx5qhssXK1lNZZ6Sru6WorykrL7nAzq+0vjQ1N+rr7d/g4pWdq4OKlh0eH2ZoatTW2Li5un+FiG1xc8/S0wIDA3R4d56iob7Av2tubJWYlU9QT7Kzsn2Aetzd2tHVxMnNusfKtLCypGtsZc7QvcfJsGRlWWhpXdrcwuvs3PDx5Pf5z/X3zfP1y/X3z7m6nru8o7y9
     pvX24u/wxtvct8PEpP//0fr6zv//0/390fX1yvPzycnJpvf3zeXlvtDQrbe3mP//1f390/Pzy+rqw9TUscDAoLq6m/f3z/X1zczMq///1/391fr60vHxy7GxlaOjifX1z///2f391/Pzzvv71qurkpeXgYqKdm1tXf//2/392XJyYv//3f3928fHrGFhVPv72v//3/393fT01erqzXl5arKynf//4v394PX121RUS/j43sDArP//
     5vr64v//6vv76HFxaVlZU0pKRUFBPfb26mRkYV5eXP////39/fr6+vX19SQkJAgICCwAAAAAowBFAAAI/wBz5WI2bZu3ceO8edvGTJjAWxAjSrwlMJcwYcwITtvIMWNDhxUnihxJsqTJkyhTqlwp0eI0b+liyvQ2rWGukgIvEjSIsGdPhdu21bT5kKXRo0iTKsUI8927ePGcpmMoTBcuXRNz7oQZ06lXqTIRLhwKUqnZs2iTCps2
     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"]
 }
 append data(__@AbOuT@__,text) "<br><br><font size=-1 color=#303030><p>System info:<ul>
     <li>Tcl/Tk version: $::tcl_version/$::tk_version<li>OS: $::tcl_platform(os), 
     $::tcl_platform(osVersion) on $::tcl_platform(machine)</ul></p></font>"
 show "__@AbOuT@__"
}

#-------------------------------------------------------------------------------
# Function       :  help::destroy
# Description    :  Destroy handler
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::destroy {{top 0}} {
 variable gw
 variable abort
 variable showing
 variable data
 variable curfilename
 variable curtopic

 if {$showing} {
  set abort 1
  after 200 "help::destroy $top"
 } else {
  catch {::destroy .dlgfind}
  foreach imn [array names data *,image] {
   catch {image delete $data($imn)}
  }
  catch {unset data}
  set curfilename ""
  set curtopic ""
  if {[info exists gw] && [winfo exists $gw]} {
   if {[string match {.tophelpwindow*} $gw]} {
    ::destroy [winfo toplevel $gw]
   } else {
    if {[wm protocol [winfo toplevel $gw] WM_DELETE_WINDOW]=="help::destroy 1"} {
     wm protocol [winfo toplevel $gw] WM_DELETE_WINDOW ""
    }
    if {$top} {
     ::destroy [winfo toplevel $gw]
    } else {
     pack forget $gw
     ::destroy $gw
    }
   }
  }
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::parsetag
# Description    :  Parsing string to list
# Parameters     :  tagparams   -
# Return         :  List of parsed params {name value name value ...}
#-------------------------------------------------------------------------------
proc help::parsetag {tagparams} {
 set res ""
 set tagparams [string trim $tagparams]
 while {$tagparams!=""} {
  if {[set sp [string first " " $tagparams]]==-1} {
   set sp 99999999
  }
  if {[set i [string first "=" $tagparams]]!=-1 && $i<$sp} {
   set param [string tolower [string trim [string range $tagparams 0 [expr $i-1]]]]
   incr i
   set tagparams [string trim [string range $tagparams $i end]]
   set lastC [string index $tagparams 0]
   if {$lastC!="\"" && $lastC!="\'"} {
    set lastC " "
   } else {
    set tagparams [string range $tagparams 1 end]
   }
   if {[set i [string first $lastC $tagparams]]==-1} {
    set value $tagparams
    set tagparams ""
   } else {
    set value [string range $tagparams 0 [expr $i-1]]
    incr i
    set tagparams [string trim [string range $tagparams $i end]]
   }
  } else {
   set param [string tolower [string trim [string range $tagparams 0 [expr $sp-1]]]]
   set tagparams [string trim [string range $tagparams $sp end]]
   set value ""
  }
  lappend res $param $value
 }
 return $res
}

#-------------------------------------------------------------------------------
# Function       :  help::showmark
# Description    :  Goto mark anisochronously
# Parameters     :  mark        - name of mark
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::showmark {mark} {
 variable w
 variable showing

 if {[catch {$w.text yview $mark}] && $showing} {
  after 200 "help::showmark \{$mark\}"
 } else {
  updatehead "__prev __next"
 }
 catch {$w.text see $mark}
}

#-------------------------------------------------------------------------------
# Function       :  help::showpos
# Description    :  Goto mark anisochronously
# Parameters     :  mark        - name of mark
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::showpos {pos} {
 variable gw
 variable showing

 if {$showing && [$gw.text get $pos]==""} {
  after 200 "help::showpos \{$pos\}"
 } else {
  catch {$gw.text yview $pos}
  catch {$gw.text see $pos}
  updatehead "__prev __next"
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::open
# Description    :  Show "Open" dialog and open help
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::open {} {
 variable gw
 variable curfilename

 set tk_strictMotif 0
 set filetypes {
          {"Help files"   {.help}         TEXT}
          {"HTML files"   {.htm .html}    TEXT}
          {"All files"    {*}                 }
          }
 set res [tk_getOpenFile -filetypes $filetypes -parent $gw \
         -initialdir [file dirname $curfilename]]
 if {$res!=""} {
  help::init $res
 } else {
  if {$curfilename==""} {
   destroy
  }
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::rightclick
# Description    :  Show popup menu
# Parameters     :  x & y   - position
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::rightclick {x y} {
 variable gw
 variable fontsize
 variable index
 variable hpos
 variable history
 variable showing
 variable insearch

 $gw.menu entryconfigure "Back" -state [expr {($hpos>0)?"normal":"disabled"}]
 $gw.menu entryconfigure "Forward" -state [expr {($hpos<([llength $history] - 1))?"normal":"disabled"}]
 $gw.menu entryconfigure "Stop" -state [expr {($showing || $insearch)?"normal":"disabled"}]
 $gw.menu entryconfigure "Copy" -state [expr {([catch {selection get}])?"disabled":"normal"}]
 $gw.menu entryconfigure "Increase font size" -state [expr {($fontsize<30)?"normal":"disabled"}]
 $gw.menu entryconfigure "Decrease font size" -state [expr {($fontsize>6)?"normal":"disabled"}]
 $gw.menu entryconfigure "Search in topics..." -state [expr {([llength $index])?"normal":"disabled"}]
 tk_popup $gw.menu $x $y
}

#-------------------------------------------------------------------------------
# Function       :  help::rightclickX
# Description    :  Show popup menu
# Parameters     :  x & y   - position
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::rightclickX {x y} {
 variable gw

 tk_popup $gw.menuX $x $y
}

#sch replace specialentities by characters
#-------------------------------------------------------------------------------
# Function       :  help::specialentities
# Description    :  replace specialentities by characters
# Parameters     :  text
# Return         :  text
#-------------------------------------------------------------------------------
proc help::specialentities {text} {
 set text [string map {\
	&apos; {'}\
	&nbsp;  { } \
	&gt;     > \
	&lt;     < \
	&amp;   {&} \
	"&#38;" {&} \
	&iexcl;  ¡ \
	&cent;   ¢ \
	&pound;  £ \
	&curren; ¤ \
	&yen;    ¥ \
	&brvbar; ¦ \
	&sect;   § \
	&uml;    ¨ \
	&copy;   © \
	&ordf;   ª \
	&laquo;  « \
	&not;    ¬ \
	&shy;    ­ \
	&reg;    ® \
	&macr;   ¯ \
	&deg;    ° \
	&plusmn; ± \
	&sup2;   ² \
	&sup3;   ³ \
	&acute;  ´ \
	&micro;  µ \
	&para;   ¶ \
	&middot; · \
	&cedil;  ¸ \
	&sup1;   ¹ \
	&ordm;   º \
	&raquo;  » \
	&frac14; ¼ \
	&frac12; ½ \
	&frac34; ¾ \
	&iquest; ¿ \
	&Agrave; À \
	&Aacute; Á \
	&Acirc;  Â \
	&Atilde; Ã \
	&Auml;   Ä \
	&Aring;  Å \
	&AElig;  Æ \
	&Ccedil; Ç \
	&Egrave; È \
	&Eacute; É \
	&Ecirc;  Ê \
	&Euml;   Ë \
	&Igrave; Ì \
	&Iacute; Í \
	&Icirc;  Î \
	&Iuml;   Ï \
	&ETH;    Ð \
	&Ntilde; Ñ \
	&Ograve; Ò \
	&Oacute; Ó \
	&Ocirc;  Ô \
	&Otilde; Õ \
	&Ouml;   Ö \
	&times;  × \
	&Oslash; Ø \
	&Ugrave; Ù \
	&Uacute; Ú \
	&Ucirc;  Û \
	&Uuml;   Ü \
	&Yacute; Ý \
	&THORN;  Þ \
	&szlig;  ß \
	&agrave; à \
	&aacute; á \
	&acirc;  â \
	&atilde; ã \
	&auml;   ä \
	&aring;  å \
	&aelig;  æ \
	&ccedil; ç \
	&egrave; è \
	&eacute; é \
	&ecirc;  ê \
	&euml;   ë \
	&igrave; ì \
	&iacute; í \
	&icirc;  î \
	&iuml;   ï \
	&eth;    ð \
	&ntilde; ñ \
	&ograve; ò \
	&oacute; ó \
	&ocirc;  ô \
	&otilde; õ \
	&ouml;   ö \
	&divide; ÷ \
	&oslash; ø \
	&ugrave; ù \
	&uacute; ú \
	&ucirc;  û \
	&uuml;   ü \
	&yacute; ý \
	&thorn;  þ \
	&yuml;   ÿ \
	&quot;  \" \
 } $text]
 while {[regexp -- {&#([0-9]+);} $text x num]} {
  regsub -all -- $x $text [binary format c $num] text
 }
 return $text
}
#/sch

#-------------------------------------------------------------------------------
# Function       :  help::focusin
# Description    :  FocusIn handler (it necessary to correct behavior if
#                :  any modal window exists)
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::focusin {rw m} {
 variable _focus_w

 if {$rw==[winfo toplevel $rw]} {

  if {[grab current .]!="" && $_focus_w==""} {
   set _focus_w [grab current .]
   catch {grab release $_focus_w}
  }
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::focusout
# Description    :  FocusOut handler (it necessary to correct behavior if
#                :  any modal window exists)
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::focusout {rw m} {
 variable _focus_w

 if {$rw==[winfo toplevel $rw]} {

  if {$_focus_w!=""} {
   catch {grab $_focus_w}
   set _focus_w ""
  }
 }
}

proc help::img_change {name p_src} {
 variable data
 variable curfilename
 variable _images

 if {[info exists data($name,image,id)]} {
  if {[info exists data($p_src,image)]} {
   catch {$data($name,image,widget).text image configure $data($name,image,id) -image $data($p_src,image)}
  } else {
   if {[catch {set data($p_src,image) [image create photo -file [file join [file dirname $curfilename] $p_src]]}]} {
    catch {set imind [$data($name,image,widget).text image configure $data($name,image,id) -image $_images(broken)]}
   } else {
    catch {set imind [$data($name,image,widget).text image configure $data($name,image,id) -image $data($p_src,image)]}
   }
  }
 }
}

proc help::id_visible {name {vis 1}} {
 variable data

 catch {$data($name,names,widget).text tag configure $data($name,names,id) -elide [expr {! $vis}]}
}

proc help::id_change {name text} {
 variable data

 catch {
  set range [$data($name,names,widget).text tag ranges $data($name,names,id)]
  set names [$data($name,names,widget).text tag names [lindex $range 0]]
  $data($name,names,widget).text configure -state normal
  eval "\$data(\$name,names,widget).text delete $range"
  $data($name,names,widget).text insert [lindex $range 0] $text $names
  $data($name,names,widget).text configure -state disabled
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::dlgfind:init
# Description    :  Create and show find dialog
# Parameters     :  mode_local  - make local(true) or global(false) find dialog
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::dlgfind:init {{mode_local 1}} {
 variable gw
 variable index

 if {!$mode_local && $index==""} {return}

 catch {::destroy .dlgfind}

 toplevel .dlgfind
 wm overrideredirect .dlgfind 1
 wm transient .dlgfind $gw
 wm withdraw .dlgfind

 variable fnd_len 1

 wm resizable .dlgfind 0 0
 frame .dlgfind.f
 frame .dlgfind.btn
 pack .dlgfind.f .dlgfind.btn -side left -expand 1 -fill y -padx 3
 if {$mode_local} {
  wm title .dlgfind "Find"
  button .dlgfind.btn.f -text "Find" -width 8 -default active -command "help::dlgfind:find"
 } else {
  wm title .dlgfind "Search in all topics"
  button .dlgfind.btn.f -text "Find" -width 8 -default active -command "help::dlgfind:search"
 }
 button .dlgfind.btn.c -text "Cancel" -width 8 -command "help::dlgfind:destroy"
 pack .dlgfind.btn.f .dlgfind.btn.c -side top -expand 0 -fill x -pady 3

 frame .dlgfind.f.text
 frame .dlgfind.f.b
 pack .dlgfind.f.text .dlgfind.f.b -side top -fill x -expand 1 -anchor w -pady 3
 label .dlgfind.f.text.l -text "Find what: "
 entry .dlgfind.f.text.e -textvariable help::fnd_str -width 30
 pack .dlgfind.f.text.l .dlgfind.f.text.e -side left -fill y -expand 1
 .dlgfind.f.text.e selection range 0 end
 frame .dlgfind.f.b.l
 frame .dlgfind.f.b.dir
 pack .dlgfind.f.b.l .dlgfind.f.b.dir -side left -anchor s

 if {$mode_local} {
  frame .dlgfind.f.b.dir.p -relief flat -borderwidth 0
  frame .dlgfind.f.b.dir.b -highlightthickness 0 -relief groove -borderwidth 2
  label .dlgfind.f.b.dir.l -highlightthickness 0 -text "Direction" \
                          -relief flat -bd 0 -padx 2 -pady 0
  frame .dlgfind.f.b.dir.b.p -relief flat -bd 0 -highlightthickness 0
  frame .dlgfind.f.b.dir.f -relief flat -bd 0 -highlightthickness 0
  set height [winfo reqheight .dlgfind.f.b.dir.l]
  .dlgfind.f.b.dir.p configure -height [expr {$height/2}]
  .dlgfind.f.b.dir.b.p configure -height [expr {$height/2+$height%2+1}]
  pack .dlgfind.f.b.dir.b.p -side top -fill x
  pack .dlgfind.f.b.dir.f -in .dlgfind.f.b.dir.b -fill both -expand yes -padx 4 -pady 4
  pack .dlgfind.f.b.dir.p -side top -fill x
  pack .dlgfind.f.b.dir.b -fill both -expand yes
  place .dlgfind.f.b.dir.l -x 5 -anchor nw -y 0

  set f .dlgfind.f.b.dir.f
  radiobutton $f.up -text "Up" -value "-backwards" -variable help::fnd_direction
  radiobutton $f.down -text "Down" -value "-forwards" -variable help::fnd_direction
  pack $f.up $f.down -side left -fill y -expand 1
 }
 checkbutton .dlgfind.f.b.l.case -text "Match case" -onvalue "" -offvalue "-nocase" -variable help::fnd_case
 checkbutton .dlgfind.f.b.l.regexp -text "Regexp expression" -onvalue "-regexp" -offvalue "" -variable help::fnd_regexp
 pack .dlgfind.f.b.l.regexp .dlgfind.f.b.l.case -side bottom -expand 1 -anchor sw
 bind .dlgfind <Return>  ".dlgfind.btn.f configure -relief sunken; update idletasks; .dlgfind.btn.f configure -relief raised; .dlgfind.btn.f invoke"
 bind .dlgfind <Escape> "help::dlgfind:destroy"
 bind .dlgfind.f.text.e <KeyRelease> help::dlgfind:checkstate
 bind .dlgfind <FocusIn>  "help::focusin %W %m"
 bind .dlgfind <FocusOut> "help::focusout %W %m"
 bind .dlgfind <Configure> "help::focusin %W %m"
 bind .dlgfind <Destroy> "help::focusout %W %m"
 bind .dlgfind <Expose> "help::focusin %W %m"
 dlgfind:checkstate

 update idletasks
 set w [winfo reqwidth .dlgfind]
 set h [winfo reqheight .dlgfind]
 set x [expr [winfo rootx $gw] + ([winfo width  $gw] - $w)/2]
 set y [expr [winfo rooty $gw] + ([winfo height $gw] - $h)/2]
 wm geometry .dlgfind "${w}x${h}+${x}+${y}"

 wm overrideredirect .dlgfind 0
 wm deiconify .dlgfind
 
 tkwait visibility .dlgfind
 catch {focus -force .dlgfind.f.text.e}
}

#-------------------------------------------------------------------------------
# Function       :  help::dlgfind:destroy
# Description    :  Destroy find dialog
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::dlgfind:destroy {} {
 variable gw

 catch {::destroy .dlgfind}
 catch {focus -force $gw.text}
}

#-------------------------------------------------------------------------------
# Function       :  help::dlgfind:checkstate
# Description    :  Modify state of "Find" button
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::dlgfind:checkstate {} {
 variable fnd_str

 if {$fnd_str==""} {
  .dlgfind.btn.f configure -state disabled
 } else {
  .dlgfind.btn.f configure -state normal
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::dlgfind:find
# Description    :  Find function
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::dlgfind:find {} {
 variable gw
 variable fnd_str
 variable fnd_len
 variable fnd_case 
 variable fnd_regexp
 variable fnd_direction

 if {![dlgfind:checkregexp $fnd_regexp $fnd_str]} {return}
 if {$fnd_direction=="-forwards"} {set offset "+${fnd_len}chars"; set stop "end"} else {set offset "-${fnd_len}chars"; set stop "1.0"}
 set pos [$gw.text index "insert$offset"]
 if {[set findpos [eval $gw.text search $fnd_case $fnd_regexp $fnd_direction -count "help::fnd_len" -- {$fnd_str} $pos $stop]]!=""} {
  $gw.text mark set insert $findpos
  $gw.text see $findpos
  $gw.text tag remove fnd 1.0 end
  $gw.text tag remove sel 1.0 end
  $gw.text tag add sel $findpos "$findpos +${fnd_len}chars"
  $gw.text tag add fnd $findpos "$findpos +${fnd_len}chars"
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::dlgfind:search
# Description    :  Find function (at all topics)
# Parameters     :  None
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::dlgfind:search {} {
 variable gw
 variable fnd_str
 variable fnd_len [string length $fnd_str]
 variable fnd_case 
 variable fnd_regexp
 variable searchedtext
 variable index
 variable data
 variable abort 0
 variable stat
 variable insearch

 if {$insearch} {
  set abort 1
  after 250 help::dlgfind:search
  return
 }

 if {![dlgfind:checkregexp $fnd_regexp $fnd_str]} {return}
 dlgfind:destroy

 if {$searchedtext==[list $fnd_str $fnd_case $fnd_regexp]} {
  set help::curtopic ""
  show "__@SearchResult@__"
  return
 }
 set insearch 1
 set searchedtext [list $fnd_str $fnd_case $fnd_regexp]
 
 set found 0
 set data(__@SearchResult@__,title) "Search results"
 set data(__@SearchResult@__,text) "<body text=black bgcolor=#ffffd2 link=#006800><ul>"
 updatehead "__stop"
 if {$index!=""} {
  if {$fnd_case==""} {
   set fstr $fnd_str
  } else {
   set fstr [string tolower $fnd_str]
  }
  if {[set pos [lsearch -exact $index "__index"]]!=-1} {
   set topics [lreplace $index $pos $pos]
  } else {
   set topics $index
  }
  foreach topic $topics {
   set _all_text $data($topic,text)
   set totallen [string length $_all_text]
   set _text_pos 0
   set stat "Searching in $data($topic,title)..."
   update
   set outtext ""
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
     append outtext [string range $_text 0 [expr {$pos - 1}]]
     set _text [string range $_text $pos end]
     if {[set pos [string first ">" $_text]]!=-1} {
      set _text [string range $_text [incr pos] end]
     } else {
      set _text ""
     }
    }
    if {!$abort} {
     append outtext $_text
    }
   }
   if {$abort} {
    break
   } else {
    set outtext [specialentities $outtext]
    if {$fnd_regexp!=""} {
     if {[eval regexp -indices $fnd_case -- \$fnd_str \$outtext rpos]} {
      set pos [lindex $rpos 0]
      set fnd_len [expr {[lindex $rpos 1] - [lindex $rpos 0] + 1}]
     } else {
      set pos -1
     }
    } else {
     if {$fnd_case==""} {
      set _text $outtext
     } else {
      set _text [string tolower $outtext]
     }
     set pos [string first $fstr $_text]
    }  
    if {$pos!=-1} {
     set str1 [string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} [string range $outtext [expr {$pos - 50}] [expr {$pos - 1}]]]
     set str2 [string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} [string range $outtext $pos [expr {$pos + $fnd_len - 1}]]]
     set str3 [string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} [string range $outtext [expr {$pos + $fnd_len}] [expr {$pos + $fnd_len + 70}]]]
     incr found
     append data(__@SearchResult@__,text) "<li><a href=\"$topic\">[string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} $data($topic,title)]</a><br>"
     append data(__@SearchResult@__,text) "<font size=-1>...$str1<font color=#000080><b>$str2</b></font>$str3...</font><br><br>"
    }
   }
  }
 }
 if {$abort} {
  append data(__@SearchResult@__,text) "</ul><font color=#c00000>Search aborted.</font></body>"
  set searchedtext ""
 } elseif {!$found} {
  append data(__@SearchResult@__,text) "</ul><font color=#c00000>No topics found for request</font> <b>\"[string map {{&} {&amp;} {>} {&gt;} {<} {&lt;}} $fnd_str]\"</b> <font color=#c00000>!</font></body>"
 } else {
  append data(__@SearchResult@__,text) "</ul></body>"
 }
 set data(__@SearchResult@__,title) "Search results ($found found)"
 set abort 0
 $gw.text configure -cursor arrow
 set stat "Done."
 set insearch 0
# updatehead "__stop"
 set help::curtopic ""
 show "__@SearchResult@__"
}

#-------------------------------------------------------------------------------
# Function       :  help::dlgfind:checkregexp
# Description    :  Check function
# Parameters     :  
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::dlgfind:checkregexp {regexp val} {
 if {$regexp=="-regexp"} {
  if {[catch {regexp -- "$val" {}}]} {
   tk_dialog .help_error_in_regexp_dialog "Error in regexp" "Error in regular expression:\n\"$val\"" "" 0 "Ok"
   return 0
  } else {
   return 1
  }
 } else {
  return 1
 }
}

#-------------------------------------------------------------------------------
# Function       :  help::add_ext
# Description    :  Loads extensions for help package
# Parameters     :  filename    - name of file to load
# Return         :  Nothing
#-------------------------------------------------------------------------------
proc help::add_ext {filename} {
 variable gw
 variable ext_loaded

 set name [file tail $filename]
 if {[lsearch $ext_loaded $name]==-1} {
  if {![catch {source $filename}]} {
   lappend ext_loaded $name           
  }
 }
}

#===============================================================================
# Initialization section
#===============================================================================
if {[catch {package require Tcl 8.1}] || [catch {package require Tk 8.1}]} {
 error "Incompatible version of Tcl/Tk. Please update your Tcl/Tk."
 exit
}
switch -- $tcl_platform(platform) {
 "windows" {
    proc help::showurl {url} {
     eval exec [auto_execok start] [list $url] &
    }
  }

 "unix" {
    proc help::showurl {url} {
     exec netscape [list $url] &
    }
  }

 "macintosh" {
    proc help::showurl {url} {
     global env

     if {![info exists env(BROWSER)]} {
      set env(BROWSER) "Browse the Internet"
     }
     if {[catch {
      AppleScript execute "tell application \"$env(BROWSER)\"
                           open url \"$url\"
                           end tell"} emsg] 
        } {
      error "Error displaying $url in browser\n$emsg"
     } 
    }
  }
}

help::add_ext [file join [file dirname [info script]] "help_chk.tcl"]
help::add_ext [file join [file dirname [info script]] "help_2htm.tcl"]
### ADD BY JAIME URZUA GREZ
help::add_ext [file join [file dirname [info script]] "help_tbl.tcl"]
