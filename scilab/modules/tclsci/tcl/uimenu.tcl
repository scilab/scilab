############################################
# TKSCILAB Gui Interface facility for Scilab
# Bertrand Guiheneuf - 1998 
############################################
#
# -->   uimenu.tcl



# ************************
# important remark : in TK, menu item are indexed begining from 1
# BUT in tcl, lists indexes begin from 0 !!!!!
# Be careful.
# ************************



# define the correspondance between the scilab 'uimenu'
# object and the tk 'menu' object
set StyTrad(menu)       uimenu;

# define the correspondance between the scilab 
# menu property name and the tk menu property name
set MenuPropTrad(label) label;
set MenuPropTrad(callback)  command;
set MenuPropTrad(tag)  tag;

proc GetRootMenu { hparent } {
    # is is used to know which is the menu associated 
    # to a toplevel window and to create it if it does 
    # not exist
    
    # INPUT: hparent : handle of the figure
    # OUTPUT: handle of the menu

    global Win WinIdx IdxMenu StyTrad;
    
    # get the real parent, ie if the parent doesn't
    # exist or is specified as 0 get the gcf.
    set parent [GetRealParent $hparent menu];

    # we will need this to get the path of the menu
    set ParentName $Win($parent);
    global "$ParentName";
    set ParentPath [set "$ParentName\(path)"];
 
    # Compute the instance menu name
    set name "$ParentName\menu" ;
    
    # if the menubar of the figure does not 
    # exist, create it!
    if { ![winfo exists $ParentPath.menu] } { 
	
	menu $ParentPath.menu;
	  
	# Update windows list
	set Win($WinIdx) "$name" ;
	# and increment the window counter


	# Store the menu win handle in its info structure 
	global  "$name";  
	set "$name\(handle)" $WinIdx;
	incr WinIdx;
  
	set "$name\(name)" $name;
	set "$name\(style)" rootmenu;
	set "$name\(control)" rootmenu;
	set "$name\(units)" "pixels";
	set "$name\(path)" $ParentPath.menu;
	set "$name\(parent)" $parent;
	set "$name\(menuparent)" 0;
	set "$name\(childrenmenu)" {};
	set "$name\(menustyle)" command;
    }

    return [set "$name\(handle)"];

}

proc CreateUIMenu { parent } {

    global Win WinIdx IdxMenu StyTrad;

    # get the real parent, ie if the parent doesn't
    # exist or is specified as 0 get the gcf.
#    set parent [GetRealParent $hparent menu];

    # we will need this to get the path of the menu
#    set ParentName $Win($parent);
#    global "$ParentName";
#    set ParentPath [set "$ParentName\(path)"];
    

    # if no control of this kind exist, initialise the counter
    # for this control. The variable containing the control counter
    # is Idx$control ie for ex. Idxpushbutton, Idxtext ...
    if { [info exists IdxMenu] != 1 } {
    set IdxMenu 0 ;
  }

  #get the current menu index and 
  #increment it
  set UiIdx $IdxMenu;
  incr IdxMenu;

  # Compute the instance control name
  set name "Menu$UiIdx" ;
  
  # Update windows list
  set Win($WinIdx) "$name" ;
  # and increment the window counter
  #puts "Creating $name";

  # Store the menu win handle in its info structure 
  global  "$name";  
  set "$name\(handle)" $WinIdx;
  incr WinIdx;
  
  set "$name\(name)" $name;
  set "$name\(style)" $StyTrad(menu);
  set "$name\(control)" menu;
  set "$name\(units)" "pixels";
  set "$name\(menustyle)" "command";
  set "$name\(childrenmenu)" {};

  # create the control and set its parent
  SetMenuParent $name $parent;
  return [set "$name\(handle)"];

}


