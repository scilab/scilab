function txt=type_infos(resval)
// Copyright INRIA

if type(resval)==10 then
  txt="String|Real";
elseif type(resval)==4 then
  txt="Boolean|Real";
elseif type(resval)==6 then
  txt="Sparse|Real";
elseif type(resval)==5 then
  resval; // Should not work if this line removed !!!
  if isreal(resval,0) then
    txt="Sparse|Real";
  else
    txt="Sparse|Complex";
  end  
else
  if isreal(resval,0) then
    txt="Double|Real";
  else
    txt="Double|Complex";
  end  
end

endfunction
