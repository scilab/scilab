

######################################################################################
 proc scrolllistbox { parent args } {
     # create a listbox end scrollbars
		frame $parent
	# Create listbox attached to scrollbars, pass thru $args
	eval {listbox $parent.list -exportselection 0\
		-yscrollcommand [list $parent.sy set] \
		-xscrollcommand [list $parent.sx set]} $args
	scrollbar $parent.sy -orient vertical \
		-command [list $parent.list yview]
	# Create extra frame to hold pad and horizontal scrollbar
	frame $parent.bottom
	scrollbar $parent.sx -orient horizontal \
		-command [list $parent.list xview]
	# Create padding based on the scrollbar width and border
	set pad [expr [$parent.sy cget -width] + 2* \
		([$parent.sy cget -bd] + \
		 [$parent.sy cget -highlightthickness])]
	frame $parent.pad -width $pad -height $pad
	# Arrange everything in the parent frame
	pack $parent.bottom -side bottom -fill x
	pack $parent.pad -in $parent.bottom -side right
	pack $parent.sx -in $parent.bottom -side bottom -fill x
	pack $parent.sy -side right -fill y
	pack $parent.list -side left -fill both -expand true
	return $parent.list
}     


######################################################################################
proc popupmenu { parent args } {
    # create a menu associated to a button 

    menubutton $parent -menu $parent.menu -relief raise
    set menu [menu $parent.menu -tearoff 0]
    
    
}
