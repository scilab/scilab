function [stk,txt,top]=sci_std()
// Copyright INRIA
txt=[]
if rhs==1 then
  [m,n]=checkdims(stk(top))
  if m==-1&n==-1 then
    x=stk(top)(1)
    set_infos([
	 'mtlb_std('+x+') may be replaced by:'
	 '   st_deviation('+x+') if '+x+' is a vector'
	 '   st_deviation('+x+',1) if '+x+' is a matrix'],1)
    stk=list('mtlb_std('+x+')','0','?','?',stk(top)(5))
  elseif m==1|n==1 then  
    stk=list('st_deviation('+stk(top)(1)+')','0','1','1',stk(top)(5))
  else
    stk=list('st_deviation('+stk(top)(1)+',1)','0','1',stk(top)(4),stk(top)(5))
  end
elseif rhs==2 then // normalized by N
  s1=stk(top-1)
  if isname(s1(1)) then
    x=s1(1)
  else
    x=gettempvar()
    txt=[txt;x+' = '+s1(1)]
  end
  
  [m,n]=checkdims(stk(top))
  if m==-1&n==-1 then
    set_infos([
	 'mtlb_std('+x+',1) may be replaced by:'
	 '   st_deviation('+x+')*sqrt(1-1/('+x+') if '+x+' is a vector'
	 '   st_deviation('+x+',1)*sqrt(1-1/size('+x+',1) if '+x+' is a matrix'],1)
    stk=list('mtlb_std('+x+',1)','0','?','?',s1(5))
  elseif m==1|n==1 then  
    stk=list('st_deviation('+x+')*sqrt(1-1/size('+x+',''*''))','0','1','1',s1(5))
  else
    stk=list('st_deviation('+x+',1)*sqrt(1-1/size('+x+',1))','0','1',s1(3),s1(5))
  end
else
  s3=stk(top)
  s2=stk(top-1)(1)
  s1=stk(top-2)
  if s3(1)=='1' then
    m='1';n=s1(3)
  elseif s3(1)=='2' then
    m=s1(2);n='1';
  else
    m='?';n='?'
  end
  if s2=='0' then
    stk=list('st_deviation('+s1(1)+','+s3(1)+')','0',m,n,s1(5))
  else
    if isname(s1(1)) then
      x=s1(1)
    else
      x=gettempvar()
      txt=[txt;x+' = '+s1(1)]
    end
    stk=list('st_deviation('+s1(1)+','+s3(1)+')*sqrt(1-1/size('+x+','+s3(1)+'))','0',m,n,s1(5))
  end
end
endfunction
