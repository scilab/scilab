// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [io,s]=syssize(sys)
//Old stuff
//  io=syssize(sys)
//  [io,ns]=syssize(sys)
//
//   sys   : syslin list
//   io    : io=[nout,nin] 
//                nout:  nb. ouputs
//                nin :  nb. inputs
//   s    : nb states.

select type(sys)
case 1 then
  io=size(sys)
  s=[]
 case 16 then
  sys1=sys(1)
  select sys1(1)
  case 'lss' then
    io=size(sys('D')),
    [s,s]=size(sys('A'))
   case 'r' then
    io=size(sys('den'))
    [lhs,rhs]=argn(0);
    if lhs==2 then  sys=tf2ss(sys);[s,s]=size(sys('A')),end
  else 
    error(97,1)
  end 
else
  error(97,1),
end
endfunction
