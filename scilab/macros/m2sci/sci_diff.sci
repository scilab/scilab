function [stk,txt,top]=sci_diff()
// Copyright INRIA
txt=[]
if stk(top-rhs+1)(5)=='4' then 
  v='bool2s('+stk(top-rhs+1)(1)+')',
else 
  v=stk(top-rhs+1)(1),
end 

if rhs==1 then
  [m,n]=checkdims(stk(top))
  x=stk(top)(1)
  if m==-1&n==-1 then
    set_infos(['mtlb_diff('+x+') may be replaced by '
	 '    '+x+'(2:$)-'+x+'(1:$-1) if '+x+'is a vector'
	 '    '+x+'(2:$,:)-'+x+'(1:$-1,:) if '+x+'is a matrix'],1)
    stk=list('mtlb_diff('+x+')','0','?','?','1')
  elseif m==1|n==1 then
    if ~isname(x) then
      x=gettempvar()
      txt=[txt;x+'='+stk(top)(1)]
    end
    stk=list(x+'(2:$)-'+x+'(1:$-1)','1','1','1','1')
  else
    if ~isname(x) then
      x=gettempvar()
      txt=[txt;x+'='+stk(top)(1)]
    end
    stk=list(x+'(2:$,:)-'+x+'(1:$-1,:)','1','1','1','1')
  end
else //diff(x,N)
  N=stk(top)(1)
  top=top-1
  [m,n]=checkdims(stk(top))
  x=stk(top)(1)
  if m==-1&n==-1 then
    set_infos([
	 'mtlb_diff('+x+','+N+') may be replaced by '
	 '    '+x+'('+addf('1',N)+':$)-'+x+'(1:$-'+N+') if '+x+' is a vector'
	 '    '+x+'('+addf('1',N)+':$,:)-'+x+'(1:$-'+N+',:) if '+x+' is a matrix'],1)
    stk=list('mtlb_diff('+x+','+N+')','0','?','?','1')
  elseif m==1|n==1 then
    if ~isname(x) then
      x=gettempvar()
      txt=[txt;x+'='+stk(top)(1)]
    end
    stk=list(x+'('+addf('1',N)+':$)-'+x+'(1:$-'+N+')','1','1','1','1')
  else
    if ~isname(x) then
      x=gettempvar()
      txt=[txt;x+'='+stk(top)(1)]
    end
    stk=list(x+'('+addf('1',N)+':$,:)-'+x+'(1:$-'+N+',:)','1','1','1','1')
  end
end
endfunction
