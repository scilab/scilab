function [x,ind] = lex_sort(x,varargin)
// Copyright INRIA
// lexicographic matrix rows  sorting
//

// default argument values
uniqueflag=%f
sel=1:size(x,2)

// get arguments
if size(varargin)>2 then error(42);end // too many arguments

for k=1:size(varargin)
  if type(varargin(k))==1 then //sel given
    sel=varargin(k)
  elseif type(varargin(k))==10 then // string given is it 'unique'?
    if varargin(k)==part('unique',1:length(varargin(k))) then
      uniqueflag=%t
    else
      error(116,k+1)  //no
    end
  else
    error(116,k+1) // incorrect arg type
  end
end

// sort rows in lexicographic order
ind = 1:size(x,1);
for i=size(sel,'*'):-1:1,
  [s,k] = sort(x(ind,sel(i)));
  ind = ind(k);
end
ind=ind(:)
ind=ind($:-1:1)
x = x(ind,:);

if uniqueflag then
  k=find(sum(abs(x(2:$,sel)-x(1:$-1,sel)),2)==0)
  x(k,:)=[]
  ind(k)=[]
end
endfunction
