function X = sylv(A,B,C,flag)
//  solve  A*X+X*B=C if flag=='c' or  A*X*B-X=C if flag=='d'
  if argn(2)<>4 then error(39),end
  if flag=='c' then 
    flag=[0 0 0], 
  elseif flag=='d' then
    flag=[1 0 0],
  else
    error(36,3)
  end
  X=linmeq(1,A,B,C,flag)
endfunction
