function f2=%r_i_s(i,j,f2,n)
// %r_i_s(i,j,r,m) 
//!
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs==3 then
    n=f2;f2=j
    if (type(i)==10|type(i)==15) then //cell or struct case
      f2=createstruct(i,f2)
      if type(i(1))<>10 & i(2)=="entries" then
	// change struct to cell
	f=getfield(1,f2);f(1)="ce"
	setfield(1,f,f2)
      end
      return
    end
    d=ones(n);
    n(i)=f2('num'),d(i)=f2('den')
  else
    d=ones(n);
    n(i,j)=f2('num'),d(i,j)=f2('den')
  end
  f2=rlist(n,d,f2('dt'))
endfunction
