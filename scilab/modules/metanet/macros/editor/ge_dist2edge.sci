function d=ge_dist2edge(arc,pt)
  rx=arc(3)/2;ry=arc(4)/2;
  x1=arc(1)+rx;
  y1=arc(2)-ry;
  t=atan(pt(2)-y1,pt(1)-x1);
  d=sqrt((pt(1)-x1-rx*cos(t))^2+(pt(2)-y1-ry*sin(t))^2)
endfunction

