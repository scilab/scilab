function  [%cpr,ok]=do_compile(scs_m)
// Copyright INRIA
par=scs_m(1);
if alreadyran then 
  //terminate current simulation
  do_terminate()
end

timer()
disablemenus()
IN=[];OUT=[];
for i=2:length(scs_m)
  if typeof(scs_m(i))=='Block' then  
    if scs_m(i).gui=='IN_f' then
      scs_m(i).gui='INPUTPORT';
      scs_m(i).model.sim='bidon'
      IN=[IN scs_m(i).model.ipar]
     elseif scs_m(i).gui=='OUT_f' then
      scs_m(i).gui='OUTPUTPORT';
      scs_m(i).model.sim='bidon'
      OUT=[OUT  scs_m(i).model.ipar]
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
