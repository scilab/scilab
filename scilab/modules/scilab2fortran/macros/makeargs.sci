function [txt]=makeargs(args)
//etant donne un vecteur de chaines de caracteres "args", makeargs(args)
//retourne une chaine de la forme arg(1)+,+....+,+arg(n)
//!
// Copyright INRIA
n=prod(size(args))
txt=' '
if n==0 then return,end
for k=1:n,
  if args(k)=='eye' then
    txt=txt+':,'
  else
    txt=txt+args(k)+','
  end,
end
txt=part(txt,2:length(txt)-1)
endfunction
