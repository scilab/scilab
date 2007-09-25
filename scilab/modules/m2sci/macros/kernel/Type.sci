function Type_tlist=Type(vtype,property)
// Copyright INRIA
// Create a new 'type' tlist for inference
rhs=argn(2)
if rhs<2 then
  Type_tlist=tlist(["type","vtype","property"],Unknown,Unknown)
  return
end

if and(vtype~=[Double,Boolean,String,Unknown,Sparse,Cell,Struct,Int,Handle]) then
  error(msprintf(gettext("errors","%s is not yet implemented."),string(vtype)))
end

if typeof(property)=="list" then
  error(gettext("errors","list of properties is not yet implemented."))
elseif and(property~=[Real,Complex,Unknown]) then
  error(msprintf(gettext("errors","wrong property %s."),string(property)))
end  

Type_tlist=tlist(["type","vtype","property"],vtype,property)
endfunction
