############################################
# TKSCILAB Gui Interface facility for Scilab
# Bertrand Guiheneuf - 1998 
############################################
#



# define the translation between 
# tk control names and scilab control names

set StyTrad(button)         pushbutton;
set StyTrad(checkbutton)    checkbox;
set StyTrad(radiobutton)    radiobutton;
set StyTrad(entry)          edit;
set StyTrad(label)          text;
set StyTrad(scale)          slider;
set StyTrad(frame)          frame;
set StyTrad(scrolllistbox)  listbox;
set StyTrad(popupmenu)      popupmenu;
    


# define the equivalence between anchor names 
# in scilab and anchor names in tk
# horizontal alignment
set HorizontalAnchorEq(center) {};
set HorizontalAnchorEq(left)   w;
set HorizontalAnchorEq(right)  e;
set HorizontalAnchorEq(e)      e; # kept for ascending compatibility
set HorizontalAnchorEq(w)      w; # kept for ascending compatibility
# vertical alignment
set VerticalAnchorEq(center) center; # kept for ascending compatibility
set VerticalAnchorEq(middle) center;
set VerticalAnchorEq(top)    n;
set VerticalAnchorEq(bottom) s;




######################################################################################
proc CreateUIControl {hparent control} {

  # input: parent : Handle on the parent window
  #        control : control style ( ex: pushbutton, ...)
  # output: handle of the control

  # Create a control and place it on (0,0)
  # the control type is given as an input string :
  # append the global variables and so on

    global Win WinIdx StyTrad;
    set parent [GetRealParent $hparent $control];
    
  # Get the good counter name 
  # and get its value
  set UiIdxName "Idx$control" ;
  global "$UiIdxName";

    # if no control of this kind exist, initialise the counter
    # for this control. The variable containing the control counter
    # is Idx$control ie for ex. Idxpushbutton, Idxtext ...
  if { [info exist "$UiIdxName"] != 1 } {
    set "$UiIdxName" 0 ;
  }

  #get the current control index and 
  #increment the control index
  set UiIdx [set "$UiIdxName"] ;
  incr "$UiIdxName" ;

  # Compute the instance control name
  set name "$control$UiIdx" ;
  

  # Update windows list
  set Win($WinIdx) "$name" ;
  
  
  # Store the control win handle in its info structure 
  global  "$name";  
  set "$name\(handle)" $WinIdx;
  
  set "$name\(name)" $name;
  set "$name\(style)" $StyTrad($control);
  set "$name\(control)" $control;
  set "$name\(units)" "pixels";
  
  # create the control   
  SetField $WinIdx parent $parent;

  # increment the handle counter
  incr WinIdx ;

  
  set path [set  "$name\(path)"];

  
  # +++++++++++++++++
  # set the defaults 
  switch -exact -- [set "$name\(style)"] {

      #  edit 
      "edit" {   
	  set "$name\(fontunits)" "pixels";
	  set "$name\(fontsize)" 12;
	  set "$name\(fontname)" "helvetica";
	  set "$name\(fontangle)" "normal";
	  set "$name\(fontweight)" "normal";
	  $path configure -textvariable "$name\(string)";
      }
      
      #  text 
      "text" {  
	  set "$name\(fontunits)" "pixels";
	  set "$name\(fontsize)" 12;
	  set "$name\(fontname)" "helvetica";
	  set "$name\(fontangle)" "normal";
	  set "$name\(fontweight)" "normal";
      }

      #  pushbutton 
      "pushbutton" { 
	  set "$name\(fontunits)" "pixels";
	  set "$name\(fontsize)" 12;
	  set "$name\(fontname)" "helvetica";
	  set "$name\(fontangle)" "normal";
	  set "$name\(fontweight)" "normal";
	  $path configure -borderwidth 1 -bg #a0a0a0 -padx 0 -pady 0;
	  SetField  [set "$name\(handle)" ] position "20|40|40|20"; 
      }

      #  radiobutton and checkbox
      "checkbox" { 
	  set "$name\(fontunits)" "pixels";
	  set "$name\(fontsize)" 12;
	  set "$name\(fontname)" "helvetica";
	  set "$name\(fontangle)" "normal";
	  set "$name\(fontweight)" "normal";
	  $path configure -variable "$name\(value)";
      }

      #  slider
      "slider" { 
	  #set "$name\(fontunits)" "pixels";
	  #set "$name\(fontsize)" 12;
	  #set "$name\(fontname)" "helvetica";
	  #set "$name\(fontangle)" "normal";
	  #set "$name\(fontweight)" "normal";
	  $path configure -variable "$name\(value)" -showvalue 0 -length 200 \
		  -sliderlength 20 -width 30 -from 0 -to 1  -orient horizontal;
      }
      
      #  frame
      "frame" { 
	  
	  $path configure -bg #a0a0a0 -relief ridge -borderwidth 3
      }
      
      # listbox 
      "listbox" { 
	  set "$name\(fontunits)" "pixels";
	  set "$name\(fontsize)" 12;
	  set "$name\(fontname)" "helvetica";
	  set "$name\(fontangle)" "normal";
	  set "$name\(fontweight)" "normal";
	  $path.list configure -bg #a0a0a0 -setgrid true;
	  $path configure -bg #d0d0d0;
      }   

      # popupmenu 
      "popupmenu" { 
	  set "$name\(fontunits)" "pixels";
	  set "$name\(fontsize)" 12;
	  set "$name\(fontname)" "helvetica";
	  set "$name\(fontangle)" "normal";
	  set "$name\(fontweight)" "normal";
	  $path configure -bg #a0a0a0 -relief raise;
      }   
      
  }
  
  
  
  Setmin $name 0;
  Setmax $name 1;
    
  # +++++++++++++++++
  bind $path <Configure> {ChgConfigure %W %w %h};
    
  
  
  
  
  
  return  [set "$name\(handle)" ];
    
    
  }
  
  
proc CloseUIControl { FigPath } {
    # answer to a close event generated by a figure

    global Win  ;
    
    # find the handle of the closed figure
    set h [FindObj "path" $FigPath];
    set name $Win($h);
    global "$name"; 
    unset "$name";
    unset "Win($h)";
    
    
}



