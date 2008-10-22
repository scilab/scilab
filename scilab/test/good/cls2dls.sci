function [s1]=cls2dls(s,t,fp)
//Syntax : s1=cls2dls(sl,t [,fp])
//
// Given sl=[a,b,c,d] (syslin list) continuous time system, cls2dls
// returns the sampled system obatined by the bilinear transform
// s=(2/t)*(z-1)/(z+1).
//
// t sampling period
// fp prevarping frequency in hertz
//!
// Copyright INRIA
[lhs,rhs]=argn(0)

if type(s)<>16 then error(91,1),end
flag=s(1); 
if flag(1)<>'lss' then error(91,1),end
if s(7)==[] then warning('cls2dls: s is assumed continuous time');s(7)='c';end
if s(7)<>'c'|s(7)=='d' then
  warning('needs a continuous system as input !')
end
fs=1/t 
if rhs==3 then fp=2*%pi*fp;fs=fp/tan(fp/fs/2)/2,end //prewarping
 
a=2*fs*eye()-s(2)
ad=a\(2*fs*eye()+s(2))
b=(ad+eye())/a*s(3);
d=s(5)+s(4)/a*s(3)
s1=syslin('d',ad,b,s(4),d,s(6))
endfunction
