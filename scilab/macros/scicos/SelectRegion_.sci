function SelectRegion_()
  %xc=%pt(1),%yc=%pt(2)
  Cmenu=[];%ppt=[];Select=[]
  winback=xget('window')
  xset('window',%win)
  aluback=xget('alufunction')
  xset('alufunction',6)
  disablemenus()
  [ox,oy,w,h,ok]=get_rectangle(%xc,%yc)
  xset('alufunction',aluback)
  xset('window',winback)
  
  kc=find(%win==windows(:,2))
  if kc==[] then
    message('This window is not an active palette')
  enablemenus()
  return
//  
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    [in,out]=get_objs_in_rect(palettes(kpal),ox,oy,w,h)
  elseif %win==curwin then //click dans la fenetre courante
    [in,out]=get_objs_in_rect(scs_m,ox,oy,w,h)
  elseif slevel>1 then
    execstr('[in,out]=get_objs_in_rect(scs_m_'+string(windows(kc,1))',ox,oy,w,h)
  else
    return
  end
  
  Select=[in',%win*ones(in')]
  enablemenus()
endfunction
  
function [in,out]=get_objs_in_rect(scs_m,ox,oy,w,h)
// Copyright INRIA
  in=[];out=[]
  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k)
    if typeof(o)=='Block'|typeof(o)=='Text' then
      x=[0 1 1 0]*o.graphics.sz(1)+o.graphics.orig(1)
      y=[0 0 1 1]*o.graphics.sz(2)+o.graphics.orig(2)
      ok=%f
    elseif  typeof(o)=='Link' then
      [x,y]=(o.xx,o.yy)
      ok=%f
    else
      x=[] // object is "Deleted"
    end

    for kk=1:size(x,'*')
      data=[(ox-x(kk))'*(ox+w-x(kk)),(oy-h-y(kk))'*(oy-y(kk))];
      if data(1)<0&data(2)<0 then ok=%t;in=[in k];break;end
    end
    if ~ok then out=[out k],end
  end
endfunction
