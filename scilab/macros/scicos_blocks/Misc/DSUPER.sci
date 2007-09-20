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
 btitre=graphics.exprs(2)(2)
 if exprs0==[] then x=arg1,return,end
 tt=exprs0(1);
 ss=list('mat',[-1,-1])
 for i=2:size(exprs0,1)
   tt=tt+','+exprs0(i),
   ss($+1)='mat';ss($+1)=[-1,-1];
 end

 execstr('[ok,'+tt+',exprs]=getvalue(btitre,exprs0,ss,exprs)')
 
 if ok then
  x=arg1
  context=[]
  for i=1:size(exprs0,1)
    context=[context;exprs0(i)+'='+strcat(sci2exp(evstr(exprs0(i))))];
  end
  [%scicos_context,ierr]=script2var(context,%scicos_context)
  if ierr<>0 then pause,end
  sblock=x.model.rpar
  [sblock,%w,needcompile2,ok]=do_eval(sblock,list())
  y=max(2,needcompile,needcompile2)
  x.graphics.exprs(1)=exprs
  x.model.rpar=sblock
 else
  x=arg1
 end

  
case 'define' then
   // never used
end
endfunction
