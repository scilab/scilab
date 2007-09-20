function [scs_m]=prt_align(%pt,scs_m)
//** 25/07/07: Al@n's patch for rotation of blocks
//Copyright INRIA
  win=%win
  xc1=%pt(1)
  yc1=%pt(2)
  //check if first click is on
  //a block
  k1=getblock(scs_m,[xc1;yc1])
  if k1<>[] then
    o1=scs_m.objs(k1)
  else
    return
  end
  //
  while %t
    [btn,%pt2,win,Cmenu]=cosclick()
    //check if second click is on
    //a block
    if Cmenu<>[] & Cmenu<>'SelectLink' then
      [%win,Cmenu,%pt]=resume(win,Cmenu,%pt2)
    end
    xc2=%pt2(1);
    yc2=%pt2(2);
    k2=getblock(scs_m,[xc2;yc2])
    if k2<>[] then
      o2=scs_m.objs(k2)
      break
    end
  end

  //TOBEDONE
  if get_connected(scs_m,k2)<>[] then
    hilite_obj(k2)  //** new
    message('Connected block can''t be aligned')
    unhilite_obj(k2)  //** new
    return
  end

  //
  [xout,yout,typout]=getoutputs(o1)
  [xin,yin,typin]=getinputs(o1)
  xx1=[xout xin]
  yy1=[yout,yin]

  //
  [xout,yout,typout]=getoutputs(o2)
  [xin,yin,typin]=getinputs(o2)
  xx2=[xout xin]
  yy2=[yout yin]

  if xx2==[]|yy2==[] then //second block has no port
    graphics2=o2.graphics;
    orig2=graphics2.orig

    graphics1=o1.graphics;
    orig1=graphics1.orig

    if abs(xc1-xc2)<abs(yc1-yc2) then //align vertically
      orig2(1)=orig1(1)
    else //align horizontally
      orig2(2)=orig1(2)
    end
  else

    if xx1<>[] & yy1<>[] then
      xxx=rotate([xx1;yy1],...
                 o1.graphics.theta*%pi/180,...
                 [o1.graphics.orig(1)+o1.graphics.sz(1)/2;...
                  o1.graphics.orig(2)+o1.graphics.sz(2)/2]);
      xx1=xxx(1,:);
      yy1=xxx(2,:);
    end

    xxx=rotate([xx2;yy2],...
                o2.graphics.theta*%pi/180,...
               [o2.graphics.orig(1)+o2.graphics.sz(1)/2;...
                o2.graphics.orig(2)+o2.graphics.sz(2)/2]);
    xx2=xxx(1,:);
    yy2=xxx(2,:);

    [m,kp1]=mini((yc1-yy1)^2+(xc1-xx1)^2)
    [m,kp2]=mini((yc2-yy2)^2+(xc2-xx2)^2)

    //
    xx1=xx1(kp1)
    yy1=yy1(kp1)

    xx2=xx2(kp2)
    yy2=yy2(kp2)

    graphics2=o2.graphics
    orig2=graphics2.orig

    if abs(xx1-xx2)<abs(yy1-yy2) then //align vertically
      orig2(1)=orig2(1)-xx2+xx1
    else //align horizontally
      orig2(2)=orig2(2)-yy2+yy1
    end
  end

  //quick update for new graphics
  //-Alan-
  gh_win = gcf(); //** get the handler of the curent window
  if gh_win.figure_id<>win then //** test coherence of current id figure
   //** TOBEDONE
   //** error or redraw
  else
   gh_curwin=gh_win
  end
  o_size = size(gh_curwin.children.children); //** the size:number of all the object
  //gh_k = o_size(1) - k2 + 1 ; //** semi empirical equation :)
  gh_k=get_gri(k2,o_size(1))
  gh_blk = gh_curwin.children.children(gh_k); //** new

  drawlater(); //** new

  diff_x=orig2(1)-graphics2.orig(1);
  diff_y=orig2(2)-graphics2.orig(2);

  move(gh_blk,[diff_x,diff_y]);  //** ..because "move()" works only in differential
  draw(gh_blk.parent);

  show_pixmap(); //** new

  graphics2.orig=orig2
  o2.graphics=graphics2

  scs_m_save=scs_m
  scs_m.objs(k2)=o2

  [scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t)
endfunction
