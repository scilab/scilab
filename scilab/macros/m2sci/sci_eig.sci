function [stk,txt,top]=sci_eig()
// Copyright INRIA
txt=[]
if rhs==1 then
  if lhs==1 then
    stk=list('spec'+'('+stk(top)(1)+')','0',stk(top)(3),'1','?')
  else
    //eig is called with two lhs arguments. Get their names
    [v,d]=lhsvarsnames()
    txt=lhsargs([d,v])+'=bdiag('+stk(top)(1)+'+0*%i,1/%eps)'
    sz=stk(top)(3)
    //eig output variable are already defined in txt, set expr_type to '-2' 
    //for both
    stk=list(list('?','-2',sz,sz,'?'),list('?','-2',sz,sz,'?'))
  end
else // more than one input argument (option or generalized eigen values?)
  if stk(top)(5)=='10' then  //option
    set_infos('Eig : '''+stk(top)(1)+''' option not translated',2)
    top=top-1
    if lhs==1 then
      stk=list('spec'+'('+stk(top)(1)+')','0',stk(top)(3),'1','?')
    else
      [v,d]=lhsvarsnames()
      txt=lhsargs([d,v])+'=bdiag('+stk(top)(1)+'+0*%i,1/%eps)'
      sz=stk(top)(3)
      stk=list(list('?','-2',sz,sz,'?'),list('?','-2',sz,sz,'?'))
    end
    return
  end
  if stk(top)(5)<>'1' then
    set_infos('Eig with 2 rhs args: generalized eigen assumed. Check ',2)
  end  
  m=stk(top-1)(3)
  n=stk(top-1)(4)
  if lhs==1 then
    al=gettempvar(1)
    be=gettempvar(2)
    txt=[txt;
        lhsargs([al,be])+' = gspec'+rhsargs([stk(top-1)(1),stk(top)(1)])+';']
    stk=list(al+'./'+be,'1',m,'1','1')
  else
    al=gettempvar(1)
    be=gettempvar(2)
    [V,D]=lhsvarsnames()
    txt=[txt;
        lhsargs([al,be,V])+' = gspec'+rhsargs([stk(top-1)(1),stk(top)(1)])+';'
        D+' = diag('+al+'./'+be+');']
    stk=list(list(' ','-2',m,n,'1'),list(' ','-2',m,n,'1'))
  end
end    
endfunction
