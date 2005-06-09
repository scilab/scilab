function c=hsvcolormap(n)
  if n<1 then error('hsvcolormap : n must be an integer greater than 1'),end
  c = hsv2rgb([(0:n-1)'/n ones(n,2)]);
endfunction
