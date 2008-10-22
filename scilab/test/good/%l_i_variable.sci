function var=%l_i_variable(field,inslist,var)
// Copyright INRIA
if field=="dims" then
  var.infer.dims=inslist
elseif field=="type" then
  var.infer.type=inslist
elseif field=="contents" then
  var.infer.contents=inslist
else
  error("Not yet implemented...")
end
endfunction
