function [r,k]=mtlb_min(a,b,c)
// Copyright INRIA
// Emulation function for min() Matlab function
// V.C.

[lhs,rhs]=argn(0);

if rhs==2 then
  if type(b)==10 then
    rhs=1
  end
end

if rhs==1 then //min(A)
  // One output arg
  if lhs==1 then
    if ~isreal(a,0) then 
      if size(a,1)==1|size(a,2)==1 then
	[r,k]=min(abs(a))
	r=a(k)
      else
	[r,k]=min(abs(a),'r')
	r=[]
	for l=1:size(a,2)
	  r=[r,a(k(l),l)]
	end
      end
    else
      if size(a,1)==1|size(a,2)==1 then
	r=min(a)
      else
	r=min(a,'r')
      end
    end
  else
    if ~isreal(a,0) then 
      if size(a,1)==1|size(a,2)==1 then
	[r,k]=min(abs(a))
	r=a(k)
      else
	[r,k]=min(abs(a),'r')
	r=[]
	for l=1:size(a,2)
	  r=[r,a(k(l),l)]
	end
      end
    else
      if size(a,1)==1|size(a,2)==1 then
	[r,k]=min(a)
      else
	[r,k]=min(a,'r')
      end
    end
  end
// min(A,B) A and B have the same size
elseif rhs==2 then 
  // Only one output arg is possible...
  
  // Special case if A==[] and B==[]
  if isempty(a) & isempty(b) then
    r=[]
    return
  end
  
  if ~isreal(a,0)|~isreal(b,0) then 
    if ~isreal(a,0)&isreal(b,0) then
      [r,k]=min(abs(a),b)
      for m=1:size(a,1)
	for n=1:size(a,2)
	  if k(m,n)==1 then
	    r(m,n)=a(m,n)
	  end
	end
      end
    elseif isreal(a,0)&~isreal(b,0) then  
      [r,k]=min(a,abs(b))
      for m=1:size(a,1)
	for n=1:size(a,2)
	  if k(m,n)==2 then
	    r(m,n)=b(m,n)
	  end
	end
      end
    else
      [r,k]=min(abs(a),abs(b))
      for m=1:size(a,1)
	for n=1:size(a,2)
	  if k(m,n)==1 then
	    r(m,n)=a(m,n)
	  else
	    r(m,n)=b(m,n)
	  end
	end
      end
    end
  else
    r=min(a,b)
  end
// min(a,[],num) with num>2 or num is a variable name
elseif rhs==3 then 
  //If num>number of dims of a
  if c>size(size(a),2) then
    r=a
    k=ones(size(a,1),size(a,2))
  else
    if c==1 then
      c="r"
    else
      c="c"
    end
    if isreal(a) then
      // a is a complex value
      [r,k]=min(a,c)
    else
      [r,k]=min(abs(a),c)
      // Replace values in r by original values of a
      if find(size(k)<>1)==[] then //Scalar
	r=a(k)
      elseif size(k,1)==1 then // Row Vector
	r=[]
	for l=1:size(k,"*")
	  r=[r,a(k(l),l)]
	end
      elseif size(k,2)==1 then // Column Vector
	r=[]
	for l=1:size(k,"*")
	  r=[r;a(l,k(l))]
	end
      else
	r=[]
	for m=1:size(k,1)
	  tmp=[]
	  for n=1:size(k,2)
	    tmp=[tmp,a(k(m,n),n)];
	  end
	  r=[r;tmp]
	end
      end
    end
  end
end
endfunction
