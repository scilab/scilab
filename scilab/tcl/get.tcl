############################################
# TKSCILAB Gui Interface facility for Scilab
# Bertrand Guiheneuf - 1998 
############################################
#




######################################################################################
proc GetField { handle field } {
# This the procedure called when get(...) is invoked by Scilab 
# There is a simple default behaviour when there is no
# a default behaviour when there is no
# specific get function associated to $field
# it just returns the value of the field 
# in a *string*.

  global Win;
  set name $Win($handle);
  global "$name";
  
  # implemented fields
  set KnownFunc  { value };

  # is it an implemented?
  set idx  [lsearch $KnownFunc $field];
    
  if { $idx != -1} {
    
    # call the specific getfield
    "Get$field" $name ;    

  } else {

    # just get the string
      if ([info exists "$name\($field)"]) {
	  return [set "$name\($field)"];
      } else {
	  return "";
      }
  }
}










proc Getvalue { name } {
    # return the value field of a control
    # when the control is a listbox, retruns a string 
    # 'idx1|idx2 ...' where idx1, .. are the indexes of
    # the selected items;

    global $name;
    
    set style [ set "$name\(style)"];
    switch -exact -- $style {
	
	listbox {
	    
	    set path [ set "$name\(path)"];
	    set sel [$path.list curselection];
	    set tmp [list];
	    for {set i 0} {$i< [llength $sel]} {incr i} {
		set current [lindex $sel $i];
		incr current; 
		lappend tmp $current;
	    }
	    
	    return [join $tmp |];
	}
	
	default {
	    if ([info exists "$name\(value)"]) {
		return [set "$name\(value)"];
	    } else {
		return "";
	    }
	}
    }

}