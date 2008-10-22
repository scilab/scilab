function txt=type_infos(val)

if isstr(val)
  txt='String|Real';
elseif issparse(val)
  if isreal(val)
    txt='Sparse|Real';
  else
    txt='Sparse|Complex';
  end
elseif islogical(val)
  txt='Boolean|Real';
else
  if isreal(val)
    txt='Double|Real';
  else
    txt='Double|Complex';
  end
end  
