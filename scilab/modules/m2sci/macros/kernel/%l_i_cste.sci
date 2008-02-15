// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function cste=%l_i_cste(field,inslist,cste)

if field=="dims" then
  cste.infer.dims=inslist
elseif field=="type" then
  cste.infer.type=inslist
else
  error(gettext("Not yet implemented."))
end
endfunction
