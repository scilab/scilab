deff('p=mypoly(x)','p=poly(x,''z'',''coeffs'')')
c=[1,2,3];
P1=ex12c(2,c);
if P1<>poly(c*2,'z','coeffs') then pause,end
