function scr=metanet(path,winsize)
// Copyright INRIA
[lhs,rhs]=argn(0) 
select rhs
case 0 then
  path=' '
  winsize=[1000 1000]
case 1 then
  if type(path)==10 then winsize=[1000 1000]
  else winsize=path, path=' ', end
else
  if rhs<>2 then error(39),end
end
// path
if type(path)<>10|prod(size(path))<>1 then
  error('The argument must be a string')
end
// winsize
s=size(winsize)
if (s(1)<>1|s(2)<>2) then
  error('The argument ""winsize"" must be a row vector with size 2')
end
scr=m6inimet(path,winsize(1),winsize(2))
