function menu_stuff()

if ~MSDOS then
  delmenu(curwin,'3D Rot.')
  delmenu(curwin,'UnZoom')
  delmenu(curwin,'Zoom')
  delmenu(curwin,'File')
else
  delmenu(curwin,'3D &Rot.')
  delmenu(curwin,'&UnZoom')
  delmenu(curwin,'&Zoom')
  delmenu(curwin,'&File')
end
menuss=menus;
menuss(1)=menus(1)(2:$);
menubar(curwin,menuss)
	  
endfunction
