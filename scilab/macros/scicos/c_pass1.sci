function  [blklst,cmat,ccmat,cor,corinv,ok]=c_pass1(scs_m,ksup)
//%Purpose
// Determine one level blocks and connections matrix
//%Parameters
// scs_m  :   scicos data structure
// ksup   :
// blklst : a list containing the "model" information structure for each block
//
// cmat   : nx4 matrix. Each row contains, in order, the block
//             number and the port number of an outgoing scicopath,
//             and the block number and the port number of the target
//             ingoing scicopath.
//
// ccmat  : same as cmat but for clock scico-paths.
// cor    : is a list with same recursive structure as scs_m each leaf 
//          contains the index of associated block in blklst 
// corinv : corinv(nb) is the path of nb ith block defined in blklst 
//          in the scs_m structure
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
sup_tab=[]
if rhs<=1 then ksup=0;end

if ksup==0 then   // main scheme
  MaxBlock=countblocks(scs_m);
  nsblk=0; // number of special blocks (clock split,clock sum,super_blocks)
  sup_tab=[]
  path=[] // for delete_unconnected 
  scs_m_s=scs_m // for delete_unconnected 
end
scs_m=delete_unconnected(scs_m); //suppress blocks with an unconnected regular port


//initialize outputs
blklst=list(),nb=0,cor=list(),corinv=list(),cmat=[],ccmat=[];ok=%t
labels=[];
n=size(scs_m)
for k=1:n, cor(k)=0;end


clksplit=[] // clock split table for this level
clksum=[] // clock sum table for this level
nrmsplit=[]
nrmsum=[]

sel=2:n
for k=sel
  o=scs_m(k)
  if o(1)=='Block' then
    model=o(3)
    sel(k-1)=0
    if o(5)=='CLKSPLIT_f' then
      nsblk=nsblk+1
      cor(k)=MaxBlock+nsblk
      clksplit=[clksplit,MaxBlock+nsblk]
    elseif o(5)=='SPLIT_f' then
      nsblk=nsblk+1
      cor(k)=MaxBlock+nsblk
      nrmsplit=[nrmsplit,MaxBlock+nsblk]
    elseif o(5)=='CLKSOM_f'|o(5)=='CLKSOMV_f' then
      nsblk=nsblk+1
      cor(k)=MaxBlock+nsblk
      clksum=[clksum,MaxBlock+nsblk]
    elseif o(5)=='NRMSOM_f' then
      nsblk=nsblk+1
      cor(k)=MaxBlock+nsblk
      nrmsum=[nrmsum,MaxBlock+nsblk]
    elseif o(5)=='SUM_f'|o(5)=='SOM_f' then
      [graphics,model]=o(2:3)
      if ~and(graphics(5)) then
	//all input ports are not connected,renumber connected ones
	//and modify signs
	connected=get_connected(scs_m,k)
	count=0;cnct=[]
	for kk=1:prod(size(connected))
	  kc=connected(kk)
	  lk=scs_m(kc);to=lk(9)
	  if to(1)==k then  // an input link
	    cnct=[cnct to(2)]
	    count=count+1
	    to(2)=count;lk(9)=to;scs_m(kc)=lk;
	  end
	end
	in=model(2)
	model(2)=in(cnct);
      end
      o(3)=model
      nb=nb+1
      corinv(nb)=k
      blklst(nb)=o(3)
      cor(k)=nb
    elseif model(1)=='super'|model(1)=='csuper' then
      path=[path k]
      nsblk=nsblk+1
      sup_tab=[sup_tab MaxBlock+nsblk]
      [graphics,model]=o(2:3)
      // check connections
//      if ~and(graphics(5))|~and(graphics(6))|..
//	  ~and(graphics(7))|~and(graphics(8)) then

