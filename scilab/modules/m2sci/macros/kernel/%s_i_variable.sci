// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function var=%s_i_variable(field,cste,var)

if field=="vtype" then
  var.infer.type.vtype=cste
elseif field=="property" then
  var.infer.type.property=cste
elseif field=="contents" then
  var.infer.contents=cste
else
  error(gettext("Not yet implemented."))
end
endfunction
