function menu_stuff()

if ~MSDOS then
  delmenu(curwin,'3D Rot.')
  delmenu(curwin,'UnZoom')
  delmenu(curwin,'Zoom')
  delmenu(curwin,'File')
else
 // French
  delmenu(curwin,'&Fichier')
  delmenu(curwin,'&Editer')
  delmenu(curwin,'&Utilitaires')
  // English
  delmenu(curwin,'&File')
  delmenu(curwin,'&Edit')
  delmenu(curwin,'&Tools')
  end
menuss=menus;
menuss(1)=menus(1)(2:$);
menubar(curwin,menuss)
	  
endfunction
