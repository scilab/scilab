function r=mtlb_isa(OBJ,class)
select class
case 'cell'
 r=%f
 if type(OBJ)==17 then
   t= getfield(1,OBJ)
   if t(1)=='ce' then r=%t,end
 end
case 'struct'
 r=%f
 if type(OBJ)==17 then
   t= getfield(1,OBJ)
   if t(1)=='st' then r=%t,end
 end
case 'char'
 r=type(OBJ)==10
case 'double'
 r=type(OBJ)==1
case 'int8'
 r=type(OBJ)==1
case 'int8'
 r=type(OBJ)==8&inttype(OBJ)==1
case 'function_handle'
 r=type(OBJ)==13
case 'numeric'
 r=type(OBJ)==1|type(OBJ)==8|type(OBJ)==5
case 'single'
  r=type(OBJ)==1
case 'sparse'
 r=type(OBJ)==5
case 'uint8'
 r=type(OBJ)==8&inttype(OBJ)==11
case 'lti'
   r=typeof(OBJ)=='state-space'
else
  error('unhandled class :'+class)
end
endfunction
