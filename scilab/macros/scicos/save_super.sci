function path=save_super(scs_m,fpath)
// given a super block definition x save_super creates a file which contains
// this super block  handling  macro definition
// Copyright INRIA
path=[]
scs_m=do_purge(scs_m)
x1=scs_m(1);nam=x1(2)(1);
nam=strsubst(nam,' ','_')
in=[];out=[];clkin=[];clkout=[];
bl='  '
com='/'+'/'
for k=2:size(scs_m)
  o=scs_m(k)
  if o(1)=='Block' then
    model=o(3)
    select o(5)
    case 'IN_f' then
      in=[in;model(3)]
    case 'OUT_f' then
      out=[out;model(2)]
    case 'CLKIN_f' then
      clkin=[clkin;model(5)]
    case 'CLKOUT_f' then
      clkout=[clkout;model(4)];
    case 'CLKINV_f' then
      clkin=[clkin;model(5)]
    case 'CLKOUTV_f' then
      clkout=[clkout;model(4)];  
    end
  end
end
model=list('super',in,out,clkin,clkout,[],[],..
      scs_m,[],'h',%f,[%f %f])
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
'    [x,newparameters,needcompile]=scicos(arg1(3)(8))'
'    arg1(3)(8)=x'
'    [ok,arg1]=adjust_s_ports(arg1)'
'    if ok then'
'      x=arg1'
'      y=needcompile'
'      typ=newparameters'
'      break'
'    end'
'  end']

model(1)='super'
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





