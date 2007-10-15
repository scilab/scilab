function fname=choosefile(path,comm,errmess)
// Copyright INRIA
// choosefile - select a file through a filter
//%Syntax
//  fname=choosefile(path)
//  fname=choosefile()
//%Parameters
// path   : character string for selection rule.
// fname  :  character string : selected file name or empty matrix if none
//!

// Copyright INRIA

[lhs,rhs]=argn(0)
select rhs
case 0 then
  path='./*'
  comm='Choose a file'
  errmess=%t
case 1 then
  comm='Choose a file'
  errmess=%t
case 2 then
  errmess=%t
end
lst=lstfiles(path)
if lst==[]&errmess then message('No such file exists'),end
if prod(size(lst))>0 then
  n=choose(lst,comm,'Cancel')
  if n<>0 then
    fname=lst(n)
  else
    fname=[]
  end
else
  fname=[]
end
endfunction
