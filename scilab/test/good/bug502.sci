// bug502

mode (-1);
clear;
 set old_style on;

x=-25:0.1:25;
y=x.^2;
plot2d(x,y);
result=execstr("for k=1:20,xclear();xtape(""replay"",0),xpause(1d5),end","errcatch","n");
xdel();

if result==0 then
	affich_result(%T,502);
else
	affich_result(%F,502);
end

// The Error Messages are:
// If the Graphic Window is "interfered" with,
// for example moved, crosses path of another object
// or even when graphic Window drop down box retracts,
// then either the x-scale or y-scale of the 2d plot changes.
