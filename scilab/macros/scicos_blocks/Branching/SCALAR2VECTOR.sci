function [x,y,typ]=SCALAR2VECTOR(job,arg1,arg2)
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
      [ok,nout,exprs]=..
	  getvalue('Set block parameters',..
		   ['size of output (-1: if don''t know)'],..
	  list('vec',1),exprs)
      if ~ok then break,end
      nout=int(nout)
      if(nout<>-1 & (nout<=0)) then
	message('size of output must be -1 or >0')
	ok=%f
      end
      if ok then
	[model,graphics,ok]=check_io(model,graphics,[1],nout,[],[])
      end	   
      if ok then
	x.graphics=graphics;x.model=model
	break
      end
    end
   case 'define' then
    nout=-1
    model=scicos_model()
    model.sim=list('scalar2vector',4)
    model.out=nout
    model.in=1
    model.blocktype='c'
    model.dep_ut=[%t %f]
    
    exprs=[string([nout])]
    gr_i=['txt=[''Scalar'';''  to '';''vector''];';
	  'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
    
    x=standard_define([2 3],model,exprs,gr_i)
  end
endfunction
