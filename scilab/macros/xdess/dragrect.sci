function [rects,btn]=dragrect(rects)
if size(rects,1)==1 then rects=rects(:),end
n=size(rects,2)

d=driver()
if d=='Rec' then driver('X11'),end
alu=xget('alufunction') ;xset('alufunction',6) //Xor mode

rep=[rects(1),rects(2),-1]

while rep(3)==-1 then
  xrects(rects) //draw
  if xget('pixmap') then xset('wshow'),end
  repn=xgetmouse()
  xrects(rects) //erase
  if xget('pixmap') then xset('wshow'),end
  rects(1:2,:)=rects(1:2,:)+(repn(1:2)-rep(1:2))'*ones(1,n)
  rep=repn
end
btn=rep(3)
driver(d) ;xset('alufunction',alu) //set modes back
endfunction
