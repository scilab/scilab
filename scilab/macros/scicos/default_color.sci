function clr=default_color(typ)
// Copyright INRIA

if typ==-1 then //event links
  clr=options('Link')(2),
elseif typ==0 then  //text, block shape,
  if size(options('Background'),'*')>=2 then //compatibility
    clr=options('Background')(2), 
  else
    clr=1
  end
elseif typ==1 then //regular links
  clr=options('Link')(1),
end
endfunction
