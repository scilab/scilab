function  [%cpr,ok]=do_compile(scs_m)
// Copyright INRIA
par=scs_m.props;
if alreadyran then 
  //terminate current simulation
  do_terminate()
end

timer()
disablemenus()
IN=[];OUT=[];
for i=1:size(scs_m.objs)
  if typeof(scs_m.objs(i))=='Block' then  
    if scs_m.objs(i).gui=='IN_f' then
      scs_m.objs(i).gui='INPUTPORT';
      scs_m.objs(i).model.sim='bidon'
      IN=[IN scs_m.objs(i).model.ipar]
     elseif scs_m.objs(i).gui=='OUT_f' then
      scs_m.objs(i).gui='OUTPUTPORT';
      scs_m.objs(i).model.sim='bidon'
      OUT=[OUT  scs_m.objs(i).model.ipar]
    end
  end
end
IN=-sort(-IN);
if or(IN<>[1:size(IN,'*')]) then 
  ok=%f;%cpr=list()
  message('Input ports are not numbered properly.')
  return
end
OUT=-sort(-OUT);
if or(OUT<>[1:size(OUT,'*')]) then 
  ok=%f;%cpr=list()
  message('Output ports are not numbered properly.')
  return
end
[bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m);
scs_m=null()
if ~ok then
  %cpr=list()
  enablemenus()
  return,
end
%cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv)
if %cpr==list() then ok=%f,end
  
enablemenus()
endfunction
