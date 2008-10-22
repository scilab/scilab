function [txt]=indent(txt)
//
//!
// Copyright INRIA
n=prod(size(txt))
if n>0 then
  bl(n,1)=' ';
  txt=bl+bl+bl+txt
end
endfunction