######################################################################################
proc deleteMenu { name } {
    # delete a menu. The menu is deleted from its parent 
    # children list. Its pathh is destroyed but the menu still
    # exists. This routine is used to move menus.
    # INPUT : name : nqme of the menu
    # OUTPUT : void
    
    global "$name" Win;
    
    if {[info exists "$name\(path)"]} {
	set path [set "$name\(path)"];
	set hmenuParent [ set "$name\(menuparent)"];
	set menuParent "$Win($hmenuParent)";
	# debug info
	#puts "\nDeleting menu $name :";
	#puts " --> menu path : $path";
	#puts " --> menu's parent handle : $hmenuParent";
	#puts " --> menu's parent name : $menuParent";
	
	global "$menuParent";
	set parentChildList  [set "$menuParent\(childrenmenu)"];
	set index [set "$name\(menuindex)"];
	set listindex [expr $index -1];
	set "$menuParent\(childrenmenu)" [lreplace $parentChildList $listindex $listindex];
	set parentPath [set "$menuParent\(path)"];
	$parentPath delete [expr $index]; 
	
	if {[llength parentChildList]==1} {
	    #puts " --> Changing parent style : $menuParent becomes 'command'";
	    menuSetCommandStyle $menuParent
	}
	if [winfo exist $path] { destroy $path }
	
    }
    
}



######################################################################################
proc SetMenuParent { name phandle } {
    # set Parent field of a menu
    # this routine is also used to create a menu
    # it is also used to change menu parent. 
    #
    # input : 'name' variable name of the menu
    #         'phandle' parent handle
    # output : void

    global "$name" Win;
    
    
    
    set phandle [GetRealParent $phandle menu];
    # delete the menu if it already exists
    #puts "\nSetting $name parent handle to $phandle";
    # to speed up menu creation:
    # if parent is already the good one
    # do nothing
    if {[info exists "$name\(parent)"]} {
	
	set oldhandle [set "$name\(parent)"];
	if { $oldhandle == $phandle } { return; }
    }
    
    deleteMenu $name; 
    
    # we have to round the handle value
    # to prevent any numerical bias introduced 
    # by scilab from killing tk
    set phandle [expr round($phandle)];
    set style [set "$name\(style)"];     # scilab's control style
    set control [set "$name\(control)"]; # tk's control style
    
    # get the parent's name
    # end create it if it does not exist
    

    set parent "$Win($phandle)";
    # get the menu_parent
    # the parent and menu_parent are different only in the case when
    # the parent is a toplevel (figure) because the menu_parent is
    # then the tpolevel menu defined with the -menu option of a 
    # toplevel (only after tk8.0)
    
    set hmenuParent [GetRealMenuParent $phandle];
    set menuParent "$Win($hmenuParent)";
    #puts "--> $name real menu parent handle: $hmenuParent";
    #puts "--> $name real menu parent name: $menuParent";

    global "$menuParent";
    set menuParentpath [set "$menuParent\(path)"];


    lappend "$menuParent\(childrenmenu)" $name;
    set index [ llength [set "$menuParent\(childrenmenu)"] ];
    
    set path "$menuParentpath.m$index" ;
    
    set "$name\(parent)" $phandle;
    set "$name\(menuparent)" $hmenuParent;
    set "$name\(path)" $path;
    
    
    # default --> to change !
    menu $path ;



    
    set "$name\(menuindex)" $index;
    

    # redraw all parent children 
    # old UpdateParents $menuParent;
    UpdateParentAdd $name
}


proc UpdateParentAdd { name } {
    # upadte the parent of a menu : sets it as cascade 
    # if necessary.
    # INPUT: name : name of the menu
    #          
    # OUTPUT: void

    global $name Win;
    
    set hparent [ set "$name\(menuparent)"]; 
    set parent $Win($hparent);
    global $parent
    set ppath [ set "$parent\(path)"];
    set path [ set "$name\(path)"];

    set parentstyle  [ set "$parent\(menustyle)"];
    if {[string compare [ set "$parent\(style)"]  uimenu] == 0 } {
	if {[string compare $parentstyle 'cascade']!=0} {
	    menuSetCascadeStyle $parent;
	}
    }
    menuAddToParent $name;
    menuSetAllProp $name;
    MenuRedrawChildren $name;
}




