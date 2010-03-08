function txt=type_infos(resStruct)

if strcmp(resStruct.class, 'char')==1
  txt='String|Real';
elseif resStruct.sparse==1
  if resStruct.complex==0
    txt='Sparse|Real';
  else
    txt='Sparse|Complex';
  end
elseif strcmp(resStruct.class, 'logical')==1
  txt='Boolean|Real';
else
  if resStruct.complex==0
    txt='Double|Real';
  else
    txt='Double|Complex';
  end
end  
