function k=getlinestyle()
[lhs,rhs]=argn(0)
curwin=xget('window')
win=max(winsid()+1)
xset('window',win);

if ~MSDOS then
  delmenu(win,'3D Rot.')
  delmenu(win,'UnZoom')
  delmenu(win,'Zoom')
  delmenu(win,'File')
else
  delmenu(win,'3D &Rot.')
  delmenu(win,'&UnZoom')
  delmenu(win,'&Zoom')
  delmenu(win,'&File')
end


x=(1:5)';
fsave=xget("font size");
xset("font size",3);

plot2d(0,0,1,"010"," ",rect=[0 0 5 12]);
xtitle(["      Select line style k by clicking in rectangle or line"])
for k=1:6
  xstringb(0,2*k-1,"k = "+string(k),1,2)
  xrect(0, 2*k+1, 1, 2);
end

lsave=xget("line style");
tsave=xget("thickness");
xset("thickness",2);
for k=1:6
  xset("line style",k);
  xpoly(x,2*k*ones(x));
end

xset("line style", lsave);
xset("thickness", tsave);
xset("font size", fsave);

done=%f;
addmenu(win,'File',['Ok','Cancel']);execstr('File_'+string(win)+'=[''done=%t;k=k;'';''done=%t;k=[]'']')

cmdok='execstr(File_'+string(win)+'(1))'
cmdcancel='execstr(File_'+string(win)+'(2))'
while %t
  [c_i,cx,cy,cw,str]=xclick();
  k=round(cy/2);
    if c_i==-2 then
    if str==cmdok then k=k1;break,end
    if str==cmdcancel then k=[];break,end
  end
  if c_i==-100 then k=[];break, end
  if c_i==0 then
    k1=round(cy/2);k1=min(k1,6);k1=max(1,k1);
    xinfo('You have chosen style number: '+string(k1))
  end
end
xdel(win)

xset('window',curwin)
endfunction
