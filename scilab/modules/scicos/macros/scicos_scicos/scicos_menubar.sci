function scicos_menubar(win,menus)
//  Scicos
//
//  Copyright (C) 2008 - INRIA - Simone Mannori
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ./license.txt
//

names = menus(1); 
for k = size(names,'*'):-1:1
  delmenu(win,names(k))
end

for k=1:size(names,'*')

  //** "Safe" version without ALT shortcut   
  //** addmenu(win, names(k), menus(k+1), list(0,names(k))); 

  //** Automatically add the [ALT] menufile shortcut  
  addmenu(win, "&"+names(k), menus(k+1), list(0,names(k))); 
end

datas = []; 

endfunction
