function x=diff(x,N,DIM)
  rhs=argn(2)
  dims=size(x),nd=prod(size(dims))
  if rhs<3 then DIM='*',end
  if rhs<2 then N=1,end
  if DIM=='r' then DIM=1,end
  if DIM=='c' then DIM=2,end
  if DIM=='*' then DIM=-1,end
  if size(DIM,'*')<>1 then 
    error('diff: third argument should be a scalar')
  end
  DIM=round(DIM)
  if DIM<>-1&DIM<1 then 
    error('diff: third argument should be positive')
  end
  if DIM>nd then x=[],return,end
  
  if type(N)<>1|size(N,'*')<>1 then 
    error('diff: second argument should be a scalar')
  end
  N=round(N)
  if N<0 then 
    error('diff: third argument should be 0 or a positive number')
  end
 
  
  if N==1 then
    if DIM==-1 then
      x=x(2:$)-x(1:$-1)
    else
      args=emptystr(1,nd)+':'
      arg1=args;arg1(DIM)='2:$';
      arg2=args;arg2(DIM)='1:$-1';
      execstr('x=x('+strcat(arg1,',')+')-x('+strcat(arg2,',')+')')
    end
  else
    for i=1:N, x=diff(x,1,DIM),end
  end
endfunction
