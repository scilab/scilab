function val=%funcall_e(field,m2scitlist)
// File generated from %PROTO_e.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// V.C.
val=[]
if field=="infer" then
  val=m2scitlist.lhs(1).infer
elseif field=="dims" then
  val=m2scitlist.lhs(1).dims
elseif field=="type" then
  val=m2scitlist.lhs(1).type
elseif field=="vtype" then
  val=m2scitlist.lhs(1).vtype
elseif field=="property" then
  val=m2scitlist.lhs(1).property
elseif field=="contents" then
  val=m2scitlist.lhs(1).contents
else
  error("Extraction of "+string(field)+" from ''"+typeof(m2scitlist)+"'' tlist is not yet implemented !")
end
endfunction
