// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function it=invsyslin(t)

  if typeof(t)<>'state-space' then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space expected.\n"),"invsyslin",1))
  end

  [p,m]=size(t.D);
  if p <> m then  
    warning(msprintf(gettext("%s: Wrong size for input argument #%d: Square system expected.\n"),"invsyslin",1)),
  end
  //
  d=pinv(t.D);
  it=syslin(t.dt,t.A-t.B*d*t.C,t.B*d,-d*t.C,d,t.X0);
endfunction
