function [fontId,fontSize]=getfont(str)
//returns the clicked font id and font size. 
[lhs,rhs]=argn(0);
fontId=[];fontSize=[];
if rhs==0 then str="a";end
curwin=xget('window')
win=max(winsid()+1)
xset('window',win);

if ~MSDOS then
  delmenu(win,'3D Rot.')
  delmenu(win,'UnZoom')
  delmenu(win,'2D Zoom')
  delmenu(win,'File')
else
  delmenu(win,'3D &Rot.')
  delmenu(win,'&UnZoom')
  delmenu(win,'2D &Zoom')
  delmenu(win,'&File')
end


x=2;
fsave=xget("font size");
xset("font size",3);

plot2d(0,0,1,"010"," ",rect=[0 0 10 20]);
xtitle(["Click to select font id (k) and font size (l)"])
for k=0:5
  xstringb(0,5+2*k-1,"k = "+string(k),1,2)
  xrect(0, 5+2*k+1, 1.5, 2);
end

for l=1:6
  xstringb(l+1/2, 16,"l = "+string(l-1),1,2)
  xrect(l+1/2, 18, 1, 2);
end
tsave=xget("thickness");
xset("thickness",2);
fsave=xget("font");

for k=-(0:5)
 for x=1:6
 xset("font",-k,x-1);
 xstring(x+1,5-2*k-0.7,str)
 end
end

xset("font",fsave(1),fsave(2));
xset("thickness",tsave);

done=%f;
addmenu(win,'File',['Ok','Cancel']);execstr('File_'+string(win)+'=[''done=%t;k=[-fontId,fontSize];'';''done=%t;k=[]'']')

cmdok='execstr(File_'+string(win)+'(1))'
cmdcancel='execstr(File_'+string(win)+'(2))'
while %t
  [c_i,cx,cy,cw,str]=xclick();
  k=round(cy/2);
    if c_i==-2 then
    if str==cmdok then fontId=fontId;k=[fontId,fontSize];break,end
    if str==cmdcancel then k=[];break,end
  end
  if c_i==-100 then k=[];break, end
  if c_i==0 then
    fontId=round(cy);fontId=fontId-5;fontId=fontId/2;fontId=min(fontId,5);fontId=max(0,fontId);
    fontSize=round(cx);fontSize=fontSize-2;fontSize=min(fontSize,5);fontSize=max(0,fontSize);
    xinfo('You have chosen (fontId, fontSize) = ( '+string(fontId)+', '+string(fontSize)+')')
  end
end
xdel(win)

xset('window',curwin)
