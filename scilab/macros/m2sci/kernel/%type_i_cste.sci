function cste=%type_i_cste(field,inslist,cste)
// Copyright INRIA

if field=="type" then
  cste.infer.type=inslist
else
  error("Not yet implemented...")
end
endfunction
