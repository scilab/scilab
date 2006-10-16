function [cpr,ok]=c_pass3(scs_m,cpr)
// reconstruct the block list structure
// Copyright INRIA
bllst=list();
corinv=cpr.corinv;
sim=cpr.sim;
for k=1:size(corinv)
  if type(corinv(k))==1 then
    if size(corinv(k),'*')==1 then
      bllst(k)=scs_m.objs(corinv(k)).model;
    else
      path=get_subobj_path(corinv(k));path($+1)='model';
      bllst(k)=scs_m(path);
    end
  else
    m=scicos_model();

    //here it is assumed that modelica blocs have only scalar inputs/outputs
    m.in=ones(1,sim.inpptr(k+1)-sim.inpptr(k));
    m.out=ones(1,sim.outptr(k+1)-sim.outptr(k));
    if sim.funtyp(k)<10000 then
      n=(sim.xptr(k+1)-sim.xptr(k))
    else
      n=2*(sim.xptr(k+1)-sim.xptr(k))
    end
    m.state=cpr.state.x(sim.xptr(k)+(0:n-1));
    m.dstate=cpr.state.z(sim.zptr(k):sim.zptr(k+1)-1);

    m.rpar=sim.rpar(sim.rpptr(k):sim.rpptr(k+1)-1);
    m.ipar=sim.ipar(sim.ipptr(k):sim.ipptr(k+1)-1);
    m.label='';
    m.sim=list(sim.funs(k),sim.funtyp(k));
    //here it is assumed that modelica blocs does not have output events
    bllst(k)=m;
  end
end
//

[inpptr,outptr,inplnk,outlnk,clkptr]=..
    sim(['inpptr','outptr','inplnk','outlnk','clkptr'])
// computes undetermined port sizes
[ok,bllst]=adjust(bllst,inpptr,outptr,inplnk,outlnk)
if ~ok then return; end

lnkptr=lnkptrcomp(bllst,inpptr,outptr,inplnk,outlnk)
//
xptr=1;zptr=1;rpptr=1;ipptr=1;xc0=[];xcd0=[];xd0=[];
rpar=[];ipar=[];initexe=[];funtyp=[];labels=[];
//
for i=1:length(bllst)
  ll=bllst(i)
  labels=[labels;ll.label];
  if type(ll.sim)==15 then funtyp(i,1)=ll.sim(2); else funtyp(i,1)=0;end
  //
  X0=ll.state(:)
  if funtyp(i,1)<10000 then
    xcd0=[xcd0;0*X0]
    xc0=[xc0;X0]
    xptr(i+1)=xptr(i)+size(ll.state,'*')
  else
    xcd0=[xcd0;X0($/2+1:$)]
    xc0=[xc0;X0(1:$/2)]
    xptr(i+1)=xptr(i)+size(ll.state,'*')/2
  end
  
  if (funtyp(i,1)==3 | funtyp(i,1)==5 | funtyp(i,1)==10005) then //sciblocks
     if ll.dstate==[] then xd0k=[]; else xd0k=var2vec(ll.dstate);end
  else
    xd0k=ll.dstate(:)
  end
  xd0=[xd0;xd0k];
  zptr=[zptr;zptr($)+size(xd0k,'*')]
  
  if (funtyp(i,1)==3 | funtyp(i,1)==5 | funtyp(i,1)==10005) then //sciblocks
    if ll.rpar==[] then rpark=[]; else rpark=var2vec(ll.rpar);end
  else
    rpark=ll.rpar(:)
  end
  rpar=[rpar;rpark]
  
  rpptr=[rpptr;rpptr($)+size(rpark,'*')]
  //
  if type(ll.ipar)==1 then 
    ipar=[ipar;ll.ipar(:)];ipptr=[ipptr;ipptr($)+size(ll.ipar,'*')]
  else
    ipptr=[ipptr;ipptr($)]
  end
  //
  if ll.evtout<>[] then  
    ll11=ll.firing
    if type(ll11)==4 then
      //this is for backward compatibility
      prt=find(ll11);nprt=prod(size(prt))
      initexe=[initexe;[i*ones(nprt,1),matrix(prt,nprt,1),zeros(nprt,1)]]
    else
      prt=find(ll11>=zeros(ll11));nprt=prod(size(prt))
      initexe=[initexe;..
	  [i*ones(nprt,1),matrix(prt,nprt,1),matrix(ll11(prt),nprt,1)]];
    end
  end
end
//initialize agenda
[tevts,evtspt,pointi]=init_agenda(initexe,clkptr)

sim.xptr=xptr
sim.zptr=zptr

sim.inpptr=inpptr
sim.outptr=outptr
sim.inplnk=inplnk
sim.outlnk=outlnk
sim.lnkptr=lnkptr
sim.rpar=rpar
sim.rpptr=rpptr
sim.ipar=ipar
sim.ipptr=ipptr
sim.clkptr=clkptr
sim.labels=labels
cpr.sim=sim;

outtb=0*ones(lnkptr($)-1,1)

if exists('%scicos_solver')==0 then %scicos_solver=0,end
if max(funtyp)>10000 &%scicos_solver==0 then
  message(['Diagram contains Implicit blocks,'
	   'Compiling for implicit Solver'])
  %scicos_solver=100
end
if %scicos_solver==100 then xc0=[xc0;xcd0],end

nb=size(clkptr,'*')-1;
iz0=zeros(nb,1);
state=cpr.state
state.x=xc0;
state.z=xd0;
state.iz=iz0;
state.tevts=tevts;
state.evtspt=evtspt;
state.pointi=pointi;
state.outtb=outtb
cpr.state=state
endfunction
