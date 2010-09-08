// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function var=M2scivar(sciname,matname,infer,call_level)
// Create a new 'm2scivar' tlist

rhs=argn(2)

if rhs<3 then
  error(gettext("Wrong number of inputs."))
end
if rhs==3 then
  call_level=0
end

if typeof(sciname)~="string" then
  error(msprintf(gettext("sciname must be a string instead of a: %s."),typeof(sciname)));
elseif typeof(matname)~="string" then
  error(msprintf(gettext("matname must be a string instead of a: %s."),typeof(matname)));
elseif typeof(infer)~="infer" then
  error(msprintf(gettext("infer must be an ''infer'' tlist instead of a: %s."),typeof(infer)));
end

var=tlist(["m2scivar","sciname","matname","infer","level"],sciname,matname,infer,call_level)

endfunction
