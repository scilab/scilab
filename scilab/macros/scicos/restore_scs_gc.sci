function restore_scs_gc(scs_gc)
// restore the current scicos graphic context
// Copyright INRIA
xsetech(scs_gc(1)(1),scs_gc(1)(2))
driver(scs_gc(2))
for i=3:size(scs_gc)
  x=scs_gc(i)
  xx=x(2);
  select size(x(2),'*'),
  case 1 then 
    xset(x(1),xx);
  case 2 then 
    xset(x(1),xx(1),xx(2));
  case 3 then 
    xset(x(1),xx(1),xx(2),xx(3));
  case 4 then 
    xset(x(1),xx(1),xx(2),xx(3),xx(4));
  case 5 then 
    xset(x(1),xx(1),xx(2),xx(3),xx(4),xx(5));
  end
end
