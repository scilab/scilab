function e=nextpow2(n)
  [f,e]=frexp(abs(n))
  k=find(f==0.5) // n(k) is a power of 2
  e(k)=e(k)-1
endfunction
