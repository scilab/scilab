function set_background()
// Copyright INRIA
if options('Background')<>[] then //compatibility
  xset('background',options('Background')(1));
else
  xset('background',8)
end