//a verifier
//      if ~and(graphics(5))|~and(graphics(6)) then
//        message(['A block has unconnected ports';'Please check'])
//	ok=%f
//	return
//      end
      connected=get_connected(scs_m,k)
      for kk=1:prod(size(connected))
	kc=connected(kk);
	lk=scs_m(kc);from=lk(8);to=lk(9);
	if to(1)==k then  // an input link
	  to(1)=MaxBlock+nsblk;
	  lk(9)=to;scs_m(kc)=lk;
	end
	if from(1)==k then  // an output link
	  from(1)=MaxBlock+nsblk;
	  lk(8)=from;scs_m(kc)=lk;
	end
      end
      [blklsts,cmats,ccmats,cors,corinvs,ok]=c_pass1(model(8),..
	  MaxBlock+nsblk)
      if ~ok then return,end
      nbs=size(blklsts)
      for kk=1:nbs
	blklst(nb+kk)=blklsts(kk)
	corinv(nb+kk)=[k,corinvs(kk)]
      end
      cors=shiftcors(cors,nb)
      if cmats<>[] then
	f=find(cmats(:,2)>0)
	if f<>[] then cmats(f,1)=cmats(f,1)+nb,end
	f=find(cmats(:,4)>0)
	if f<>[] then cmats(f,3)=cmats(f,3)+nb,end
	cmat=[cmat;cmats]
      end
      if ccmats<>[] then
	f=find(ccmats(:,2)>0)
	if f<>[] then ccmats(f,1)=ccmats(f,1)+nb,end
	f=find(ccmats(:,4)>0)
	if f<>[] then ccmats(f,3)=ccmats(f,3)+nb,end
	ccmat=[ccmat;ccmats]
      end
      cor(k)=cors
      nb=nb+nbs
      path($)=[]
    elseif o(5)=='IN_f' then
      if ksup==0 then
	message('Input port must be only used in a Super Block')
	ok=%f
	return
      end
      connected=get_connected(scs_m,k)
      if connected==[] then
	message(['A Super Block Input port is unconnected';
	    'Please check'])
	ok=%f
	return
      end
      lk=scs_m(connected)
      model=o(3)
      //ipar=model(9) contient le numero de port d'entree affecte
      //a ce bloc
      from=[-ksup -model(9)]
      lk(8)=from;scs_m(connected)=lk
    elseif o(5)=='OUT_f' then
      if ksup==0 then
	message('Output port must be only used in a Super Block')
	ok=%f
	return
      end
      connected=get_connected(scs_m,k)
      if connected==[] then
	message(['A Super Block Output port is unconnected';
	    'Please check'])
	ok=%f
	return
      end
      lk=scs_m(connected)
      model=o(3)
      //ipar=model(9) contient le numero de port de sortie affecte
      //a ce bloc
      to=[-ksup -model(9)]
      lk(9)=to;scs_m(connected)=lk
    elseif o(5)=='CLKIN_f'|o(5)=='CLKINV_f' then
      if ksup==0 then
	message('Clock Input port must be only used in a Super Block')
	ok=%f
	return
      end
      connected=get_connected(scs_m,k)
      if connected==[] then
	message(['A Super Block Clock Input port is unconnected';
	    'Please check'])
	ok=%f
	return
      end
      lk=scs_m(connected)
      //ipar=model(9) contient le numero de port d'entree affecte
      //a ce bloc
      from=[-ksup -model(9)]
      lk(8)=from;scs_m(connected)=lk
    elseif o(5)=='CLKOUT_f'|o(5)=='CLKOUTV_f' then
      if ksup==0 then
	message('Clock Output port must be only used in a Super Block')
	ok=%f
	return
      end
      connected=get_connected(scs_m,k)
      if connected==[] then
	message(['A Super Block Clock Output port is unconnected';
	    'Please check'])
	ok=%f
	return
      end
      lk=scs_m(connected)
      model=o(3)
      //ipar=model(9) contient le numero de port de sortie affecte
      //a ce bloc
      to=[-ksup -model(9)]
      lk(9)=to;scs_m(connected)=lk
    else
      graphics=o(2)
      nb=nb+1
      corinv(nb)=k
      model=o(3)
      if model(1)(1)=='scifunc' then
	if model(9)==0 then
	  message('A scifunc block has not been defined')
	  ok=%f
	  return
	end
	model(1)=list(genmac(model(9),size(model(2),'*'),size(model(3),'*')),3)
      end
      if type(model(1))==15 then
	if int(model(1)(2)/1000)==1 then   //fortran block
	  funam=model(1)(1)
	  if ~c_link(funam) then
	    tt=graphics(4)(2);
	    [ok]=do_forcomlink(funam,tt)
	  end
	elseif int(model(1)(2)/1000)==2 then   //C block
	  funam=model(1)(1)
	  if ~c_link(funam) then
	    tt=graphics(4)(2);
	    [ok]=do_ccomlink(funam,tt)
	  end
	end
      end
      blklst(nb)=model
      cor(k)=nb
    end
  elseif o(1)=='Deleted'|o(1)=='Text' then
    sel(k-1)=0
  end
end
if ksup==0&nb==0 then
  message('Empty diagram')
  ok=%f
  return
end

//loop on links
sel(find(sel==0))=[]
for k=sel
  o=scs_m(k);
  [ct,from,to]=o(7:9);
  if from(2)<0& from(1)<0then
    //fil issu d'un port d'entree d'un super block
    //on remet la valeur de from(1) au numero du superbloc dans scs_m
    from(1)=-from(1)
  elseif or(from(1)==sup_tab) then //fil provenant d'un super block
//    from(2)=-from(2)
  else
    from(1)=cor(from(1)),
  end
  if to==[] then pause,end
  if to(2)<0&to(1)<0 then //fil connecte a un port de sortie d'un super block
      to(1)=-to(1)
  elseif or(to(1)==sup_tab)  then //fil connecte a un super block
