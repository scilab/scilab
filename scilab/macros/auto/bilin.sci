function Sl2=bilin(Sl1,v)
// Copyright INRIA
[A,B,C,D]=abcd(Sl1);
dom=Sl1('dt');
[ra,ca] = size(A);
a=v(1);d=v(2);c=v(3);b=v(4);
i=inv(a*eye(ra,ra)-c*A);
AB=(b*A-d*eye(ra,ra))*i;
BB=(a*b-c*d)*i*B;
CB=C*i;
DB=D+c*C*i*B;
if dom=='c' then Sl2=syslin('d',AB,BB,CB,DB);
  else Sl2=syslin('c',AB,BB,CB,DB);end

  
endfunction
