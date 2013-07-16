
function [x,y,typ] = BUG_8531( o )

    xf=40;
    yf=40;

    [orig,sz,orient]=(o.graphics.orig,o.graphics.sz,o.graphics.flip);

    if orient then
        x=orig(1)+sz(1)+xf/40;
    else
        x=orig(1)-xf/40;
    end

    y1=orig(2)+sz(2)*3/4;
    y2=orig(2)+sz(2)/4;

    x=[x,x];
    y=[y1,y2];

    typ=[2,2];

endfunction

