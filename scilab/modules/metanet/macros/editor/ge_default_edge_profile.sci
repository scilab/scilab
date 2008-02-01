function xyp=ge_default_edge_profile()
//returns the normalize arc profile used by default
  xp=[0 1 3 4]'/4;
  yp=[0 1 1 0]';
  xyp=[xp yp]
  use_splin=%t
  if use_splin then
    xn=(0:0.1:1)';
    yn = interp(xn, xyp(:,1), xyp(:,2), splin(xyp(:,1), xyp(:,2)) );
    xyp=[xn yn];
  end
endfunction
