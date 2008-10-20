function [x,y,typ]=LOGICAL_OP(job,arg1,arg2)
x=[];y=[];typ=[]
select job
 case 'plot' then
  VOP=['AND', 'OR', 'NAND', 'NOR', 'XOR','NOT']
  OPER=VOP(evstr( arg1.graphics.exprs(2))+1)
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
    [ok,nin,rule,exprs]=getvalue('Set parameters',..
	['number of inputs';..
	 'Operator: AND (0), OR (1), NAND (2), NOR (3), XOR (4), NOT (5)'],..
	list('vec',1,'vec',1),exprs)
    if ~ok then break,end
    nin=int(nin);rule=int(rule);
    if nin<1 then
      message('Number of inputs must be >=1 ')
    elseif (rule<0)|(rule>5) then
      message('Incorrect operator '+string(rule)+' ; must be 0 to 5.')
    elseif (rule==5)&(nin>1) then
      message('Only one input allowed for NOT operation')
    else
      if (rule<>5)&(nin==1) then
	[model,graphics,ok]=check_io(model,graphics,-ones(nin,1),1,[],[])
      else
	[model,graphics,ok]=check_io(model,graphics,-ones(nin,1),-1,[],[])
      end
      if ok then
	graphics.exprs=exprs;
	model.ipar=[rule],
	x.graphics=graphics;x.model=model
	break
      end
    end
  end
case 'define' then
  in=[-1;-1]
  ipar=[0]
  nin=2
  
  model=scicos_model()
  model.sim=list('logicalop',4)
  model.in=in
  model.out=-1
  model.ipar=ipar
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(nin);string(ipar)]
  gr_i=['xstringb(orig(1),orig(2),[''Logical Op'';OPER],sz(1),sz(2),''fill'');']
  x=standard_define([2.5 2],model,exprs,gr_i)
end
endfunction
