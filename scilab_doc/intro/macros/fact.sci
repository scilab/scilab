function [x]=fact(k)
// Copyright INRIA
k=int(k);
  if k<1 then, 
     k=1; 
  end,
x=1;
  for j=1:k,
     x=x*j;
   end;

