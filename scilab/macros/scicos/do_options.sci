function [edited,options]=do_options(opt,flag)
//
// Copyright INRIA
//if xget('use color')==1
  colors=string(1:xget("lastpattern")+2);
//else
//  colors=['black','pat 1','pat 2','pat 3','pat 4','pat 5','pat 6','pat 7',...
//	  'pat 8','pat 9','pat 10','pat 11','pat 12','pat 13','pat 14',..
//	  'pat 15','white'];
//end
fontsSiz=['08','10','12','14','18','24'];
fontsIds=[ 'Courrier','Symbol','Times','Times Italic','Times Bold',..
	'Times B. It.'];
marksIds=['.','+','x','*','diamond fill.','diamond','triangle up',..
	'triangle down','trefle','circle'];
DashesIds=['Solid','-2-  -2-','-5-  -5-','-5-  -2-','-8-  -2-',..
	'-11- -2-','-11- -5-'];
//
ok=%f
edited=%f
options=opt
if flag=='3D' then
  With3D=options('3D')(1)
  if type(With3D)==4 then
    with3d=0;with3d(With3D)=1
  else
    with3d=With3D
  end
  Color3D=options('3D')(2)
  l3d=list('3D Shape',with3d+1,['No','Yes']);
  lcol_3d=list('colors 3D shape color',Color3D,colors);
  rep=x_choices('3D shape settings',list(l3d,lcol_3d));
  if rep<>[] then
    ok=%t
    options('3D')(1)=rep(1)==2
    options('3D')(2)=rep(2)
  end
elseif flag=='Background' then
  bac=options('Background')
  if bac==[] then bac=[8 1],end //compatibility
  if size(bac,'*')<2 then bac(2)=1,end //compatibility
  lcols_bg=list('colors Background',bac(1),colors);
  lcols_fg=list('colors Foreground',bac(2),colors);
  rep=x_choices('Background/Foreground color settings',list(lcols_bg,lcols_fg));
  if rep<>[] then
    ok=%t
    options('Background')=rep
  end
elseif flag=='LinkColor' then
  lcols_rl=list('colors regular links',options('Link')(1),colors);
  lcols_el=list('colors event links  ',options('Link')(2),colors);
  rep=x_choices('Default regular and event link colors',list(lcols_rl,lcols_el))
  if rep<>[] then
    ok=%t
    options('Link')=rep
  end
elseif flag=='ID' then  
  lfid_l=list('Link ID fontId',options('ID')(2)(1)+1,fontsIds);
  lfiz_l=list('Link ID fontsize',options('ID')(2)(2)+1,fontsSiz);
  lfid_b=list('Block ID fontId',options('ID')(1)(1)+1,fontsIds);
  lfiz_b=list('Block ID fontsize',options('ID')(1)(2)+1,fontsSiz);
  rep=x_choices('ID font definitions',list(lfid_l,lfiz_l,lfid_b,lfiz_b))
  if rep<>[] then
    ok=%t
    options('ID')(1)=rep(1:2)-1
    options('ID')(2)=rep(3:4)-1
  end
elseif flag=='Cmap' then
  cmap=options('Cmap')
  while %t do
    [ok,R,G,B]=getvalue(['Enter RGB description of new colors';
	'Each component must be greater or equal to 0'
	'and less or equal to 1'],['R','G','B'],..
	list('vec','-1','vec','size(x1,''*'')','vec','size(x1,''*'')'),..
	[' ',' ',' '])
    if ~ok then break,end
    if or(R<0|R>1)|or(G<0|G>1)|or(B<0|B>1) then
      x_message('RGB components are out of [0 1]')
    else
      break
    end
  end
  if ok then
    options('Cmap')=[options('Cmap');[R(:),G(:),B(:)]]
    if options('Background')==xget('lastpattern')+2 then
      options('Background')=options('Background')+size(R,'*')
    end
  end
  
end
if ok then
  edited=or(opt<>options)
end
    
endfunction
