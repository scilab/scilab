function y=median(x,orient)
// Copyright INRIA 
//
// NOTES
//    - modified by farid.belahcene:the case when x is an hypermatrix
//    - new syntaxes: median(x,'m') and median(x,dim)  

[lhs,rhs]=argn(0)
if argn(2)<2 then
  orient=0
else
  if orient=='r' then 
    orient=1
  elseif orient=='c' then 
    orient=2
  elseif orient=='*' then 
    orient=0
  elseif orient=='m' then 
    orient=find(size(x)>1,1)
    if orient==[] then orient=1,end
  else
    if type(orient)<>1|size(orient,'*')<>1|~isreal(orient)|orient<=0 then
      error('median: second argument should be  ''r'', ''c'',''m'' or a positive number')
    end
  end
end
  
if orient==0 then
  n=size(x,'*');
  x=gsort(x(:),'g','i')
  if 2*int(n/2)==n then
    y = (x(n/2)+x(n/2+1))/2;
  else 
    y = x((n+1)/2);
  end  
else
  if orient>ndims(x) then y=x; return;  end
  xsize=size(x);
  orient_above_size=xsize(orient+1:$);N=prod(orient_above_size)
  orient_below_size=xsize(1:orient-1);M=prod(orient_below_size)
  orient_size=xsize(1:orient);P=prod(orient_size)
  y=[];
  n=xsize(orient)
  for k=1:N
    for i=1:M
      ytemp=gsort(x(i+(0:n-1)*M+(k-1)*P),'r','i');
      if 2*int(n/2)==n then
	y($+1) = (ytemp(n/2,:)+ytemp(n/2+1,:))/2;
      else 
	y($+1) = ytemp((n+1)/2,:);
      end
    end
  end
  xsize(orient)=1;
  y=matrix(y,xsize);
end

endfunction
