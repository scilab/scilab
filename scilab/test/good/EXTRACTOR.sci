function [x,y,typ]=EXTRACTOR(job,arg1,arg2)
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
      
      [ok,ind,exprs]=..
	  getvalue('Set block parameters',..
		   ['indices to extract'],..
		   list('vec',-1),exprs)
      if ~ok then break,end
      ind=int(ind);ind=ind(:);
      [model,graphics,ok]=check_io(model,graphics,[-1],size(ind,1),[],[])
      if ok then
	model.ipar=ind
	graphics.exprs=exprs;
	x.graphics=graphics;x.model=model
	break
      end
    end
   case 'define' then
    ind=1
    model=scicos_model()
    model.sim=list('extractor',4)
    model.in=-1
    model.out=1
    model.blocktype='c'
    model.dep_ut=[%t %f]
    
    exprs=[sci2exp(ind)]
    
     gr_i=['txt=[''Extractor''];';
        'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
     

    x=standard_define([2 2],model,exprs,gr_i)
  end
endfunction
