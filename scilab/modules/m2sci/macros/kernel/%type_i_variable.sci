// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function var=%type_i_variable(field,inslist,var)

if typeof(inslist)=="ce" then
  if field=="type" then
    var.infer=inslist
  else
    error(gettext("Not yet implemented."))
  end
elseif typeof(inslist)=="st" then
  pause
else
  if or(typeof(var.infer)==["ce","st"]) then
    if field=="type" then
      dims=list()
      for kd=1:size(var.infer.dims,"*")
	dims(kd)=var.infer.dims(kd)
      end
      var.infer=Infer(dims,Type())
      var.infer.type=inslist
    else
    error(gettext("Not yet implemented."))
    end
  else 
    if field=="type" then
      var.infer.type=inslist
    else
    error(gettext("Not yet implemented."))
    end
  end
end
endfunction
