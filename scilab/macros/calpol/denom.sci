function den=denom(r)
//returns the denominator of a rational matrix 
//%Syntax: den=denom(r)
//with
//r: rational function matrix (may be polynomial or scalar matrix)
//den: polynomial matrix
//!
// Copyright INRIA
select type(r)
case 1 then
  den=ones(r);
case 2 then
  den=ones(r);
  
//-compat next case retained for list/tlist compatibility
case 15 then
  r1=r(1);
  if r1(1)<>'r' then error(92,1),end
  den=r(3)
case 16 then
  r1=r(1);
  if r1(1)<>'r' then error(92,1),end
  den=r(3)
else
  error(92,1)
end
endfunction
