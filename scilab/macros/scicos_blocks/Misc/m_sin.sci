function [out1,out2]=m_sin(t,x,z,up,uc,clock,flag,rpar,ipar)
//if flag>0 then write(0,'m_sin t='+string(t)+' flag ='+string(flag)),end
// Copyright INRIA
out1=[];out2=[];
select flag
case 1 then
  out1=sin(rpar(1)*t+rpar(2))
case 2 then
   message('?')
case 3 then 
  message('?')
case -1 then //initialisation
  model=t
  label='Sin'
  state=[]
  dstate=[]
  rpar=[1;0]
  model=list(model(1),[],1,[],[],state,dstate,rpar,[],'c',-1,[%f %t],' ',list())
  out1=list(model,label)
case -2 then //update
  model=t
  label=x
  if label==[] then
    [ok,gain,phase,label1]=getvalue('Set Sin block parameters',..
	['Frequency';'Phase'],list('vec',1,'vec',1))
  else
  
    [ok,gain,phase,label1]=getvalue('Set Sin block parameters',..
	['Frequency';'Phase'],list('vec',1,'vec',1),label)
  end
  if ok then
    model(8)=[gain;phase]
    label=label1
  end
  out1=list(model,label)
end
