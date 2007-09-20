function [scs_m]=do_turn(%pt,scs_m,theta)
// Copyright INRIA
//** x,07/07 Alan : do_turn update theta value of blocks
//**                and text (redraw it)

  //** get the current win ID
  win = %win;

  //** get the handle of the current window
  gh_curwin = gh_current_window ; //** acquire the current window handler
  o_size = size(gh_curwin.children.children) ; //** o_size(1) is the number of compound object

  //** get the mouse coord.
  xc = %pt(1);
  yc = %pt(2);

  //**--------- check Select ------------------
  SelectSize = size(Select);
  SelectSize = SelectSize(1);

  if SelectSize<>0 then
    if SelectSize==1 & Select(1,2)==%win then
      k= Select(1,1)
    elseif SelectSize>1 then
      if find(Select(:,2)==%win)<>[] then
         //scs_m=do_multiturn(scs_m,win)
         k = getobj(scs_m,[xc;yc]);
      else
        k = getobj(scs_m,[xc;yc]);
      end
    else
      k = getobj(scs_m,[xc;yc]);
    end
  else
    k = getobj(scs_m,[xc;yc]);
  end

  //**--------- check k and scs_m.objs(k) ------------------
  if k==[] then
    return
  end //** if you click in the void ... return back

  o = scs_m.objs(k)
  if typeof(o)=='Link' then
    return
  end //**disable rotation for link

  //**--------- scs_m theta update -------------------------
  geom = o.graphics ;
  geom.theta = geom.theta + theta
  while geom.theta>=360 then
    geom.theta=geom.theta-360;
  end
  while geom.theta<=-360 then
    geom.theta=geom.theta+360;
  end
  o.graphics = geom;

  if get_connected(scs_m,k)<>[] then //** see message

    connected=[scs_m.objs(k).graphics.pin',...
               scs_m.objs(k).graphics.pout',...
               scs_m.objs(k).graphics.pein',...
               scs_m.objs(k).graphics.peout']

    gh_link_i   = [];
    xm = []; //** init
    ym = [];
    if connected<>[] then //** check if external link are present
       orig=o.graphics.orig
       sz=o.graphics.sz
       //theta=o.graphics.theta
       for l=1:length(connected) //** scan all the connected links
         i  = connected(l)  ;
         if i<>0 then
           oi = scs_m.objs(i) ;
           gh_i = get_gri(i,o_size(1)); //** calc the handle of all the connected link(s)
           gh_link_i = [gh_curwin.children.children(gh_i)]; //** vector of handles
           [xl, yl, ct, from, to] = (oi.xx, oi.yy, oi.ct, oi.from, oi.to);
            //**------------------------------------------
            if from(1)==k then
              tmp_data = gh_link_i.children.data
              xxx = rotate([xl(1);yl(1)],theta*%pi/180,...
                           [orig(1)+sz(1)/2;orig(2)+sz(2)/2])
              gh_link_i.children.data = [[xxx(1),xxx(2)];tmp_data(2:$ , 1:$)];
              oi.xx(1) = xxx(1)
              oi.yy(1) = xxx(2)
              scs_m.objs(i) = oi;
            end

            if to(1)==k then
              tmp_data = gh_link_i.children.data
              xxx = rotate([xl($);yl($)],theta*%pi/180,...
                           [orig(1)+sz(1)/2;orig(2)+sz(2)/2])
              gh_link_i.children.data = [tmp_data(1:$-1 , 1:$);[xxx(1),xxx(2)]];
              oi.xx($) = xxx(1)
              oi.yy($) = xxx(2)
              scs_m.objs(i) = oi;
            end
         end
       end
     end
  end

  //**------ draw object in new graphics structure ---------
  gr_k=get_gri(k,o_size(1))
  drawlater();
  update_gr(gr_k,o)
  draw(gh_curwin.children);
  show_pixmap() ;

  scs_m_save = scs_m ; //** ... for undo ...
  scs_m.objs(k)=o

  [scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t)

endfunction