proc menuAddToParent { name } {
    # add a menu to its parent
    # INPUT 
    #  name : name of the menu
    # OUTPUT : void

    global $name Win;

    set hparent [ set "$name\(menuparent)"];
    set parent $Win($hparent);
    global $parent
    set ppath [ set "$parent\(path)"];
    set path [ set "$name\(path)"];
    set index  [ set "$name\(menuindex)"];
    set menustyle [ set "$name\(menustyle)"];
    #$ppath delete $index;
    switch -exact -- $menustyle {
	cascade { $ppath insert $index cascade -menu $path }
	command { $ppath insert $index command  }
    }


}

proc menuSetCascadeStyle { name } {
    # change the menuStyle of a menu as a cascade
    # INPUT :
    #  name :  name of the menu which style to change
    #  
    # OUTPUT : void

    global Win $name;
    
    set hparent [ set "$name\(menuparent)"];

    # We change the style only when the menu is not a 
    # menubar (root menu)
    if { $hparent != 0 } {
	set parent $Win($hparent);
	global $parent;
	set ppath [ set "$parent\(path)"];
	set path [ set "$name\(path)"];
	set index  [ set "$name\(menuindex)"];
	$ppath delete $index;
	$ppath insert $index cascade -menu $path;
	#puts "\nReseting $name style as 'cascade', now posting $path";
	set "$name\(menustyle)" cascade;
	menuSetAllProp $name;
	
    }
}

proc menuSetCommandStyle { name } {
    # change the menuStyle of a menu as a command
    # INPUT :
    #  name :  name of the manu
    #  
    # OUTPUT : void

    global Win $name;
    
    set hparent [ set "$name\(menuparent)"];

    # We change the style only when the menu is not a 
    # menubar (root menu)
    if { $hparent != 0 } {
	set parent $Win($hparent);
	global $parent
	
	# debug info
	#puts "\nChanging menu style of $name to 'command'";
	#puts " --> $name parent handle : $hparent";
	#puts " --> $name parent name : hparent";

	set ppath [ set "$parent\(path)"];
	#puts " --> $name parent path : hparent";
	
	
	set index  [ set "$name\(menuindex)"];
	$ppath delete $index;
	$ppath insert $index command ;
	
	set "$name\(menustyle)" command;
	menuSetAllProp $name;
	
    }
}


######################################################################################
proc resetParent { name phandle } {
    # reset the path of a menu and recreate it
    # input : 'name' variable name of the menu
    #         'phandle' parent handle
    # output : void

    global "$name";
    set rdrw 0;
    
    if {[info exists "$name\(path)"]} {  
	set path [set "$name\(path)"];
	set rdrw 1;
	if [winfo exist $path] { destroy $path }
    }

    
  
    global Win;
    
    # get the parent's name
    set phandle [GetRealParent $phandle menu];
    
    
    set parent "$Win($phandle)";
    # get the menu_parent
    # the parent and menu_parent are different only in the case when
    # the parent is a toplevel (figure) because the menu_parent is
    # then the tpolevel menu defined with the -menu option of a 
    # toplevel (only after tk8.0)
    
    set hmenuParent [GetRealMenuParent $phandle];
     
    set menuParent "$Win($hmenuParent)";
    
    global "$menuParent";
    set menuParentpath [set "$menuParent\(path)"];
    
    set path "$menuParentpath.obj$name" ;
    set "$name\(path)" $path;
    
    # default --> to change !
    menu $path;

}






proc MenuRedrawChildren { name } {
    # redraws the children of a menu

    # INPUT: name : name of the menu which sons to 
    #          redraw
    # OUTPUT:
    
    global $name Win;

    
    # get the path of the menu
    set path [ set "$name\(path)"];
    
    # get the children list
    set ChildList [set "$name\(childrenmenu)"];
    set NbChild [llength $ChildList];
    
    # delete all the children menu
    $path delete 1 $NbChild;
    set handle [set "$name\(handle)"];
    # redraw all the children
    for {set i 0} { $i < $NbChild } {incr i} {
	# Get children name
	set cname [lindex $ChildList $i];
	resetParent $cname $handle;
	MenuRedrawChildren $cname
	global "$cname";
	# Get its path
	set cpath [set "$cname\(path)"];
	# and its style 
	set cstyle [set "$cname\(menustyle)"];
	
	# Now add the child menu to the current menu
	switch -exact -- $cstyle {
	    cascade { $path add cascade -menu $cpath; } 
	    command { $path add command; }
	}
	menuSetAllProp $cname;

    }
    
}



