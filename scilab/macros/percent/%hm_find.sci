function [i,j]=%hm_find(Mb)
// it seems it must have at most 2 output arguments as the internal 
//  find
  lhs=argn(1); N=Mb.dims; n=length(N) 
  i=find(Mb.entries)
  if lhs==2 then
    j=zeros(n-1,length(i))
    for k=1:n-1
      j(k,:)=modulo(int((i-1)/prod(N(1:k))),N(k+1))+1     
    end
    i=modulo(i-1,N(1))+1
  end
endfunction
