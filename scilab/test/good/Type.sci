function Type_tlist=Type(vtype,property)
// Copyright INRIA
// Create a new 'type' tlist for inference
rhs=argn(2)
if rhs<2 then
  Type_tlist=tlist(["type","vtype","property"],Unknown,Unknown)
  return
end

if and(vtype~=[Double,Boolean,String,Unknown,Sparse,Cell,Struct,Int,Handle]) then
  error("Type(): "+string(vtype)+" is not yet implemented !")
end

if typeof(property)=="list" then
  error("Type(): list of properties is not yet implemented !")
elseif and(property~=[Real,Complex,Unknown]) then
  error("Type(): wrong property "+string(property)+"!")   
end  

Type_tlist=tlist(["type","vtype","property"],vtype,property)
endfunction
