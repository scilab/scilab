function demo_spline()
clear;lines(0);
demo_help demo_scilab
set('old_style','off');
set("figure_style","new");
//xset("wpos",600,16);xset("wdim",600*0.9,400*0.9);
SetPosition() ;


// example 4 : trilinear interpolation (see splin3d help
//             page which have the same example with
//             tricubic spline interpolation)
getf("SCI/demos/interp/interp_demo.sci"); 
func =  "v=(x-0.5).^2 + (y-0.5).^3 + (z-0.5).^2";
deff("v=f(x,y,z)",func);
n = 5; 
x = linspace(0,1,n); y=x; z=x;
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
// compute (and display) the linear interpolant on some slices
m = 41;
dir = ["z="  "z="  "z="  "x="  "y="];
val = [ 0.1   0.5   0.9   0.5   0.5];
ebox = [0 1 0 1 0 1];

XF=[]; YF=[]; ZF=[]; VF=[];
for i = 1:length(val)
   [Xm,Xp,Ym,Yp,Zm,Zp] = slice_parallelepiped(dir(i), val(i), ebox, m, m, m);
   Vm = linear_interpn(Xm,Ym,Zm, x, y, z, V);
   [xf,yf,zf,vf] = nf3dq(Xm,Ym,Zm,Vm,1);
   XF = [XF xf]; YF = [YF yf]; ZF = [ZF zf]; VF = [VF vf]; 
   Vp =  linear_interpn(Xp,Yp,Zp, x, y, z, V);
   [xf,yf,zf,vf] = nf3dq(Xp,Yp,Zp,Vp,1);
   XF = [XF xf]; YF = [YF yf]; ZF = [ZF zf]; VF = [VF vf]; 
end
drawlater();
nb_col = 128;
vmin = min(VF); vmax = max(VF);
color = dsearch(VF,linspace(vmin,vmax,nb_col+1));
xset("colormap",jetcolormap(nb_col));
xbasc();
xset("hidden3d",xget("background"));
colorbar(vmin,vmax);
plot3d(XF, YF, list(ZF,color), flag=[-1 6 4]);
//xtitle("tri-linear interpolation of "+func);
a = gca() ;
t = a.title ;
t.text = "tri-linear interpolation of "+func ;
t.font_size = 3 ;
//xselect();
drawnow();
 realtimeinit(1.0) ;
 for i=1:20
   realtime(i);
 end

//realtimeinit(0.1);for k=1:10,realtime(k),end;
xdel(winsid());

endfunction
