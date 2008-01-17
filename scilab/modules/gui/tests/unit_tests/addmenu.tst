// Unitary tests for Scilab Java menu created using addmenu
// Vincent COUVERT - INRIA 2008

// Add menus to main window (console)
addmenu('Menu_without_submenu')
Menu_without_submenu='disp(''hello'')'

addmenu('Hello',['Franck';'Peter'])
Hello=['disp(''hello Franck'')';'disp(''hello Peter'')']

addmenu('Bye',list(0,'French_Bye'))
French_Bye='disp(''Au revoir'')'

// Add menus to a graphic window
addmenu(0, 'Menu_without_submenu')
Menu_without_submenu_0='disp(''hello'')'

addmenu(0,'Hello',['Franck';'Peter'])
Hello_0=['disp(''hello Franck'')';'disp(''hello Peter'')']

addmenu(0, 'Bye',list(0,'French_Bye'))
French_Bye_0='disp(''Au revoir'')'

