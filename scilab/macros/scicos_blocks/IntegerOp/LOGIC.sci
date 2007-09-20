function [x,y,typ]=LOGIC(job,arg1,arg2)
// Copyright INRIA
  x=[];y=[],typ=[]
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
    x=arg1
    graphics=arg1.graphics;exprs=graphics.exprs
    model=arg1.model;
    while %t do
      [ok,mat,herit,exprs]=getvalue('Set Logic parameters',..
	  ['Truth table';'Inherit(0=no 1=yes)'],..
	  list('mat',[-1,-2],'vec',1),exprs)
      if ~ok then break,end
      nout=size(mat,2)
      nin=(log(size(mat,1))/log(2))
      u1=floor(nin)
      if (herit<>0) then herit=1;end;
      if (u1<>nin) then message ("The number of rows of the truth table must be a power of two");ok=%f;end
      if (find(mat(:)<>0&mat(:)<>1)<>[]) then message ("number in truth table must be 0 or 1");ok=%f;end
      if ok then
        in=[ones(nin,1) ones(nin,1)]
        out=[ones(nout,1) ones(nout,1)]
        it=5*ones(1,nin)
      	ot=5*ones(1,nout)
      	[model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),ones(1-herit,1),[])
      end
      if ok then
	graphics.exprs=exprs;
	mat=int8(mat);
	model.opar=list(mat);
	x.graphics=graphics;x.model=model;
	break
      end
    end
   case 'define' then
   mat=[0;0;0;1]
   model=scicos_model()
   model.sim=list('logic',4)
   model.in=[1;1]
   model.in2=[1;1]
   model.out=1
   model.out2=1
   model.evtin=1
   model.intyp=[5 5]
   model.outtyp=5
   model.opar=list(int8(mat));
   model.blocktype='c'
   model.firing=%f
   model.dep_ut=[%t %f]
   exprs=[sci2exp(mat);sci2exp(0)]
   gr_i=['xstringb(orig(1),orig(2),[''Logic''],sz(1),sz(2),''fill'');']
   x=standard_define([2 2],model,exprs,gr_i)
  end
endfunction