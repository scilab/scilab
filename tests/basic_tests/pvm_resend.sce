while %t 
  a=pvm_recv(pvm_parent(),0);
  if type(a)==10 then
    if a=='exit' then exit; end
  end
  pvm_send(pvm_parent(),a,0);
end
