function path=save_super(scs_m,fpath)
// given a super block definition scs_m save_super creates 
// this super block's interfacing function
// Copyright INRIA
path=[]
scs_m=do_purge(scs_m)
nam=scs_m.props.title(1);
nam=strsubst(nam,' ','_')
in=[];out=[];clkin=[];clkout=[];
bl='  '
com='/'+'/'
for k=1:lstsize(scs_m.objs)
  o=scs_m.objs(k)
  if typeof(o)=='Block' then
    model=o.model
    select o.gui
    case 'IN_f' then
      in=[in;model.out]
    case 'OUT_f' then
      out=[out;model.in]
    case 'CLKIN_f' then
      clkin=[clkin;model.evtout]
    case 'CLKOUT_f' then
      clkout=[clkout;model.evtin];
    case 'CLKINV_f' then
      clkin=[clkin;model.evtout]
    case 'CLKOUTV_f' then
      clkout=[clkout;model.evtin]; 
    end
  end
end
model=scicos_model()
  model.sim='super'
  model.in=in
  model.out=out
  model.evtin=clkin
  model.evtout=clkout
  model.rpar=scs_m
  model.blocktype='h'
  model.dep_ut=[%f %f]
  

ppath=getparpath(scs_m,[])


// form text of the macro
txt=[
'function [x,y,typ]='+nam+'(job,arg1,arg2)';
'x=[];y=[],typ=[]';
'select job';
'case ''plot'' then';
'  standard_draw(arg1)';
'case ''getinputs'' then';
'  [x,y,typ]=standard_inputs(arg1)';
'case ''getoutputs'' then';
'  [x,y,typ]=standard_outputs(arg1)';
'case ''getorigin'' then';
'  [x,y]=standard_origin(arg1)';
'case ''set'' then'
'  while %t do'
'    [x,newparameters,needcompile]=scicos(arg1.model.rpar)'
'    arg1.model.rpar=x'
'    [ok,arg1]=adjust_s_ports(arg1)'
'    if ok then'
'      x=arg1'
'      y=needcompile'
'      typ=newparameters'
'      break'
'    end'
'  end']

t1=sci2exp(model,'model');
txt=[
    txt;
'case ''define'' then'
   bl(ones(size(t1,1),1))+t1;
'  gr_i=''xstringb(orig(1),orig(2),'''''+nam+''''',sz(1),sz(2),''''fill'''')'';'
'  x=standard_define([2 2],model,[],gr_i)';
'end']
path=stripblanks(fpath)+'/'+nam+'.sci'
[u,err]=file('open',path,'unknown')
if err<>0 then
  message(path+': Directory or file write access denied')
  return
end
write(u,txt,'(a)')
file('close',u)
endfunction
