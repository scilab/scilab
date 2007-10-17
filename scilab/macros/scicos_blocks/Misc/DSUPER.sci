function [x,y,typ]=DSUPER(job,arg1,arg2)
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
 y=needcompile // in case leaving with Exit x=arg1
 typ=list()
 graphics=arg1.graphics;
 exprs=graphics.exprs(1)
 exprs0=graphics.exprs(2)(1)
 btitre=graphics.exprs(2)(2)(1)
 bitems=graphics.exprs(2)(2)(2:$)
 if exprs0==[] then x=arg1,return,end

 tt='%scicos_context.'+exprs0(1);
 for i=2:size(exprs0,1)
   tt=tt+',%scicos_context.'+exprs0(i),
 end

 ss=graphics.exprs(2)(3)
 %scicos_context=struct()
 execstr('[ok,'+tt+',exprs]=getvalue(btitre,bitems,ss,exprs)')
 
 if ok then
  x=arg1
  context=[x.model.rpar.props.context]
  [%scicos_context,ierr]=script2var(context,%scicos_context)
  if ierr==0 then 
    sblock=x.model.rpar
    [sblock,%w,needcompile2,ok]=do_eval(sblock,list())
    y=max(2,needcompile,needcompile2)
    x.graphics.exprs(1)=exprs
    x.model.rpar=sblock
  else
    message(lasterror())
  end
 else
  x=arg1
 end

  
case 'define' then
   // never used
end
endfunction
