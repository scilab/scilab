function cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv)
// cor    ; correspondance table with initial block ordering
//
// bllst: list with nblk elts where nblk denotes number of blocks.
//        Each element must be a list with 12 elements:
//          1- function name (in string form if fortran routine) 
//          2- vector of number of inputs
//          3- vector of number of ouputs
//          4- vector of number of clock inputs 
//          5- vector of number of clock outputs
//          6- vector (column) of continuous initial condition
//          7- vector (column) of discrete initial condition
//          8- vector (column) of real parameters
//          9- vector (column) of integer parameters
//          10- string: 'z' if zero-crossing,
//                      'l' logical
//          11- vector of size <number of clock outputs> including
//              preprogrammed event firing times (<0 if no firing) 
//              or [for backward compatibility]
//              boolean vector: i-th entry %t if initially output is fired
//          12- boolean vector (1x2): 1st entry for dependence on u, 2nd on t 
//
// connectmat: nx4 matrix. Each row contains, in order, the block
//             number and the port number of an outgoing scicopath,
//             and the block number and the port number of the target
//             ingoing scicopath.
//
// clkconnect: same as connectmat but for clock scicopaths.
//
// define some constants
// Copyright INRIA
show_trace=%f
if show_trace then disp('c_pass1:'+string(timer())),end

if bllst==list() then
  message(['No block can be activated'])
  cpr=list()
  ok=%f;
  return
end



clkptr=1,cliptr=1,typ_l=[],dep_ut=[]
nblk=size(bllst)

//take care of the heritage

[bllst,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,..
    dep_ut,typ_l,typ_r,typ_m,tblock,typ_cons,ok]=mini_extract_info(bllst,..
    connectmat,clkconnect)

if show_trace then disp('c_pass20:'+string(timer())),end

//if ~ok then 
  // heritage block !
  [outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)
  [clkconnect,exe_cons]=pak_ersi(connectmat,clkconnect,dep_ut,typ_r,..
      typ_l,outoin,outoinptr,tblock,typ_cons,clkptr)
//end

if show_trace then disp('c_pass21:'+string(timer())),end

done=%f
while ~done
  //replace all synchro (l) blocks recursively
  [clkptr,cliptr,typ_l,dep_ut,typ_m]=make_ptr(bllst,clkptr,cliptr,typ_l,..
      dep_ut,typ_m)

  if show_trace then disp('c_pass3001:'+string(timer())),end
  
  clkconnect=cleanup(clkconnect)
  
  if show_trace then disp('c_pass3011:'+string(timer())),end
  
  [ok,done,bllst,connectmat,clkconnect,typ_l,typ_m,corinv]=paksazi(bllst,..
      connectmat,clkconnect,corinv,clkptr,cliptr,typ_l,typ_m,dep_ut)
  
  if show_trace then disp('c_pass300011:'+string(timer())),end
  if ~ok then
    cpr=list()
    return
  end
end

if show_trace then disp('c_pass31:'+string(timer())),end

//extract various info from bllst
[lnkptr,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,..
    xptr,zptr,rpptr,ipptr,xc0,xd0,rpar,ipar,dep_ut,..
    typ_z,typ_s,typ_x,typ_m,funs,funtyp,initexe,labels,..
    bexe,boptr,blnk,blptr,ok]=extract_info(bllst,..
    connectmat,clkconnect)

if ~ok then 
  cpr=list()
  return,
end


if ~or(typ_x) & or(typ_z) then
  message(['For using treshold, you need to have'
      'a continuous system with state in your diagram.';
      'You can include DUMMY CLSS block (linear palette)'
      'in your diagram.']);
  cpr=list()
  ok=%f;
  return
end

if show_trace then disp('c_pass41:'+string(timer())),end


//form a matrix which gives destinations of each block
[outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)
[evoutoin,evoutoinptr]=synch_clkconnect(typ_s,clkconnect)
//
if show_trace then disp('c_pass50:'+string(timer())),end
//
// discard duplicate calls to the same block port
// and group calls to different ports of the same block
// to compute execution table and its pointer.
[ordptr1,execlk,execlk0,execlk_cons]=..
    discard(clkptr,cliptr,clkconnect,exe_cons)

clkconnect=[];exe_cons=[]

if show_trace then disp('c_pass501:'+string(timer())),end

// Set execution scheduling tables 
[ordptr,ordclk,cord,iord,oord,zord,critev,ok]=scheduler(inpptr,..
    outptr,clkptr,execlk,execlk0,execlk_cons,ordptr1,outoin,outoinptr,..
    evoutoin,evoutoinptr,typ_z,typ_x,typ_s,bexe,boptr,blnk,blptr);

if ~ok then 
  cpr=list()
  return,
end

if show_trace then disp('c_pass51:'+string(timer())),end
//form scicos arguments
izptr=ones(nblk+1,1)

ztyp=0*ones(typ_z)
ztyp(typ_z)=1

simtp=['scs','funs','xptr','zptr','izptr','inpptr','outptr','inplnk',..
    'outlnk','lnkptr','rpar','rpptr',..
    'ipar','ipptr','clkptr','ordptr','execlk','ordclk','cord','oord',..
    'zord','critev','nb','ztyp','nblk','ndcblk','subscr','funtyp',..
    'iord','labels']

subscr=[]
ncblk=0;nxblk=0;ndblk=0;ndcblk=0;
sim=tlist(simtp,funs,xptr,zptr,izptr,..
    inpptr,outptr,inplnk,outlnk,..
    lnkptr,rpar,rpptr,ipar,ipptr,clkptr,..
    ordptr,execlk,ordclk,cord,oord,zord,..
    critev(:),size(typ_z,'*'),ztyp,nblk,ndcblk,subscr,funtyp,iord,labels);

//initialize agenda
[tevts,evtspt,pointi]=init_agenda(initexe,clkptr)
if show_trace then disp('c_pass61:'+string(timer())),end

