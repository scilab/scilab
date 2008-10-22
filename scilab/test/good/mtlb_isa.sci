function r=mtlb_isa(OBJ,class)
select class
case "logical"
  r=type(OBJ)==4
case "char"
  r=type(OBJ)==10
case "numeric"
  r=or(type(OBJ)==[1,5,8])
case "int8"
  r=typeof(OBJ)=="int8"
case "uint8"
  r=typeof(OBJ)=="uint8"
case "int16"
  r=typeof(OBJ)=="int16"
case "uint16"
  r=typeof(OBJ)=="uint16"
case "int32"
  r=typeof(OBJ)=="int32"
case "uint32"
  r=typeof(OBJ)=="uint32"
case "single"
  r=type(OBJ)==1
case "double"
  r=type(OBJ)==1
case "cell"
  r=typeof(OBJ)=="ce" 
case "struct"
  r=typeof(OBJ)=="st" 
case "function_handle"
 r=type(OBJ)==13
case "sparse"
 r=type(OBJ)==5
case "lti"
   r=typeof(OBJ)=="state-space"
else
  error("Unhandled class: "+class)
end
endfunction
