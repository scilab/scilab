function demo_3dplot()
demo_help demo_3dplot

// exemple of colorbar
x = linspace(0,1,41);
z = cos(2*%pi*x)'*sin(2*%pi*x);
zm = min(z); zM = max(z);
zz = abs(0.5*cos(2*%pi*x)'*cos(2*%pi*x));
zzm = min(zz); zzM = max(zz);
xbasc();
xset("colormap",jetcolormap(64))
set("figure_style",'new');
new_styl = get("figure_style") == "new";
if new_styl then, drawlater(), end

SetPosition() ;
subplot(2,2,1)
   colorbar(zm,zM)
   Sgrayplot(x,x,z, strf="031", rect=[0 0 1 1])
   xtitle("a Sgrayplot with a colorbar")
subplot(2,2,2)
   colorbar(zm,zM)
   plot3d1(x,x,z)
   xtitle("a plot3d1 with a colorbar")
subplot(2,2,3)
colorbar(zzm,zzM)
plot3d1(x,x,zz)
xtitle("a plot3d1 with a colorbar")
subplot(2,2,4)
   colorbar(zzm,zzM)
   Sgrayplot(x,x,zz, strf="031", rect=[0 0 1 1])
   xtitle("a Sgrayplot with a colorbar")
   
SetPosition();
if new_styl then, drawnow(), end

realtimeinit(1.0)
for i=1:10, 
  realtime(i);
end

xdel() ;


//example of spline2d
// example 3 : not_a_knot spline and monotone sub-spline
//             on a step function
a = 0; b = 1; c = 0.25; d = 0.75;
// create interpolation grid
n = 11;
x = linspace(a,b,n);
ind = find(c <= x & x <= d); 
z = zeros(n,n); z(ind,ind) = 1;  // a step inside a square
// create evaluation grid
np = 300; // 220;
xp = linspace(a,b, np);
[XP, YP] = ndgrid(xp, xp);
zp1 = interp2d(XP, YP, x, x, splin2d(x,x,z));
zp2 = interp2d(XP, YP, x, x, splin2d(x,x,z,"monotone"));
// plot
drawlater()
xbasc()
SetPosition() ;
xset("colormap",jetcolormap(128))
subplot(1,2,1)
   plot3d1(xp, xp, zp1, flag=[-2 6 4])
   //xtitle("spline (not_a_knot)")
   a = gca() ;
   t = a.title ;
   t.text = "spline (not_a_knot)" ;
   t.font_size = 3 ;
subplot(1,2,2)
   plot3d1(xp, xp, zp2, flag=[-2 6 4])
   //xtitle("subspline (monotone)")
   f= gcf();
   f.color_map = jetcolormap(512) ;
   a = gca() ;
   t = a.title ;
   t.text = "subspline (monotone)" ;
   t.font_size = 3 ;

drawnow() ;
   
realtimeinit(1.0)
for i=1:10, 
 realtime(i);
end

xdel() ;

endfunction
