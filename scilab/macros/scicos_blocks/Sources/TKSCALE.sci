function [x,y,typ]=TKSCALE(job,arg1,arg2)
  //Source block; output defined by tk widget scale
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
    graphics=arg1.graphics;exprs=graphics.exprs
    model=arg1.model;
    [ok,a,b,f,exprs]=getvalue('Set scale block parameters',..
        ['Min value';'Max value';'Normalization'],..
        list('vec',1,'vec',1,'vec',1),exprs)
    // tk widget returns a scalar, the value is divided by
    // Normalization factor
    if ok then
      graphics.exprs=exprs
      model.rpar=[a;b;f]
      x.graphics=graphics;x.model=model
    end
    case 'define' then
    a=-10;b=10;f=1;// default parameter values
    model=scicos_model()
    model.sim=list('tkscaleblk',5)
    model.out=1
    model.evtin=1
    model.rpar=[a;b;f]
    model.blocktype='d'
    model.dep_ut=[%f %f]
    exprs=[sci2exp(a);sci2exp(b);sci2exp(f)]
    gr_i=['xstringb(orig(1),orig(2),''TK Scale'',sz(1),sz(2),''fill'')']
    x=standard_define([3 2],model,exprs,gr_i)
  end
endfunction 




