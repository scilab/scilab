function [x,y,typ]=SAT_f(job,arg1,arg2)
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
    [ok,minp,maxp,pente,label]=getvalue('Set Saturation parameters',..
	['Min';'Max';'Slope'],list('vec',1,'vec',1,'vec',1),label)
    if ~ok then break,end
    if maxp<=0  then
      message('Max must be strictly positive')
    elseif minp>=0  then
      message('Min must be strictly negative')
    elseif pente<=0 then
      message('Slope must be strictly positive')
    else
      rpar=[minp/pente;maxp/pente;pente]
      model(8)=rpar
      model(11)=[] //compatibility
      graphics(4)=label
      x(2)=graphics;x(3)=model
      break
    end
  end
case 'define' then
  minp=-1;maxp=1;slope=1;rpar=[minp;maxp;slope]
  model=list('lusat',1,1,[],[],[],[],rpar,[],'c',[],[%t %f],' ',list())
  label=[string(minp);string(maxp);string(slope)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=orig(1)+[4/5;1/2+1/5;1/2-1/5;1/5]*sz(1);';
    'yy=orig(2)+[1-1/5;1-1/5;1/5;1/5]*sz(2);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick)']
  x=standard_define([2 2],model,label,gr_i)
end




