function val=%operatio_e(field,m2scitlist)
// File generated from %PROTO_e.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// V.C.
val=[]
if field=="infer" then
  val=m2scitlist.out(1).infer
elseif field=="dims" then
  val=m2scitlist.out(1).infer.dims
elseif field=="type" then
  val=m2scitlist.out(1).infer.type
elseif field=="vtype" then
  val=m2scitlist.out(1).infer.type.vtype
elseif field=="property" then
  val=m2scitlist.out(1).infer.type.property
elseif field=="contents" then
  val=m2scitlist.out(1).infer.contents
else
  error("Extraction of "+string(field)+" from ''"+typeof(m2scitlist)+"'' tlist is not yet implemented !")
end
endfunction
