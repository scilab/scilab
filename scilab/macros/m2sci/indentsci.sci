function [txt]=indentsci(txt)
//
//!
// Copyright INRIA
if txt<>[] then
  bl='  '
  txt=bl(ones(prod(size(txt)),1))+txt
end



