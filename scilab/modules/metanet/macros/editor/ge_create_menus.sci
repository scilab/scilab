
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_create_menus(win)
  toolbar(win, "off");
  delmenu(win,gettext("&File"));
  delmenu(win,gettext("&Tools"));
  delmenu(win,gettext("&Edit"));
  delmenu(win,gettext("&?"));
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
