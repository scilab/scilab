function op=%st_i_operatio(field,insst,op)
// Copyright INRIA
if field=="contents" then
  op.out(1).infer.contents=insst
else
  error("Not yet implemented...")
end
endfunction
