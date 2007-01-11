function [rects,btn]=dragrect(rects)
  if size(rects,1)==1 then rects=rects(:),end
  n=size(rects,2)
  f=gcf();pix=f.pixmap;f.pixmap='on';
  xrects(rects) 
  R=gce(); //Compound of rectangles
  rep=[rects(1),rects(2),-1]
  while rep(3)==-1 then
    show_pixmap()
    repn=xgetmouse()
    if repn(3)==-100  then //window has been closed 
      btn=repn(3)
      return
    end
    rects(1:2,:)=rects(1:2,:)+(repn(1:2)-rep(1:2))'*ones(1,n);
    move(R,repn(1:2)-rep(1:2))
    rep=repn
  end
  f.pixmap=stripblanks(pix)
  delete(R)
  btn=rep(3)
endfunction
