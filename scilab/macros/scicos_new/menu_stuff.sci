function menu_stuff()

if ~MSDOS then
  delmenu(curwin,'3D Rot.')
  delmenu(curwin,'UnZoom')
  delmenu(curwin,'Zoom')
  delmenu(curwin,'Edit')
  delmenu(curwin,'File')
else
  hidetoolbar(curwin)
 // French
  delmenu(curwin,'&Fichier')
  delmenu(curwin,'&Editer')
  delmenu(curwin,'&Outils')
  // English
  delmenu(curwin,'&File')
  delmenu(curwin,'&Edit')
  delmenu(curwin,'&Tools')
  end
menuss=menus;
menuss(1)=menus(1)(2:$);
menubar(curwin,menuss)
	  
endfunction
