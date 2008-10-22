
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=ge_shortcut(c)

  r=%t
  
  ShortCuts=EGdata.ShortCuts
  Menus=EGdata.Menus
  
  k=find(ShortCuts(:,1)==ascii(c))

  if k<>[] then 
    m=[]
    for i=1:size(Menus)
      l=find(Menus(i)==ShortCuts(k,2))
      if l<>[] then m=i;break;end
    end
    if m==[] then x_message('incoherent shortcut definitions'),r=%f,return,end
    execstr('ge_'+convstr(Menus(m)(1))+'(l-1,win)')
  end
  
endfunction
