function b = bonecolormap(m)

//Gray-scale with a tinge of blue color map.

b = (7*graycolormap(m) + fliplr(hotcolormap(m)))/8;

endfunction
