function [cpr,ok]=c_pass3(scs_m,cpr)
// reconstruct the block list structure
// Copyright INRIA
bllst=list();
corinv=cpr.corinv
for k=1:size(corinv)
  if size(corinv(k),'*')==1 then
    bllst(k)=scs_m.objs(corinv(k)).model;
  else
    path=get_subobj_path(corinv(k));path($+1)='model';
    bllst(k)=scs_m(path);
  end
end
//
sim=cpr.sim
[inpptr,outptr,inplnk,outlnk,clkptr]=..
    sim(['inpptr','outptr','inplnk','outlnk','clkptr'])
// computes undetermined port sizes
[ok,bllst]=adjust(bllst,inpptr,outptr,inplnk,outlnk)
if ~ok then return; end

lnkptr=lnkptrcomp(bllst,inpptr,outptr,inplnk,outlnk)
//
xptr=1;zptr=1;rpptr=1;ipptr=1;xc0=[];xcd0=[];xd0=[];
rpar=[];ipar=[];initexe=[];funtyp=[];
//
for i=1:length(bllst)
  ll=bllst(i)
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
  
  if funtyp(i,1)==3 then //sciblocks
    xd0k=var2vec(ll.dstate)
  else
    xd0k=ll.dstate(:)
  end
  xd0=[xd0;xd0k];
  zptr=[zptr;zptr($)+size(xd0k,'*')]
  
  if funtyp(i,1)==3 then //sciblocks
    rpark=var2vec(ll.rpar)
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
izptr=ones(length(bllst)+1,1);
sim.xptr=xptr
sim.zptr=zptr
sim.izptr=izptr
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
cpr.sim=sim;

outtb=0*ones(lnkptr($)-1,1)

if exists('%scicos_solver')==0 then %scicos_solver=0,end
if max(funtyp)>10000 &%scicos_solver==0 then
  message(['Diagram contains Implicit blocks,'
	   'Compiling for implicit Solver'])
  %scicos_solver=100
end
if %scicos_solver==100 then xc0=[xc0;xcd0],end

iz0=[];
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