//    to(2)=-to(2)
  else
    to(1)=cor(to(1)),
  end
  if ct(2)==1 then
    cmat=[cmat;[from(1),from(2),to(1),to(2)]];
  else
    ccmat=[ccmat;[from(1),from(2),to(1),to(2)]];
  end
end

// strip super block input connection
//==========================================
to_kill=[]
for k=sup_tab //loop on super blocks
  fn=find(cmat(:,1)==k); //super block inputs
  if fn<>[] then
    ni=max(abs(cmat(fn,2))) //number of super block input ports
    for kp=1:ni //loop on ports
      ip=find(cmat(fn,2)==-kp);fnp=fn(ip),
      to=[cmat(fnp(1),1), -cmat(fnp(1),2)]
      c=find(abs(cmat(:,3:4)-ones(cmat(:,1))*to)*[1;1]==0) //connected blocks
      if c<>[] then
	cmat(c,3:4)=ones(c')*cmat(fnp(1),3:4);to_kill=[to_kill;fnp(1)],
	// handle nrmsum outputs (many links issued from a single port)
	for ii=2:size(fnp,'*')
	  cmat=[cmat;[cmat(c,1:2),ones(c')*cmat(fnp(ii),3:4)] ]
	  to_kill=[to_kill;fnp(ii)],
	end
      else
//	to_kill=[to_kill;fnp(:)], not useful (unconnected blocks
//      are detected previously.
      end
    end
  end
end
cmat(to_kill,:)=[];to_kill=[]
[nc,nw]=size(cmat)
// strip super block output  port connection
//===========================================
for k=sup_tab //loop on super blocks
  fn=find(cmat(:,3)==k); //super block outputs
  if fn<>[] then
    no=max(abs(cmat(fn,4))); //number of super block output ports
    for kp=1:no //loop on ports
      ip=find(cmat(fn,4)==-kp);fnp=fn(ip);
      to=[cmat(fnp(1),3), -cmat(fnp(1),4)]
      c=find(abs(cmat(:,1:2)-ones(cmat(:,1))*to)*[1;1]==0) ;//connected blocks
      if c<>[] then
	cmat(c,1:2)=ones(c')*cmat(fnp(1),1:2);to_kill=[to_kill;fnp(1)];
      	// handle nrmsum outputs (many links issued from a single port)
	for ii=2:size(fnp,'*')
	  cmat=[cmat;[ones(c')*cmat(fnp(ii),1:2),cmat(c,3:4)] ];
	  to_kill=[to_kill;fnp(ii)];
	end
      else
	to_kill=[to_kill;fnp(1)];
      end
    end
  end
end
cmat(to_kill,:)=[]




[nc,nw]=size(cmat)
if nc<>0 then 
  //strip splits and nrmsums
  //============
  to_kill=[]
  for ksplit=nrmsplit
    kfrom=find(cmat(:,1)==ksplit); //links coming from the split
    kto=find(cmat(:,3)==ksplit); // link going to the split
    if ~or(to_kill==kto) then to_kill=[to_kill,kto];end
    cmat(kfrom,1:2)=cmat(kto*ones(kfrom'),1:2);
  end
end

cmat(to_kill,:)=[];to_kill=[]
// strip nrmsum
[nc,nw]=size(cmat)
if nc<>0 then 
  for ksum=nrmsum
    //link(s) coming from the nrmsum.
    //Due to previous substitutions, many links may go out of the nrmsum
    kfrom=find(cmat(:,1)==ksum); // links coming from the nrmsum
    kto=find(cmat(:,3)==ksum); // links going to the nrmsum
    if ~or(to_kill==kfrom(1)) then to_kill=[to_kill,kfrom(1)];end
    cmat(kto,3:4)=cmat(kfrom(1)*ones(kto'),3:4);
    kfrom(1)=[];
    nto=size(kto,'c');
    //add new links
    for k=kfrom
      if ~or(to_kill==k(1)) then to_kill=[to_kill,k(1)];end
      nc=size(cmat,'r');
      cmat=[cmat;cmat(kto,:)];
      cmat(nc+1:nc+nto,3:4)=cmat(k*ones(kto'),3:4);
    end
    cmat(to_kill,:)=[];to_kill=[]
    [nc,nw]=size(cmat)
  end
end



[nc,nw]=size(ccmat)
if nc==0 then return,end
//strip clksplit and clksum blocks and change corresponding links
//===============================================================
// strip clksplit
to_kill=[]
for ksplit=clksplit
  kfrom=find(ccmat(:,1)==ksplit); //links coming from the clksplit
  kto=find(ccmat(:,3)==ksplit); // link going to the clksplit
  if ~or(to_kill==kto) then to_kill=[to_kill,kto];end
  ccmat(kfrom,1:2)=ccmat(kto*ones(kfrom'),1:2);
end


ccmat(to_kill,:)=[];to_kill=[]
// strip clksum
[nc,nw]=size(ccmat)
if nc==0 then return,end


for ksum=clksum
  //link(s) coming from the clksum.
  //Due to previous substitutions, many links may go out of the clksum
  kfrom=find(ccmat(:,1)==ksum); // links coming from the clksum
  kto=find(ccmat(:,3)==ksum); // links going to the clksum
  if ~or(to_kill==kfrom(1)) then to_kill=[to_kill,kfrom(1)];end
  if kfrom(1)<>[] then
    ccmat(kto,3:4)=ccmat(kfrom(1)*ones(kto'),3:4);
    kfrom(1)=[];
  else
    if ~or(to_kill==kto(1)) then to_kill=[to_kill,kto(1)];end
  end
  nto=size(kto,'c');
  //add new links
  for k=kfrom
    if ~or(to_kill==k(1)) then to_kill=[to_kill,k(1)];end
    nc=size(ccmat,'r');
    ccmat=[ccmat;ccmat(kto,:)];
    ccmat(nc+1:nc+nto,3:4)=ccmat(k*ones(kto'),3:4);
  end
  ccmat(to_kill,:)=[];to_kill=[]
  [nc,nw]=size(ccmat)
end


if nc==0 then return,end

// strip super block input  clock connection
//==========================================
to_kill=[]

for k=sup_tab //loop on super blocks
  fn=find(ccmat(:,1)==k); //super block inputs
  if fn<>[] then
    ni=max(abs(ccmat(fn,2))) //number of super block input ports
    for kp=1:ni //loop on ports
      ip=find(ccmat(fn,2)==-kp);fnp=fn(ip),
      to=[ccmat(fnp(1),1), -ccmat(fnp(1),2)]
      c=find(abs(ccmat(:,3:4)-ones(ccmat(:,1))*to)*[1;1]==0) //connected blocks
      if c<>[] then
	ccmat(c,3:4)=ones(c')*ccmat(fnp(1),3:4);to_kill=[to_kill;fnp(1)],
	// handle clock sum outputs (many links issued from a single port)
	for ii=2:size(fnp,'*')
	  ccmat=[ccmat;[ccmat(c,1:2),ones(c')*ccmat(fnp(ii),3:4)] ]
	  to_kill=[to_kill;fnp(ii)],
	end
      else
	to_kill=[to_kill;fnp(:)], //required????? SS 01/09/2000
      end
    end
  end
end
ccmat(to_kill,:)=[];to_kill=[]
[nc,nw]=size(ccmat)
if nc==0 then return,end
// strip super block output  clock connection
//===========================================
for k=sup_tab //loop on super blocks
  fn=find(ccmat(:,3)==k); //super block links connected to outputs ports
  if fn<>[] then
    no=max(abs(ccmat(fn,4))); //number of super block output ports
    for kp=1:no //loop on ports
      ip=find(ccmat(fn,4)==-kp);fnp=fn(ip);//super block links connected to port kp
      to=[ccmat(fnp(1),3), -ccmat(fnp(1),4)]
      //current level blocks connected to the port kp of super block k:
      c=find(abs(ccmat(:,1:2)-ones(ccmat(:,1))*to)*[1;1]==0) ;
      if c<>[] then //there is blocks connected to this super block output port
	ccmat(c,1:2)=ones(c')*ccmat(fnp(1),1:2);to_kill=[to_kill;fnp(1)];
	// handle clock sum outputs (many links issued from a single port)
	for ii=2:size(fnp,'*')
	  ccmat=[ccmat;[ones(c')*ccmat(fnp(ii),1:2),ccmat(c,3:4)] ];
	  to_kill=[to_kill;fnp(ii)];
	end
      else //there is no block connected to this super block output port
	//super bloc links going to this output port can be deleted
	to_kill=[to_kill;fnp(:)]; //required!!!! SS 01/09/2000 
      end
    end
  end
end
ccmat(to_kill,:)=[]

if ksup==0 then
  k1=find(ccmat(:,1)>MaxBlock)
  k1n=find(ccmat(k1,2)<0)
  ccmat(k1(k1n),:)=[]
  k1(k1n)=[]
  if k1<>[] then
    message(['Warning: may be event loop'])
    ok=%f;return
  end
  k2=find(ccmat(:,3)>MaxBlock)
  k2n=find(ccmat(k2,4)<0)
  ccmat(k2(k2n),:)=[]
  k2(k2n)=[]
  if k2<>[] then
    message(['Warning: may be event loop'])
    ok=%f;return
  end
  if or(cmat(:,1)>MaxBlock)|or(cmat(:,3)>MaxBlock)
    message('Unexpected problem, please report')
    ok=%f;return
  end
end





