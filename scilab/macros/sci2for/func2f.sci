function [stk,nwrk,t1,top]=func2f(op,stk,nwrk)
// genere la traduction de l'ensemble des primitives 
//!
// Copyright INRIA
lhs=evstr(op(4))
rhs=abs(evstr(op(3)))
t1=[]
if exists('f_'+op(2)) then
  execstr('[stkr,nwrk,t1,top]=f_'+op(2)+'(nwrk)')
  for k=rhs-lhs:-1:1, stk(top+k)=null(),end
  if lhs>1 then
    top=top-1
    for k=1:lhs
      top=top+1
      stk(top)=stkr(k)
    end
  else
    stk(top)=stkr
  end
else
  args=[]
  for k=1:rhs
    s=stk(top)
    args=[s(1),args]
    top=top-1
  end
  typ='?',typ=typ(ones(1,lhs))'
  rsiz='1';rsiz=rsiz(ones(1,lhs))'
  csiz=rsiz;
  [o,nwrk,t1]=outname(nwrk,typ,rsiz,csiz)
  for k=1:lhs, args=[args,o(k)],end
  txt=['c TRANSLATION OF '+op(2)+' FUNCTION CALL NOT PROPERLY DONE';
       t1
       gencall([op(2),args])]
  stkr=list()
  for k=1:lhs
    top=top+1
    stk(top)=list(o(k),'-1',typ(k),rsiz(k),csiz(k))
  end
end
endfunction
