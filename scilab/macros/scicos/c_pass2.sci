function cpr=c_pass2(bllst,connectmat,clkconnect,cor,corinv)
// cor    ; correspondance table with initial block ordering
//
// bllst: list with nblk elts where nblk denotes number of blocks.
//        Each element must be a list with 12 elements:
//          1- a list containing function name and function type
//          2- vector of number of inputs
//          3- vector of number of ouputs
//          4- vector of number of clock inputs 
//          5- vector of number of clock outputs
//          6- vector (column) of continuous initial condition
//          7- vector (column) of discrete initial condition
//          8- vector (column) of real parameters
//          9- vector (column) of integer parameters
//          10- string: 'l' for synchro (ifthenelse,eselect) or 'm'
//          (memo) or 'x' for blocks that need to be called during
//          integration even in the absence of state (only if with workspace)
//          11- vector of size <number of clock outputs> including
//              preprogrammed event firing times (<0 if no firing) 
//          12- boolean vector (1x2): 1st entry for dependence on u,
//          2nd on t 
//          13- block label
//          14- number of zero crossings
//          15- number of modes
//          16- empty list (equation for modelica blocks)
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

  //???
  global need_newblk
  need_newblk=%t

  if bllst==list() then
    message(['No block can be activated'])
    cpr=list()
    ok=%f;
    return
  end

  if exists('%scicos_solver')==0 then %scicos_solver=0,end

  clkptr=1,cliptr=1,typ_l=[],dep_ut=[]
  nblk=size(bllst)

  //take care of the heritage

  [bllst,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,..
   dep_ut,typ_l,typ_r,typ_m,tblock,typ_cons,typ_zx,ok]=mini_extract_info(bllst,..
						  connectmat,clkconnect)

  if show_trace then disp('c_pass20:'+string(timer())),end

  //if ~ok then 
  // heritage block !
  [outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)
  
  [critev]=critical_events(connectmat,clkconnect,dep_ut,typ_r,..
				 typ_l,typ_zx,outoin,outoinptr,clkptr)
  
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
    
    [ok,done,bllst,connectmat,clkconnect,typ_l,typ_m,critev,..
     corinv]=paksazi(bllst,connectmat,clkconnect,corinv,clkptr,cliptr,typ_l,..
		     typ_m,critev,dep_ut)
    
    if show_trace then disp('c_pass300011:'+string(timer())),end
    if ~ok then
      cpr=list()
      return
    end
  end

 
  if show_trace then disp('c_pass31:'+string(timer())),end

  //extract various info from bllst
  [lnkptr,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,..
   xptr,zptr,typ_mod,rpptr,ipptr,xc0,xcd0,xd0,rpar,ipar,dep_ut,..
   typ_z,typ_s,typ_x,typ_m,funs,funtyp,initexe,labels,..
   bexe,boptr,blnk,blptr,ok]=extract_info(bllst,connectmat,clkconnect);
  typ_z0=typ_z;

  if ~ok then
     message('Problem in port size');
    cpr=list()
    return,
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
  [ordptr,ordclk,cord,iord,oord,zord,..
   typ_z,ok]=scheduler(inpptr,outptr,clkptr,execlk,execlk0,..
			execlk_cons,ordptr1,outoin,outoinptr,..
			evoutoin,evoutoinptr,typ_z,typ_x,typ_s,..
			bexe,boptr,blnk,blptr);
//  critev=zeros(critev);
//  critev(1:size(critev_p,1))=critev_p;
  

  if ~ok then 
    cpr=list()
    return,
  end
  

  if show_trace then disp('c_pass51:'+string(timer())),end
  //form scicos arguments

  nb=size(typ_z,'*');
  zcptr=ones(nb+1,1);
  modptr=ones(nb+1,1);
  for i=1:nb
    zcptr(i+1)=zcptr(i)+typ_z(i)
    modptr(i+1)=modptr(i)+sign(typ_z(i))*typ_mod(i);
  end
    
  ztyp=sign(typ_z0)  //completement inutile pour simulation
                     // utiliser pour la generation de code
		     
  if xptr($)==1 & zcptr($)>1 then
    message(['No continuous-time state. Thresholds are ignored; this '; ..
	     'may be OK if you don''t generate external events with them.'; ..
	     'If you want to reactivate the thresholds, the you need'; ..
	     'to include a block with continuous-time state in your diagram.'; ..
	     'You can for example include DUMMY CLSS block (linear palette).']);
  end

  subscr=[]
  ncblk=0;nxblk=0;ndblk=0;ndcblk=0;
  sim=scicos_sim(funs=funs,xptr=xptr,zptr=zptr,zcptr=zcptr,..
		 inpptr=inpptr,outptr=outptr,inplnk=inplnk,outlnk=outlnk,..
		 lnkptr=lnkptr,rpar=rpar,rpptr=rpptr,ipar=ipar,ipptr=ipptr,..
		 clkptr=clkptr,ordptr=ordptr,execlk=execlk,ordclk=ordclk,..
		 cord=cord,oord=oord,zord=zord,critev=critev(:),..
		 nb=nb,ztyp=ztyp,nblk=nblk,ndcblk=ndcblk,..
		 subscr=subscr,funtyp=funtyp,iord=iord,labels=labels,modptr=modptr);

  //initialize agenda
  [tevts,evtspt,pointi]=init_agenda(initexe,clkptr)
  if show_trace then disp('c_pass61:'+string(timer())),end

  outtb=0*ones(lnkptr($)-1,1)
