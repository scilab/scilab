function var=%st_i_variable(field,insst,var)
// Copyright INRIA
if field=="contents" then
  var.infer.contents=insst
else
  error("Not yet implemented...")
end
endfunction
