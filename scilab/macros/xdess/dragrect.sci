function rects=dragrect(rects)
if size(rects,1)==1 then rects=rects(:),end
n=size(rects,2)

d=driver()
if d=='Rec' then driver('X11'),end
alu=xget('alufunction') ;xset('alufunction',6) //Xor mode

rep=[rects(1),rects(2),-1]

while rep(3)==-1 then
  xrects(rects)
  repn=xgetmouse()
  xrects(rects)
  rects(1:2,:)=rects(1:2,:)+(repn(1:2)-rep(1:2))'*ones(1,n)
  rep=repn
end
driver(d) ;xset('alufunction',alu) //set modes back

