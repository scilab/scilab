function [cpr,ok]=c_pass3(scs_m,cpr)
// reconstruct the block list structure
// Copyright INRIA
bllst=list();
corinv=cpr(4)
for k=1:size(corinv)
  if size(corinv(k),'*')==1 then
    bllst(k)=scs_m(corinv(k))(3);
  else
    path=get_subobj_path(corinv(k));path($+1)=3;
    bllst(k)=scs_m(path);
  end
end
//
sim=cpr(2)
[inpptr,outptr,inplnk,outlnk,clkptr]=..
    sim(['inpptr','outptr','inplnk','outlnk','clkptr'])
// computes undetermined port sizes
[ok,bllst]=adjust(bllst,inpptr,outptr,inplnk,outlnk)
if ~ok then return; end

lnkptr=lnkptrcomp(bllst,inpptr,outptr,inplnk,outlnk)
//
xptr=1;zptr=1;rpptr=1;ipptr=1;xc0=[];xd0=[];
rpar=[];ipar=[];initexe=[];funtyp=[];
//
for i=1:length(bllst)
  ll=bllst(i)
  if type(ll(1))==15 then funtyp(i,1)=ll(1)(2); else funtyp(i,1)=0;end
  //
  xc0=[xc0;ll(6)(:)];
  xptr=[xptr;xptr($)+size(ll(6),'*')]
  
  
  if funtyp(i,1)==3 then //sciblocks
    xd0k=var2vec(ll(7))
  else
    xd0k=ll(7)(:)
  end
  xd0=[xd0;xd0k];
  zptr=[zptr;zptr($)+size(xd0k,'*')]
  
  if funtyp(i,1)==3 then //sciblocks
    rpark=var2vec(ll(8))
  else
    rpark=ll(8)(:)
  end
  rpar=[rpar;rpark]
  
  rpptr=[rpptr;rpptr($)+size(rpark,'*')]
  //
  if type(ll(9))==1 then 
    ipar=[ipar;ll(9)(:)];ipptr=[ipptr;ipptr($)+size(ll(9),'*')]
  else
    ipptr=[ipptr;ipptr($)]
  end
  //
  if ll(5)<>[] then  
    ll11=ll(11)
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
sim('xptr')=xptr
sim('zptr')=zptr
sim('izptr')=izptr
sim('inpptr')=inpptr
sim('outptr')=outptr
sim('inplnk')=inplnk
sim('outlnk')=outlnk
sim('lnkptr')=lnkptr
sim('rpar')=rpar
sim('rpptr')=rpptr
sim('ipar')=ipar
sim('ipptr')=ipptr
sim('clkptr')=clkptr
cpr(2)=sim;

outtb=0*ones(lnkptr($)-1,1)
iz0=[];
state=cpr(1)
state('x')=xc0;
state('z')=xd0;
state('iz')=iz0;
state('tevts')=tevts;
state('evtspt')=evtspt;
state('pointi')=pointi;
state('outtb')=outtb
cpr(1)=state








