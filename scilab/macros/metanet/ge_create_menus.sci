function ge_create_menus(win)
//Copyright INRIA
//Author : SeRge Steer 2002


if ~MSDOS then
  delmenu(win,'3D Rot.')
  delmenu(win,'UnZoom')
  delmenu(win,'2D Zoom')
  delmenu(win,'Edit')
  delmenu(win,'File')
else 
  hidetoolbar(win)
  // French
  delmenu(win,'&Fichier')
  delmenu(win,'&Editer')
  delmenu(win,'&Outils')
  // English
  delmenu(win,'&File')
  delmenu(win,'&Edit')
  delmenu(win,'&Tools')
end
menus=list([])
for k=1:length(Menus)
  delmenu(win,Menus(k)(1))
end
for k=1:length(Menus)
  if size(Menus(k),'*') >1 then
    addmenu(win,Menus(k)(1),Menus(k)(2:$),list(2,'ge_'+convstr(Menus(k)(1))))
  else
    addmenu(win,Menus(k),list(2,'ge_'+convstr(Menus(k))))
  end
end
	  
endfunction
