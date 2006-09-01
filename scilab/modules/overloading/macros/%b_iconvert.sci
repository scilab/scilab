function r=%b_iconvert(val,it)
//Author S. Steer INRIA
  [m,n]=size(val)
  if m*n==0 then r=[],return,end
  k=find(val)
  r(m,n)=iconvert(0,it)
  r(find(val))=iconvert(1,it)
endfunction
