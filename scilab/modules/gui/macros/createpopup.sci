// Vincent COUVERT - Copyright INRIA 2008
// Create a popupmenu and waits for a user input
function Cmenu = createpopup(listOfMenus)

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
      error(gettext("createPopupItems: empty cascading menu."));
    end
  end
end
endfunction
