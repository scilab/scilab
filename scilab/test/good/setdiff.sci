function [a,ka]=setdiff(a,b)
//Copyrigth INRIA
// returns a values which are not in b
  if b==[] then //b is empty reurn a!
    ka=1:size(a,'*')
    return
  end

  [a,ka]=unique(a);
  na=size(a,'*');

  b=unique(b(:));

  [x,k]=sort([a(:);b]);
  if type(a)==1 then x=x($:-1:1);k=k($:-1:1);,end
  d=find(x(2:$)==x(1:$-1));  //index of common entries in sorted table
  if d<>[] then k([d;d+1])=[],end

  keep = find(k <= na); 
  a = a(k(keep));
  ka = ka(k(keep));
endfunction
