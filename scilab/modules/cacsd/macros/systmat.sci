// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2016 - INRIA
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function Sm=systmat(Sl);
// System matrix of the linear system Sl 
// in state-space form.
// Sm = [-sE + A   B;
//      [    C     D]
// To get the zeros use trzeros
//!
  if typeof(Sl)=="state-space" then
    if Sl.dt=="d"|Sl.dt==[] then
      s=poly(0,"z");
    else
      s=poly(0,"s");
    end
    Sm=[-s*eye(Sl.A)+Sl.A, Sl.B;
         Sl.C,             Sl.D];
  elseif typeof(Sl)=="des" then
    s=poly(0,"s");
    Sm=[-s*Sl.E+Sl.A,   Sl.B;
         Sl.C,          Sl.D];
  else
    error(msprintf(_("%s: Wrong type for input argument #%d: state space or a descriptor representation expected.\n"),"systmat",1))
  end
endfunction
