function op=%ce_i_operatio(field,insce,op)
// Copyright INRIA
if field=="contents" then
  op.out(1).infer.contents=insce
else
  error("Not yet implemented...")
end
endfunction
