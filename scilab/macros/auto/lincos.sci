function sys=lincos(scs_m,x0,u0,param)
// NAME
// lincos - Constructs by linearization a linear state-space 
// model from a general dynamical system described by a
// scicos diagram

// CALLING SEQUENCE
//
// sys= lincos(scs_m [,x0,u0 [,param] ])
// 
//
// PARAMETERS
//
// scs_m: a Scicos data structure
// x0: column vector. Continuous state around which linearization to be done (default 0)
// u0: column vector. Input around which linearization to be done (default 0)
// state: state of scicos diagram 
// param: list with two elements (default list(1.d-6,0))
//   param(1): scalar. Perturbation level for linearization; the following variation is used
//             del([x;u])_i = param(1)+param(1)*1d-4*abs([x;u])_i
//   param(2): scalar. Time t.
//
// sys: state-state 
//
// DESCRIPTION
// Constructs by linearization a linear state-space 
// model from a general dynamical system described by a
// scicos diagram scs_m. Input and output ports, normally
// used inside superblocks, should be used to specify
// inputs and outputs in the scicos diagram. Suppose the
// scicos diagram to be linearized is called mysystem and
// it is saved in mysystem.cos in the current directory. The scicos 
// diagram scs_m can be obtained either by
//             scs_m = scicos('mysystem.cos');
// followed by a quit in the scicos menu, or by 
//             load('mysystem.cos')
// which creates by default a variable called scs_m.


[lhs,rhs]=argn(0)
IN=[];OUT=[];
for i=2:length(scs_m)
  if scs_m(i)(1)=='Block' then  
    if scs_m(i)(5)=='IN_f' then
      scs_m(i)(5)='INPUTPORT';
      IN=[IN scs_m(i)(3)(9)]
    elseif scs_m(i)(5)=='OUT_f' then
      scs_m(i)(5)='OUTPUTPORT';
      OUT=[OUT  scs_m(i)(3)(9)]
    end
  end
end
IN=-sort(-IN);
if or(IN<>[1:size(IN,'*')]) then 
  error('Input ports are not numbered properly.')
end
OUT=-sort(-OUT);
if or(OUT<>[1:size(OUT,'*')]) then 
  error('Output ports are not numbered properly.')
end
//load scicos lib
load('SCI/macros/scicos/lib')
//compile scs_m
[bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m);
if ~ok then
  error('Diagram does not compile in pass 1');
end
%cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv);
if %cpr==list() then 
  ok=%f,
end
if ~ok then
  error('Diagram does not compile in pass 2');
end 
sim=%cpr(2);state=%cpr(1);
//
lnkptr=sim('lnkptr');inplnk=sim('inplnk');inpptr=sim('inpptr');
outlnk=sim('outlnk');outptr=sim('outptr');ipptr=sim('ipptr');

ki=[];ko=[];nyptr=1;
for kfun=1:length(sim('funs'))
  if sim('funs')(kfun)=='output' then
    sim('funs')(kfun)='bidon'
    ko=[ko;[kfun,sim('ipar')(ipptr(kfun))]];

  elseif sim('funs')(kfun)=='input' then
    sim('funs')(kfun)='bidon'
    ki=[ki;[kfun,sim('ipar')(ipptr(kfun))]];
    
  end
end
[junk,ind]=sort(-ko(:,2));ko=ko(ind,1);
[junk,ind]=sort(-ki(:,2));ki=ki(ind,1);

pointo=[];
for k=ko' 
  pointo=[pointo;[lnkptr(inplnk(inpptr(k))):lnkptr(inplnk(inpptr(k))+1)-1]']
end
pointi=[];

for k=ki' 
  pointi=[pointi;[lnkptr(outlnk(outptr(k))):lnkptr(outlnk(outptr(k))+1)-1]']
end

nx=size(state.x,'*');nu=size(pointi,'*');ny=size(pointo,'*');

if rhs<3 then 
  x0=zeros(nx,1);u0=zeros(nu,1);
else
  if size(x0,'*')<>nx | size(u0,'*')<>nu then
    error('u0 or x0 does not have the correct size')
  end
end
if rhs==4 then 
  del = param(1)+param(1)*1d-4*abs([x0;u0])
  t=param(2)
else
  del=1.d-6*(1+1d-4*abs([x0;u0]))
  t=0
end
  
x0=x0(:);u0=u0(:)
  
state('x')=x0;
state('outtb')(pointi)=u0;
[state,t]=scicosim(state,t,t,sim,'linear',[.1,.1,.1,.1]);
y0=state('outtb')(pointo);
xp0=state('x');
zo0=[xp0;y0];

F=zeros(nx+ny,nx+nu);
z0=[x0;u0];
zer=zeros(nx+nu,1);

for i=1:nx+nu
  dz=zer;dz(i)=del(i);
  z=z0+dz;
  state('x')=z(1:nx);
  state('outtb')(pointi)=z(nx+1:nx+nu);
  [state,t]=scicosim(state,t,t,sim,'linear',[.1,.1,.1,.1]);
  zo=[state('x');state('outtb')(pointo)];
  F(:,i)=(zo-zo0)/del(i);
end  
sys=syslin('c',F(1:nx,1:nx),F(1:nx,nx+1:nx+nu),F(nx+1:nx+ny,1:nx),F(nx+1:nx+ny,nx+1:nx+nu));


