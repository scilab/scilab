function [X,U,Y,XP]=steadycos(scs_m,X,U,Y,Indx,Indu,Indy,Indxp,param)
// NAME
// steadycos - Finds an equilibrium state of a general 
// dynamical system described by a scicos diagram

// CALLING SEQUENCE
//
// [X,U,Y,XP]=steadycos(scs_m,X,U,Y,Indx,Indu,Indy [,Indxp [,param ] ])
//
// scs_m: a Scicos data structure
// X: column vector. Continuous state. Can be set to [] if zero.
// U: column vector. Input. Can be set to [] if zero.
// Y: column vector. Output. Can be set to [] if zero.
// Indx: index of entries of X that are not fixed. If all can vary, set to 1:$
// Indu: index of entries of U that are not fixed. If all can vary, set to 1:$
// Indy: index of entries of Y that are not fixed. If all can vary, set to 1:$
// Indxp: index of entries of XP (derivative of x) that need not be zero. 
//        If all can vary, set to 1:$. Default [].
// param: list with two elements (default list(1.d-6,0))
//   param(1): scalar. Perturbation level for linearization; the following variation is used
//             del([x;u])_i = param(1)+param(1)*1d-4*abs([x;u])_i
//   param(2): scalar. Time t.
//

[lhs,rhs]=argn(0)
if rhs==7 then
  Indxp=[ ];param=list(1.d-6,0)
elseif rhs==8 then
  param=list(1.d-6,0)
else
  error('wrong number of arguments. 7 or 8 expected.')
end


IN=[];OUT=[];
for i=1:length(scs_m.objs)
  if typeof(scs_m.objs(i))=='Block' then  
    if scs_m.objs(i).gui=='IN_f' then
      scs_m.objs(i).gui='INPUTPORT';
      IN=[IN scs_m.objs(i).model.ipar]
    elseif scs_m.objs(i).gui=='OUT_f' then
      scs_m.objs(i).gui='OUTPUTPORT';
      OUT=[OUT  scs_m.objs(i).model.ipar]
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
%cpr=newc_pass2(bllst,connectmat,clkconnect,cor,corinv);
if %cpr==list() then 
  ok=%f,
end
if ~ok then
  error('Diagram does not compile in pass 2');
end 
sim=%cpr.sim;state=%cpr.state;
//
lnkptr=sim.lnkptr;inplnk=sim.inplnk;inpptr=sim.inpptr;
outlnk=sim.outlnk;outptr=sim.outptr;ipptr=sim.ipptr;

ki=[];ko=[];nyptr=1;
for kfun=1:length(sim.funs)
  if sim.funs(kfun)=='output' then
    sim.funs(kfun)='bidon'
    ko=[ko;[kfun,sim.ipar(ipptr(kfun))]];

  elseif sim.funs(kfun)=='input' then
    sim.funs(kfun)='bidon'
    ki=[ki;[kfun,sim.ipar(ipptr(kfun))]];
    
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

if X==[] then X=zeros(nx,1);end
if Y==[] then Y=zeros(ny,1);end
if U==[] then U=zeros(nu,1);end
if param(1)==0 then param(1)=1.d-6;end
t=param(2)

ux0=[U(Indu);X(Indx)];
sindu=size(U(Indu),'*');sindx=size(X(Indx),'*');
[err,uxopt,gopt]=optim(cost,ux0)
U(Indu)=uxopt(1:sindu);
X(Indx)=uxopt(sindu+1:sindx+sindu);
state.x=X;
state.outtb(pointi)=U;
[state,t]=scicosim(state,t,t,sim,'linear',[.1,.1,.1,.1]);
XP=state.x;
Y=state.outtb(pointo);

endfunction

function [f,g,ind]=cost(ux,ind)
state;
X;
U;
X(Indx)=ux(sindu+1:sindx+sindu);
U(Indu)=ux(1:sindu);
state.x=X;
state.outtb(pointi)=U;
[state,t]=scicosim(state,t,t,sim,'linear',[.1,.1,.1,.1]);
zer=ones(X);zer(Indxp)=0;xp=zer.*state.x;
y=state.outtb(pointo);
zer=ones(y);zer(Indy)=0;err=zer.*(Y-y);
f=.5*(norm(xp,2)+norm(err,2));

sys=lincos(scs_m,X,U,param)
g=xp'*[sys.B(:,Indu) sys.A(:,Indx)]-..
    err'*[sys.D(:,Indu) sys.C(:,Indx)];
endfunction
