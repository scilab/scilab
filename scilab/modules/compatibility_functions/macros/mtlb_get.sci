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
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'currentcharacter' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'currentmenu' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'currentobject' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'currentpoint' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'fixedcolors' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'inverthardcopy' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'keypressfcn' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'menubar' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'mincolormap' then
    v=xget('colormap')
    v=size(v,1)
  case 'name' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'nextplot' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'numbertitle' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'paperunits' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'paperorientation' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'paperposition' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'papersize' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'papertype' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'pointer' then
    v='arrow'
  case 'position' then
    o=xget('wpos')
    sz=xget('wdim')
    v=[o(:);sz(:)]'
  case 'resize' then
    v='on'
  case 'resizefcn' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'selectiontype' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'sharecolors' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'units' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'windowbuttondownfcn' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'windowbuttonmotionfcn' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'windowbuttonupfcn' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'buttondownfcn' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'children' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'clipping' then
    v=xget('clipping')
    if v(1)<>0 then v='on',else v='off',end
  case 'interruptible' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'parent' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'type' then
    v='figure'
  case 'userdata' then
    error(msprintf(gettext("errors","End of file reached before all matrix is read."),property));
  case 'visible' then    
    v='on'
  end   
end
xset('window',win)
endfunction
