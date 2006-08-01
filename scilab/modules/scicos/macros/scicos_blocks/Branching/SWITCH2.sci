function [x,y,typ]=SWITCH2(job,arg1,arg2)
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
      [ok,rule,thra,nzz,exprs]=getvalue('Set parameters',..
		['pass first input if: u2>=a (0), u2>a (1), u2~=a (2)';..
		    'threshold a';'use zero crossing: yes (1), no (0)'],..
		    list('vec',1,'vec',1,'vec',1),exprs)
      if ~ok then break,end
      rule=int(rule);
      if (rule<0) then rule=0, end
      if (rule>2) then rule=2, end
      graphics.exprs=exprs;
      model.ipar=rule
      model.rpar=thra
      if nzz<>0 then 
	model.nmode=1
	model.nzcross=1
      else
	model.nmode=0
	model.nzcross=0
      end
      x.graphics=graphics;x.model=model
      break
    end
   case 'define' then
    in=[-1;1;-1]
    ipar=[0] // rule
    nzz=1
    rpar=0
    
    model=scicos_model()
    model.sim=list('switch2',4)
    model.in=in
    model.out=-1
    model.ipar=ipar
    model.rpar=rpar
    model.nzcross=nzz
    model.nmode=1
    model.blocktype='c'
    model.dep_ut=[%t %f]
    
    exprs=[string(ipar);string(rpar);string(nzz)]
    
    gr_i=['xstringb(orig(1),orig(2),[''switch''],sz(1),sz(2),''fill'');']
    x=standard_define([2 2],model,exprs,gr_i)
  end
endfunction
