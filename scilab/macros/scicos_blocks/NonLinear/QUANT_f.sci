function [x,y,typ]=QUANT_f(job,arg1,arg2)
// Copyright INRIA
x=[];y=[],typ=[];
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
  while %t then
    [ok,pas,meth,label]=getvalue('Set parameters',..
	['Step';'Quantization Type (1-4)'],..
	list('vec',1,'vec',1),label)
    if ~ok then break,end
    if meth<1|meth>4 then
      message('Quantization Type must be from 1 to 4')
    else
      rpar=pas
      model(8)=rpar
      model(9)=meth
      select meth
      case 1 then
	model(1)='qzrnd'
      case 2 then
	model(1)='qztrn'
      case 3 then
	model(1)='qzflr'
      case 4  then
	model(1)='qzcel'
      end
      graphics(4)=label
      model(11)=[] //compatibility
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  pas=0.1;rpar=pas
  meth=1
  model=list('qzrnd',-1,-1,[],[],[],[],rpar,meth,'c',[],[%t %f],' ',list())
  label=[string(pas);string(meth)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+[1;2;2;3;3;4;4]/5*sz(1);';
    'yy= orig(2)+[1;1;2;2;3;3;4]/5*sz(2);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']
  x=standard_define([2 2],model,label,gr_i)
end




