// bug502

mode (-1)
clear
 set old_style on

// correct=%F
x=-25:0.1:25;
y=x.^2;
plot2d(x,y)
//for k=1:20,xclear();xtape('replay',0),xpause(1d5),end
result=execstr("for k=1:20,xclear();xtape(""replay"",0),xpause(1d5),end","errcatch","n")
xdel()
affich_result(result==0, 502)

clear

//    The Error Messages are:
//   If the Graphic Window is "interfered" with,
// for example moved, crosses path of another object
// or even when graphic Window drop down box retracts,
// then either the x-scale or y-scale of the 2d plot changes.