//  mod=0*ones(modptr($)-1,1)
  iz0=zeros(nb,1);

  if max(funtyp)>10000 &%scicos_solver==0 then
    message(['Diagram contains implicit blocks,'
	     'compiling for implicit Solver'])
    %scicos_solver=100
  end
  if %scicos_solver==100 then xc0=[xc0;xcd0],end
  state=scicos_state()
    state.x=xc0
    state.z=xd0
    state.iz=iz0
    state.tevts=tevts
    state.evtspt=evtspt
    state.pointi=pointi
    state.outtb=outtb
//    state.mod=mod

  cpr=scicos_cpr(state=state,sim=sim,cor=cor,corinv=corinv);

  if show_trace then disp('c_pass71:'+string(timer())),end
  clearglobal need_newblk
endfunction

function [ordptr2,ordclk,cord,iord,oord,zord,typ_z,ok]=..
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

  if and(ordptr1<>ordptr2) then disp("serious bug,report");pause;end
  //ordptr=[ordptr1,ordptr2];

  
  n=size(cord,1)

  vec=-ones(1,nblk);
  vec(cord(:,1))=0;
  
  typp=zeros(typ_s);typp(typ_s)=1
  
  
 

  ext_cord1=cord;
  j=1
  while %t
    ii=ext_cord1(j,1)
    if typ_s(ii)
      for i=[clkptr(ii):clkptr(ii+1)-1]
	 ext_cord1=[ext_cord1;ordclk([ordptr1(i):ordptr1(i+1)-1],:)];
      end
    end
    j=j+1
    if j>size(ext_cord1,1) then break;end
  end
  
  // code to replace faulty unique which reorders
  yy=ext_cord1(:,1)'
  [xx,kkn]=unique(yy);
  ext_cord=yy(-sort(-kkn))
  
 
  //ext_cord=unique(ext_cord1(:,1)');
  
  //for i=ext_cord
  //  if typ_s(i) then typ_z(i)=clkptr(i+1)-clkptr(i)-1;end
  //end  // adding zero crossing surfaces to cont. time synchros
  
  //a supprimer
  [ext_cord_old,ok]=new_tree3(vec,dep_ut,typp);
  if or(sort(ext_cord_old)<>sort(ext_cord)) then pause,end
  //
  //pour mettre a zero les typ_z qui ne sont pas dans ext_cord
  //noter que typ_z contient les tailles des nzcross (peut etre >1)
  typ_z(ext_cord)=-typ_z(ext_cord)
  typ_z=-min(typ_z,0)
  
  if ~ok then disp('serious bug, report.');pause;end
  // ext_cord=ext_cord(n+1:$);

  typ_z_save=typ_z
  
  fin=0
  while ~fin
    fin=1
    for i=ext_cord($:-1:1)
      for ii=[outoin(outoinptr(i):outoinptr(i+1)-1,1)',..
	      evoutoin(evoutoinptr(i):evoutoinptr(i+1)-1,1)']
	//ii est un block affecte par changement de sortie du 
	//i-eme block de oord
	if typ_z(ii) then 
	  if typ_z(i)==0 then typ_z(i)=1;fin=0;end
	end
	if typ_x(ii) then 
	  if ~typ_x(i) then typ_x(i)=%t;fin=0;end
	end
	if typ_z(i)&typ_x(i) then break,end
      end
    end
  end
  //supprimer les blocks a supprimer

  ind=find(typ_z(cord(:,1)));
  zord=cord(ind,:)
  ind=find(typ_x(cord(:,1)));
  oord=cord(ind,:)
  
typ_z=typ_z_save
  //critev: vecteur indiquant si evenement est important pour tcrit
  //Donc les blocks indiques sont des blocks susceptibles de produire
  //des discontinuites quand l'evenement se produit
  maX=max([ext_cord1(:,1);ordclk(:,1)])+1;
  cordX=ext_cord1(:,1)*maX+ext_cord1(:,2);

  // 1: important; 0:non
  n=clkptr(nblk+1)-1 //nb d'evenement
		     
  //a priori tous les evenemets sont non-importants
  //critev=zeros(n,1)
  for i=1:n
    for hh=ordptr1(i):ordptr1(i+1)-1
      jj= ordclk(hh,1) //block excite par evenement i
      //Following line is not correct because of ever active synchros
      if or(jj*maX+ordclk(hh,2)==cordX) then
	ordclk(hh,2)=-ordclk(hh,2)
      end
    end
  end
endfunction

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
endfunction

function [okk,done,bllst,connectmat,clkconnect,typ_l,typ_m,critev,corinv]=..
      paksazi(bllst,connectmat,clkconnect,corinv,clkptr,cliptr,..
	      typ_l,typ_m,critev,dep_ut)
  global need_newblk  
  okk=%t
  nblk=length(bllst)
  nblkorg=nblk
  if ~or(typ_l) then
    done=%t;
    return
  end
  change=%f
  if need_newblk then
    
    [junk,ii]=sort(-clkconnect(:,3))
    clkconnect=clkconnect(ii,:)
    id=find(typ_l(clkconnect(:,3)))
    ltmp=clkconnect(id,3)
    dl=[1;ltmp(2:$)-ltmp(1:$-1);1];
    idl=find(dl<>0)
    ki=0

    for lb=find(typ_l)
      ki=ki+1
      indx=id([idl(ki):idl(ki+1)-1])

      if indx==[] then 
	message(['A synchro block is inactive';'cannot be compile']);
	okk=%f;return
      end
      if or(clkconnect(indx,1)==lb) then 
	message(['Algebraic loop detected';'on activation links']);
	okk=%f;return
      end
      nn=size(indx,'*')
      need_newblk=%f
      if nn>=2 then
	need_newblk=%t
	indxo=find(clkconnect(:,1)==lb)
	indy=find(connectmat(:,3)==lb)
	if size(indy,'*')>1 then 
	  disp('Synchro block cannot have more than 1 input')
	end

	for k=2:nn
	  critev=[critev;critev(clkptr(lb):clkptr(lb+1)-1)]
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
  end
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

  bexe=bclkconnect(:,2);
  bbclk=bclkconnect(:,1);
  sbb=size(bbclk,1);
  boptr=duplicate(1:sbb,bbclk-[0;bbclk(1:$-1)]);
  nbo=size(boptr,1);
  boptr=[boptr;(sbb+1)*ones(nblk-nbo+1,1)]

  //
  junk=max(connectmat(:,2))*connectmat(:,1)+connectmat(:,2)-1;
  [junk,ind]=sort(-junk);
  connectmat=connectmat(ind,:);
  //
  bconnectmat=connectmat(:,[1 3]);

  //
  blnk=bconnectmat(:,2);
  bblk=bconnectmat(:,1);
  sbb=size(bblk,1);
  blptr=duplicate(1:sbb,bblk-[0;bblk(1:$-1)]);
  nbo=size(blptr,1);
  blptr=[blptr;(sbb+1)*ones(nblk-nbo+1,1)]

  tclkconnect=clkconnect(~typ_l(clkconnect(:,1)),:);
  tcon=clkptr(tclkconnect(:,1))+tclkconnect(:,2)-1;
  texeclk=tclkconnect(:,[3 4]);

  //
  sbb=size(tcon,1);
  ordptr1=duplicate(1:sbb,tcon-[0;tcon(1:$-1)]);
  nbo=size(ordptr1,1);
  ordptr1=[ordptr1;(sbb+1)*ones(clkptr($)-nbo,1)]
  //
  clkconnect=[clkconnect0;clkconnect];
  con=[con0;con];
  //
  pointer=[];

  for o=0:clkptr($)-1
    if o==0 then 
      texeclki=texeclk0; 
    else 
      texeclki=texeclk(ordptr1(o):ordptr1(o+1)-1,1);
    end
    
    test=or(typ_l(texeclki))
    if test then
      vec=-ones(1,nblk);
      vec(texeclki')=zeros(texeclki)';

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
	  mod=bllst(bl);mod.blocktype='s',bllst(bl)=mod,typ_l(bl)=%f
	  pointer=pointer(find(clkconnect(pointer,3)<>bl));
	  yek=ones(pointer');
	  clkconnect(pointer,:)=..
	      [yek*bl,yek,clkconnect(pointer,[3 4])];
	  //connect all the event outputs of the logical block to ....
	  for bl_out=2:clkptr(bl+1)-clkptr(bl)
	    clkconnect=[clkconnect;[yek*bl,bl_out*yek,clkconnect(pointer,[3 4])]];
	  end
	  //
	  need_newblk=%t
	  okk=%t,done=%f;return
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
endfunction

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
endfunction

function a=mysum(b)
  if b<>[] then a=sum(b), else a=[], end


endfunction

function [lnkptr,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,..
	  xptr,zptr,typ_mod,rpptr,ipptr,xc0,xcd0,xd0,rpar,ipar,dep_ut,..
	  typ_z,typ_s,typ_x,typ_m,funs,funtyp,initexe,labels,..
	  bexe,boptr,blnk,blptr,ok]=extract_info(bllst,..
						 connectmat,clkconnect)
  ok=%t
  nbl=length(bllst)
  clkptr=zeros(nbl+1,1);clkptr(1)=1
  cliptr=clkptr;inpptr=cliptr;outptr=inpptr;
  xptr=1;zptr=1;
  rpptr=clkptr;ipptr=clkptr;
  //
  xc0=[];xcd0=[];xd0=[];rpar=[];ipar=[];

  fff=ones(nbl,1)==1
  dep_ut=[fff,fff];typ_z=zeros(nbl,1);typ_s=fff;typ_x=fff;typ_m=fff;typ_mod=zeros(nbl,1);

  initexe=[];
  funs=list();
  funtyp=zeros(typ_z)
  labels=[]
  //
  //
  
  [ok,bllst]=adjust_inout(bllst,connectmat)
  
  // placed here to make sure nzcross and nmode correctly updated
  if ~ok then 
    lnkptr=[],inplnk=[],outlnk=[],clkptr=[],cliptr=[],inpptr=[],outptr=[],..
	   xptr=[],zptr=[],rpptr=[],ipptr=[],xc0=[],xcd0=[],xd0=[],rpar=[],ipar=[],dep_ut=[],..
	   typ_z=[],typ_s=[],typ_x=[],typ_m=[],funs=[],funtyp=[],initexe=[],labels=[],..
	   bexe=[],boptr=[],blnk=[],blptr=[],
    return;
  end
  for i=1:nbl
    ll=bllst(i)
    
    if type(ll.sim)==15 then
      funs(i)=ll.sim(1)
      funtyp(i,1)=ll.sim(2)
    else
      funs(i)=ll.sim;
      funtyp(i,1)=0;
    end
    if funtyp(i,1)>999&funtyp(i,1)<10000 then
      if ~c_link(funs(i)) then
	x_message(['A C or Fortran block is used but not linked';
		   'You can save your compiled diagram and load it';
		   'This will automatically link the C or Fortran function'])
      end
    end
    inpnum=ll.in;outnum=ll.out;cinpnum=ll.evtin;coutnum=ll.evtout;
    //
    inpptr(i+1)=inpptr(i)+size(inpnum,'*')
    outptr(i+1)=outptr(i)+size(outnum,'*')
    cliptr(i+1)=cliptr(i)+size(cinpnum,'*')
    clkptr(i+1)=clkptr(i)+size(coutnum,'*')
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
    xd0=[xd0;xd0k]
    zptr(i+1)=zptr(i)+size(xd0k,'*')
    typ_mod(i)=ll.nmode;
    if typ_mod(i)<0 then
      message('Number of modes in block '+string(i)+'cannot b"+...
	      " e determined')
      ok=%f
    end
    
    //  
    if (funtyp(i,1)==3 | funtyp(i,1)==5 | funtyp(i,1)==10005) then //sciblocks
      if ll.rpar==[] then rpark=[]; else rpark=var2vec(ll.rpar);end
    else
      rpark=ll.rpar(:)
    end
    rpar=[rpar;rpark]
    rpptr(i+1)=rpptr(i)+size(rpark,'*')
    if type(ll.ipar)==1 then 
      ipar=[ipar;ll.ipar(:)]
      ipptr(i+1)=ipptr(i)+size(ll.ipar,'*')
    else
      ipptr(i+1)=ipptr(i)
    end
    //
    //    typ_z(i)=ll.blocktype=='z'
    typ_z(i)=ll.nzcross
    if typ_z(i)<0 then
      message('Number of zero crossings in block '+string(i)+'cannot b"+...
	      " e determined')
      ok=%f
    end
    typ_s(i)=ll.blocktype=='s'
    typ_x(i)=ll.state<>[]|ll.blocktype=='x' // some blocks like delay
                                            // need to be in oord even
                                            // without state
					    
    typ_m(i)=ll.blocktype=='m'
    dep_ut(i,:)=(ll.dep_ut(:))'
    //
    if ~typ_s(i)&ll.evtout<>[] then  
      ll11=ll.firing
      prt=find(ll11>=zeros(ll11))
      nprt=prod(size(prt))
      initexe=[initexe;..
	       [i*ones(nprt,1),matrix(prt,nprt,1),matrix(ll11(prt),nprt,1)]];
    end

    if type(ll.label)==10 then
      labels=[labels;ll.label(1)]
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

//  [ok,bllst]=adjust_inout(bllst,connectmat)
//  if ~ok then 
//    lnkptr=[],inplnk=[],outlnk=[],clkptr=[],cliptr=[],inpptr=[],outptr=[],..
//	  xptr=[],zptr=[],rpptr=[],ipptr=[],xc0=[],xcd0=[],xd0=[],rpar=[],ipar=[],dep_ut=[],..
//	  typ_z=[],typ_s=[],typ_x=[],typ_m=[],funs=[],funtyp=[],initexe=[],labels=[],..
//	  bexe=[],boptr=[],blnk=[],blptr=[],
//    return;
//  end
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
      lnkbsz=[lnkbsz;bllst(connectmat(jj,1)).out(connectmat(jj,2))]
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
    nm=bllst(m).out(n)
    if nm<1 then 
      //pause
      under_connection(corinv(m),n,nm,-1,0,0),ok=%f,return,end
    siz_unco=maxi(siz_unco,nm)
  end
  lnkptr=[lnkptr;lnkptr($)+siz_unco]
  outlnk(unco)=maxi(outlnk)+1
endfunction

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
endfunction

function [clkptr,cliptr,typ_l,dep_ut,typ_m]=..
      make_ptr(bllst,clkptr,cliptr,typ_l,dep_ut,typ_m)
  nblk0=size(clkptr,'*')
  nbl=size(bllst)
  if nbl<nblk0 then return; end

  i=nblk0;ll=bllst(i)
  cliptr1=zeros(nbl-nblk0,1);clkptr1=cliptr1;
  cliptr1(1)=cliptr($)+sum(ll.evtin)
  clkptr1(1)=clkptr($)+sum(ll.evtout)
  typ_l1=cliptr1==1;dep_ut1=[typ_l1,typ_l1];
  typ_l1(1)=ll.blocktype=='l';typ_m1(1)=ll.blocktype=='m';dep_ut1(1,:)=ll.dep_ut;
  j=1
  for i=nblk0+1:nbl
    j=j+1
    ll=bllst(i)
    cliptr1(j)=cliptr1(j-1)+sum(ll.evtin)
    clkptr1(j)=clkptr1(j-1)+sum(ll.evtout)
    typ_l1(j)=ll.blocktype=='l'
    typ_m1(j)=ll.blocktype=='m'
    dep_ut1(j,:)=ll.dep_ut
  end
  cliptr=[cliptr;cliptr1]
  clkptr=[clkptr;clkptr1]
  typ_l=[typ_l;typ_l1]
  typ_m=[typ_m;typ_m1]
  dep_ut=[dep_ut;dep_ut1];
endfunction

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
	for k=outoinptr(i):outoinptr(i+1)-1
	  ii=outoin(k,1);
	  if dep_ut(ii,1)&vec(ii)>-1 then
	    fini=%f;
	    vec(ii)=j;   
	  end
	end
      end
    end
    if fini then break;end
  end
  [k,ord]=sort(-vec);
  ord(find(k==1))=[];
  ord=ord(:)
endfunction

function [ok,bllst]=adjust_inout(bllst,connectmat)
  nlnk=size(connectmat,1)
  for hhjj=1:length(bllst)+1
    for hh=1:length(bllst)+1
      ok=%t
      for jj=1:nlnk
	nout=bllst(connectmat(jj,1)).out(connectmat(jj,2))
	nin=bllst(connectmat(jj,3)).in(connectmat(jj,4))
	if (nout>0&nin>0) then
	  if nin<>nout then
	    bad_connection(corinv(connectmat(jj,1)),connectmat(jj,2),nout,..
			   corinv(connectmat(jj,3)),connectmat(jj,4),nin)
	    ok=%f;return
	  end
	elseif (nout>0&nin<0) then 
	  ww=find(bllst(connectmat(jj,3)).in==nin)
	  bllst(connectmat(jj,3)).in(ww)=nout
	  ww=find(bllst(connectmat(jj,3)).out==nin)
	  bllst(connectmat(jj,3)).out(ww)=nout
	  ww=find(bllst(connectmat(jj,3)).out==0)
	  if (ww<>[]&mini(bllst(connectmat(jj,3)).in(:))>0) then
	    bllst(connectmat(jj,3)).out(ww)=sum(bllst(connectmat(jj,3)).in(:))
	  end
	  if bllst(connectmat(jj,3)).nzcross==nin then
	    bllst(connectmat(jj,3)).nzcross=nout
	  end
	  if bllst(connectmat(jj,3)).nmode==nin then
	    bllst(connectmat(jj,3)).nmode=nout
	  end
	  
	  
	elseif (nin>0&nout<0) then 
	  ww=find(bllst(connectmat(jj,1)).out==nout)
	  bllst(connectmat(jj,1)).out(ww)=nin

	  ww=find(bllst(connectmat(jj,1)).in==nout)
	  bllst(connectmat(jj,1)).in(ww)=nin
	  
	  ww=find(bllst(connectmat(jj,1)).in==0)
	  if (ww<>[]&mini(bllst(connectmat(jj,1)).out(:))>0) then 
	    bllst(connectmat(jj,1)).in(ww)=sum(bllst(connectmat(jj,1)).out)
	  end
	  
	  if bllst(connectmat(jj,1)).nzcross==nout then
	    bllst(connectmat(jj,1)).nzcross=nin
	  end
	  if bllst(connectmat(jj,1)).nmode==nout then
	    bllst(connectmat(jj,1)).nmode=nin
	  end

	elseif (nin==0) then
	  ww=bllst(connectmat(jj,3)).out(:)
	  if mini(ww)>0 then 
	    if nout>0 then
	      if sum(ww)==nout then
		bllst(connectmat(jj,3)).in(connectmat(jj,4))=nout
	      else
		bad_connection(corinv(connectmat(jj,3)),0,0,-1,0,0)
		ok=%f;return
	      end
	    else
	      bllst(connectmat(jj,3)).in(connectmat(jj,4))=sum(ww)
	      ok=%f
	    end
	  else      
	    nww=ww(find(ww<0))

	    if norm(nww-nww(1),1)==0 & nout>0 then
	      bllst(connectmat(jj,3)).in(connectmat(jj,4))=nout
	      k=(nout-sum(ww(find(ww>0))))/size(nww,'*')

	      if k==int(k)&k>0 then
		bllst(connectmat(jj,3)).out(find(ww<0))=k
	      else
		bad_connection(corinv(connectmat(jj,3)),0,0,-1,0,0)
		ok=%f;return
	      end
	    else
	      ok=%f
	    end
	  end

	elseif (nout==0) then
	  ww=bllst(connectmat(jj,1)).in(:)
	  if mini(ww)>0 then 
	    if nin>0 then
	      if sum(ww)==nin then
		bllst(connectmat(jj,1)).out(connectmat(jj,2))=nin
	      else
		bad_connection(corinv(connectmat(jj,1)),0,0,-1,0,0)
		ok=%f;return
	      end
	    else
	      bllst(connectmat(jj,1)).out(connectmat(jj,2))=sum(ww)
	      ok=%f
	    end
	  else      
	    nww=ww(find(ww<0))
	    if norm(nww-nww(1),1)==0 & nin>0 then
	      bllst(connectmat(jj,1)).out(connectmat(jj,2))=nin
	      k=(nin-sum(ww(find(ww>0))))/size(nww,'*')
	      if k==int(k)&k>0 then
		bllst(connectmat(jj,1)).in(find(ww<0))=k
	      else
		bad_connection(corinv(connectmat(jj,1)),0,0,-1,0,0)
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
    message(['Not enough information to find port sizes';
	     'I try to find the problem']);  
    findflag=%f
    for jj=1:nlnk
      nout=bllst(connectmat(jj,1)).out(connectmat(jj,2))
      nin=bllst(connectmat(jj,3)).in(connectmat(jj,4))
      if nout<=0&nin<=0 then
	findflag=%t;
      	ninnout=under_connection(corinv(connectmat(jj,1)),connectmat(jj,2),nout,..
				 corinv(connectmat(jj,3)),connectmat(jj,4),nin)
	if ninnout==[] then ok=%f;return;end
	if ninnout<=0  then ok=%f;return;end
	ww=find(bllst(connectmat(jj,1)).out==nout)
	bllst(connectmat(jj,1)).out(ww)=ninnout
	//
	if bllst(connectmat(jj,1)).nzcross==nout then
	    bllst(connectmat(jj,1)).nzcross=ninnout
	end
	if bllst(connectmat(jj,1)).nmode==nout then
	  bllst(connectmat(jj,1)).nmode=ninnout
	end
	//
	ww=find(bllst(connectmat(jj,1)).in==nout)
	bllst(connectmat(jj,1)).in(ww)=ninnout
	
	ww=find(bllst(connectmat(jj,1)).in==0)
	if (ww<>[]&mini(bllst(connectmat(jj,1)).out(:))>0) then 
	  bllst(connectmat(jj,1)).in(ww)=sum(bllst(connectmat(jj,1)).out)
	end
	//
	ww=find(bllst(connectmat(jj,3)).in==nin)
	bllst(connectmat(jj,3)).in(ww)=ninnout
	//
	if bllst(connectmat(jj,3)).nzcross==nin then
	  bllst(connectmat(jj,3)).nzcross=ninnout
	end
	if bllst(connectmat(jj,3)).nmode==nin then
	  bllst(connectmat(jj,3)).nmode=ninnout
	end

	//
	ww=find(bllst(connectmat(jj,3)).out==nin)
	bllst(connectmat(jj,3)).out(ww)=ninnout
	ww=find(bllst(connectmat(jj,3)).out==0)
	if (ww<>[]&mini(bllst(connectmat(jj,3)).in(:))>0) then
	  bllst(connectmat(jj,3)).out(ww)=sum(bllst(connectmat(jj,3)).in(:))
	end
	//bllst(connectmat(jj,1)).out(connectmat(jj,2))=ninnout
	//bllst(connectmat(jj,3)).in(connectmat(jj,4))=ninnout
      end
    end

    if ~findflag then 
      message(['I cannot find a link with undetermined size';
	       'My guess is that you have a block with unconnected';
	       'undetermined  output ports']);
      ok=%f;return;
    end
  end
endfunction

function ninnout=under_connection(path_out,prt_out,nout,path_in,prt_in,nin)
// alert for badly connected blocks
// path_out : Path of the "from block" in scs_m
// path_in  : Path of the "to block" in scs_m
//!

if path_in==-1 then
  hilite_obj(scs_m.objs(path_out));
  message(['One of this block''s outputs has negative size';
      'Please check.'])
  hilite_obj(scs_m.objs(path_out));
  ninnout=0
  return
end

lp=mini(size(path_out,'*'),size(path_in,'*'))
k=find(path_out(1:lp)<>path_in(1:lp))
path=path_out(1:k(1)-1) // common superbloc path
if (k <> []) then
  path_out=path_out(k(1)) // "from" block number
  path_in=path_in(k(1))   // "to" block number
end
if isdef('Code_gene_run') then
  mxwin=maxi(winsid())
  path=path+1 // Consider locally compiled superblock as a superblock
  for k=1:size(path,'*')
    hilite_obj(all_scs_m.objs(numk(k)))
    scs_m=all_scs_m.objs(numk(k)).model.rpar;
    scs_show(scs_m,mxwin+k)
  end
  hilite_obj(scs_m.objs(path_out))
  if or(path_in<>path_out) then hilite_obj(scs_m.objs(path_in)),end
  ninnout=evstr(dialog(['Hilited block(s) have connected ports ';
      'with  sizes that cannot be determined by the context';
      'what is the size of this link'],'1'))
  for k=size(path,'*'):-1:1,xdel(mxwin+k),end
  scs_m=null()
  unhilite_obj(all_scs_m.objs(numk(1)))
else
  if path==[] then
    hilite_obj(scs_m.objs(path_out))
    if or(path_in<>path_out) then hilite_obj(scs_m.objs(path_in)),end
    
    ninnout=evstr(dialog(['Hilited block(s) have connected ports ';
	'with  sizes that cannot be determined by the context';
	'what is the size of this link'],'1'))
    hilite_obj(scs_m.objs(path_out))
    if or(path_in<>path_out) then hilite_obj(scs_m.objs(path_in)),end
  else  
    mxwin=maxi(winsid())
    for k=1:size(path,'*')
      hilite_obj(scs_m.objs(path(k)))
      scs_m=scs_m.objs(path(k)).model.rpar;
      scs_show(scs_m,mxwin+k)
    end
    hilite_obj(scs_m.objs(path_out))
    if or(path_in<>path_out) then hilite_obj(scs_m.objs(path_in)),end
    ninnout=evstr(dialog(['Hilited block(s) have connected ports ';
	'with  sizes that cannot be determined by the context';
	'what is the size of this link'],'1'))
    for k=size(path,'*'):-1:1,xdel(mxwin+k),end
    scs_m=null()
    unhilite_obj(scs_m.objs(path(1)))
  end
end
endfunction

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

endfunction
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
endfunction

function [bllst,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,..
	  dep_ut,typ_l,typ_r,typ_m,tblock,typ_cons,typ_zx,ok]=mini_extract_info(bllst,..
						  connectmat,clkconnect)

  ok=%t
  nbl=length(bllst)
  clkptr=zeros(nbl+1,1);clkptr(1)=1
  cliptr=clkptr;inpptr=cliptr;outptr=inpptr;
  fff=ones(nbl,1)==1
  dep_ut=[fff,fff];typ_l=fff;typ_r=fff;typ_cons=fff;typ_m=fff;typ_zx=fff;
  tblock=fff
  //tblock=[]  // specifies blocks that must be connected to time event.
  //
  for i=1:nbl
    ll=bllst(i)
    if (ll.state==[]&ll.nzcross==0) then typ_zx(i)=%f;end
    inpnum=ll.in;outnum=ll.out;cinpnum=ll.evtin;coutnum=ll.evtout;
    //    
    if cinpnum==[] then
      // this block inherits
      ok=%f
      typ_r(i)=~ll.dep_ut(2)
      tblock(i)=ll.dep_ut(2)
      //if block depends on time but has no event input port
      if ~ll.dep_ut(2) then 
	//inherits from the inputs
	cinpnum=ones(inpnum)
	bllst(i).evtin=cinpnum  //XXXXXXXXXXXXXXXXXXXXX
      end
      //
    else
      tblock(i)=ll.dep_ut(2);typ_r(i)=%f
    end
    inpptr(i+1)=inpptr(i)+size(inpnum,'*')
    outptr(i+1)=outptr(i)+size(outnum,'*')
    cliptr(i+1)=cliptr(i)+size(cinpnum,'*')
    clkptr(i+1)=clkptr(i)+size(coutnum,'*')
    //
    typ_l(i)=ll.blocktype=='l';typ_m(i)=ll.blocktype=='m';dep_ut(i,:)=(ll.dep_ut(:))';
    typ_cons(i)=cinpnum==[]&inpnum==[]&~ll.dep_ut(2)
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
endfunction

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
endfunction

function   clkconnect=cleanup(clkconnect)
  mm=maxi(clkconnect)+1
  cc=clkconnect(:,4)+mm*clkconnect(:,3)+clkconnect(:,2)*mm^2+..
     clkconnect(:,1)*mm^3
  [cc1,ind]=sort(-cc)
  clkconnect=clkconnect(ind,:)
  ind=find(cc1(2:$)-cc1(1:$-1)==0)
  clkconnect(ind,:)=[]
endfunction

function  [r,ok]=new_tree2(vec,outoin,outoinptr,dep_ut)
  dd=zeros(dep_ut);dd(dep_ut)=1;
  [r,ok2]=sci_tree2(vec,outoin,outoinptr,dd)
  ok=ok2==1
endfunction

function  [r,ok]=new_tree3(vec,dep_ut,typ_l)
  dd=zeros(dep_ut);dd(dep_ut)=1;
  [r2,ok2]=sci_tree3(vec,dd,typ_l,bexe,boptr,blnk,blptr)
  r=r2'
  ok=ok2==1
endfunction

function  [r,ok]=new_tree4(vec,outoin,outoinptr,typ_r)
  nd=zeros(size(vec,'*'),(max(outoin(:,2))+1));
  ddd=zeros(typ_r);ddd(typ_r)=1; 
  [r1,r2]=sci_tree4(vec,outoin,outoinptr,nd,ddd)
  r=[r1',r2']
  ok=%t
endfunction



function [critev]=critical_events(connectmat,clkconnect,dep_ut,typ_r,..
				 typ_l,typ_zx,outoin,outoinptr,clkptr)

  typ_c=typ_l<>typ_l;
  typ_r=typ_r|dep_ut(:,2)
  
  done1=%f
  while ~done1
    done1=%t
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
  
    done=%f;
    while ~done
      done=%t;

      for jj=find(typ_l&(~typ_c));
	if ~or(jj==clkconnect(:,3)) then
	  typ_r(clkconnect(find(jj==clkconnect(:,1)),3))=%t
	  clkconnect(find(jj==clkconnect(:,1)),:)=[];
	  typ_c(jj)=%t;
	  done1=%f
	  done=%f
	end
      end
    end
  end

 
  critev=zeros(clkptr($)-1,1);
  for bb=1:size(clkptr,1)-1
    for i=clkptr(bb):clkptr(bb+1)-1
     if or(typ_zx(clkconnect(find((clkconnect(:,1)==bb)&..
	  (clkconnect(:,2)==i-clkptr(bb)+1)),3))) then
	critev(i)=1
      end
    end
  end
 
endfunction

