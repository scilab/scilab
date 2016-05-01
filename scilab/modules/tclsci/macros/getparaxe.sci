function [haxe] = getparaxe( h )

    htmp = h;
    haxe = [];
    while htmp.type<>"Axes" do
        htmp = htmp.parent
    end

    haxe = htmp;
endfunction
