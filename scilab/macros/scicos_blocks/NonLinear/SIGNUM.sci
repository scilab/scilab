function [x,y,typ]=SIGNUM(job,arg1,arg2)
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
      [ok,zcr,exprs]=..
	  getvalue('Set block parameters',..
		   ['use zero_crossing (1: yes) (0:no)'],..
		   list('vec',1),exprs)
      if ~ok then break,end
      graphics.exprs=exprs
      if ok then
	if zcr<>0 then 
	  model.nmode=-1;model.nzcross=-1;
	else
	  model.nmode=0;model.nzcross=0;
	end 
	x.graphics=graphics;x.model=model
	break
      end
    end
   case 'define' then
    nu=-1
    model=scicos_model()
    model.sim=list('signum',4)
    model.in=nu
    model.out=nu
    model.nzcross=nu
    model.nmode=nu
    model.blocktype='c'
    model.dep_ut=[%t %f]
    
    exprs=[string([1])]
    gr_i=['txt=[''Sign''];';
	  'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
    
    x=standard_define([2 2],model,exprs,gr_i)
  end
endfunction
