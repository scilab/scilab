// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function op=%s_i_operatio(field,cste,op)
if field=="vtype" then
  op.type.vtype=cste
elseif field=="property" then
  op.type.property=cste
else
  error(gettext("Not yet implemented."))
end
endfunction
