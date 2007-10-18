function operation=%contents_i_operatio(field,contlist,operation)
// Copyright INRIA
// V.C.

if field=="contents" then
  operation.out(1).infer.contents=contlist
else
  error(gettext("Not yet implemented."))
end
endfunction
