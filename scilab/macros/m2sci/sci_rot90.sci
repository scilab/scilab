function [stk,txt,top]=sci_rot90()
// Copyright INRIA
txt=[]
if rhs==1 then
  if isname(stk(top)(1)) then
    stk=list(stk(top)(1)+'(:,$:-1:1).''','0',stk(top)(3),stk(top)(4),'1')
  else
    v=gettempvar()
    txt=v+' = '+stk(top)(1)
    stk=list(v+'(:,$:-1:1).''','0',stk(top)(3),stk(top)(4),'1')
  end
else
  if isname(stk(top-1)(1)) then
    v=stk(top-1)(1)
  else
    v=gettempvar()
    txt=v+' = '+stk(top-1)(1)
  end
  
  k=stk(top)(1)
  if isnum(k) then
    k=evstr(k)
    k = k-fix(k./4).*4;
    if k<0 then k = k+4;end
    if k==1 then
      stk=list(v+'(:,$:-1:1).''','0',stk(top)(3),stk(top)(4),'1')
    elseif k==2 then
      stk=list(v+'($:-1:1,$:-1:1)','0',stk(top)(3),stk(top)(4),'1')
    elseif k==3 then
      stk=list(v+'($:-1:1,:).''','0',stk(top)(3),stk(top)(4),'1')
    else
      stk=list(stk(top-1)(1),'0',stk(top)(3),stk(top)(4),'1')
      txt=[]
    end
    top=top-1
    return
  elseif isname(stk(top)(1)) then
    k=stk(top)(1)
  else
    k=gettempvar(1)
    txt=k+' = '+stk(top)(1)
  end
  B=gettempvar(2)
  txt=[txt;
      k+' = '+k+'-fix('+k+'./4).*4;'
      'if '+k+'<0 then '+k+' = '+k+'+4;end'
      'if '+k+'==1 then'
      '  '+B+' = '+v+'(:,$:-1:1).'''
      'elseif '+k+'==2 then'
      '  '+B+' = '+v+'($:-1:1,$:-1:1)'
      'elseif '+k+'==3 then'
      '  '+B+' = '+v+'($:-1:1,:).'''
      'else'
      '  '+B+' = '+v
      'end']

  stk=list(B,'0',stk(top-1)(3),stk(top-1)(4),stk(top-1)(5))
  top=top-1
end
endfunction
