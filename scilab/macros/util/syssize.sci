function [io,s]=syssize(sys)
//Old stuff
//  io=syssize(sys)
//  [io,ns]=syssize(sys)
//
//   sys   : syslin list
//   io    : io=[nout,nin] 
//                nout:  nb. ouputs
//                nin :  nb. inputs
//   s    : nb states.
// Copyright INRIA
select type(sys)
case 1 then
  io=size(sys)
  s=[]
 case 16 then
  sys1=sys(1)
  select sys1(1)
  case 'lss' then
    io=size(sys('D')),
    [s,s]=size(sys('A'))
   case 'r' then
    io=size(sys('den'))
    [lhs,rhs]=argn(0);
    if lhs==2 then  sys=tf2ss(sys);[s,s]=size(sys('A')),end
  else 
    error(97,1)
  end 
else
  error(97,1),
end
endfunction
