function r=mtlb_length(a)
//used by mfile2sci translator  to emulate "length" when translator as no
//type information on the length argument
//You may replace this function call by 
//  length(..)     if argument is a character string
//  max(size(..))  else
//!
// Copyright INRIA
if type(a)==10 then
  r=length(a)
else
  r=max(size(a))
end
