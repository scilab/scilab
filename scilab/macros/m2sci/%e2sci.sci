function [stk,txt,top]=%e2sci()
// genere le code relatif a l'extraction d'une sous matrice
//!
// Copyright INRIA
txt=[]
rhs=maxi(0,abs(evstr(op(3)))-1)
sn=stk(top);top=top-1
s2=stk(top)
if rhs==1 then
  // jpc 6 oct 16h pause
  if type(s2(1))==1 then // recursive extraction
    n=s2(1)(1)
    m=s2(1)(2)
    ex=sn(1)
    if m>1 then n=n-1,end
    for k=1:n
      ik=s2(k+1)
      //if ik(5)=='10' then
      //  ex=ex+'.'+part(ik(1),2:length(ik(1))-1)
      //else
	ex=ex+'('+ik(1)+')'
      //end
    end
    if m>1 then
      args=[]
      for k=1:m,args=[args s2(n+1+k)(1)],end
      ex=ex+'('+makeargs(args)+')'
      stk=list(ex,'0','?','1','?')
    else
      stk=list(ex,'0','?','1','?')
    end
  elseif sn(5)=='10' then // extraction pour les chaines
    stk=list('part('+sn(1)+','+s2(1)+')','0','1','1',sn(5))
  else
    [mn,nn]=checkdims(sn);
    [m2,n2]=checkdims(s2);
    if s2(1)==':' then
      if mn<>-1&nn<>-1 then
	stk=list(sn(1)+'('+s2(1)+')','0',string(mn*nn),'1',sn(5))
      else
	stk=list(sn(1)+'('+s2(1)+')','0','?','1',sn(5))
      end
    elseif mn==1 then
      stk=list(sn(1)+'('+s2(1)+')','0','1','?',sn(5))
    elseif nn==1 then
      stk=list(sn(1)+'('+s2(1)+')','0','?','1',sn(5))
    elseif n2==1 then
      stk=list(sn(1)+'('+s2(1)+')','0','?','1',sn(5))
    elseif m2==1 then
      stk=list(sn(1)+'('+s2(1)+')','0','1','?',sn(5))
    elseif mn<>-1&nn<>-1 then
      stk=list(sn(1)+'('+s2(1)+').''','0','?','1',sn(5))
    else // at leat one dimension unknown
      v='mtlb_e'+rhsargs([sn(1),s2(1)])
      set_infos([v+' may be replaced by '+sn(1)+'('+s2(1)+')'
	        '   '+sn(1)+'('+s2(1)+') if '+sn(1)+' is a vector,'
		'   '+sn(1)+'('+s2(1)+').'' if '+sn(1)+' is a matrix.'],1)
      stk=list(v,'0','?','?',sn(5))
    end
  end
else
  s1=stk(top-1);top=top-1
  if sn(5)=='10' then // extraction pour les chaines 
    if s1(3)=='1'&s1(4)=='1' then
      stk=list('part('+sn(1)+'('+s1(1)+'),'+s2(1)+')','0','1','1',sn(5))
    elseif s1(3)=='1' then
      stk=list('part('+sn(1)+'('+s1(1)+'),'+s2(1)+')','0',s1(4),'1',sn(5))
    elseif s1(4)=='1' then
      stk=list('part('+sn(1)+'('+s1(1)+'),'+s2(1)+')','0',s1(3),'1',sn(5))
    else
      stk=list('part('+sn(1)+'('+s1(1)+'),'+s2(1)+')','0','?','1',sn(5))
    end
  elseif s1(3)=='1'&s1(4)=='1' then
    stk=list(sn(1)+rhsargs([s1(1),s2(1)]),'0','1','?',sn(5))
  elseif  s2(3)=='1'&s2(4)=='1' then
    stk=list(sn(1)+rhsargs([s1(1),s2(1)]),'0','?','1',sn(5))
  else
    stk=list(sn(1)+rhsargs([s1(1),s2(1)]),'0','?','?',sn(5))
  end
end
