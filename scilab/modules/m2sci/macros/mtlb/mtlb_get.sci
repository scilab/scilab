function v=mtlb_get(H,property)
// Copyright INRIA
[lhs,rhs]=argn()
win=xget('window')
xset('window',h)
if rhs==1 then
else
  v=[]
  select convstr(property)
  case 'backingstore' then
  case 'color' then
  case 'colormap' then
    v=xget('colormap')
  case 'currentaxes' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'currentcharacter' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'currentmenu' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'currentobject' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'currentpoint' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'fixedcolors' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'inverthardcopy' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'keypressfcn' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'menubar' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'mincolormap' then
    v=xget('colormap')
    v=size(v,1)
  case 'name' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'nextplot' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'numbertitle' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'paperunits' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'paperorientation' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'paperposition' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'papersize' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'papertype' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'pointer' then
    v='arrow'
  case 'position' then
    o=xget('wpos')
    sz=xget('wdim')
    v=[o(:);sz(:)]'
  case 'resize' then
    v='on'
  case 'resizefcn' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'selectiontype' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'sharecolors' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'units' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'windowbuttondownfcn' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'windowbuttonmotionfcn' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'windowbuttonupfcn' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'buttondownfcn' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'children' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'clipping' then
    v=xget('clipping')
    if v(1)<>0 then v='on',else v='off',end
  case 'interruptible' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'parent' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'type' then
    v='figure'
  case 'userdata' then
    error('mtlb_get, no equivalent to property :'+property)
  case 'visible' then    
    v='on'
  end   
end
xset('window',win)
endfunction
