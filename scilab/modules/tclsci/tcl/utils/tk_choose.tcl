namespace eval tkChoose {

    # Author : S. Mottelet, Tue Jan 24 12:01:25 CET 2006
    # Use of a namespace makes things easier (no global variables to pass
    # arguments from the Scilab level).
    
    # Modified by P. Marechal, Tue Feb 14 08:00:00 CET 2006
    # => Add a scrollbar if the number of items reachs 30 elements.

    set relief raised
    set t .tkChoose
    set fontSize 12
    if {$::tcl_platform(os)=="Windows NT"} {
        set fontSize 9
    }

    catch {font create chooseFont -family Helvetica -size $fontSize}
    catch {font create chooseBoldFont -family Helvetica -size $fontSize -weight bold}

    toplevel $t
    bind $t <Destroy> {
        set tkChoose::result -1
    }
   
    # Convert (if necessary) the array of strings to a \n
    # separated string

    if ![set length [array size title]] {
        set titleString $title
    } else {
        for {set k 1} {$k<$length} {incr k} {
            append titleString $title($k,1)\n
        }
        append titleString $title($k,1)
    }

    label $t.label -text $titleString -justify left -font chooseBoldFont
    
    scrollbar $t.scroll -relief sunken -command "$t.list yview"

    if [set length [array size items]] {
        if {$length>=30} then {
            set list_height 30
        } else {
            set list_height 0
        }
    } else {
        set list_height 0
    }

    listbox $t.list -relief sunken -yscrollcommand "$t.scroll set" -bg white -height $list_height -width 0 -font chooseFont
    
    bind  $t.list  <ButtonRelease> {
      bind $tkChoose::t <Destroy> {}
      catch {set tkChoose::result [$tkChoose::t.list nearest %y]}
    }

    # Insert the string or the array strings in the listbox

    if ![set length [array size items]] {
        $t.list insert end $items    
    } else {
        for {set k 1} {$k<=$length} {incr k} {
            $t.list insert end $items($k,1)
        }
    }

    button $t.b -text $button -relief $relief -font chooseFont -command {
        destroy $tkChoose::t
    }

    # Adjust padding space when there is no title

    set listPady {0 0}
    if {$titleString!=""} {
        pack $t.label -pady 10 -padx 10
    } else {
        set listPady {10 0}
    }
    
    pack $t.b -pady 10 -side bottom
    
    # Display the listbox and the scrollbar if needed
    
    if {$length>=30} then {
        pack $t.list $t.scroll -side left -fill y -pady $listPady -padx 10
    } else {
        pack $t.list -pady $listPady -padx 10
    }
 
    wm resizable $t 0 0
    
    # The event loop occurs at the Scilab level 
    # see SCI/modules/tclsci/macros/tk_choose.sci
}
