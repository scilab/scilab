function variable=%contents_i_variable(field,contlist,variable)
// Copyright INRIA
// V.C.

if field=="contents" then
  variable.infer.contents=contlist
else
  error("Not yet implemented...")
end
endfunction
