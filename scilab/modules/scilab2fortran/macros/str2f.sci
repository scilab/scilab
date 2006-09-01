function [stk,top]=str2f(ch,stk)
// traduit la definition d'une chaine de caracteres
//!
// Copyright INRIA
quote=''''
nch=length(ch)
k=1
while k<=nch
  if part(ch,k)==quote then
    ch=part(ch,1:k)+quote+part(ch,k+1:nch)
    nch=nch+1;k=k+1
  end
  k=k+1
end
top=top+1
stk(top)=list(quote+ch+quote,'-3','10','1','1')
endfunction
