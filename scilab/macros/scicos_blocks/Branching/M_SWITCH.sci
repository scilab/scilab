function [x,y,typ]=M_SWITCH(job,arg1,arg2)
x=[];y=[];typ=[]
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
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;
  while %t do
    [ok,nin,base,rule,exprs]=getvalue('Set parameters',..
	['number of inputs';'zero base indexing (0), otherwise 1';..
	 'rounding rule: int (0), round (1), ceil (2), floor (3)'],..
	list('vec',1,'vec',1,'vec',1),exprs)
    if ~ok then break,end
    nin=int(nin);base=int(base);
    if nin<2 then
      message('Number of inputs must be >=2 ')
     elseif ~((base==1)|(base==0)) then
      message('base indexing must be 1 or 0')
    elseif ~((rule==1)|(rule==0)|(rule==2)|(rule==3)) then
      message('incorrect rounding rule')
    else
      [model,graphics,ok]=check_io(model,graphics,[1;-ones(nin,1)],-1,[],[])
      if ok then
	graphics.exprs=exprs;
	model.ipar=[base;rule],
	x.graphics=graphics;x.model=model
	break
      end
    end
  end
case 'define' then
  in=[1;-1;-1]
  ipar=[1;3]
  nin=2
  
  model=scicos_model()
  model.sim=list('mswitch',4)
  model.in=in
  model.out=-1
  model.ipar=ipar
  model.blocktype='c'
  model.dep_ut=[%t %f]
  
  exprs=[string(nin);string(ipar)]
  
  gr_i=['d=sz(2)/(1+evstr(arg1.graphics.exprs(1)))';
   'xsegs([orig(1),orig(1)+sz(1)],[orig(2)+sz(2)-d,orig(2)+sz(2)-d])';
  'xstringb(orig(1),orig(2)+sz(2)-d,''control'',sz(1),d,''fill'')';
  'xstringb(orig(1),orig(2),[''M_Port'';''switch''],sz(1),sz(2)-d,''fill'');']
  x=standard_define([2 2],model,exprs,gr_i)
end
endfunction
