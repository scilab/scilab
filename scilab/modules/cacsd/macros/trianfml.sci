function [f,sexp]=trianfml(f,sexp)
// [f [,sexp]]=trianfml(f [,sexp])  Triangularization of the symbolic matrix
// f ; Triangularization is performed by elementary row operations;
// sexp is a set of common expressions stored by the algorithm.
//!
//
// Copyright INRIA
[lhs,rhs]=argn(0)
[mf,nf]=size(f)
f_tra=f
//
// row loop
// ------------------------------
//
for nli=1:mf
  // 
  [mf_tra,nf_tra]=size(f_tra)
  //recherche du pivot  dans la premiere colonne
  c1=f_tra(:,1);
  lc=length(c1);[lc,klc]=sort(lc);
  pivot='0';l=mf_tra+1;
  while (pivot=='0'&l>1),
  l=l-1; 
         if c1(klc(l))<>'0' then pivot=c1(klc(l)),end,
  end
 if pivot<>'0' then 
  l=klc(l)
  //
  if l<>1 then f_tra([1 l],:)=f_tra([l 1],:);c1([1 l])=c1([l 1]);end
  //
  for k=2:mf_tra,
      if c1(k)<>'0' then
        f_tra(k,:)=cmb_lin(pivot,f_tra(k,:),c1(k),f_tra(1,:))
        f_tra(k,1)='0'
        if rhs==2 then
          ns=prod(size(sexp)),
          for kl=2:nf_tra
            if length(f_tra(k,kl))>20 then
               ns=ns+1
               sexp(ns)=f_tra(k,kl)
               f_tra(k,kl)='%('+string(ns)+')'
            end;
          end;
        end;
      end;
  end;
  f(mf-mf_tra+1:mf,nf-nf_tra+1:nf)=f_tra
  f_tra=f_tra(2:mf_tra,2:nf_tra)
 end
end;
endfunction
