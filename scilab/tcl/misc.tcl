############################################
# TKSCILAB Gui Interface facility for Scilab
# Bertrand Guiheneuf - 1998 
############################################
#
# -->   misc.tcl
# 	$Id: misc.tcl,v 1.1 2001/04/26 07:49:14 scilab Exp $	


######################################################################################
proc Redraw { handle } {
# Completely redraw an object by reseting all its 
# property 

    global Win;
    set name $Win($handle);
    global "$name";
    set style [set "$name\(style)" ];
    
    switch -exact -- $style {
	figure {
	}
	default {
	    set fields [array names $name];
	    
	    foreach x $fields {
		
		if {[string compare $x "parent"] != 0} {
		    SetField $handle $x [set "$name\($x)"];
		}
	    }
	    
	}
    }



}






######################################################################################
proc GetRealParent {hparent control} {
    # this routine is used to determine the real
    # parent of an object.
    # for example the parent of a figure (toplevel) is
    # always ".", ie the window with handle 0
    # if the specified parent is not valid or if the 
    # parent is "." and the object is not a figure,
    # then the parent is created

    # INPUT: 
    #  hparent : handle of the virtual parent
    #  control : type of the control (in tk)
    # OUTPUT:
    #  handle of the real parent

    global FigList gcf;
    
    switch -exact -- $control {
	menu {
	    if { $hparent >=1000 } { return $hparent}
	    if { [lsearch -exact $FigList $hparent] != -1 } {
		set h $hparent;
	    } else {
		set h [getgcf];
	    }
	    return $h;
	}

	default {
	    if { $hparent == 0 } {
		# the hparent handle is 0 so we have to determine a 
		# hparent figure
		set hparent [getgcf];
	    }
	    return $hparent;
	}
    }
}


proc getgcf {} {
    # return the current figure and create it
    # if necessary
    # INPUT : void
    # OUTPUT : handle of the current figure 

    global gcf;
    
    if { $gcf == 0 } {
	# there is no current figure, so lets create one
	set h [ CreateFigure 0]
	set hparent $h
    } else {
	# the current figure is OK
	set hparent $gcf
    }
    return $hparent;
}


######################################################################################
proc FindObj { field value } {
    # returns the handle of an object which field $field
    # matches $value
    
    global Win;
    
    
    
    set Windexes [array names Win];
    set len [llength $Windexes];
    
    set handle -1;
    
    for { set Idx 0 } { $Idx < $len } { incr Idx } {
	
	set obj "$Win([lindex $Windexes $Idx])";
	global $obj;
	
	if { [info exists "$obj\($field)"] } {
	    set tmp [set "$obj\($field)"];
	    if { [winfo exists [set "$obj\(path)"] ] } {
	    if { [string compare $tmp  $value]==0 } {
		set handle [lindex $Windexes $Idx];
		break;
	    }
	}
    }
}

return $handle;

}

######################################################################################
proc FindFig { field value } {
    # returns the handle of an object which field $field
    # matches $value
    
    global Win;
    
    
    
    set Windexes [array names Win];
    set len [llength $Windexes];
    
    set handle -1;
    
    for { set Idx 0 } { $Idx < $len } { incr Idx } {
	
	set obj "$Win([lindex $Windexes $Idx])";
	global $obj;
	
	if { [info exists "$obj\($field)"] } {
	    set tmp [set "$obj\($field)"];
	    
	    if { [string compare $tmp  $value]==0 } {
		set handle [lindex $Windexes $Idx];
		break;
	    }
	    
	}
    }

return $handle;

}


######################################################################################



  proc ChgConfigure { WinPath width height } {
      # called when the parent of a window chages its size
      
      
      global Win;

      # find the handle of the closed figure
      set h [FindObj "path" $WinPath];
      set name $Win($h);
      global "$name";
      
      set "$name\(w)" $width;
      set "$name\(h)" $height;
      set style [set "$name\(style)"];
      #if {[info exist "$name\(position)" ]} { SetField $h position  [set "$name\(position)"]; }
      if {[string compare $style slider]==0} { SetField $h position  [set "$name\(position)"]; }
      if {[info exist "$name\(fontsize)" ]} { SetField $h fontsize  [set "$name\(fontsize)"]; }
      
    
      
  }



