
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_set_nodes_id(w)
  global EGcurrent
  execstr('global EGdata_'+string(EGcurrent))

  if type(w)==1 then
    w=find((0:4)==w)
  else
    w=find(['Nothing','Number','Name','Demand','Label']==w)
  end
  if w<>[] then       
    execstr('EGdata_'+string(EGcurrent)+'.NodeId='+string(w-1))
  end
endfunction
