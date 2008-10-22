function s1=%r_l_r(s1,s2)
// %r_l_r(s1,s2)   s1\s2
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
[n,m]=size(s1('den'))
if n<>m then error(43),end
if m*n==1 then
    s1=%r_m_r(rlist(s1('den'),s1('num'),s1('dt')),s2)
else
    // reduction de s1 sous la forme D1**(-1)* N1 (D1 diagonale)
    p=s1('num')
    s1=s1('den')
    for l=1:n
      [pp,fact]=lcm(s1(l,:))
      p(l,:)=p(l,:).*fact
      s2(l,:)=s2(l,:)*pp
    end
    s1=invr(p)*s2,
end
endfunction
