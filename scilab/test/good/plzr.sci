function plzr(a,b,c,d)
//
// Copyright INRIA
[lhs,rhs]=argn(0)
if type(a)==16 then
  if rhs<>1 then error(39),end
  a1=a(1);
  select a1(1)
    case 'lss' then [a,b,c,d]=a(2:5)
    case 'r' then a=tf2ss(a),[a,b,c,d]=a(2:5)
    else error(97,1)
  end;
end;

dr=spec(a)
[al,be]=tr_zer(a,b,c,d)
nr=al./be
ni=imag(nr);nr=real(nr)
di=imag(dr);dr=real(dr)
//
mxx=maxi([nr;dr;1]*1.1)
mnx=mini([nr;dr;-1]*1.1)
my=maxi(abs([ni;di;1])*1.1)
xselect();
rect=[mnx, -my, mxx, my];
wdim=xget('wdim')
dx=(mxx-mnx)/wdim(1);dy=2*my/wdim(2)
if dy>dx then 
  ax=(dy*wdim(1)-mxx+mnx)/2
  mxx=mxx+ax;mnx=mnx-ax
elseif dy<dx then
  ay=(dx*wdim(2)-2*my)/2
  my=my+ay
end
rect=[mnx, -my, mxx, my];
plot2d(0,0,1,"051"," ",rect);

xx=xget("mark")
xset("mark",xx(1),xx(1)+1);
if prod(size(nr))<>0 then
  plot2d(nr,ni,[-9,3],"100",'Zeros')
  strf='100'
  pos=6
else
  strf='100'
  pos=3
end;
plot2d(dr,di,[-2,pos],strf,'Poles');
plot2d([mnx;mxx],[0;0],4,"000",' ')
plot2d([0;0],[-my;my],4,"000",' ')

xarc(-1,1,2,2,0,360*64)
xtitle('transmission zeros and poles','real axis','imag. axis');
xset("mark",xx(1),xx(2));
endfunction
