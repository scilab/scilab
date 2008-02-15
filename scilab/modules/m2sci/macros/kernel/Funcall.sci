// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function funcall_tlist=Funcall(name,lhsnb,rhslist,lhslist)
// Copyright INRIA
// Create a new function tlist

rhs=argn(2)

// Verify input value
if rhs~=4 then
  error(gettext("Wrong number of inputs."));
elseif typeof(name)~="string" then
  error(msprintf(gettext("name must be a string instead of a: %s."),typeof(name))); 
elseif typeof(lhsnb)~="constant" then
  error(msprintf(gettext("lhsnb must be a constant instead of a: %s."),typeof(lhsnb))); 
elseif typeof(rhslist)~="list" & rhslist<>[] then
  error(msprintf(gettext("rhslist must be a list instead of a: %s."),typeof(rhslist))); 
elseif typeof(lhslist)~="list" then
  error(msprintf(gettext("lhslist must be a list instead of a: %s."),typeof(lhslist))); 
end  

funcall_tlist=tlist(["funcall","name","lhsnb","rhs","lhs"],name,lhsnb,rhslist,lhslist)
endfunction