proc menuSetField { handle property value } {
    # set a menu property. The default behaviour is the following:
    # store the property string of the menu, and if the parent exists,
    # set the property. The parent presence can't be avoided because
    # in tk, the parent holds all the property.
    
    # INPUT: 
    #  handle : menu handle
    #  property: property to set up
    #  value: property value

    # OUTPUT: void
    
    global Win MenuPropTrad;
    set name $Win($handle);
    global "$name"; 
    
   
    
    switch -exact -- $property {
	parent {
	    SetMenuParent $name $value;
	}

	callback {
	    menuSetCallback $name $value;
	}
	
	default {
	    set parent [set "$name\(menuparent)"];
	    set parentName $Win($parent);
	    
	    # determinate the entry index in the parent 
	    # children list
	    global "$parentName";
	    set index [set "$name\(menuindex)"];
	    
	    
	    # set the property in the parent
	    set path [set "$parentName\(path)"];
	    
	    set tkprop [set "MenuPropTrad\($property)"];
	    if {[string compare $tkprop "tag"] != 0} {
		$path entryconfigure $index -$tkprop $value;
	    }
	    set "$name\($property)" $value; 
	    #puts "\nSetting $name $property to $value:";
	    #puts " --> $name parent name : $parentName";
	    #puts " --> $name parent path : $path";
	    #puts " --> $name index in parent child list : $index";
	}
	
    }
}


proc menuSetAllProp { name } {
    # set all the property of a menu 

    # INPUT 
    #  name : menu name

    # OUTPUT : void

    global "$name";
    set handle [set "$name\(handle)"];

    foreach prop { label callback} {
	if { [info exists "$name\($prop)"] } {
	    set value [set "$name\($prop)"];
	    menuSetField $handle $prop $value;
	}

    }

}

    

proc GetRealMenuParent { hparent } {
    # determine the real parent of a menu.
    # This routined is usefull to take into account
    # the case where the parent is not a menu but a figure.
    
    # INPUT: hparent : handle of the parent
    # OUTPUT: handle of the real menu_parent

    
    global Win;
    # get the parent's name
    set parent "$Win($hparent)";
    global "$parent";
    
    # initialise the return value
    set handle $hparent;

    set parentStyle [set "$parent\(style)"];
    
    # if the parent is the figure, then the real parent
    # is the associated menubar.
    if { [string compare $parentStyle "figure"]==0 } {
	# when the figure has a menu, its name is:
	set parentname "$parent\menu";
	global "$parentname";
	
	if { [info exists $parentname ]==1 } {    
	    set handle [set "$parentname\(handle)"];
	} else {    
	    set handle [GetRootMenu $hparent ];
	}
    }
  
    
    return $handle;

}





proc menuSetCallback { name callback } {
    # set the command to execute when a menu is 
    # clicked on. When the man has children, the
    # command is executed before the submenu is posted
    # 
    # INPUT : 
    #   handle : handle of the menu
    #   callback : string containing the command 
    #             to execute (in scilab language)
    # OUTPUT : void

    global Win $name;

    

    set style [set "$name\(menustyle)"];
    set command "SciCallback $name $callback";
    
    switch -exact -- $style {

	command { 
	    set parent [set "$name\(menuparent)"];
	    set parentName $Win($parent);

	    global "$parentName";
	    set index [set "$name\(menuindex)"];
	    
	    # set the property in the parent
	    set path [set "$parentName\(path)"];
	    
	    $path entryconfigure $index -command $command;
	    set "$name\(callback)" $command; 

	}
    
	cascade {
	    set path [set "$name\(path)"];
	    $path configure -postcommand $command;
	    set "$name\(callback)" $command; 
	}

    }


}