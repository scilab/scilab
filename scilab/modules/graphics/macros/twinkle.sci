function twinkle(h,n)

[lhs,rhs]=argn(0)
if rhs==1 then
  if (type(h)<>9 | size(h,"*")<>1) then
    error("twinkle: first argument must be a graphics handle")
  end
  n=5;
elseif rhs==2 then
  if (type(h)<>9 | size(h,"*")<>1) then
    error("twinkle: first argument must be a graphics handle")
  end
  if (type(n)<>1 | size(n,"*")<>1) then
    error("twinkle: second argument must be an integer")
  end	
else
  error("function ""twinkle"" must have one or two arguments")
end

f=h;
while f.type<>"Figure"
  f=f.parent;
end
old_pixmap=f.pixmap;
f.pixmap="on";
show_pixmap();
realtimeinit(0.1); realtime(0);
k=0;
if h.type<>"Axes" then
  v=h.visible;
  for i=1:n
    k=k+2;
    h.visible="off";
    show_pixmap();
    realtime(k);
    h.visible="on";
    show_pixmap();
    realtime(k+1);
  end
  h.visible=v;
else
  v=h.axes_visible;
  for i=1:n
    k=k+2;
    h.axes_visible="off";
    show_pixmap();
    realtime(k);
    h.axes_visible="on";
    show_pixmap();
    realtime(k+1);
  end
  h.axes_visible=v;
end

f.pixmap=stripblanks(old_pixmap);

endfunction
