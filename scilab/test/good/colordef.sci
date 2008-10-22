function colordef(a1,a2)
  if type(a1)==9 then //a handle
    f=gcf();a=gca();a1=a2
    if f.type<>'Figure' then 
      error('First argument shoud be a handle on a figure'),
    end
  elseif a1=='new' then
    n=winsid();if n==[] then n=0;else n=max(n+1),end
    f=scf(n)
    a=gca();a1=a2
  else
    f=gdf();a=gda();
  end
  
  if type(a1)<>10 then  error(44,1),  end

  select convstr(a1)
  case 'white'
    f.color_map=[jetcolormap(64);[1 1 1]*0.8];
    f.background=65;f.foreground=-1;
    a.background=-2;a.foreground=-1;
    a.font_color=a.foreground;
    a.mark_foreground=a.foreground;
  case 'black'
    if MSDOS then c=0.2, else c=0.35,end
    f.color_map=[jetcolormap(64);[1 1 1]*c];
    f.background=65;f.foreground=-2;
    a.background=-1;a.foreground=-2;
    a.font_color=a.foreground;
    a.mark_foreground=a.foreground;
  case 'none'
    f.color_map=[hsvcolormap(64)];
    f.background=-1;f.foreground=-2;
    a.background=-1;a.foreground=-2;
    a.font_color=a.foreground;
    a.mark_foreground=a.foreground;
  else
    error('Unknown color default option '+a1)
  end
endfunction
