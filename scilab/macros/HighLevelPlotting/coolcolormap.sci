function c = coolcolormap(m)

// Shades of cyan and magenta color map.

r = (0:m-1)'/max(m-1,1);
c = [r 1-r ones(m,1)]; 

endfunction
