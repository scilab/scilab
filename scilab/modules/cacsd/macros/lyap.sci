function X = lyap(A,C,flag)
//  solve  A'*X+X*A=C if flag=='c' or  A'*X*A-X=C if flag=='d'
  if argn(2)<>3 then error(39),end
  flag=part(flag,1)
  if flag=='c' then 
    flag=[0 0], 
  elseif flag=='d' then
    flag=[1 0],
  else
    error(36,3)
  end
  X=linmeq(2,A,C,flag)
endfunction
