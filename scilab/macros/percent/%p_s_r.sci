function f=%p_s_r(m,f)
//f=%p_s_r(m,f) <=> f=  m-f
//!
// Copyright INRIA
[num,den]=f(['num','den'])
[nf,mf]=size(f)
[nm,mm]=size(m)

if nf>=0&nm>=0 then
  if size(num,'*')==1&size(m,'*')>1 then
    den=den*ones(m)
  end
  [num,den]=simp(-num+m.*den,den)
else
  //at leat one matrix is eye*x
  if nf<0&nm<0 then
    [num,den]=simp(-num+m.*den,den)
  elseif nf<0 then
    [num,den]=simp(-num+m.*den,den*ones(m))
  elseif nm<0 then
    [num,den]=simp(-num+m*eye(den).*den,den)
  end
end
f=rlist(num,den,f('dt'))
endfunction
