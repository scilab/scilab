function [k1,k2]=getmark()
//returns the clicked markid (with a minus sign) 
//and mark size. 
[lhs,rhs]=argn(0)
k1=[];k2=[];
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


x=2;
fsave=xget("font size");
xset("font size",3);

plot2d(0,0,1,"010"," ",rect=[0 0 10 20]);
xtitle([" Select mark style k and mark size l"])
for k=0:9
  xstringb(0,2*k-1,"k = "+string(-k),1,2)
  xrect(0, 2*k+1, 1.5, 2);
end

for l=1:6
  xstringb(l+1/2, 19,"l = "+string(l-1),1,2)
  xrect(l+1/2, 21, 1, 2);
end
tsave=xget("thickness");
xset("thickness",2);
ms=xget("mark size");

for k=-(1:9)
 for x=1:6
 xset("mark size",x-1);
 plot2d(1+x,-2*k,style=k,axesflag=0);
 end
end
k=0;
x=1:6;
plot2d(1+x,0*x,style=0,axesflag=0);

xset("font size",fsave);
xset("thickness",tsave);
xset("mark size",ms);

done=%f;
addmenu(win,'File',['Ok','Cancel']);execstr('File_'+string(win)+'=[''done=%t;k=[-k1,k2];'';''done=%t;k=[]'']')

cmdok='execstr(File_'+string(win)+'(1))'
cmdcancel='execstr(File_'+string(win)+'(2))'
while %t
  [c_i,cx,cy,cw,str]=xclick();
  k=round(cy/2);
    if c_i==-2 then
    if str==cmdok then k1=-k1;k=[k1,k2];break,end
    if str==cmdcancel then k=[];break,end
  end
  if c_i==-100 then k=[];break, end
  if c_i==0 then
    k1=round(cy/2);k1=min(k1,9);k1=max(0,k1);
    k2=round(cx);k2=k2-2;k2=min(k2,5);k2=max(0,k2);
    xinfo('You have chosen (mark, mark size) = ( '+string(-k1)+', '+string(k2)+')')
  end
end
xdel(win)

xset('window',curwin)
endfunction
