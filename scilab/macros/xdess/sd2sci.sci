function txt=sd2sci(sd,zoom,orig)
// Copyright INRIA
[l,mac]=where()
scsmode=find(mac=='scicos')<>[]
[lhs,rhs]=argn(0)
pat=33
thick=1
fnt=[2 1]
dash=33
symb=[0 0]
if sd(1)<>'sd' then error('first argument has incorrect data type'),end
if rhs<3 then orig=['0';'0'],end
if type(orig)==1 then zoom=string(orig),end
if rhs<2 then zoom=['1';'1'],end
if type(zoom)==1 then zoom=string(zoom),end
if size(zoom,'*')==1 then zoom(2)=zoom(1),end
rect=sd(2)
xmx=rect(3);ymx=rect(4)
dx=abs(rect(3)-rect(1));dx=10^(-log(dx/100)/log(10))*10
dy=abs(rect(4)-rect(2));dy=10^(-log(dy/100)/log(10))*10
dxy=maxi(dx,dy)
txt=[]
txt='xsetech([0 0 1 1],'+sci2exp(sd(2))+')'
for k=3:size(sd)
  o=sd(k)
  if type(o)==15 then
    select o(1)
    case 'rect' then
      [x1,x2,y1,y2]=o(2:5)
      x1=round(dx*x1)/dx
      x2=round(dx*x2)/dx
      y1=round(dy*y1)/dy
      y2=round(dy*y2)/dy
      xi=string(mini(x1,x2));
      w=string(abs(x1-x2));
      yi=string(max(y1,y2));
      h=string(abs(y1-y2));
      txt=[txt;'xrect('+strcat([zsx(xi),zsy(yi),zx(w),zy(h)],',')+');']
    case 'frect' then
      [x1,x2,y1,y2]=o(2:5)
      x1=round(dx*x1)/dx
      x2=round(dx*x2)/dx
      y1=round(dy*y1)/dy
      y2=round(dy*y2)/dy
      xi=string(mini(x1,x2));
      w=string(abs(x1-x2));
      yi=string(max(y1,y2));
      h=string(abs(y1-y2));
      color=string(pat)
      if scsmode then color='scs_color('+color+')',end
      txt=[txt;'xrects(['+strcat([zsx(xi),zsy(yi),zx(w),zy(h)],';')+'],'+color+');']  
    case 'fleche' then
      x=round(dx*o(2))/dx;y=round(dy*o(3))/dy
      txt=[txt;'xarrows('+zsx(sci2exp(x))+','+zsy(sci2exp(y))+',-1,-1);']
    case 'ligne' then
      z=o(2)';
      x=zsx(strcat(sci2exp(round(dx*z(:,1))/dx)))
      y=zsy(strcat(sci2exp(round(dy*z(:,2))/dy)))
      txt=[txt;'xpoly('+x+','+y+',""lines"");']
    case 'cercle' then
      c=o(2);r=round(dxy*o(3))/dxy
      c1=round(dx*c(1))/dx
      c2=round(dy*c(2))/dy
      xx=zsx(string(c1-r))
      yy=zsy(string(c2+r))
      rx=zx(string(2*r))
      ry=zy(string(2*r))
      txt=[txt;'xarc('+strcat([xx,yy,rx,ry,'0','64*360'],',')+');']
    case 'fcercle' then
      c=o(2);r=round(dxy*o(3))/dxy
      c1=round(dx*c(1))/dx
      c2=round(dy*c(2))/dy
      xx=zsx(string(c1-r))
      yy=zsy(string(c2+r))
      rx=zx(string(2*r))
      ry=zy(string(2*r))
      txt=[txt;'xfarc('+strcat([xx,yy,rx,ry,'0','64*360'],',')+');']      
    case 'comm' then
      z=o(2)
      com=sci2exp(o(3))
      x=zsx(string(round(dx*z(1))/dx))
      y=zsy(string(round(dy*z(2))/dy))
      txt=[txt;'xstring('+strcat([x,y,com,'0,0'],',')+');']
    case 'patts' then
      color=string(o(2))
      if scsmode then color='scs_color('+color+')',end
      txt=[txt;'xset(''pattern'','+color+')']
      pat=o(2)
    case 'thick' then
      txt=[txt;'xset(''thickness'','+string(o(2))+')']  
      thick=o(2)
    case 'symbs' then
      txt=[txt;'xset(''mark'','+string(o(2))+','+string(o(3))+')']
      symb=[o(2),o(3)]
    case 'dashs' then
      color=string(o(2))
      if scsmode then color='scs_color('+color+')',end
      txt=[txt;'xset(''dashes'','+color+')']
      dash=o(2)
    case 'fligne' then
      z=o(2)';
      x=zsx(strcat(sci2exp(round(dx*z(:,1))/dx)))
      y=zsy(strcat(sci2exp(round(dy*z(:,2))/dy)))
      txt=[txt;
	  'xpoly('+x+','+y+',''lines'')';
	  'xfpoly('+x+','+y+',1)'];
    case 'points' then 
      z=o(2)';
      x=zsx(strcat(sci2exp(round(dx*z(:,1))/dx)))
      y=zsy(strcat(sci2exp(round(dy*z(:,2))/dy)))
      txt=[txt;
	  'xpoly('+x+','+y+',""marks"")'];
    case "clipon" then
      txt=[txt;'xclip(''clipgrf'')']
    case "clipoff" then
      txt=[txt;'xclip()']     
    else

    end
  end
  
end
endfunction
function t=zx(t)
if zoom(1)<>'1' then t($)=mulf(t($),zoom(1)),end



endfunction
function t=zy(t)
if zoom(2)<>'1' then t($)=mulf(t($),zoom(2)),end


endfunction
function t=zsx(t)
if zoom(1)<>'1' then t($)=mulf(t($),zoom(1)),end
if orig(1)<>'0' then t(1)=addf(orig(1),t(1)),end

endfunction
function t=zsy(t)
if zoom(2)<>'1' then t($)=mulf(t($),zoom(2)),end
if orig(2)<>'0' then t(1)=addf(orig(2),t(1)),end
endfunction
