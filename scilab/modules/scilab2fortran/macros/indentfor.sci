function [txt]=indentfor(txt)
//
//!
// Copyright INRIA
k=find(part(txt,1)==' ')
n=prod(size(k))
bl(n,1)=' ';
txt(k)=bl+bl+bl+txt(k)
endfunction
