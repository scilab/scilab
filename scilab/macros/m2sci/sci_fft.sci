function [stk,txt,top]=sci_fft()
// Copyright INRIA
txt=[]
if rhs==1 then
  [m,n]=checkdims(stk(top))
  x=stk(top)(1)
  if m==-1&n==-1 then
    set_infos(['mtlb_fft('+x+') may be replaced by '
	       '     fft('+x+',-1)'+' if '+x+'is a vector'],1)
    stk=list('mtlb_fft('+x+')','0',stk(top)(3),stk(top)(4),'1')
  elseif m==1|n==1 then
    stk=list('fft('+x+',-1)','0',stk(top)(3),stk(top)(4),'1')
  else
    stk=list('mtlb_fft('+x+')','0',stk(top)(3),stk(top)(4),'1')
  end
elseif rhs==2 then
  stk=list('mtlb_fft('+stk(top-1)(1)+','+stk(top)(1)+')','0','?','?','1')
elseif rhs==3 then
  job=stk(top)(1)
  if stk(top-1)(1)=='[]' then
    if job=='1' then
      y=lhsvarsnames()
      if y==[] then y=gettempvar();typ='0';else typ='-2',end
      if isname(stk(top-2)(1)) then 
	r=stk(top-2)(1),
	else 
	r=gettempvar(1),
	txt=[txt;r+'='+stk(stk(top-2)(1))+';']
      end
      txt=[txt;
	  y+'=[];for %xk='+r+','+y+'=['+y+',fft(%xk,-1)];end']
      stk=list(y,typ,stk(top)(3),stk(top)(4),'1')
    elseif job=='2' then
      y=lhsvarsnames()
      if y==[] then y=gettempvar(1);typ='0';else typ='-2',end
      if isname(stk(top-2)(1)) then 
	r=stk(top-2)(1),
	else 
	r=gettempvar(),
	txt=[txt;r+'='+stk(stk(top-2)(1))+';']
      end
      txt=[txt;
	  y+'=[];for %j=1:size('+r+',1),'+y+'=['+y+';fft('+r+'(%j,:),-1)];end']
      stk=list(y,typ,stk(top)(3),stk(top)(4),'1')
    else
      args=[];for k=1:3,args=[stk(top)(1),args];top=top-1,end;top=top+1
      stk=list('mtlb_fft'+rhsargs(args),'0',stk(top)(3),stk(top)(4),'1')
    end
  else
    args=[];for k=1:3,args=[stk(top)(1),args];top=top-1,end;top=top+1
    stk=list('mtlb_fft'+rhsargs(args),'0','?','?','1')
  end
end

