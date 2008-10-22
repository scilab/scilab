// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Unitary tests for Scilab Java menu created using addmenu

// Add menus to main window (console)
if getscilabmode() == 'STD' then
 
  addmenu('Menu_without_submenu')
  Menu_without_submenu='disp(''hello'')'

  addmenu('Hello',['Franck';'Peter'])
  Hello=['disp(''hello Franck'')';'disp(''hello Peter'')']

  addmenu('Bye',list(0,'French_Bye'))
  French_Bye='disp(''Au revoir'')'
end

// Add menus to a graphic window
addmenu(0, 'Menu_without_submenu')
Menu_without_submenu_0='disp(''hello'')'

addmenu(0,'Hello',['Franck';'Peter'])
Hello_0=['disp(''hello Franck'')';'disp(''hello Peter'')']

addmenu(0, 'Bye',list(0,'French_Bye'))
French_Bye_0='disp(''Au revoir'')'

// a rough test which used to freeze Scilab
for i = 1:1000,
  addmenu(0,'Hello',['john','micheal','ernest','franck','eddie','marcel','nestor','Hypolite','Peter','Jack','Patrick','Rodrigo']);
  addmenu(0,'Hello2',['john','micheal','ernest','franck','eddie','marcel','nestor','Hypolite','Peter','Jack','Patrick','Rodrigo']);
  delmenu(0,'Hello');
  delmenu(0,'Hello2');
end

