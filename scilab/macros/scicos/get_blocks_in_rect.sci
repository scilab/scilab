function [in,out]=get_blocks_in_rect(scs_m,ox,oy,w,h)
// Copyright INRIA
in=[];out=[]
for k=2:size(scs_m)
  o=scs_m(k)
  if o(1)=='Block'|o(1)=='Text' then
    // check is block is outside rectangle
    orig=o(2)(1)
    sz=o(2)(2)
    x=[0 1 1 0]*sz(1)+orig(1)
    y=[0 0 1 1]*sz(2)+orig(2)
    ok=%f
    for kk=1:4
      data=[(ox-x(kk))'*(ox+w-x(kk)),(oy-h-y(kk))'*(oy-y(kk))];
      if data(1)<0&data(2)<0 then ok=%t;in=[in k];break;end
    end
    if ~ok then out=[out k],end
  end
end
