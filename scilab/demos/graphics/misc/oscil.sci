function [yd]=fty(t,y)
//
//  RHS of Differential equation
//
en = ener(t)
if y(1)<0 then
  if en<.5 then
    yd(1) = 0;
  else
    yd(1) = l.*(e-r*y(2));
  end;
  yd(2)=-y(2)/(r*c);
else
  yd(1)=l*(e*en-y(2)*r);
  yd(2)=(y(1)-y(2))/(r*c);
end;

function [en]=ener(t)
//
// Energy
//
t1=t-pe1*int(t/pe1);
if t1>pe2 then en=0; else en=1; end;

function [en]=ene1(t,pe1,pe2)
t1=t-pe1*int(t/pe1)
en=round( ones(t1)-t1/pe1 + (.5-(1.-pe2/pe1))*ones(t1) )

function [en]=ene2(t,pe1,pe2)
t1=t/pe1-int(t/pe1)
en=round( ones(t1)-t1 + (.5-(1-pe2/pe1))*ones(t1) )

function [en]=ene3(t,pe1,pe2)
en=round( ones(t)-t/pe1+int(t/pe1) + (.5-(1-pe2/pe1))*ones(t) )

