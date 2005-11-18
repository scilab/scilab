function y=median(x,orient)
// Copyright INRIA 
//
// NOTES
//    - modified by farid.belahcene:the case when x is an hypermatrix
//    - new syntaxes: median(x,'m') and median(x,dim)  

[lhs,rhs]=argn(0)
if rhs==1 then
  n=size(x,'*');
  x=matrix(x,-1,1)
  y=median(x,1)
elseif orient=='r' then
  y=median(x,1);
elseif orient=='c' then
  y=median(x,2);
elseif orient=='m' then
  n=find(size(x)>1,1);
  if n == []  then 
    y = median(x,1);
  else 
    y = median(x,n);
  end  
elseif orient>=1 & orient-floor(orient)==0 then
  if orient>ndims(x) then
  y=x;
  return
  end
  xsize=size(x);
  orient_above_size=xsize(orient+1:$);
  orient_below_size=xsize(1:orient-1);
  orient_size=xsize(1:orient);
  y=[];
  for k=1:prod(orient_above_size)
    for i=1:prod(orient_below_size)
      ytemp=[];
      for j=1:xsize(orient)
	ytemp($+1)=x(i+(j-1)*prod(orient_below_size)+(k-1)*prod(orient_size));
      end
      ytemp=gsort(ytemp,'r','i');
      if 2*int(xsize(orient)/2)==xsize(orient) then
	y($+1) = (ytemp(xsize(orient)/2,:)+ytemp(xsize(orient)/2+1,:))/2;
      else 
	y($+1) = ytemp((xsize(orient)+1)/2,:);
      end
    end
  end
  ysize=xsize;
  ysize(orient)=1;
  y=matrix(y,ysize);
else
  error('median : second argument must be ''r'', ''c'',''m'', or a positive integer')
end

endfunction
