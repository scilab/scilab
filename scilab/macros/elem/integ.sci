function som = integ(y,x,orient) 
rhs=argn(2)
if size(y,1) == 1 then y = y(:);end
if rhs == 1 then 
  x = 1:size(y,1);
  orient='*'
elseif rhs==2 then
  if size(y,1)>1 &size(x,'*')==1 then
    orient=x
    x = 1:size(y,orient);
  else
    orient='*'
  end
end
if orient=='*' then
  y=y(:)
  if size(x,'*') ~= size(y,1)
    error('X and Y have incomptaible size')
  end
  dy = y(1:$-1) + y(2:$) ;
  dx = (x(2:$)-x(1:$-1))/2 ;
  som = matrix(dx,1,-1)*dy ;
elseif orient==1|orient=='r' then
  if size(x,'*') ~= size(y,1)
    error('X and Y have incomptaible size')
  end
  dy = y(1:$-1,:) + y(2:$,:) ;
  dx = (x(2:$)-x(1:$-1))/2 ;
  som = matrix(dx,1,-1)*dy ;
elseif orient==2|orient=='c' then
  if size(x,'*') ~= size(y,2)
    error('X and Y have incomptaible size')
  end
  dy = y(:,1:$-1) + y(:,2:$) ;
  dx = (x(2:$)-x(1:$-1))/2 ;
  som = dy*matrix(dx,-1,1) ;
end
