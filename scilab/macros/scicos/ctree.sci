function [ord,ok]=ctree(vec,in,depu)
//sctree(nb,vec,in,depu,outptr,cmat,ord,nord,ok,kk)
// Copyright INRIA
jj=find(depu);dd=zeros(depu);dd(jj)=ones(jj)';depu=dd;
nb=prod(size(vec));kk=zeros(vec);
[ord,ok]=sctree(vec,in,depu,outptr,cmatp);
ok=ok==1;
ord=ord';
endfunction
