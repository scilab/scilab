function [x,y,typ]=INTRP2BLK_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1(2);label=graphics(4)
  model=arg1(3);
  while %t do
    [ok,a,b,c,label]=getvalue('Set Interpolation block parameters',..
	['X coord.';'Y coord.';'Z values'],list('vec',-1,'vec',-1,'mat',[-1,-1]),label)
    if ~ok then break,end
    if size(a,'*') <> size(c,'c') | size(b,'*') <> size(c,'r') then
      message('incompatible dimension')
    elseif min(a(2:$)-a(1:$-1)) <=0 | min(b(2:$)-b(1:$-1)) <=0 then
      message('X and Y must be strictly increasing')
    else
      if ok then
	graphics(4)=label
	model(8)=[a(:);b(:);c(:)]
	model(9)=[size(a,'*');size(b,'*')]
	x(2)=graphics;x(3)=model
	break
      end
    end
  end
case 'define' then
  a=[0;1];b=[0;1];c=[0 1;1 2]
  model=list(list('intrp2',1),[1;1],1,[],[],[],[],[a;b;c(:)],[2;2],'c',[],[%t %f],' ',list())
  label=[strcat(sci2exp(a));strcat(sci2exp(b));strcat(sci2exp(c,0))]
  gr_i=['xstringb(orig(1),orig(2),''interp2'',sz(1),sz(2),''fill'');']
  x=standard_define([3 2],model,label,gr_i)
end




