function val=%m2scivar_e(field,m2scitlist)
// Copyright INRIA
// V.C.

val=[]
if field=="infer" then
  val=m2scitlist.infer
elseif field=="dims" then
  val=m2scitlist.infer.dims
elseif field=="type" then
  val=m2scitlist.infer.type
elseif field=="vtype" then
  val=m2scitlist.infer.type.vtype
elseif field=="property" then
  val=m2scitlist.infer.type.property
elseif field=="contents" then
  val=m2scitlist.infer.contents
else
  error("Extraction of "+string(field)+" from ''"+typeof(m2scitlist)+"'' tlist is not yet implemented !")
end
endfunction
