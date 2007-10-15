function menu_stuff()
// Copyright INRIA

if ~MSDOS then
  delmenu(curwin,'3D Rot.')
  delmenu(curwin,'UnZoom')
  delmenu(curwin,'Zoom')
  delmenu(curwin,'Edit')
  delmenu(curwin,'File')
  delmenu(curwin,'Insert')
else
  hidetoolbar(curwin)
 // French
  delmenu(curwin,'&Fichier')
  delmenu(curwin,'&Editer')
  delmenu(curwin,'&Outils')
  delmenu(curwin,'&Inserer')
  // English
  delmenu(curwin,'&File')
  delmenu(curwin,'&Edit')
  delmenu(curwin,'&Tools')
  delmenu(curwin,'&Insert')
  end
menuss=menus;
menuss(1)=menus(1)(2:$);
menubar(curwin,menuss)
	  
endfunction
