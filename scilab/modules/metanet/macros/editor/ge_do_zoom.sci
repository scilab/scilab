
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function EGdata=ge_do_zoom(EGdata)

  Z=[12.5 25:25:400]
  c=x_choose(['Other value',string(Z)+'%'],'Choose a a zoom factor')
  if c==0 then return,end
  c=c-1;
  if c==0 then 
    cl=string(100*EGdata.Zoom)
    while c<=0 then
      [ok,c,cl]=getvalue('Enter a zoom factor (%)','Factor',list('vec',1),cl)
      if ~ok then return,end
    end
  else
    c=Z(c)
  end
  
  EGdata.Zoom=c/100;
  EGdata.Edited=%t
  ge_do_replot(EGdata.GraphList)
endfunction
