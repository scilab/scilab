function [x,y,typ]=RELATIONALOP(job,arg1,arg2)
x=[];y=[];typ=[]
select job
 case 'plot' then
  VOP=['==', '~=', '<', '<=', '>=','>']
  OPER=VOP(evstr( arg1.graphics.exprs(1))+1)
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,rule,zcr,exprs]=getvalue('Set parameters',..
	['Operator: == (0), ~= (1), < (2), <= (3), >= (4), > (5)';..
	'Use zero crossing (no: 0), (yes: 1)'],..
	list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    rule=int(rule);if zcr<>0 then zcr=1,end
    if (rule<0)|(rule>5) then
      message('Incorrect operator '+string(rule)+' ; must be 0 to 5.')
    else
      graphics.exprs=exprs;
      model.ipar=[rule],
      model.nzcross=zcr,
      model.nmode=zcr,
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  ipar=[2]
  model=scicos_model()
  model.sim=list('relationalop',4)
  model.in=[1;1]
  model.out=1
  model.ipar=ipar
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(ipar);string(0)]
  gr_i=['xstringb(orig(1),orig(2),[''Relational'';''Op : ''+OPER],sz(1),sz(2),''fill'');']
  x=standard_define([2.5 2],model,exprs,gr_i)
end
endfunction
