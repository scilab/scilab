function addmenu(varargin)
// Add menus in Scilab Windows
// See comments in the code for different implementations

// Copyright INRIA

// Allan CORNET 2005
// Vincent COUVERT 2007: Java version

rhs=argn(2);

select rhs
case 1
  if type(varargin(1))==10
    // addmenu(menu_label)
    // Create a menu in Scilab main Window
    // The callback is the variable named menu_label
    
    if size(varargin(1),"*") <> 1
      error(_("First parameter must be a single string."));
    end
    
    addSingleMenu(0, varargin(1));

  else
    error(_("First parameter must be a string."));
  end
case 2
  if type(varargin(1))==10 & type(varargin(2))==10
    // addmenu(menu_label, submenus_labels)
    // Create a menu in Scilab main Window with submenus
    // The callbacks are the items of variable named menu_label
    
    if size(varargin(1),"*") <> 1
      error(_("First parameter must be a single string."));
    end
    
    // No verification for submenus labels
    
    addMenuWithSubMenus(0, varargin(1), varargin(2));
    
  elseif type(varargin(1))==10 & type(varargin(2))==15
    // addmenu(menu_label, action_in_a_list)
    // Create a menu in Scilab main Window with submenus
    // The callbacks are the items of variable named menu_label
    
    if size(varargin(1),"*") <> 1
      error(_("First parameter must be a single string."));
    end
    
    if size(varargin(2)) <> 2
      error(_("Second parameter must be a two-item list."));
    end
    
    addSingleMenuCallback(0, varargin(1), varargin(2));

  elseif type(varargin(1))==1 & type(varargin(2))==10  
    // addmenu(graphic_window_id,menu_label)
    // Create a menu in Scilab main Window
    // The callback is the variable named menu_label
    
    if size(varargin(1),"*") <> 1
      error(_("First parameter must be a single value."));
    end
    
    if size(varargin(2),"*") <> 1
      error(_("Second parameter must be a single string."));
    end
    
    addSingleMenu(scf(varargin(1)), varargin(2))
  
  else
    error(_("Bad use of addmenu with two input parameters."));
  end
case 3
  if type(varargin(1))==10 & type(varargin(2))==10 & type(varargin(3))==15
    // addmenu(menu_label, submenus_labels, actions_in_a_list)
    // Create a menu in Scilab main Window with submenus
    // The callbacks are given in the third parameter

    if size(varargin(1),"*") <> 1
      error(_("First parameter must be a single string."));
    end
    
    // No verification for submenus labels size
    
    if size(varargin(3)) <> 2
      error(_("Third parameter must be a two-item list."));
    end

    addMenuSubMenusCallback(0, varargin(1), varargin(2), varargin(3));
    
  elseif type(varargin(1))==1 & type(varargin(2))==10 & type(varargin(3))==10
    // addmenu(graphic_window_id, menu_label, submenus_labels)
    // Create a menu in Scilab main Window with submenus
    // The callbacks are the items of variable named menu_label
    
    if size(varargin(1),"*") <> 1
      error(_("First parameter must be a single value."));
    end
    
    if size(varargin(2),"*") <> 1
      error(_("Second parameter must be a single string."));
    end
  
    // No verification for submenus labels
    
    addMenuSubMenus(scf(varargin(1)), varargin(2), varargin(3));
    
  elseif type(varargin(1))==1 & type(varargin(2))==10 & type(varargin(3))==10
    // addmenu(graphic_window_id, menu_label, action_in_a_list)
    // Create a menu in Scilab main Window with submenus
    // The callbacks are the items of variable named menu_label

    if size(varargin(1),"*") <> 1
      error(_("First parameter must be a single value."));
    end
    
    if size(varargin(2),"*") <> 1
      error(_("Second parameter must be a single string."));
    end

    if size(varargin(3)) <> 2
      error(_("Second parameter must be a two-item list."));
    end

    addSingleMenuCallback(scf(varargin(1)), varargin(2), varargin(3));
    
  else
    error(_("Bad use of addmenu with three input parameters."));
  end
case 4
    // addmenu(graphic_window_id, menu_label, submenus_labels, action_in_a_list)
    // Create a menu in a Scilab graphic Window with submenus
    // The callbacks are given in a list
    
    if size(varargin(1),"*") <> 1
      error(_("First parameter must be a single value."));
    end
    
    if size(varargin(2),"*") <> 1
      error(_("Second parameter must be a single string."));
    end
    
    // No verification for submenus labels
    
    if size(varargin(4)) <> 2
      error(_("Second parameter must be a two-item list."));
    end

    addMenuSubMenusCallback(scf(varargin(1)), varargin(2), varargin(3), varargin(4));

else
  error(_("Wrong number of input parameters."));
end
endfunction
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Add a menu with no callback given by the user
//------------------------------------------------------------------------------
function addSingleMenu(fig, menulabel)
UImenu("parent", fig, "label", menulabel);//, "callback", "execstr(""" + menulabel + """)");
endfunction
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Add a menu with callback given by the user
//------------------------------------------------------------------------------
function addSingleMenuCallback(fig, menulabel, callback)

// TODO: Code for callback should be factorized with addSingleMenuWithSubMenusWithCallback
callbackType = callback(1);
if type(callbackType)<>1 | size(callbackType,"*")<>1
  error(_("Callback type must be a 0, 1 or 2"));
end

callbackStr = callback(2);
if size(callbackStr,"*")<>1
  error(_("Callback must be a single string."));
end

warning(_("Callback list not yet managed in addmenu."));
// TODO: end of code to factorize

UImenu("parent",fig,"label",menulabel);

endfunction
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Add a menu and its submenus with no callback given by the user
//------------------------------------------------------------------------------
function addMenuSubMenus(fig, menulabel, submenuslabels)

h0 = UImenu("parent", fig, "label", menulabel);

for K=1:size(submenuslabels,"*")
  UImenu("parent", h0, "label", submenuslabels(K));//, "callback", "execstr("""+submenuslabels+"("+K+")"")");
end

endfunction
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Add a menu and its submenus with callback given by the user
//------------------------------------------------------------------------------
function addMenuSubMenusCallback(fig, menulabel, submenuslabels, callback)

// TODO: Code for callback should be factorized with addSingleMenuWithCallback
callbackType = callback(1);
if type(callbackType)<>1 | size(callbackType,"*")<>1
  error(_("Callback type must be a 0, 1 or 2"));
end

callbackStr = callback(2);
if size(callbackStr,"*")<>1
  error(_("Callback must be a single string."));
end

warning(_("Callback list not yet managed in addmenu."));
// TODO: end of code to factorize

h0 = UImenu("parent",fig,"label",menulabel);

for K=1:size(submenuslabels,"*")
  UImenu("parent",h0,"label",submenuslabels(K));
end
endfunction
//------------------------------------------------------------------------------
