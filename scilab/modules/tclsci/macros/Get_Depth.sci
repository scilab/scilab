function depth = Get_Depth(f,h)

    depth = 2;

    while  h.parent <> f
        h = h.parent;
        depth = depth + 1;
    end

endfunction
