// Vincent COUVERT - Copyright INRIA 2008
function Cmenu = createpopup(listOfMenus)

menuLabels = [];

// Read the labels
for menu=listOfMenus
  if type(menu) == 10
    menuLabels($+1) = menu;
  else
    error(gettext("createPopup: cascading menus not yet implemented"));
  end
end

// Create the popyp
Cmenu = uipopup(menuLabels);
endfunction
