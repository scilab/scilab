function clf(varargin)
  if size(varargin)==0 then
    h=gcf()
    job='clear'
  elseif size(varargin)==1 then
    if type(varargin(1))==1 then // win num given
      f=gcf();h=scf(varargin(1));scf(f);
      job='clear'
    elseif type(varargin(1))==9 then // handle given
      h=varargin(1);job='clear'
    elseif type(varargin(1))==10 then // job given
      h=gcf()
      job=varargin(1)
    end
  elseif size(varargin)==2 then
    if type(varargin(1))==1 then // win num given
      f=gcf();h=scf(varargin(1));scf(f);
    elseif type(varargin(1))==9 then // handle given
      h=varargin(1);
    end
    job=varargin(2);
  else
    error('clf must be called with at most 2 arguments')
  end
  if and(job<>['clear','reset']) then 
    error('clf: job argument possible values are '"clear"' or '"reset"'')
  end
  if stripblanks(h.type)<>'Figure' then
    error('handle should be of type '"Figure"'')
  end
  c=h.children;
  vis=h.visible;h.visible='off';
  if size(c,'*')>1 then c($)=[],end
  delete(c)
  h.visible=vis
  if job=='reset' then
    props=['axes_size','auto_resize','figure_name','color_map','pixmap','pixel_drawing_mode','background','visible','rotation_style']
    df=gdf()
    for p=props
      h(p)=df(p)
    end
  end
endfunction
