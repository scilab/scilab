############################################
# TKSCILAB Gui Interface facility for Scilab
# Bertrand Guiheneuf - 1998 
############################################
#
# 	$Id: callbacks.tcl,v 1.2 2004/05/25 16:41:09 steer Exp $	


######################################################################################
proc SciCallback { name callback } {
    # called in all control callbacks 
    
    global "$name";
    set handle [set "$name\(handle)"];
    
    global gcbo;
    set old_gcbo $gcbo;
    set gcbo $handle;
    
    # call the scilab routine (in C) allowing to execute a 'scilab' string
    # it passes the whole string to the scilab parser
    set com "gcbo=$gcbo; $callback; gcbo=$old_gcbo;"
    ScilabEval $com;
    set gcbo $old_gcbo;
    
 }

proc SliderSciCallback { name callback slidervalue} {
# called in slider control callbacks 

   global "$name";
   set handle [set "$name\(handle)"];
   
   global gcbo;
   set gcbo $handle;

    # call the scilab routine (in C) allowing to execute a 'scilab' string
    # it passes the whole string to the scilab parser
   ScilabEval $callback;
    
 }



######################################################################################
proc popupsel { name itemnb } {
# answer to a popup item selevtion 

    global "$name";
    set path [set "$name\(path)"];
    set string [set "$name\(string)"];
    
    set "$name\(value)" $itemnb
    set item  [split $string "|"];
    set label [lindex $item $itemnb];

    $path configure -text $label;

    # call the popupmenu callback command
    if [ info exists "$name\(callback)"] { 
	set callback [ set "$name\(callback)"]
        SciCallback $name $callback
    }

}



