// Vincent COUVERT - Copyright INRIA 2008
function createpopup(listOfMenus)

menuLabels = [];

// Read the labels
for menu=listOfMenus
  if type(menu) == 10
    menuLabels($+1) = menu;
  else
    erreor(gettext("createPopup: cascading menus not yet implemented"));
  end
end

// Create the popyp
mpopup(menuLabels);

endfunction
