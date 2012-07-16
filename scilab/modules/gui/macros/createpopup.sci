// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Create a popupmenu and waits for a user input
function Cmenu = createpopup(listOfMenus)

warnobsolete("uicontextmenu", "5.4.1");

// Example of use
if argn(2) <=0 then
  listOfMenus = list("menu1",..
                     "menu2",..
                     list("menuwithsubmenus", ..
                          "submenu1",..
                          list("submenu2", ..
                               "subsubmenu1",..
                               list("subsubmenu2", "subsubsubmenu"),..
                               "subsubumen3"), ..
                          "submenu3"),..
                     "menu3");
end

// Create a contextmenu
h = uicontextmenu();

// Create its items
createPopupItems(h, listOfMenus);

// Display and wait for a user answer
Cmenu = uiwait(h);

endfunction

// Vincent COUVERT - Copyright INRIA 2008
function createPopupItems(parent, listOfMenus)

for menu=listOfMenus
  if type(menu) == 10
    h1 = uimenu("Parent", parent, "label", menu);
  else
    if length(menu) >= 2
      h1 = uimenu("Parent", parent, "label", menu(1));
      menu(1)=null();
      createPopupItems(h1, menu);
    else
      error(gettext("createPopupItems: found an empty cascading menu."));
    end
  end
end
endfunction