statetp=['xcs','x','z','iz','tevts','evtspt','pointi','outtb']
outtb=0*ones(lnkptr($)-1,1)
iz0=[]
state=tlist(statetp,xc0,xd0,iz0,tevts,evtspt,pointi,outtb);
cpr=list(state,sim,cor,corinv)

if show_trace then disp('c_pass71:'+string(timer())),end

/////////////////////////////////////////////////////////////////////

function [ordptr2,ordclk,cord,iord,oord,zord,critev,ok]=..
    scheduler(inpptr,..
    outptr,clkptr,execlk,execlk0,execlk_cons,ordptr1,outoin,outoinptr,..
    evoutoin,evoutoinptr,typ_z,typ_x,typ_s,bexe,boptr,blnk,blptr);
//
nblk=size(typ_x,1)
if execlk0<>[] then
  //compute cord
  t_var_blk=execlk0(:,1)
  wec=zeros(1,nblk)
  wec(t_var_blk')=execlk0(:,2)'
  vec=-ones(1,nblk)
  vec(t_var_blk)=0*t_var_blk' // time varying blocks
  [r,ok]=new_tree2(vec,outoin,outoinptr,dep_ut)
  cord=[r,wec(r)']
else
  cord=[]
end

//
//compute iord
if execlk_cons<>[] then
  vec=-ones(1,nblk)
  no_tu_dep=execlk_cons(:,1)
  wec=zeros(1,nblk)
  wec(no_tu_dep')=execlk_cons(:,2)'
  vec(no_tu_dep)=0*no_tu_dep'
  [r,ok]=new_tree2(vec,outoin,outoinptr,dep_ut)
  iord=[r,wec(r)']
else
  iord=[]
end
//
if ~ok then 
  message('Algebraic loop detected; cannot be compiled.');
  ordptr2=[],ordclk=[],cord=[],iord=[],oord=[],zord=[],critev=[]
  return,
end

ordclk=[]
ordptr2=ordptr1
for o=1:clkptr(nblk+1)-1
  vec=-ones(1,nblk);
  wec=zeros(1,nblk);
  vec(execlk(ordptr1(o):ordptr1(o+1)-1,1)')=..
      zeros(execlk(ordptr1(o):ordptr1(o+1)-1,1))';
  wec(execlk(ordptr1(o):ordptr1(o+1)-1,1)')=..
      execlk(ordptr1(o):ordptr1(o+1)-1,2)';  
  [r,ok]=new_tree2(vec,outoin,outoinptr,dep_ut)
  
  if ~ok then 
    message('Algebraic loop detected; cannot be compiled.');
    ordptr2=[],ordclk=[],cord=[],iord=[],oord=[],zord=[],critev=[]
    return,
  end
  //
  r=[r,wec(r)']
  ordptr2(1+o)=size(r,1)+ordptr2(o)
  ordclk=[ordclk;r]
end

if ordptr1<>ordptr2 then disp("serious bug,report");pause;end
//ordptr=[ordptr1,ordptr2];

zord=cord
oord=cord
n=size(cord,1)

    vec=-ones(1,nblk);
    vec(cord(:,1))=0;
    
    //    [ext_cord,ok]=new_tree3(vec,dep_ut,typ_s);
typp=zeros(typ_s);typp(typ_s)=1
    [ext_cord,ok]=new_tree3(vec,dep_ut,typp);

    if ~ok then disp('serious bug, report.');pause;end
    ext_cord=ext_cord(n+1:$);


for iii=n:-1:1
  i=cord(iii,1)
  fl=%f
  fz=%f
//  if typ_s(i) then fz=%t;fl=%t; end
  for ii=[outoin(outoinptr(i):outoinptr(i+1)-1,1)',..
      evoutoin(evoutoinptr(i):evoutoinptr(i+1)-1,1)']
    //ii est un block affecte par changement de sortie du 
    //i-eme block de oord
    //    if ii<=nxblk | ii>nb then fz=%t;end
    if typ_z(ii) then fz=%t;end
    if typ_x(ii) then fl=%t;end
    if fl&fz then break,end
    //si ii est un block integre (continu avec etat) 
    //il faut garder i
//    for l=iii+1:n
      //si ii est un block qu'on a decide de garder 
      //il faut garder i
      if or(ii==[zord(iii+1:$,1)',ext_cord]) then fz=%t; end
      if or(ii==[oord(iii+1:$,1)',ext_cord]) then fl=%t; end
//      if fl&fz then break,end
//    end
    if fl&fz then break; end
  end
  //mettre a zero si block doit etre supprimer
  if ~fl&~typ_x(i) then oord(iii,1)=0; end
  if ~fz&~typ_z(i) then zord(iii,1)=0; end
end

//supprimer les blocks a supprimer
oord=oord(oord(:,1)<>zeros(oord(:,1)),:);
zord=zord(zord(:,1)<>zeros(zord(:,1)),:)

//critev: vecteur indiquant si evenement est important pour tcrit
//ordclk_fut et ordptr3 sont l'analogue de ordclk et ordptr2 sauf
//pour le fait que la dependance en temps n'est pas pris en compte.
//Donc les blocks indiques sont des blocks susceptibles de produire
//des discontinuites quand l'evenement se produit

// 1: important; 0:non
n=clkptr(nblk+1)-1 //nb d'evenement
//a priori tous les evenemets sont non-importants
critev=zeros(n,1)
for i=1:n
  fl=%f
  for hh=ordptr1(i):ordptr1(i+1)-1
  jj= ordclk(hh,1) //block excite par evenement i
    if ~(ordclk(hh,2)==0) then
      for ii=[outoin(outoinptr(jj):outoinptr(jj+1)-1,1)',..
	  evoutoin(evoutoinptr(jj):evoutoinptr(jj+1)-1,1)']
	//block excite par block excite par evenement i
	//si il est integre, i est important
	if typ_x(ii) | typ_z(ii) then fl=%t;break; end
      end
    end
    if fl then break;end
  end
  if fl then critev(i,1)=1; end
end


function [ord,ok]=tree3(vec,dep_ut,typ_l)
//compute blocks execution tree
ok=%t
nb=size(vec,'*')
for j=1:nb+2
  fini=%t
  for i=1:nb
    if vec(i)==j-1&typ_l(i)<>-1 then 
      if j==nb+2 then 
	message('algebraic loop detected');ok=%f;ord=[];return;
      end
      if typ_l(i)==1 then
	fini=%f;
	kk=bexe(boptr(i):boptr(i+1)-1)';
      else
	kk=[];
	for ii=blnk(blptr(i):blptr(i+1)-1)'
	  if vec(ii)>-1 & (dep_ut(ii,1) | (typ_l(ii)==1)) then
	    fini=%f;
	    kk=[kk ii];
	  end
	end
      end
      vec(kk)=j*ones(kk) ;   //disp(vec)
    end
  end
  if fini then break;end
end
[k,ord]=sort(-vec);
ord(find(k==1))=[];


function [okk,done,bllst,connectmat,clkconnect,typ_l,typ_m,corinv]=..
    paksazi(bllst,connectmat,clkconnect,corinv,clkptr,cliptr,..
    typ_l,typ_m,dep_ut)
okk=%t
nblk=length(bllst)
nblkorg=nblk
if ~or(typ_l) then
  done=%t;
  return
end
change=%f
for lb=find(typ_l)
  indx=find(clkconnect(:,3)==lb) 
  if indx==[] then 
     message(['A synchro block is inactive';'cannot be compile']);
     okk=%f;return
  end
  if or(clkconnect(indx,1)==lb) then 
       message(['Algebraic loop detected';'on activation links']);
       okk=%f;return
  end
  nn=size(indx,'*')
  if nn>=2 then
    indxo=find(clkconnect(:,1)==lb)
    indy=find(connectmat(:,3)==lb)
    if size(indy,'*')>1 then 
      disp('Synchro block cannot have more than 1 input')
    end
    for k=2:nn
      clkconnect(indx(k),3)=nblk+1;
      bllst(nblk+1)=bllst(lb);
      corinv(nblk+1)=corinv(lb);
      tmp=clkconnect(indxo,:);
      yek=ones(tmp(:,1))
      clkconnect=[clkconnect;[yek*(nblk+1),tmp(:,[2 3 4])]]
      nblk=nblk+1
    end
    onn=ones(nn-1,1)
    connectmat=[connectmat;..
	[onn*connectmat(indy,[1 2]),[nblkorg+1:nblk]',onn]]
    change=%t
    nblkorg=nblk
  end
end
if change then done=%f;return; end
//

clkconnecttmp=clkconnect;
clkconnect=clkconnecttmp(find(clkconnecttmp(:,1)<>0),:);
clkconnect0=clkconnecttmp(find(clkconnecttmp(:,1)==0),:);
bclkconnect0=clkconnect0(:,[1 3]);
con0=zeros(clkconnect0(:,1));

texeclk0=bclkconnect0(find(bclkconnect0(:,1)==0),2);


con=clkptr(clkconnect(:,1))+clkconnect(:,2)-1;
[junk,ind]=sort(-con);con=-junk;
clkconnect=clkconnect(ind,:);
//
bclkconnect=clkconnect(:,[1 3]);
boptr=1;
bexe=[];
for i=1:nblk
  r=bclkconnect(find(bclkconnect(:,1)==i),2);
  bexe=[bexe;r];
  boptr=[boptr;boptr($)+size(r,1)];
end
//
bconnectmat=connectmat(:,[1 3]);
blptr=1;
blnk=[];

for i=1:nblk
  r=bconnectmat(find(bconnectmat(:,1)==i),2);
  blnk=[blnk;r];
  blptr=[blptr;blptr($)+size(r,1)];
end  
//
tclkconnect=clkconnect(~typ_l(clkconnect(:,1)),:);
tcon=clkptr(tclkconnect(:,1))+tclkconnect(:,2)-1;
texeclk=tclkconnect(:,[3 4]);

ordptr1=1;
for i=1:clkptr($)-1
  tmp=find(tcon<=i);
  if tmp==[] then 
    ordptr1(i+1)=ordptr1(i);
  else
    ordptr1(i+1)=max(tmp)+1;
  end
end
//
clkconnect=[clkconnect0;clkconnect];
con=[con0;con];
//
pointer=[];
for o=0:clkptr($)-1
  if o==0 then texeclki=texeclk0; else texeclki=texeclk(ordptr1(o):ordptr1(o+1)-1,1);end
  if texeclki<>[] then
    vec=-ones(1,nblk);
    vec(texeclki')=zeros(texeclki)';
    
    //    [r,ok]=new_tree3(vec,dep_ut,typ_l);
typ_lm=zeros(typ_l);typ_lm(typ_l)=1;typ_lm(typ_m)=-1;
[r,ok]=new_tree3(vec,dep_ut,typ_lm);

    if ~ok then 
      message('Algebraic loop detected; cannot be compiled.');
      bllst=[];connectmat=[];clkconnect=[];typ_l=[];corinv=[]
      okk=%f;done=%t;return,
    end
    
    pointer=find(con==o)
    for bl=r
      if typ_l(bl) then
	mod=bllst(bl);mod(10)='s',bllst(bl)=mod,typ_l(bl)=%f
	pointer=pointer(find(clkconnect(pointer,3)<>bl));
	yek=ones(pointer');
	clkconnect(pointer,:)=..
	    [yek*bl,yek,clkconnect(pointer,[3 4])];
	//connect all the event outputs of the logical block to ....
	for bl_out=2:clkptr(bl+1)-clkptr(bl)
	  clkconnect=[clkconnect;[yek*bl,bl_out*yek,clkconnect(pointer,[3 4])]];
	end
	//
	ok=%f,return
      else
	pointer=pointer(find(clkconnect(pointer,3)<>bl))
      end
    end
  end
  if pointer<>[] then warning('problem1');pause;end
end;
//

if or(typ_l) then warning('problem2');pause;end
//
okk=%t;done=%t;

function [ordptr1,execlk,clkconnectj0,clkconnectj_cons]=..
    discard(clkptr,cliptr,clkconnect,exe_cons)

if exe_cons<>[] then
  clkconnectj=exe_cons
  mma=maxi(clkconnectj(:,2))+1
  con=mma*(clkconnectj(:,1))+clkconnectj(:,2)
  [junk,ind]=sort(-con);con=-junk
  clkconnectj=clkconnectj(ind,:)
  // discard duplicate calls to the same block port
  if size(con,'*')>=2 then
    clkconnectj(find(con(2:$)-con(1:$-1)==0),:)=[]
  end
  // group calls to different ports of the same block.
  clkconnectj=[clkconnectj(:,1),2^(clkconnectj(:,2)-ones(clkconnectj(:,2)))]
  clkconnectj=int(clkconnectj)
  con=clkconnectj(:,1)
  clkconnectj_cons=[]
  if size(con,'*')>=2 then 
    iini=[find(con(2:$)-con(1:$-1)<>0),size(clkconnectj,1)]
  else
    iini=1
  end
  for ii=iini
    clkconnectj_cons=[clkconnectj_cons;[clkconnectj(ii,1),..
	mysum(clkconnectj(find(clkconnectj(:,1)==clkconnectj(ii,1)),2))]]
  end
else
  clkconnectj_cons=[]
end

clkconnecttmp=clkconnect
clkconnect=clkconnecttmp(find(clkconnecttmp(:,1)<>0),:)
clkconnect0=clkconnecttmp(find(clkconnecttmp(:,1)==0),:)
if clkconnect0<>[] then
  clkconnectj=[clkconnect0(:,3),clkconnect0(:,4)]
//  con=cliptr(clkconnectj(:,1))+clkconnectj(:,2)-ones(clkconnectj(:,2))
  mma=maxi(clkconnectj(:,2))+1
  con=mma*clkconnectj(:,1)+clkconnectj(:,2)
  //
  [junk,ind]=sort(-con);con=-junk
  clkconnectj=clkconnectj(ind,:)
  // discard duplicate calls to the same block port
  if size(con,'*')>=2 then
    clkconnectj(find(con(2:$)-con(1:$-1)==0),:)=[]
  end
  // group calls to different ports of the same block.
  clkconnectj=[clkconnectj(:,1),2^(clkconnectj(:,2)-ones(clkconnectj(:,2)))]
  clkconnectj=int(clkconnectj)
  con=clkconnectj(:,1)
  clkconnectj0=[]
  if size(con,'*')>=2 then 
    iini=[find(con(2:$)-con(1:$-1)<>0),size(clkconnectj,1)]
  else
    iini=1
  end
  for ii=iini
    clkconnectj0=[clkconnectj0;[clkconnectj(ii,1),..
	mysum(clkconnectj(find(clkconnectj(:,1)==clkconnectj(ii,1)),2))]]
  end
else
  clkconnectj0=[]
end



con=clkptr(clkconnect(:,1))+clkconnect(:,2)-1
[junk,ind]=sort(-con);con=-junk
clkconnect=clkconnect(ind,:)
//
  ordptr1=1
  for i=1:clkptr($)-1
    tmp=find(con<=i)
    if tmp==[] then 
      ordptr1(i+1)=ordptr1(i)
    else
      ordptr1(i+1)=max(tmp)+1
    end
  end
execlk=[]
new_ordptr1=1

if show_trace then disp('c_pass50001:'+string(timer())),end

for j=1:clkptr($)-1
  if ordptr1(j)<>ordptr1(j+1) then
    clkconnectj=[clkconnect(ordptr1(j):ordptr1(j+1)-ones(ordptr1(j+1)),3),..
	clkconnect(ordptr1(j):ordptr1(j+1)-1,4)]
//    con=cliptr(clkconnectj(:,1))+clkconnectj(:,2)-ones(clkconnectj(:,2))
    mma=maxi(clkconnectj(:,2))+1
    con=mma*clkconnectj(:,1)+clkconnectj(:,2)
    [junk,ind]=sort(-con);con=-junk
    clkconnectj=clkconnectj(ind,:)
    // discard duplicate calls to the same block port
    if size(con,'*')>=2 then
      clkconnectj(find(con(2:$)-con(1:$-1)==0),:)=[]
    end
    // group calls to different ports of the same block.
    clkconnectj=[clkconnectj(:,1),2^(clkconnectj(:,2)-ones(clkconnectj(:,2)))]
    clkconnectj=int(clkconnectj)
    con=clkconnectj(:,1)
    clkconnectjj=[]
    if size(con,'*')>=2 then 
      iini=[find(con(2:$)-con(1:$-1)<>0),size(clkconnectj,1)]
    else
      iini=1
    end
    for ii=iini
      clkconnectjj=[clkconnectjj;[clkconnectj(ii,1),..
	  mysum(clkconnectj(find(clkconnectj(:,1)==clkconnectj(ii,1)),2))]]
    end
  else
    clkconnectjj=[]
  end

  execlk=[execlk;clkconnectjj]
  new_ordptr1=[new_ordptr1;new_ordptr1($)+size(clkconnectjj,1)]
end
ordptr1=new_ordptr1


function a=mysum(b)
if b<>[] then a=sum(b), else a=[], end


function [lnkptr,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,..
    xptr,zptr,rpptr,ipptr,xc0,xd0,rpar,ipar,dep_ut,..
    typ_z,typ_s,typ_x,typ_m,funs,funtyp,initexe,labels,..
    bexe,boptr,blnk,blptr,ok]=extract_info(bllst,..
    connectmat,clkconnect)

ok=%t
nbl=length(bllst)
clkptr=zeros(nbl+1,1);clkptr(1)=1
cliptr=clkptr;inpptr=cliptr;outptr=inpptr;
//clkptr=1;cliptr=1;
//inpptr=1;outptr=1;
xptr=1;zptr=1;
rpptr=clkptr;ipptr=clkptr;
//rpptr=1;ipptr=1;

//
xc0=[];xd0=[];rpar=[];ipar=[];

fff=ones(nbl,1)==1
dep_ut=[fff,fff];typ_z=fff;typ_s=fff;typ_x=fff;typ_m=fff;

initexe=[];
funs=list();
funtyp=zeros(typ_z)
labels=[]
//
//
for i=1:nbl
  ll=bllst(i)
  if type(ll(1))==15 then
    funs(i)=ll(1)(1)
    funtyp(i,1)=ll(1)(2)
  else
    funs(i)=ll(1);
    funtyp(i,1)=0;
  end
  if funtyp(i,1)>999 then
    if ~c_link(funs(i)) then
      x_message(['A C or Fortran block is used but not linked';
	  'You can save your compiled diagram and load it';
	  'This will automatically link the C or Fortran function'])
    end
  end
  inpnum=ll(2);outnum=ll(3);cinpnum=ll(4);coutnum=ll(5);
  //
  inpptr(i+1)=inpptr(i)+size(inpnum,'*')
outptr(i+1)=outptr(i)+size(outnum,'*')
cliptr(i+1)=cliptr(i)+size(cinpnum,'*')
clkptr(i+1)=clkptr(i)+size(coutnum,'*')
  //
  xc0=[xc0;ll(6)(:)]
    xptr(i+1)=xptr(i)+size(ll(6),'*')
  
  if funtyp(i,1)==3 then //sciblocks
    if ll(7)==[] then xd0k=[]; else xd0k=var2vec(ll(7));end
  else
    xd0k=ll(7)(:)
  end
  xd0=[xd0;xd0k]
  zptr(i+1)=zptr(i)+size(xd0k,'*')
  //  
  if funtyp(i,1)==3 then //sciblocks
    if ll(8)==[] then rpark=[]; else rpark=var2vec(ll(8));end
  else
    rpark=ll(8)(:)
  end
  rpar=[rpar;rpark]
  rpptr(i+1)=rpptr(i)+size(rpark,'*')
  if type(ll(9))==1 then 
    ipar=[ipar;ll(9)(:)]
    ipptr(i+1)=ipptr(i)+size(ll(9),'*')
  else
    ipptr(i+1)=ipptr(i)
  end
  //
  typ_z(i)=ll(10)=='z'
  typ_s(i)=ll(10)=='s'
  typ_x(i)=ll(6)<>[]
  typ_m(i)=ll(10)=='m'
  dep_ut(i,:)=(ll(12)(:))'
  //
  if ll(5)<>[] then  
    ll11=ll(11)
    prt=find(ll11>=zeros(ll11))
    nprt=prod(size(prt))
    initexe=[initexe;..
	[i*ones(nprt,1),matrix(prt,nprt,1),matrix(ll11(prt),nprt,1)]];
  end

  if size(ll)>=13 then
    if type(ll(13))==10 then
      labels=[labels;ll(13)(1)]
    else
      labels=[labels;' ']
    end
  else
    labels=[labels;' ']
  end
end

clkconnect=clkconnect(find(clkconnect(:,1)<>0),:);

con=clkptr(clkconnect(:,1))+clkconnect(:,2)-1;
[junk,ind]=sort(-con);con=-junk;
clkconnect=clkconnect(ind,:);
//
bclkconnect=clkconnect(:,[1 3]);
boptr=1;
bexe=[];
for i=1:nbl
  r=bclkconnect(find(bclkconnect(:,1)==i),2);
  bexe=[bexe;r];
  boptr=[boptr;boptr($)+size(r,1)];
end
//
bconnectmat=connectmat(:,[1 3]);
blptr=1;
blnk=[];

for i=1:nbl
  r=bconnectmat(find(bconnectmat(:,1)==i),2);
  blnk=[blnk;r];
  blptr=[blptr;blptr($)+size(r,1)];
end  
//



[ok,bllst]=adjust_inout(bllst,connectmat)
nlnk=size(connectmat,1)
inplnk=zeros(inpptr($)-1,1);outlnk=zeros(outptr($)-1,1);ptlnk=1;
lnkbsz=[]
for jj=1:nlnk
  ko=outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)
  ki=inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)
  if ko<>0 & ki<>0 then
    if ko>ki then 
      outlnk(outlnk>ko)=outlnk(outlnk>ko)-1
      outlnk(outlnk==ko)=ki
      inplnk(inplnk>ko)=inplnk(inplnk>ko)-1
      inplnk(inplnk==ko)=ki
      ptlnk=-1+ptlnk
      lnkbsz(ko)=[]
    elseif ki>ko
      outlnk(outlnk>ki)=outlnk(outlnk>ki)-1
      outlnk(outlnk==ki)=ko
      inplnk(inplnk>ki)=inplnk(inplnk>ki)-1
      inplnk(inplnk==ki)=ko
      ptlnk=-1+ptlnk
      lnkbsz(ki)=[]
    end
    
  elseif ko<>0 then
    inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)=ko
  elseif ki<>0 then
    outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)=ki
  else
    outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)=ptlnk
    inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)=ptlnk
    lnkbsz=[lnkbsz;bllst(connectmat(jj,1))(3)(connectmat(jj,2))]
    ptlnk=1+ptlnk
  end
end
lnkptr=cumsum([1;lnkbsz])


//store unconnected outputs, if any, at the end of outtb
unco=find(outlnk==0);
if unco==[] then return;end
siz_unco=0
for j=unco
  m=maxi(find(outptr<=j))
  n=j-outptr(m)+1
  siz_unco=maxi(siz_unco,bllst(m)(3)(n))
end
lnkptr=[lnkptr;lnkptr($)+siz_unco]
outlnk(unco)=maxi(outlnk)+1

  
function [outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)
outoin=[];outoinptr=1
nblk=size(inpptr,1)-1
for i=1:nblk
  k=outptr(i):outptr(i+1)-1
  ii=[]
  for j=outlnk(k)'
    ii=[ii,find(inplnk==j)]
  end
  outoini=[];jj=0
  for j=ii
    m=maxi(find(inpptr<=j))
    n=j-inpptr(m)+1
    outoini=[outoini;[m,n]]
    jj=jj+1
  end
  outoinptr=[outoinptr;outoinptr($)+jj]
  outoin=[outoin;outoini]
end


function [clkptr,cliptr,typ_l,dep_ut,typ_m]=..
                  make_ptr(bllst,clkptr,cliptr,typ_l,dep_ut,typ_m)
nblk0=size(clkptr,'*')
nbl=size(bllst)
if nbl<nblk0 then return; end

i=nblk0;ll=bllst(i)
cliptr1=zeros(nbl-nblk0,1);clkptr1=cliptr1;
cliptr1(1)=cliptr($)+sum(ll(4))
clkptr1(1)=clkptr($)+sum(ll(5))
typ_l1=cliptr1==1;dep_ut1=[typ_l1,typ_l1];
typ_l1(1)=ll(10)=='l';typ_m1(1)=ll(10)=='m';dep_ut1(1,:)=ll(12);
j=1
for i=nblk0+1:nbl
  j=j+1
  ll=bllst(i)
  cliptr1(j)=cliptr1(j-1)+sum(ll(4))
  clkptr1(j)=clkptr1(j-1)+sum(ll(5))
  typ_l1(j)=ll(10)=='l'
  typ_m1(j)=ll(10)=='m'
  dep_ut1(j,:)=ll(12)
end
cliptr=[cliptr;cliptr1]
clkptr=[clkptr;clkptr1]
typ_l=[typ_l;typ_l1]
typ_m=[typ_m;typ_m1]
dep_ut=[dep_ut;dep_ut1];


function [ord,ok]=tree2(vec,outoin,outoinptr,dep_ut)
//compute blocks execution tree
ok=%t;
nb=size(vec,'*');
for j=1:nb+2
  fini=%t
  for i=1:nb
    if vec(i)==j-1 then 
      if j==nb+2 then 
	message('algebraic loop detected');ok=%f;ord=[];return;
      end
      //      kk=[];
      for k=outoinptr(i):outoinptr(i+1)-1
	ii=outoin(k,1);
	if dep_ut(ii,1)&vec(ii)>-1 then
	  fini=%f;
	  //	 kk=[kk ii];
	  vec(ii)=j;   
	end
      end
      //      vec(kk)=j*ones(kk) ;   
    end
  end
  if fini then break;end
end
[k,ord]=sort(-vec);
ord(find(k==1))=[];
ord=ord(:)


function [ok,bllst]=adjust_inout(bllst,connectmat)
nlnk=size(connectmat,1)
for hhjj=1:length(bllst)
for hh=1:length(bllst)
  ok=%t
  for jj=1:nlnk
    nout=bllst(connectmat(jj,1))(3)(connectmat(jj,2))
    nin=bllst(connectmat(jj,3))(2)(connectmat(jj,4))
    if (nout>0&nin>0) then
      if nin<>nout then
	bad_connection(corinv(connectmat(jj,1)),connectmat(jj,2),nout,..
	    corinv(connectmat(jj,3)),connectmat(jj,4),nin)
	ok=%f;return
      end
    elseif (nout>0&nin<0) then 
      ww=find(bllst(connectmat(jj,3))(2)==nin)
      bllst(connectmat(jj,3))(2)(ww)=nout
      
      ww=find(bllst(connectmat(jj,3))(3)==nin)
      bllst(connectmat(jj,3))(3)(ww)=nout
      
      ww=find(bllst(connectmat(jj,3))(3)==0)
      if (ww<>[]&mini(bllst(connectmat(jj,3))(2)(:))>0) then
	bllst(connectmat(jj,3))(3)(ww)=sum(bllst(connectmat(jj,3))(2)(:))
      end
      
    elseif (nin>0&nout<0) then 
      ww=find(bllst(connectmat(jj,1))(3)==nout)
      bllst(connectmat(jj,1))(3)(ww)=nin

      ww=find(bllst(connectmat(jj,1))(2)==nout)
      bllst(connectmat(jj,1))(2)(ww)=nin
      
      ww=find(bllst(connectmat(jj,1))(2)==0)
      if (ww<>[]&mini(bllst(connectmat(jj,1))(3)(:))>0) then 
	bllst(connectmat(jj,1))(2)(ww)=sum(bllst(connectmat(jj,1))(3))
      end
      

    elseif (nin==0) then
      ww=bllst(connectmat(jj,3))(3)(:)
      if mini(ww)>0 then 
	if nout>0 then
	  if sum(ww)==nout then
	    bllst(connectmat(jj,3))(2)(connectmat(jj,4))=nout
	  else
	    bad_connection(corinv(connectmat(jj,3)))
	    ok=%f;return
	  end
	else
	  bllst(connectmat(jj,3))(2)(connectmat(jj,4))=sum(ww)
	  ok=%f
	end
      else      
	nww=ww(find(ww<0))
	if norm(nww-nww(1),1)==0 & nout>0 then
	  bllst(connectmat(jj,3))(2)(connectmat(jj,4))=nout
	  k=(nout-sum(ww(find(ww>0))))/size(nww,'*')
	  if k==int(k) then
	    bllst(connectmat(jj,3))(3)(find(ww<0))=k
	  else
	    bad_connection(corinv(connectmat(jj,3)))
	    ok=%f;return
	  end
	else
	  ok=%f
	end
      end

    elseif (nout==0) then
      ww=bllst(connectmat(jj,1))(2)(:)
      if mini(ww)>0 then 
	if nin>0 then
	  if sum(ww)==nin then
	    bllst(connectmat(jj,1))(3)(connectmat(jj,2))=nin
	  else
	    bad_connection(corinv(connectmat(jj,1)))
	    ok=%f;return
	  end
	else
	  bllst(connectmat(jj,1))(3)(connectmat(jj,2))=sum(ww)
	  ok=%f
	end
      else      
	nww=ww(find(ww<0))
	if norm(nww-nww(1),1)==0 & nin>0 then
	  bllst(connectmat(jj,1))(3)(connectmat(jj,2))=nin
	  k=(nout-sum(ww(find(ww>0))))/size(nww,'*')
	  if k==int(k) then
	    bllst(connectmat(jj,1))(2)(find(ww<0))=k
	  else
	    bad_connection(corinv(connectmat(jj,1)))
	    ok=%f;return
	  end
	else
	  ok=%f
	end
      end	

    else
      //case where both are negative
      ok=%f
    end
  end
if ok then return, end
end
message('Not enough information to determine port sizes');  
  for jj=1:nlnk
    nout=bllst(connectmat(jj,1))(3)(connectmat(jj,2))
    nin=bllst(connectmat(jj,3))(2)(connectmat(jj,4))
    if nout<=0&nin<=0 then
      	ninnout=under_connection(corinv(connectmat(jj,1)),connectmat(jj,2),nout,..
	    corinv(connectmat(jj,3)),connectmat(jj,4),nin)
	if ninnout==[] then ok=%f;return;end
	if ninnout<=0  then ok=%f;return;end
      bllst(connectmat(jj,1))(3)(connectmat(jj,2))=ninnout
      bllst(connectmat(jj,3))(2)(connectmat(jj,4))=ninnout
    end
  end
end

function ninnout=under_connection(path_out,prt_out,nout,path_in,prt_in,nin)
// alert for badly connected blocks
// path_out : Path of the "from block" in scs_m
// path_in  : Path of the "to block" in scs_m
//!
  lp=mini(size(path_out,'*'),size(path_in,'*'))
  k=find(path_out(1:lp)<>path_in(1:lp))
  path=path_out(1:k(1)-1) // common superbloc path
  path_out=path_out(k(1)) // "from" block number
  path_in=path_in(k(1))   // "to" block number
  
  if path==[] then
    hilite_obj(scs_m(path_out))
    if or(path_in<>path_out) then hilite_obj(scs_m(path_in)),end

      ninnout=evstr(dialog(['Hilited block(s) have connected ports ';
	'with  sizes that cannot be determiend by the context';
	'what is the size of this link'],'1'))
    hilite_obj(scs_m(path_out))
    if or(path_in<>path_out) then hilite_obj(scs_m(path_in)),end
  else
    mxwin=maxi(winsid())
    for k=1:size(path,'*')
      hilite_obj(scs_m(path(k)))
      scs_m=scs_m(path(k))(3)(8);
      scs_show(scs_m,mxwin+k)
    end
    hilite_obj(scs_m(path_out))
    if or(path_in<>path_out) then hilite_obj(scs_m(path_in)),end
      ninnout=evstr(dialog(['Hilited block(s) have connected ports ';
	'with  sizes that cannot be determiend by the context';
	'what is the size of this link'],'1'))
    for k=size(path,'*'):-1:1,xdel(mxwin+k),end
    scs_m=null()
    unhilite_obj(scs_m(path(1)))
  end



function [clkconnect,exe_cons]=pak_ersi(connectmat,clkconnect,..
    dep_ut,typ_r,typ_l,outoin,outoinptr,tblock,typ_cons,clkptr)

//add every event to the time event because time includes all events
all_out=[]
for k=1:size(clkptr,1)-1
  if ~typ_l(k) then
    kk=[1:(clkptr(k+1)-clkptr(k))]'
    all_out=[all_out;[k*ones(kk),kk]]
  end
end
all_out=[all_out;[0,0]]

//add time event if needed
ind=find(tblock)
ind=ind(:)
for k=ind'
  clkconnect=[clkconnect;[all_out,ones(all_out)*[k,0;0,0]]]
end
if show_trace then disp('c_pass4444:'+string(timer())),end
ind1=find(typ_cons)
ind=[ind;ind1(:)]
exe_cons=[ind,zeros(ind)]

vec=-ones(1,nblk);
vec(ind)=0
[r,ok]=new_tree4(vec,outoin,outoinptr,typ_r)
exe_cons=[exe_cons;r]

if show_trace then disp('c_pass4445:'+string(timer())),end

[clkr,clkc]=size(clkconnect);
mm=maxi(clkconnect)+1;
cll=clkconnect(:,1)*mm+clkconnect(:,2);
[cll,ind]=sort(-cll);
clkconnect=clkconnect(ind,:);
cll=[-1;-cll;mm];
ii=find(cll(2:$)-cll(1:$-1)<>0)


for k=1:size(ii,'*')-1
  oo=[ii(k):ii(k+1)-1]
  vec=-ones(1,nblk);
  vec(clkconnect(oo,3))=0

  [r,ok]=new_tree4(vec,outoin,outoinptr,typ_r)
  m=size(r,1)
  r=[clkconnect(ii(k),1)*ones(m,1),clkconnect(ii(k),2)*ones(m,1),r]
  clkconnect=[clkconnect;r]
end




if show_trace then disp('c_pass4446:'+string(timer())),end 

function [r,ok]=tree4(vec,outoin,outoinptr,typ_r)
//compute blocks which inherit
ok=%t;
nb=size(vec,'*');
r=[];
for j=1:nb-1
  fini=%t
  for i=1:nb
    if vec(i)==j-1 then 
      for k=outoinptr(i):outoinptr(i+1)-1
	ii=outoin(k,1);
	if (vec(ii)>-1)|typ_r(ii) then
	  fini=%f;
	  vec(ii)=j;   
	end
	if typ_r(ii) then 
	  r=[r;outoin(k,:)]
	end
      end
    end
  end
  if fini then break;end
end


function [bllst,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,..
    dep_ut,typ_l,typ_r,typ_m,tblock,typ_cons,ok]=mini_extract_info(bllst,..
    connectmat,clkconnect)

ok=%t
nbl=length(bllst)
clkptr=zeros(nbl+1,1);clkptr(1)=1
cliptr=clkptr;inpptr=cliptr;outptr=inpptr;
//clkptr=1;cliptr=1;inpptr=1;outptr=1;
fff=ones(nbl,1)==1
dep_ut=[fff,fff];typ_l=fff;typ_r=fff;typ_cons=fff;typ_m=fff;
tblock=fff
//dep_ut=[];typ_l=[];typ_r=[];typ_cons=[]
//tblock=[]  // specifies blocks that must be connected to time event.
//
for i=1:nbl
  ll=bllst(i)
  inpnum=ll(2);outnum=ll(3);cinpnum=ll(4);coutnum=ll(5);
  //    
  if cinpnum==[]&ll(10)<>'z' then
    // this block inherits
    ok=%f
    typ_r(i)=~ll(12)(2)
    tblock(i)=ll(12)(2)
    //if block depends on time but has no event input port
    if ~ll(12)(2) then 
      //inherits from the inputs
      cinpnum=ones(inpnum)
      bllst(i)(4)=cinpnum  //XXXXXXXXXXXXXXXXXXXXX
    end
    //
  else
    tblock(i)=ll(12)(2);typ_r(i)=%f
  end
  inpptr(i+1)=inpptr(i)+size(inpnum,'*')
  outptr(i+1)=outptr(i)+size(outnum,'*')
  cliptr(i+1)=cliptr(i)+size(cinpnum,'*')
  clkptr(i+1)=clkptr(i)+size(coutnum,'*')
  //
  typ_l(i)=ll(10)=='l';typ_m(i)=ll(10)=='m';dep_ut(i,:)=(ll(12)(:))';
  typ_cons(i)=[cinpnum==[]&inpnum==[]&~ll(12)(2)]
//
end
if show_trace then disp('c_pass22222222:'+string(timer())),end //'
nlnk=size(connectmat,1)
inplnk=zeros(inpptr($)-1,1);outlnk=zeros(outptr($)-1,1);ptlnk=1;

for jj=1:nlnk
  ko=outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)
  ki=inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)
  if ko<>0 & ki<>0 then
    if ko>ki then 
      outlnk(outlnk>ko)=outlnk(outlnk>ko)-1
      outlnk(outlnk==ko)=ki
      inplnk(inplnk>ko)=inplnk(inplnk>ko)-1
      inplnk(inplnk==ko)=ki
      ptlnk=-1+ptlnk
    elseif ki>ko
      outlnk(outlnk>ki)=outlnk(outlnk>ki)-1
      outlnk(outlnk==ki)=ko
      inplnk(inplnk>ki)=inplnk(inplnk>ki)-1
      inplnk(inplnk==ki)=ko
      ptlnk=-1+ptlnk
    end
    
  elseif ko<>0 then
    inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)=ko
  elseif ki<>0 then
    outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)=ki
  else
    outlnk(outptr(connectmat(jj,1))+connectmat(jj,2)-1)=ptlnk
    inplnk(inpptr(connectmat(jj,3))+connectmat(jj,4)-1)=ptlnk
    ptlnk=1+ptlnk
  end
end

//store unconnected outputs, if any, at the end of outtb
unco=find(outlnk==0);
if unco==[] then return;end
outlnk(unco)=maxi(outlnk)+1

  
function [evoutoin,evoutoinptr]=synch_clkconnect(typ_s,clkconnect)
nblk=size(typ_s,'*')
evoutoin=[];evoutoinptr=1
for i=1:nblk
  if typ_s(i) then
    dd=clkconnect(clkconnect(:,1)==i,3)
  else 
    dd=[]
  end
  evoutoin=[evoutoin;dd]
  evoutoinptr=[evoutoinptr;evoutoinptr($)+size(dd,'*')]
end

  
function   clkconnect=cleanup(clkconnect)
mm=maxi(clkconnect)+1
cc=clkconnect(:,4)+mm*clkconnect(:,3)+clkconnect(:,2)*mm^2+..
    clkconnect(:,1)*mm^3
[cc1,ind]=sort(-cc)
clkconnect=clkconnect(ind,:)
ind=find(cc1(2:$)-cc1(1:$-1)==0)
clkconnect(ind,:)=[]


function  [r,ok]=new_tree2(vec,outoin,outoinptr,dep_ut)
dd=zeros(dep_ut);dd(dep_ut)=1;
[r,ok2]=sci_tree2(vec,outoin,outoinptr,dd)
ok=ok2==1

function  [r,ok]=new_tree3(vec,dep_ut,typ_l)
dd=zeros(dep_ut);dd(dep_ut)=1;
//ddd=zeros(typ_l);ddd(typ_l)=1; 
[r2,ok2]=sci_tree3(vec,dd,typ_l,bexe,boptr,blnk,blptr)
r=r2'
ok=ok2==1

function  [r,ok]=new_tree4(vec,outoin,outoinptr,typ_r)
nd=zeros(size(vec,'*'),(max(outoin(:,2))+1));
ddd=zeros(typ_r);ddd(typ_r)=1; 
[r1,r2]=sci_tree4(vec,outoin,outoinptr,nd,ddd)
r=[r1',r2']
ok=%t

