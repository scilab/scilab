function l=rc_ce2list(rc)
// Copyright INRIA
// Scilab Project - V. Couvert
// Convert a rc of type [[...,...],...] to a list of operands

l=list()
while typeof(rc)=="operation"
  if rc.operator=="rc" then
    if l<>list() then
      for ki=lstsize(l):-1:1
	l($+1)=l($)
      end
    end
    l(1)=rc.operands(2)
    rc=rc.operands(1)
  else
    break
  end
end
for ki=lstsize(l):-1:1
  l($+1)=l($)
end
l(1)=rc

if lstsize(l)==1 then
  l=l(1)
end

endfunction
