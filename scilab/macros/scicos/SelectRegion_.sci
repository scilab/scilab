function SelectRegion_()
// Copyright INRIA
//** 10 July 2006: find a
//** xx/01/07 : Alan - review (introduce rubberbox here)
//**            Objects must be totally included in
//*             "rubber box" in order to be selected.

  if with_gtk()|MSDOS then  // first click under windows treated as
                            // press move (always the case under gtk)
    Cmenu =[]; Select=[] ;
  else
    Cmenu =[]; %ppt=[]; Select=[] ;
  end
  
  gh_winback = gcf(); //** save the current figure 
  gh_percentwin = scf(%win);

  drawnow()
  //[ox,oy,w,h,ok] = get_rectangle(%xc,%yc)
  [rect,button] = rubberbox([%pt(1); %pt(2); 0; 0], %t) ;
  if or(button == [2 5 12 -100]) then // right button exit OR active window has been closed

    return ; //** ---> Exit point
  end
  ox=rect(1),oy=rect(2),w=rect(3),h=rect(4);
  clear rect

  scf(gh_winback); //** restore the current figure
  kc = find(%win==windows(:,2))

  if kc==[] then
    message('This window is not an active palette');
    return ; //** Exit point

  elseif windows(kc,1)<0 then //click inside a palette window
    kpal=-windows(kc,1);
    [in,out] = get_objs_in_rect(palettes(kpal),ox,oy,w,h)

  elseif %win==curwin then //click inside the current window
    [in,out] = get_objs_in_rect(scs_m,ox,oy,w,h)

  elseif slevel>1 then
    execstr('[in,out]=get_objs_in_rect(scs_m_'+string(windows(kc,1))',ox,oy,w,h)

  else
    return ; //** ---> Exit point
  end

  clear ox,clear oy,clear w, clear h;
  Select=[in',%win*ones(in')]

endfunction

//**---------------------------------------------------------------------------
function [in,out] = get_objs_in_rect(scs_m,ox,oy,w,h)
// Copyright INRIA
  in = []; out = [] ; ok = %f;
  for k=1:lstsize(scs_m.objs)
    ok = %f;
    o=scs_m.objs(k)
    if typeof(o)=='Block'|typeof(o)=='Text' then
//       x=[0 1 1 0]*o.graphics.sz(1)+o.graphics.orig(1)
//       y=[0 0 1 1]*o.graphics.sz(2)+o.graphics.orig(2)
      if (ox <= o.graphics.orig(1)) & ...
         (oy >= o.graphics.orig(2)+o.graphics.sz(2)) & ...
         ((ox+w) >= (o.graphics.orig(1)+o.graphics.sz(1))) & ...
         ((oy-h) <= o.graphics.orig(2)) then
           ok=%t
           in=[in k]
      end

     elseif  typeof(o)=='Link' then
//       [x,y]=(o.xx,o.yy)
      if (ox <= max(o.xx)) & ...
         (oy >= max(o.yy)) & ...
         ((ox+w) >= max(o.xx)) & ...
         ((oy-h) <= min(o.yy)) then
           ok=%t
           in=[in k]
      end

    else
      x=[] // object is "Deleted"
    end

//     for kk=1:size(x,'*')
//       data=[(ox-x(kk))'*(ox+w-x(kk)),(oy-h-y(kk))'*(oy-y(kk))];
//       if data(1)<0&data(2)<0 then ok=%t;in=[in k];break;end
//     end

    if ~ok then out=[out k],end

  end
endfunction
