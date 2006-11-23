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
//          12- boolean vector (column): 1:nin entry for dependence on u,
//          last on t 
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

  show_pause=%f

  show_comment=%f

  global need_newblk
  need_newblk=%t

  if bllst==list() then
    message(['No block can be activated'])
    cpr=list()
    ok=%f;
    return
  end
  
  if exists('%scicos_solver')==0 then %scicos_solver=0,end

  clkptr=1,cliptr=1,typ_l=[],
  nblk=size(bllst)

  //take care of the heritage

  [bllst,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,dep_u,dep_uptr,dep_t,..
   typ_l,typ_r,typ_m,tblock,typ_cons,typ_zx,ok]=mini_extract_info(bllst,..
                                                connectmat,clkconnect)

  if show_trace then disp('c_pass20:'+string(timer())),end
  if ~ok then 
      cpr=list()
      return
  end
    
  //if ~ok then 
  // heritage block !
  [outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)
  
  [critev]=critical_events(connectmat,clkconnect,dep_t,typ_r,..
			   typ_l,typ_zx,outoin,outoinptr,clkptr)
  
  [clkconnect,exe_cons]=pak_ersi(connectmat,clkconnect,typ_r,..
				 typ_l,outoin,outoinptr,tblock,typ_cons,clkptr)
  //end
  
  if or(typ_l) then
    
    clkconnecttmp=clkconnect(find(clkconnect(:,1)<>0),:);
    n_clkco=size(clkconnecttmp,1)
    for i=1:n_clkco
      if typ_l(clkconnecttmp(i,1)) & clkconnecttmp(i,1)==clkconnecttmp(i,3) then
	message(['Algebrique loop detected';'on activation links']);
	cpr=list();return
      end
    end 
    
    [ordclk,ordptr,cord,ordoclk,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,..
     dep_uptr,corinv,clkptr,cliptr,critev,ok]=paksazi(typ_l,clkconnect,..
     connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)
    
    if ~ok then 
      cpr=list()
      return
    end
    
    clkconnect=cleanup(clkconnect)
    
  else
    blocs_traites=[]
    //on ordonnance les blocs
    ordoclk=get_clocks(clkconnect,clkptr);
    ordoclk=gsort(ordoclk,'lr','i')
    //existe le temps continu?
    temps_continu=%f
    if ordoclk(1)==0 then
      temps_continu=%t
      ordoclk(1,:)=[]
    end
    nblock=size(typ_l,1);
    ordptr=1;
    ordclk=[];
    cord=[];
    k=1
    if temps_continu then
      [bouclalg,vec,primary]=ordo3(0,0,clkconnect,connectmat,dep_t,dep_u,dep_uptr);
      if bouclalg then
	message('Algebrique  loop detected; cannot be compiled.');
	cpr=list()
	return,
      end
      // discardprimary duplicate calls to the same block port and group calls to 
      //different ports of the same block to compute execution table and its pointer.
      primary=discardprimary(primary)
      primary=gsort(primary,'lr','i')
      vecordo=vec(primary(:,1))
      ordoclk0=ordo_vecport(primary,vecordo)
      cord=ordoclk0; 
//      ordoclkcont=ordoclk0
    end

    while k<=size(ordoclk,1) 
      n_out=clkptr(ordoclk(k,1)+1)-clkptr(ordoclk(k,1))
      if n_out>0 then
	for j=1:n_out
          [bouclalg,vec,primary]=ordo3(ordoclk(k,1),j,clkconnect,connectmat,dep_t,dep_u,dep_uptr);
	  if bouclalg then
	    message('Algebrique  loop detected; cannot be compiled.');
	    cpr=list()
	    return,
	  end
	  primary=discardprimary(primary)	
	  primary=gsort(primary,'lr','i')
	  vecordo=vec(primary(:,1))
	  ordoclk0=ordo_vecport(primary,vecordo)	  
	  [ordptr,ordclk,blocs_traites]=set_ordclk(ordclk,ordoclk0,..
						   ordoclk(k),j,ordptr,blocs_traites)
          ordoclk=set_primary_clkport(ordoclk,ordoclk0,k)
	end      
      end
      k=k+1      
    end
  end
  
  if show_pause then 
    disp('fin de paksazi')
    pause
  end
  
  if show_trace then disp('c_pass31:'+string(timer())),end
  
  //extract various info from bllst
  [lnkptr,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,xptr,zptr,typ_mod,rpptr,ipptr,..
   xc0,xcd0,xd0,rpar,ipar,typ_z,typ_x,typ_m,funs,funtyp,initexe,labels,bexe,boptr,..
   blnk,blptr,ok]=extract_info(bllst,connectmat,clkconnect,typ_l);
  typ_z0=typ_z;

  if ~ok then
    message('Problem in port size');
    cpr=list()
    return,
  end

  if show_trace then disp('c_pass41:'+string(timer())),end

  //form a matrix which gives destinations of each block
  [outoin,outoinptr]=conn_mat(inpptr,outptr,inplnk,outlnk)
  [evoutoin,evoutoinptr]=synch_clkconnect(typ_l,clkconnect)
  //
  if show_trace then disp('c_pass50:'+string(timer())),end
  
  [execlk_cons]=discard(clkptr,cliptr,clkconnect,exe_cons)

  clkconnect=[];exe_cons=[]

  if show_trace then disp('c_pass501:'+string(timer())),end

  // Set execution scheduling tables 
  [ordclk,iord,oord,zord,typ_z,ok]=scheduler(inpptr,outptr,clkptr,execlk_cons,..
   outoin,outoinptr,evoutoin,evoutoinptr,typ_z,typ_x,typ_l,bexe,boptr,blnk,blptr,..
   ordclk,ordptr,cord,dep_t,dep_u,dep_uptr);
  
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
		 clkptr=clkptr,ordptr=ordptr,execlk=ordclk,ordclk=ordclk,..
		 cord=cord,oord=oord,zord=zord,critev=critev(:),..
		 nb=nb,ztyp=ztyp,nblk=nblk,ndcblk=ndcblk,..
		 subscr=subscr,funtyp=funtyp,iord=iord,labels=labels,modptr=modptr);

  //initialize agenda
  [tevts,evtspt,pointi]=init_agenda(initexe,clkptr)
  if show_trace then disp('c_pass61:'+string(timer())),end

  outtb=0*ones(lnkptr($)-1,1)
  //mod=0*ones(modptr($)-1,1)
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
  //  state.mod=mod
		     
  cpr=scicos_cpr(state=state,sim=sim,cor=cor,corinv=corinv);

  if show_trace then disp('c_pass71:'+string(timer())),end
  clearglobal need_newblk
endfunction

//donne les sources d'activation du schéma
function [vec_clk]=get_clocks(clkconnect,clkptr)
 vec_clk=[]
 if (find(clkconnect(:,1)==0) ~=[]) then
    //activation continue
    vec_clk=[vec_clk;0 0];
 end
 for blk=1:size(clkptr,1)-1       
   if ~typ_l(blk) then
     for port=1:clkptr(blk+1)-clkptr(blk)
       vec_clk=[vec_clk; blk port];           
     end
   end
 end
endfunction

//donne l'horloge dont dépend blk
function [act_clk]=get_actclk(blk,vec_clk)
  act_clk=[]
  [ancetres]=get_allparents(blk)
  k=1
  for i=1:size(ancetres,1)
    if find(ancetres(i)==act_clk)==[] & find(ancetres(i,1)==vec_clk)~=[] then
      act_clk(k)=ancetres(i)
      k=k+1;
    end
  end
endfunction

//insere le vecteur primary dans vec_clk après la ligne comptenant le bloc i
function vec_clk0=set_primary_clk(vec_clk,primary,i)

  if vec_clk~=[] then
    n_vc0=find(vec_clk==i)
    n_vc0=n_vc0($)
    vec_clk0=vec_clk(1:n_vc0)
    vec_clk1=vec_clk(n_vc0+1:size(vec_clk,1))
    for k=1:size(primary,1)
      if find(primary(k)==vec_clk0)==[] then
	vec_clk0($+1)=primary(k)
      end
    end
    for k=1:size(vec_clk1,1)
      if find(vec_clk1(k)==vec_clk0)==[] then
	vec_clk0($+1)=vec_clk1(k)
      end
    end
  else
    vec_clk0=primary
  end

endfunction

//insere la sous-matrice primary dans vec_clk après la ligne k
function vec_clk0=set_primary_clkport(vec_clk,primary,i)

  if vec_clk~=[] then
    vec_clk0=vec_clk(1:i,:)
    vec_clk1=vec_clk(i+1:size(vec_clk,1),:)
    for k=1:size(primary,1)
      f=find(primary(k,1)==vec_clk0(:,1))
      if f==[] then
	vec_clk0=[vec_clk0;primary(k,:)]
      end
    end
    n_vc1=size(vec_clk1,1)
    if n_vc1>0 then
      for k=1:n_vc1
	f=find(vec_clk1(k,1)==vec_clk0(:,1))
	if f==[] then
	  vec_clk0=[vec_clk0;vec_clk1(k,:)]
	end
      end
    end
  else
    vec_clk0=primary
  end
  
endfunction

//insere la sous-matrice ordoclk0 dans ordclk après le block k
function [ordptr,ordclk,blocs_traites]=set_ordclk(ordclk,..
						  ordoclk0,k,j,ordptr,blocs_traites)
  if ordoclk0~=[] then
    if ordclk==[] then
      ordclk=[ordclk;ordoclk0];
      ordptr($+1)=ordptr($)+size(ordoclk0,1);
      blocs_traites=[blocs_traites;k] //k
    else
      m=max(find(blocs_traites==k))
      if j>1 & m~=[] then
	ordclk=[ordclk(1:ordptr(m+1)-1,:);ordoclk0;ordclk(ordptr(m+1):$,:)]
	ordptr=[ordptr(1:m+1);ordptr(m+1:$)+size(ordoclk0,1)]
	blocs_traites=[blocs_traites(1:m);k;blocs_traites(m+1:$)]
      else
	ind=find(blocs_traites <k)
	m=max(ind)
	if m==size(blocs_traites,1) then
	  ordclk=[ordclk;ordoclk0]
	  ordptr($+1)=ordptr($)+size(ordoclk0,1);
	  blocs_traites=[blocs_traites;k]
	else
	  if m~=[] then
	    ordclk=[ordclk(1:ordptr(m+1)-1,:);ordoclk0;ordclk(ordptr(m+1):$,:)]
	    ordptr=[ordptr(1:m+1);ordptr(m+1:$)+size(ordoclk0,1)]
	    blocs_traites=[blocs_traites(1:m);k;blocs_traites(m+1:$)]
	  else
	    ordclk=[ordoclk0;ordclk]
	    ordptr=[1;ordptr+size(ordoclk0,1)]
	    blocs_traites=[k;blocs_traites]
	  end
	end
      end
    end
  else
    if j>1 & find((blocs_traites==k))~=[] then
      m=max(find(blocs_traites==k))
      ordptr=[ordptr(1:m+1);ordptr(m+1:$)+size(ordoclk0,1)]
      blocs_traites=[blocs_traites(1:m);k;blocs_traites(m+1:$)]
    else
      ind=find(blocs_traites <k)
      m=max(ind)
      if m==size(blocs_traites,1) then
	ordptr($+1)=ordptr($)+size(ordoclk0,1);
	blocs_traites=[blocs_traites;k]
      else
	if m~=[] then
	  ordptr=[ordptr(1:m+1);ordptr(m+1:$)+size(ordoclk0,1)]
	  blocs_traites=[blocs_traites(1:m);k;blocs_traites(m+1:$)]
	else
	  ordptr=[1;ordptr+size(ordoclk0,1)]
	  blocs_traites=[k;blocs_traites]
	end
      end
    end
  end
endfunction

//donne les blocs activés par blk port, le 1er argument permet d'ajouter des blocs 
//aux blocs déjà trouvésen cas de 1er appel de la fonction on utilise donc [] 
//en 1er argument
function [children]=get_children(children,blk,port)
  k=size(children,1)+1;
  f=find(clkconnect(:,1)==blk)
  n_f=size(f,2)
  if n_f>0 then
    for i=1:n_f
      clki3=clkconnect(f(i),3)
      if clkconnect(f(i),2)==port & find(clki3==children)==[] then
	children(k,1)=clki3
	k=k+1
      end
    end
  end
endfunction

function [children]=get_childrenport(children,blk,port)
  k=size(children,1)+1;
  f=find(clkconnect(:,1)==blk)
  n_f=size(f,2)
  if n_f>0 then
    for i=1:n_f
      if clkconnect(f(i),2)==port then
	clki3=clkconnect(f(i),3)
	clki4=clkconnect(f(i),4)
	g=find(clki3==children(:,1))
	if g==[] | (g~=[] & children(g,2)~=clki4) then
	  children(k,1)=clki3
	  children(k,2)=clki4
	  k=k+1
	end
      end
    end
  end
endfunction

//donne les blocs activés par blk, quel que soit le port d'activation.
function [children]=get_children2(children,blk)
  k=size(children,1)+1;
  f=find(clkconnect(:,1)==blk)
  n_f=size(f,2)
  if n_f>0 then
    for i=1:n_f
      clki3=clkconnect(f(i),3)
      if find(clki3==children)==[] then
	children(k,1)=clki3
	k=k+1
      end
    end
  end
endfunction

//donne les blocs-port activés par blk, quel que soit le port d'activation.
function [children]=get_children2port(children,blk)
  k=size(children,1)+1;
  f=find(clkconnect(:,1)==blk)
  n_f=size(f,2)
  if n_f>0 then
    for i=1:n_f
      clki3=clkconnect(f(i),3)
      clki4=clkconnect(f(i),4)
      g=find(clki3==children(:,1))
      if g==[] | (g~=[] & children(g,2)~=clki4)
	children(k,1)=clki3
	children(k,2)=clki4
	k=k+1
      end
    end
  end
endfunction

//donne les blocs activés par blk port, et leur descendance (récursif) 
function [local_children]=get_allchildren(blk,port)
  local_children=get_children([],blk,port)
  i=1
  while i<=size(local_children,1)
    if typ_l(local_children(i)) then
      local_children=get_children2(local_children,local_children(i))
    end
    i=i+1
  end
endfunction

function [local_children]=get_allchildrenport(blk,port)
  local_children=get_childrenport([],blk,port)
  i=1
  while i<=size(local_children,1)
    if typ_l(local_children(i)) then
      local_children=get_children2port(local_children,local_children(i))
    end
    i=i+1
  end
endfunction

//donne les blocs activés par blk, et leur descendance (récursif) 
function [local_children]=get_allchildren2(block)
  local_children=get_children2([],block)
  i=1
  while i<=size(local_children,1)
    if typ_l(local_children(i)) then
      local_children=get_children2(local_children,local_children(i))
    end
    i=i+1
  end
endfunction

function [local_children]=get_allchildren2port(block)
  local_children=get_children2port([],block)
  i=1
  while i<=size(local_children,1)
    if typ_l(local_children(i)) then
      local_children=get_children2port(local_children,local_children(i))
    end
    i=i+1
  end
endfunction

//donne les blocs activant blk
function [parents]=get_parents(parents,blk)
  f=find(clkconnect(:,3)==blk)
  n_f=size(f,2)
  if n_f>0 then
    for i=1:n_f
      clki1=clkconnect(f(i),1)
      clki2=clkconnect(f(i),2)
      g=find(clki1==parents(:,1))
      if g==[] | (g~=[] & parents(g,2)~=clki2) then
	parents=[parents;clki1,clki2]
      end
    end
  end
endfunction

//donne les blocs activant blk, et leurs ascendants (récursif)
function [local_parents]=get_allparents(block)
  n_clkco=size(clkconnect,1)
  local_parents=get_parents([],block)
  i=1
  while i<=size(local_parents,1)
    if local_parents(i,1)~=0
      local_parents=get_parents(local_parents,local_parents(i))
    end
    i=i+1
  end
endfunction

function [allpar,allparptr]=ini_allpar(clkconnect,vec)
  n_v=size(vec,1)
  allpar=[]
  allparptr=1
  for z=1:n_v
    allpar_tmp=get_allparents(vec(z))
    allpar=[allpar;allpar_tmp]
    allparptr=[allparptr;allparptr($)+size(allpar_tmp,1)]
  end
endfunction

//appelé par allpar(allparptr(blk):allparptr(blk+1)-1,:)
function [allpar,allparptr]=maj_allpar(allpar,allparptr,clkconnect,oldvec,vec,amaj)
  children=[]
  for i=1:size(amaj,1)
    children=[children;get_allchildren2(amaj(i));amaj(i)]
  end
  children=unique(children)
  amaj=intersection(children,oldvec)
  
  //on supprime des blocs à vec_plus
  newless=setdiff(oldvec,vec)
  newless=unique([amaj;newless])
  allparmaj=[]
  dontmaj=[]
  vecmaj=[]
  allparmajptr=1
  if newless~=[] then
    for i=1:size(newless,1)
      veci=find(newless(i)==oldvec)
      allparold=allpar(allparptr(veci):allparptr(veci+1)-1,:)
      allparnew=get_allparents(newless(i))
      if size(allparold,1)==size(allparnew,1) then
	allparold=gsort(allparold,'lr','i')
	allparnew=gsort(allparnew,'lr','i')
	if allparold==allparnew then
	  if show_comment then
	    disp('pas besoin de màj les parents du bloc '+string(newless(i)))
	  end
	  dontmaj=[dontmaj;newless(i)]
	else
	  allparmaj=[allparmaj;allparnew]
	  allparmajptr=[allparmajptr;allparmajptr($)+size(allparnew,1)]
	  vecmaj=[vecmaj;newless(i)]
	end
      else
	allparmaj=[allparmaj;allparnew]
	allparmajptr=[allparmajptr;allparmajptr($)+size(allparnew,1)]
	vecmaj=[vecmaj;newless(i)]
      end
    end
    newless=setdiff(newless,dontmaj)
    [allpar,allparptr,oldvec]=maj_del(allpar,allparptr,oldvec,newless)
  end
  
  amaj=intersection(children,vec)
  //on ajoute des blocs à vec_plus
  newplus=setdiff(vec,oldvec)
  newplus=unique([amaj;newplus])
  newplus=setdiff(newplus,dontmaj)
  if newplus~=[] then
    [allpar,allparptr,oldvec]=maj_add(allpar,allparptr,oldvec,vec,..
				      newplus,allparmaj,allparmajptr,vecmaj)
  end
  
endfunction

function [allpar,allparptr,oldvec]=maj_del(allpar,allparptr,oldvec,newless)
  
  for i=1:size(newless,1)
    f=find(newless(i)==oldvec)
    if f==size(oldvec,1) then
      allpar=[allpar(1:allparptr(f)-1,:)]
      allparptr($)=[]
    else
      if f==1 then
	allpar=[allpar(allparptr(f+1):$,:)]
	allparptr(1)=[]
	allparptr=allparptr-allparptr(1)+1
      else
	allpar=[allpar(1:allparptr(f)-1,:);allpar(allparptr(f+1):$,:)]
	allparptr=[allparptr(1:f);allparptr(f+2:$)-(allparptr(f+1)-allparptr(f))]
	g=find(allparptr<0)
	allparptr(g)=[]
      end
    end
    oldvec(f)=[]
  end
endfunction

function [allpar,allparptr,oldvec]=maj_add(allpar,allparptr,oldvec,vec,..
					   newplus,allparmaj,allparmajptr,vecmaj)
  
  for i=1:size(newplus,1)
    f=find(newplus(i)==vec)
    if f~=[] then
      g=find(newplus(i)==vecmaj)
      if g~=[] then
	allpar_tmp=allparmaj(allparmajptr(g):allparmajptr(g+1)-1,:)
      else
	allpar_tmp=get_allparents(vec(f))
      end
      if allpar==[] then
	allpar=allpar_tmp
	allparptr=[1;1+size(allpar_tmp,1)]
      else	
	if f==1 then
	  allpar=[allpar_tmp;allpar]
	  allparptr=[1;allparptr+size(allpar_tmp,1)]
	else
	  if f==size(vec,1) | f==size(oldvec,1)+1 then
	    allpar=[allpar;allpar_tmp]
	    allparptr=[allparptr(1:f);allparptr(f)+size(allpar_tmp,1)]
	  else
	    allpar=[allpar(1:allparptr(f)-1,:);allpar_tmp;allpar(allparptr(f):$,:)]
	    allparptr=[allparptr(1:f);allparptr(f)+size(allpar_tmp,1);...
	    allparptr(f+1:$)+size(allpar_tmp,1)]
	  end
	end
      end
      oldvec=[oldvec;newplus(i)]
      oldvec=gsort(oldvec,'g','i')
    end
  end
endfunction

//function [vec]=my_setdiff(vec,blk)
//  for i=1:size(blk,1)
//    f=find(vec==blk(i))'
//    vec(f)=[]
//  end
//  vec=gsort(vec,'g','i')
//endfunction

function [done,blk_duplicated,primary,wire,typ_l,clkconnect,connectmat,bllst,..
          dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev,vec_plus,allpar,..
          allparptr,amaj,oldvec,majplus]=get_wire2(wire,blk,port,typ_l,clkconnect,..
          connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev,..
          vec_plus,allpar,allparptr)

  primary=get_children([],blk,port)
  //on enleve blk de primary (cas des horloges)
  primary=setdiff(primary,blk)
  n_p=size(primary,1)
  done=%f
  blk_duplicated=[]
  amaj=[]
  majplus=[]
  oldvec=[]
  if n_p>1 then
    i=1
    while ~done & i<=n_p 
      if typ_l(primary(i)) then
	n_out=clkptr(primary(i,1)+1)-clkptr(primary(i,1))
	l=1
	while ~done & l<=n_out 
	  children=get_allchildren(primary(i),l)
	  if children~=[] then
	    childplus=intersection(children,vec_plus)
	    if childplus~=[] then
	      //penser à mettre à jour vec_plus quand on fait union ou duplique
	      n_cp=size(childplus,1)
	      j=1
	      while ~done & j<=n_cp		
		childj=childplus(j)
		//if (~typ_l(childj) | (typ_l(childj) & find(clkconnect(:,1)==childj)~=[])) then
		if find(childj==primary)~=[] & ~done then
		  //si le bloc à dupliquer est fils direct du primary et de blk 
		  //on remplace l'activation de blk par l'union des sorties 
		  //du primary on teste si le(s) lien(s) activant childj est
		  //(sont) les mêmes que ceux activant primary(i) 
		  p1=get_parents([],primary(i))
		  p2=get_parents([],childj)
		  p12=[]
		  n_p1=size(p1,1)
		  p12=is_matinmat(p1,p2)
		  children=[primary(i);children]
		  blkp=intersection(children,p2(:,1))
		  if p12==p1 then
		    ind2=sameport(clkconnect,blkp,childj,blk,port)
		    if ind2~=[] then
		      if show_comment then 
			disp('del_union:')
			disp('on enleve le lien entre '+string(blkp)+' et '+string(childj)+..
                             '  avec clk port = '+string(blk)+' '+string(port))
		      end
		      if blkp==[] then
			disp('pb: le lien n existe pas')		
		      end
		      [clkconnect]=del_union(ind2,clkconnect)
		      done=%t  
		      blk_duplicated=[]
		      oldvec=vec_plus
		      newparents=get_parents([],childj)
		      if size(newparents,1)==size(p2,1) then
			newparents=gsort(newparents,'lr','i')
			p2=gsort(p2,'lr','i')
			if newparents~=p2 then
			  amaj=childj
			end
		      end
		      majplus=childj
		      //sortir de toute les boucles et refaire le wire de primary
		    else
		      //si childplus est activé par un port différent du lien direct
		      if show_comment then 
			disp('do_union:')
			disp('bloc '+string(childj)+' reçoit en entrée l union des sorties du bloc '+string(primary(i))+' avec clk port = '+string(blk)+' '+string(port))
		      end
		      [clkconnect]=do_union(childj,primary(i),blk,port,p12,clkconnect,..
                                            connectmat,bllst)
		      done=%t  
		      blk_duplicated=[]
		      oldvec=vec_plus
		      newparents=get_parents([],childj)
		      if size(newparents,1)==size(p2,1) then
			newparents=gsort(newparents,'lr','i')
			p2=gsort(p2,'lr','i')
			if newparents~=p2 then
			  amaj=childj
			end
		      end
		      majplus=childj
		    end
		  else
		    if show_comment then 
		      disp('duplique1:')
		      disp('pas équivalence de l union des sorties du bloc '+..
                            string(primary(i)))
		    end
		    [typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,..
                     clkptr,cliptr,critev]=duplique1(primary(i),typ_l,clkconnect,connectmat,..
		     bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)
		    blk_duplicated=[blk_duplicated;size(typ_l,1)]
		    done=%t
		    oldvec=vec_plus
		    newparents=get_parents([],primary(i))
		    if size(newparents,1)==size(p2,1) then
		      newparents=gsort(newparents,'lr','i')
		      p2=gsort(p2,'lr','i')
		      if newparents~=p2 then
			amaj=primary(i)
		      end
		    end
		    amaj=[amaj;blk_duplicated]
		    majplus=[childj;get_children2([],primary(i));primary(i);blk_duplicated]
		  end
		  
		else
		  blk1=[]
		  primary1=setdiff(primary,primary(i))
		  if or(typ_l(primary1)) then
		    veci=find(childj==vec_plus)
		    parents=allpar(allparptr(veci):allparptr(veci+1)-1,:)
		    n_p1=size(primary1,1)
		    for k=1:n_p1
		      if find(primary1(k)==parents(:,1))~=[] then
			blk1=primary1(k)
		      end
		    end
		    if blk1~=[] & ~done then
		      blk1=blk1(1)
		      if show_comment then 
			disp('il existe un primary different dans le wire')
		      end
		      if ~done & typ_l(blk1) then 
			//non exclusif:
			//on teste si les 2 sorties du if sont utilisées
			double_if=find(blk1==clkconnect(:,1))
			n_if=size(double_if,2)
			outif=[]
			k=1
			for j1=1:n_if
			  outif(k)=clkconnect(double_if(j1),2)
			  k=k+1
			end
			//on cherche le fils direct de blk1 appartenant à parents
			child2=get_children2([],blk1)
			parentsj=[childj;parents(:,1)]
			blk2=[]
			for j2=1:size(child2,1)
			  if find(child2(j2)==parentsj)~=[] then
			    blk2=child2(j2)
			  end
			end
			if blk2==[] then
			  disp('pb: blk2 n existe pas') 			  
			end
			if find(1==outif)~=[] & find(2==outif)~=[] then
			  //les 2 sorties du bloc if sont utilisées
			  if show_pause then 
			    disp('duplique_2:')
			    disp('duplique bloc '+string(blk1)+' qui active le bloc '+..
                                 string(blk2))
			  end
			  parentsdirect=get_parents([],blk2)
			  [blk_duplicated,clkconnect,connectmat,bllst,typ_l,dep_t,..
                           dep_u,dep_uptr,corinv,clkptr,cliptr,critev]=duplique_2(blk1,..
                           primary(i),blk,port,blk2,clkconnect,connectmat,bllst,typ_l,..
                           dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)
			  done=%t
			  oldvec=vec_plus
			  newparents=get_parents([],blk2)
			  if size(newparents,1)==size(parentsdirect,1) then
			    newparents=gsort(newparents,'lr','i')
			    p2=gsort(p2,'lr','i')
			    if newparents~=parentsdirect then
			      amaj=blk2
			    end
			  end
			  amaj=[amaj;blk_duplicated]	
			  majplus=[childj;blk_duplicated]
			  //sortir de toute les boucles et refaire le wire de primary
			else
			  if show_comment then
			    disp('duplique_union:')
			    disp('bloc '+string(blk1)+' est dupliqué. Il reçoit en entrée une sortie du bloc '+string(primary(i))+'. Le bloc dupliqué active le bloc '+string(blk2))
			   
			    if ~typ_l(blk1) then
			      disp('pb: le bloc '+string(blk1)+' n est pas typ_l')	      
			    end
			  end
			  parentsdirect=get_parents([],blk2)  
			  [blk_duplicated,clkconnect,connectmat,bllst,typ_l,dep_t,..
                           dep_u,dep_uptr,corinv,clkptr,cliptr,critev]=duplique_union(blk1,..
                           primary(i),blk,port,blk2,clkconnect,connectmat,bllst,typ_l,..
                           dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)
			  done=%t  
			  oldvec=vec_plus
			  newparents=get_parents([],blk2)
			  if size(newparents,1)==size(parentsdirect,1) then
			    newparents=gsort(newparents,'lr','i')
			    p2=gsort(p2,'lr','i')
			    if newparents~=parentsdirect then
			      amaj=blk2
			    end
			  end
			  amaj=[amaj;blk_duplicated]
			  majplus=[childj;blk2;get_children2([],blk1);blk_duplicated]			  
			  //sortir de toute les boucles et refaire le wire de primary
			end
		      end
		    end
		  end
		end		
	      //end //pour le if typ_l(childj)
		j=j+1
	      end 
	    end
	  end
	  l=l+1
	end
      end
      i=i+1
    end
  end
endfunction

function [ind2]=sameport(clkconnect,blkp,childj,blk,port)
  ind=[]
  ind2=[]
  g=find(clkconnect(:,3)==childj)
  if g~=[] then
    for m=1:size(g,2)
      if clkconnect(g(m),1)==blkp & clkconnect(g(m),3)==childj then
	ind=[ind;g(m)]
      end
    end
    if ind~=[] then
      for m=1:size(ind,1)
	for mm=1:size(g,2)
	  if clkconnect(g(mm),1)==blk & clkconnect(g(mm),2)==port & clkconnect(g(mm),4)==clkconnect(ind(m),4) then
	    ind2=[ind2;ind(m)]
	  end
	end
      end
    end
  end
endfunction

//enlève l'activation entre clk0 port0 et blk
//et la remplace par l'union des sorties du bloc blk0
function [clkconnect]=del_union(ind2,clkconnect)
//on enlève les lignes de clkconnect liant blk0 et blk
  clkconnect(ind2,:)=[]  
endfunction

//enlève l'activation entre clk0 port0 et blk
//et la remplace par l'union des sorties du bloc blk0
function [clkconnect]=do_union(blk,blk0,clk0,port0,p12,clkconnect,connectmat,bllst)
  n_clkco=size(clkconnect,1);
  //clk0=blk de get_wire
  //port0=port de get_wire
  //blk0: blk donnant l'union de ses sorties
  //blk: blk recevant l'union
  
  f=find(clkconnect(:,3)==blk)
  n_f=size(f,2)
  for k=1:n_f
    if clkconnect(f(k),1)==clk0 & clkconnect(f(k),2)==port0 then
      clkconnect(f(k),1)=blk0
      clkconnect(f(k),2)=1
      clkco4=clkconnect(f(k),4)
    end
  end
  
  n_out=clkptr(blk0+1)-clkptr(blk0)
  for k=2:n_out
    n_clkco=n_clkco+1
    clkconnect(n_clkco,:)=[blk0,k,blk,clkco4]
  end
  
  //on enlève la ligne de clkconnect liant p12 et blk
  n_p=size(p12,1)
  if n_p>1 then
    f1=find(p12(:,1)==clk0)
    for k=1:size(f1,2)
      if p12(f1(k),2)==port0 then
	p12(f1,:)=[]
	n_p=n_p-1
      end
    end
    for k=1:n_p
      f2=find(p12(k,1)==clkconnect(:,1))
      for l=1:size(f2,2)
	if clkconnect(f2(l),:)==[p12(k,1),p12(k,2),blk,clkco4] then
	  clkconnect(f2(l),:)=[]
	end
      end
    end
  end
endfunction

//on enlève le lien entre blk et blk2 pour le remplacer, au lieu de dupliquer blk 
//puis de lui donner l'union des sorties de blk0, on duplique blk autant de fois que 
//blk0 a de sorties chaque nouveau bloc reçoit une activation par un port différent 
//de blk0 les nouveaux blocs activent blk2 par le port clk0
function [blk_duplicated,clkconnect,connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,..
          corinv,clkptr,cliptr,critev]=duplique_2(blk,blk0,clk0,port0,blk2,clkconnect,..
	  connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)

  //clk0=blk de get_wire
  //port0=port de clk0
  //blk0: bloc du primary
  //blk: bloc à dupliquer
  //blk2: bloc activé par blk
  //disp('dans duplique_2')
  blk_duplicated=[]
  n_clkco=size(clkconnect,1);
  n_clkco0=n_clkco;
  nblock=size(typ_l,1);
  nblock0=nblock;
  n_cmat=size(connectmat,1);
  
  f=find(clkconnect(:,3)==blk2)
  n_f=size(f,2)
  for k=1:n_f
    if clkconnect(f(k),1)==blk then
      clkconnect(f(k),1)=nblock+1
      port1=clkconnect(f(k),2)
      clkco4=clkconnect(f(k),4)
      ind=f(k)
    end
  end
  
  n_out=clkptr(blk0+1)-clkptr(blk0)
  for k=1:n_out
    nblock=nblock+1
    blk_duplicated=[blk_duplicated;nblock]
    clkconnect($+1,:)=[blk0,k,nblock,1]
    if k>1 then
      clkconnect($+1,:)=[nblock,port1,blk2,clkco4]
    end
  end
  
  n_bd=size(blk_duplicated,1)
  f=find(connectmat(:,3)==blk)
  n_f=size(f,2)
  for j=1:n_f
    for k=1:n_bd
      n_cmat=n_cmat+1
      connectmat(n_cmat,:)=connectmat(f(j),:)
      connectmat(n_cmat,3)=blk_duplicated(k)
    end
  end   
  g=find(clkconnect(:,3)==blk)  
  for k=1:n_bd
    for j=1:size(g,2)
      if clkconnect(g(j),:)~=[clk0,port0,blk,1] then
	n_clkco=n_clkco+1
	clkconnect(n_clkco,:)=clkconnect(g(j),:)
	clkconnect(n_clkco,3)=blk_duplicated(k)
      end
    end
    critev=[critev;critev(clkptr(blk):clkptr(blk+1)-1)]
    bllst(blk_duplicated(k))=bllst(blk)
    typ_l(blk_duplicated(k))=%t
    
// on peut dupliquer dep_u en dhors de la boucle
	
    dep_t(blk_duplicated(k))=dep_t(blk)
    dep_u=[dep_u;dep_u(dep_uptr(blk))]
    dep_uptr($+1)=dep_uptr($)+1

    corinv(blk_duplicated(k))=corinv(blk)
    clkptr(blk_duplicated(k)+1)=clkptr(blk_duplicated(k))+clkptr(blk+1)-clkptr(blk)
    cliptr(blk_duplicated(k)+1)=cliptr(blk_duplicated(k))+cliptr(blk+1)-cliptr(blk)
  end  
endfunction

//idem duplique_2, mais comme le bloc à dupliquer n'à qu'un port de connecté,
//au lieu de le dupliquer deux fois, on ne le duplique qu'une fois,
//et on remplace le lien entre clk0 port0 et blk par un lien entre blk0 1 et blk
function [blk_duplicated,clkconnect,connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,..
          corinv,clkptr,cliptr,critev]=duplique_union(blk,blk0,clk0,port0,blk2,..
          clkconnect,connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,corinv,clkptr,..
          cliptr,critev)

  //clk0=blk de get_wire
  //port0=port de clk0
  //blk0: bloc du primary
  //blk: bloc à dupliquer
  //blk2: bloc activé par blk
  
  blk_duplicated=[]
  n_clkco=size(clkconnect,1);
  nblock=size(typ_l,1);
  nblock0=nblock;
  n_cmat=size(connectmat,1);
  
  g=find(clkconnect(:,3)==blk)
  n_g=size(g,2)
  for k=1:n_g
    if clkconnect(g(k),1)==clk0 & clkconnect(g(k),2)==port0 then
      clkconnect(g(k),1)=blk0
      clkconnect(g(k),2)=1
      clkco4=clkconnect(g(k),4)
      ind=k
    end
  end
  
  n_out=clkptr(blk0+1)-clkptr(blk0)
  for k=2:n_out
    nblock=nblock+1
    blk_duplicated=[blk_duplicated;nblock]
    clkconnect=[clkconnect;blk0,k,nblock,clkco4]
  end
  
  n_bd=size(blk_duplicated,1)
  f=find(connectmat(:,3)==blk)
  n_f=size(f,2)
  for j=1:n_f
    for k=1:n_bd
      cmat_tmp=connectmat(f(j),:)
      cmat12=cmat_tmp(1,1:2)
      cmat4=cmat_tmp(1,4)
      connectmat=[connectmat;cmat12,blk_duplicated(k),cmat4]
    end
  end

  h=find(clkconnect(:,1)==blk)
  
  for k=1:n_bd
    for j=1:size(h,2)
      clkco_tmp=clkconnect(h(j),2:4)
      clkconnect=[clkconnect;blk_duplicated(k),clkco_tmp]
    end
    for j=1:n_g
      if j~=ind then
	clkco_tmp=clkconnect(g(j),:)
	clkco12=clkco_tmp(1,1:2)
	clkco4=clkco_tmp(1,4)
	clkconnect=[clkconnect;clkco12,blk_duplicated(k),clkco4]
      end
    end
    critev=[critev;critev(clkptr(blk):clkptr(blk+1)-1)]
    bllst(blk_duplicated(k))=bllst(blk)
    typ_l(blk_duplicated(k))=%t
    
// on peut dupliquer dep_u en dhors de la boucle
	
    dep_t(blk_duplicated(k))=dep_t(blk)
    dep_u=[dep_u;dep_u(dep_uptr(blk))]
    dep_uptr($+1)=dep_uptr($)+1

    corinv(blk_duplicated(k))=corinv(blk)
    clkptr(blk_duplicated(k)+1)=clkptr(blk_duplicated(k))+clkptr(blk+1)-clkptr(blk)
    cliptr(blk_duplicated(k)+1)=cliptr(blk_duplicated(k))+cliptr(blk+1)-cliptr(blk)
  end  
endfunction

function [is,n]=is_lineinmat(line,vec)
  is=%f
  n=[]
  f=find(vec(:,1)==line(1,1))
  for i=1:size(f,2)
    if vec(f(i),2)==line(1,2) then
      is=%t 
      n=f(i)
    end
  end
endfunction

function [vec3]=is_matinmat(vec1,vec2)
  is=%f
  vec3=[]
  n_v1=size(vec1,1)
  for i=1:n_v1
    vec1i=vec1(i,:)
    [is,n]=is_lineinmat(vec1i,vec2)
    if is then
      vec3=[vec3;vec1i]
    end
  end
endfunction

function [parents]=get_parents2(parents,blk,port)
  k=size(parents,1)+1;
  f=find(clkconnect(:,3)==blk)
  n_f=size(f,2)
  if n_f>0 then
    for i=1:n_f
      if clkconnect(f(i),4)==port then
	clki1=clkconnect(f(i),1)
	clki2=clkconnect(f(i),2)
	g=find(clki1==parents(:,1))
	if g==[] | (g~=[] & parents(g,2)~=clki2) then
	  parents(k,1)=clki1
	  parents(k,2)=clki2
	  k=k+1
	end
      end
    end
  end
endfunction

//suppression des liens inutiles
function [clkconnect,amaj]=find_del_inutile(clkconnect,vec_plus,typ_l)
  amaj=[]
  for i=1:size(vec_plus,1)
    blk=vec_plus(i,1)
    port=vec_plus(i,2)
    parents=get_parents2([],blk,port)
    j=1
    done=%f
    n_p=size(parents,1)
    while j<=n_p & ~done
      par1=parents(j,1)
      if par1~=0 & typ_l(par1) then
	n_out=clkptr(par1+1)-clkptr(par1)
	f=find(par1==parents(:,1))
	if size(f,2)==n_out then
	  if show_comment then
	    disp('del_inutile:')
	    disp('les liens entre les blocs '+string(par1)+' et '+string(blk)+..
                 ' sont supprimés')
	  end
	  [clkconnect]=del_inutile(clkconnect,par1,n_out,blk,port)
	  done=%t
	  amaj=[amaj;par1;blk]
	end
      end
      j=j+1
    end
  end
endfunction

function [clkconnect]=del_inutile(clkconnect,blk0,n_out,blk,port)
  f=find(clkconnect(:,1)==blk0)
  del=[]
  for i=1:size(f,2)
    if clkconnect(f(i),3)==blk & clkconnect(f(i),4)==port then
      del=[del;f(i)]
    end
  end
  clkconnect(del,:)=[]
  p=get_parents([],blk0)
  n_p=size(p,1)
  for i=1:n_p
    clkconnect($+1,:)=[p(i,1),p(i,2),blk,port]
  end
endfunction

//on cherche les if activés 2 fois
function [done,blk_duplicated,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,..
          corinv,clkptr,cliptr,critev,amaj]=if_double_activated(vec_clk,typ_l,..
          clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)
  amaj=[]
  n_clkco=size(clkconnect,1);
  nblock=size(typ_l,1);
  n_cmat=size(connectmat,1);
  blk_duplicated=[]
  done=%f
  for i=1:nblock
    if typ_l(i) then
      double_if=find(i==clkconnect(:,3))
      n_if=size(double_if,2)
      if n_if>1 then
	act_clk1=get_actclk(clkconnect(double_if(1),1),vec_clk)
	act_clk2=get_actclk(clkconnect(double_if(2),1),vec_clk)
	if act_clk1==act_clk2 then
	  if show_comment then
	    disp('duplique1')
	    disp('bloc if '+string(i)+' activé 2 fois => on le duplique')
	  end
	  [typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,..
           cliptr,critev]=duplique1(i,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,..
                                    dep_uptr,corinv,clkptr,cliptr,critev) 
	  amaj=[amaj;i;get_children2([],i);nblock]
	  n_clkco=size(clkconnect,1);
	  nblock=length(bllst);
	  n_cmat=size(connectmat,1);
	  blk_duplicated=[blk_duplicated;nblock]
	  done=%t
	end
      end
    end
  end
endfunction

//on duplique les blocs if activés 2x par une horloge commune
function [typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,..
          cliptr,critev]=duplique1(blk,typ_l,clkconnect,connectmat,bllst,dep_t,..
                                   dep_u,dep_uptr,corinv,clkptr,cliptr,critev)

  n_clkco=size(clkconnect,1);
  nblock=size(typ_l,1);
  n_cmat=size(connectmat,1);

  for k=1:n_cmat
    if connectmat(k,3)==blk then
      connectmat(n_cmat+1,:)=connectmat(k,:)
      connectmat(n_cmat+1,3)=nblock+1
      n_cmat=n_cmat+1
    end
  end
  
  for k=1:n_clkco
    if clkconnect(k,1)==blk then
      clkconnect(n_clkco+1,:)=clkconnect(k,:)
      clkconnect(n_clkco+1,1)=nblock+1
      n_clkco=n_clkco+1
    end
  end

  f=find(blk==clkconnect(:,3))
  clkconnect(f(1),3)=nblock+1  
  critev=[critev;critev(clkptr(blk):clkptr(blk+1)-1)]
  bllst(nblock+1)=bllst(blk)
  typ_l(nblock+1)=%t
  
  dep_t(nblock+1)=dep_t(blk)
  dep_u=[dep_u;dep_u(dep_uptr(blk))]
  dep_uptr($+1)=dep_uptr($)+1

  corinv(nblock+1)=corinv(blk)
  clkptr(nblock+2)=clkptr(nblock+1)+clkptr(blk+1)-clkptr(blk)
  cliptr(nblock+2)=cliptr(nblock+1)+cliptr(blk+1)-cliptr(blk)
endfunction

function [allchildren2,allchildren2ptr,children2,children2ptr,..
          children,childrenptr]=vec_allchildren2()

  allchildren2=[]
  allchildren2ptr=1
  
  children2=[]
  children2ptr=1
  
  children=[]
  childrenptr=1
  k=1
  
  for i=1:nblock
    //calcul des allchildren2
    allchildren2_tmp=get_allchildren2(i)
    allchildren2=[allchildren2;allchildren2_tmp]
    allchildren2ptr(i+1)=size(allchildren2_tmp,1)+allchildren2ptr(i)

    //calcul des children2
    
    children2_tmp=get_children2([],i)
    children2=[children2;children2_tmp]
    children2ptr(i+1)=size(children2_tmp,1)+children2ptr(i)
    //appelé par children2
    //=allchildren2(allchildren2ptr(blk):allchildren2ptr(blk+1)-1)
    
    //calcul des children

    n_out=clkptr(i+1)-clkptr(i)
    if n_out>0 then
      for j=1:n_out
	k=k+1
	children_tmp=get_children([],i,j)
	children=[children;children_tmp]
	childrenptr(k)=size(children_tmp,1)+childrenptr(k-1)
      end
    else
      k=k+1
      childrenptr(k)=childrenptr(k-1)
    end
    
  end
endfunction


function [bouclalg,vec,primary]=ordo2(blk,port,clkconnect,connectmat,..
				      primary,dep_t,dep_u,dep_uptr)

  counter2=0
  oldvec2=[]
  blk_duplicated=[]
  done=%f
  fromfixe=%f
  bouclalg=%f
  n_clkco=size(clkconnect,1);
  nblock=size(typ_l,1);
  n_cmat=size(connectmat,1);

  //initialisation de vec
  //on initialise vec à -1
  vec=-ones(nblock,1)
  //on cherche tous les enfants de clk
  all=get_allchildrenport(blk,port)
  //on met tous les enfants de clk à 0
  vec(all(:,1))=0
  
  //on cherche les enfants directs: primary
  primary=get_childrenport([],blk,port)
  //on met les enfants à 1
  vec(primary(:,1))=1

  //on enlève blk de primary (cas des clk)
  f1=find(primary(:,1)==blk)
  n_f1=size(f1,2)
  if n_f1>0 then
    for i1=1:n_f1
      if primary(f1(i1),2)==port then
	primary(f1(i1),:)=[]
      end
    end
  end
  // To fix the problem with algebraic loops when blk is typ_l
  if blk>0 then  // exclude the block "always active"
    if typ_l(blk) then primary=[primary;blk,1],end
  end
  
  n_p=size(primary,1)
  if n_p>0 then
    gap=%f
    while ~fromfixe & ~bouclalg & ~gap
      for i=1:n_p
	primary1=primary(i,1)
        if typ_l(primary1) then   //RN
	  w=get_allchildren2port(primary1)
	else
	  w=[]
        end
	w=[primary(i,:);w]
	//on cherche d'où vient l'entrée des blocs: g

	//if dep_ut(primary1,1) then
	indport=find(dep_u(dep_uptr(primary1):dep_uptr(primary1+1)-1));
	g=[];
	if ( indport ~= [] ) then
	  wu=[primary1*ones(size(indport','*'),1) indport'];	
  	  for i=1:size(wu,1)
            f=find(connectmat(:,3)==wu(i,1) & connectmat(:,4)==wu(i,2))'
            g=[g;connectmat(f,1)]
          end
	  //g=connectmat(find(connectmat(:,3)==primary1),1)
	  //on garde dans g les termes >-1
	  //i.e. les blocs activés par l'horloge clk
	  g=g(find(vec(g)>-1))
	  if g~=[] then
	    vec(primary1)=max(vec(primary1),max(vec(g))+1)
	  end
	end
	vec(w(:,1))=vec(primary1)
      end

      gap=find_gap(vec(primary(:,1)))
      
      if vec==oldvec2 then
	fromfixe=%t
      else
	oldvec2=vec
      end
      counter2=counter2+1
      if counter2>2*nblock | gap then
	//améliorer la borne inf!?
	bouclalg=%t
	disp('Algebric loop detected in ordo2')
      end
    end
  end
endfunction

function [bouclalg,vec,primary]=ordo3(blk,port,clkconnect,connectmat,..
				      dep_t,dep_u,dep_uptr)
  counter2=0
  oldvec2=[]
  blk_duplicated=[]
  done=%f
  fromfixe=%f
  bouclalg=%f
  n_clkco=size(clkconnect,1);
  nblock=size(typ_l,1);
  n_cmat=size(connectmat,1);

  //initialisation de vec
  //on initialise vec à -1
  vec=-ones(nblock,1)
  //on cherche tous les enfants de clk
  all=get_allchildrenport(blk,port)
  //on met tous les enfants de clk à 0
  vec(all(:,1))=0
  //on cherche les enfants directs: primary
  primary=get_childrenport([],blk,port)
  //on met les enfants à 1
  vec(primary(:,1))=1

  n_p=size(primary,1)  
  if n_p>1 then
    gap=%f
    while ~fromfixe & ~bouclalg & ~gap
      for i=1:n_p
	w=primary(i)

	indport=find(dep_u(dep_uptr(w):dep_uptr(w+1)-1));
	g=[];
	if ( indport ~= [] ) then
	  wu=[w*ones(size(indport','*'),1) indport'];	
  	  for i=1:size(wu,1)
            f=find(connectmat(:,3)==wu(i,1) & connectmat(:,4)==wu(i,2))'
            g=[g;connectmat(f,1)]
          end
	  //if dep_ut(w,1) then
	  //g=connectmat(find(connectmat(:,3)==w),1)
	  if (g ~= []) then
	    vec(w)=vec(w)-1
	    vec(w)=max(vec(w),max(vec(g)))+1
	  end
	end
      end

      gap=find_gap(vec(primary(:,1)))

      if vec==oldvec2 then
	fromfixe=%t
      else
	oldvec2=vec
      end
      counter2=counter2+1
      
      if counter2>2*nblock | gap then
	//améliorer la borne inf!?
	bouclalg=%t
	disp('Algebraic loop detected in ordo3')
      end
    end
  end
endfunction

function [bouclalg,vec,primary,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,..
	  dep_uptr,corinv,blk_duplicated,clkptr,cliptr,critev]=ini_ordo(blk,port,..
          clkconnect,connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,corinv,clkptr,..
          cliptr,critev)

  bouclalg=%f
  nblock=size(typ_l)
  vec=-ones(nblock)
  //on cherche les enfants directs: primary
  primary=get_childrenport([],blk,port)
  vec(primary(:,1))=1
  //on enlève blk de primary (cas des clk)
  f1=find(primary(:,1)==blk)
  n_f1=size(f1,2)
  if n_f1>0 then
    for i1=1:n_f1
      if primary(f1(i1),2)==port then
	primary(f1(i1),:)=[]
      end
    end
  end
  n_p=size(primary,1)
  if n_p>0 then
    //on regarde s'il y a plus d'un bloc logique dans primary
    logic=0
    for i=1:n_p
      if typ_l(primary(i)) then
	logic=logic+1
      end
    end
    if logic<2 then
      [bouclalg,vec,primary]=ordo2(blk,port,clkconnect,connectmat,..
                                   primary,dep_t,dep_u,dep_uptr)
    else
      [bouclalg,vec,primary,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,..
       corinv,blk_duplicated,clkptr,cliptr,critev]=ordo1(primary,blk,port,clkconnect,..
		  connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)
    end
  end
  primary=get_childrenport([],blk,port)
endfunction

//On cherche les blocs à dupliquer qui empêche l'ordonnancement du 1er niveau
//le vec de sortie nous permet de l'ordancer
function [bouclalg,vec,primary,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,..
          dep_uptr,corinv,blk_duplicated,clkptr,cliptr,critev]=ordo1(primary,blk,port,..
	  clkconnect,connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,corinv,clkptr,..
          cliptr,critev)

  //on initialise les tests d'arret
  counter1=0
  counter2=0
  oldvec1=[]
  oldvec2=[]
  blk_duplicated=[]
  done=%f
  pointfixe=%f
  fromfixe=%f
  bouclalg=%f
  n_clkco=size(clkconnect,1);
  nblock=size(typ_l,1);
  n_cmat=size(connectmat,1);

  //initialisation de vec
  //on initialise vec à -1
  vec=-ones(nblock,1)
  //on cherche tous les enfants de clk
  all=get_allchildrenport(blk,port)
  //on met tous les enfants de clk à 0
  vec(all(:,1))=0
  //on met les enfants à 1
  vec(primary(:,1))=1

  while ~pointfixe & ~bouclalg
    while ~fromfixe & ~bouclalg & ~find_gap(vec(primary(:,1)))
      for i=1:n_p
       	w0=primary(i,:)
        if typ_l(primary(i,1)) then //RN	
	  w=get_allchildren2port(primary(i,1))
        else
	  w=[]
        end
	//on enleve les blocs du primary qui se trouve dans w
	n_w=size(w,1)
	if n_w>0 then
	  del1=[]
	  for k1=1:n_p
	    f=find(primary(k1,1)==w(:,1))
	    del1=[del1,f]
	  end
	  w(del1',:)=[]
	end
	w=[w0;w]
        //wu=unique(w(find(dep_ut(w(:,1),1)),1))

	wu=[];
	for i=1:size(w,1)
	  indport=find(dep_u(dep_uptr(w(i,1)):dep_uptr(w(i,1)+1)-1));
	  if (indport ~= []) then
	    wu=[wu;w(i,1)*ones(size(indport','*'),1) indport'];
          end
	end

        //on cherche d'où vient l'entrée des blocs: g
        if (wu ~=[]) then
	  //ind=dsearch(connectmat(:,3:4),wu,'d')
	  //h=connectmat(find(ind),1)

	  h=[];
  	  for i=1:size(wu,1)
            f=find(connectmat(:,3)==wu(i,1) & connectmat(:,4)==wu(i,2))'
            h=[h;connectmat(f,1:2)]
          end

	  h=h(find(vec(h(:,1))>-1),1)
	  if h ~= [] then
	    vec(w(:,1))=vec(w(:,1))-1
	    vec(w(:,1))=max([vec(w(:,1));vec(h(:,1))])+1
	  else  //RN
	    vec(w(:,1))=max(vec(w(:,1))) //RN
	  end           
        else
	  vec(w(:,1))=max(vec(w(:,1)))
        end
      end

      if vec==oldvec2 then
	fromfixe=%t
      else
	oldvec2=vec
      end
      counter2=counter2+1
      if counter2>2*nblock then
	//améliorer la borne inf!?
	bouclalg=%t
	disp('boucle algébrique détectée dans ordo1')
	disp('le vec ne converge pas...')
      end
    end

    gap=find_gap(vec(primary(:,1)))

    if (gap | oldvec1==vec | counter1>=n_p) & ~bouclalg then
      children=get_allchildrenport(blk,port)
      n_c=size(children,1)
      for i=1:n_c
	if typ_l(children(i,1)) then
	  w=get_children2port([],children(i,1))
	  dupl=find(vec(w(:,1))==max(vec(w(:,1))))
	  n_w=size(w,1)
	  if size(dupl,2)<n_w then
	    //si tous les enfants d'un même bloc n'ont pas le même n°, on duplique le parent
	    blk0=children(i,1)
	    port0=children(i,2)
	    blk1=w(dupl(1),1)
	    port1=w(dupl(1),2)
	    //rq: blk1 peut être un vecteur
	    if show_comment then
	      disp('duplique le bloc '+string(blk0)+' qui active le bloc '+string(blk1))
	      disp('trouvé au bout de la '+string(counter1+1)+ 'ème boucle')
	    end
	    [typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,..
             cliptr,critev]=duplique3(blk0,blk1,port1,clkconnect,connectmat,bllst,typ_l,..
             dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev);

	    n_clkco=size(clkconnect,1);
	    nblock=size(typ_l,1);
	    blk_duplicated=[blk_duplicated;nblock];
	    n_cmat=size(connectmat,1);
	    vec(nblock)=max(vec(w(:,1)))
	    vec(blk0)=max(vec(get_allchildren2(blk0)))
	    primary=[primary;nblock,port0]
	    n_p=n_p+1
	    done=%t
	  end
	end
      end
    end
    if (~done & oldvec1==vec) then
      pointfixe=%t
    end
    done=%f
    fromfixe=%f
    oldvec1=vec
    counter1=counter1+1
    counter2=0
  end //fin du while
  if gap then
    bouclalg=%t
    disp('Algebraic loop detected in ordo1')
  end  
endfunction

//On cherche les blocs à dupliquer qui empêche l'ordonnancement du 1er niveau
//le vec de sortie nous permet de l'ordancer détermine s'il existe un trou dans le vec
//exemple: vec=[1;3;5;4] il manque un 2
function [gap]=find_gap(vec)
  gap=%f
  vec_sort=gsort(vec,'g','i')
  n_vs=size(vec_sort,1)
  i=1
  if n_vs>1 then
    while ~gap & i<n_vs
      if vec_sort(i+1)-vec_sort(i)>1 then
	gap=%t;
      end
      i=i+1
    end
  end
endfunction

//on duplique le bloc blk0:
//on remplace le lien entre blk0 et blk1 par un lien entre le nouveau bloc et blk1
//le nouveau reçoit la même activation que blk0
function [typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,..
          cliptr,critev]=duplique3(blk0,blk1,port1,clkconnect,connectmat,bllst,typ_l,..
          dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)

  //blk0: bloc à dupliquer
  //blk1: bloc qui est activé par blk
  n_clkco=size(clkconnect,1);
  nblock=size(typ_l,1);
  n_cmat=size(connectmat,1);
  
  // ligne de clkconnect contenant le lien entre blk0 et blk1
  f=find(clkconnect(:,3)==blk1)
  n_f=size(f,2)
  for k=1:n_f
    if clkconnect(f(k),1)==blk0 & clkconnect(f(k),4)==port1 then
      clkconnect(f(k),1)=nblock+1
    end
  end
  
  f=find(clkconnect(:,3)==blk0)
  n_f=size(f,2)
  for k=1:n_f
    clkconnect(n_clkco+1,:)=clkconnect(f(k),:)
    clkconnect(n_clkco+1,3)=nblock+1
    n_clkco=n_clkco+1
  end
  
  f=find(connectmat(:,3)==blk0)
  n_f=size(f,2)
  for k=1:n_f
    connectmat(n_cmat+1,:)=connectmat(f(k),:)
    connectmat(n_cmat+1,3)=nblock+1
    n_cmat=n_cmat+1
  end
  
  critev=[critev;critev(clkptr(blk0):clkptr(blk0+1)-1)]
  bllst(nblock+1)=bllst(blk0)
  typ_l(nblock+1)=%t
  
  dep_t(nblock+1)=dep_t(blk0)
  dep_u=[dep_u;dep_u(dep_uptr(blk0))]
  dep_uptr($+1)=dep_uptr($)+1

  corinv(nblock+1)=corinv(blk0)
  clkptr(nblock+2)=clkptr(nblock+1)+clkptr(blk0+1)-clkptr(blk0)
  cliptr(nblock+2)=cliptr(nblock+1)+cliptr(blk0+1)-cliptr(blk0)
endfunction

//donne l'ordre de compilation des blocs du primary
//convertit le numéro de ligne de vecordo en numéro de bloc
function ordoclk=ordo_vecport(primary,vecordo)
  ordoclk=[]
  while vecordo~=[]
    minim=find(min(vecordo)==vecordo)
    n_m=size(minim,1)
    for i=1:n_m
      ordoclk=[ordoclk;primary(minim(i),:)]
      vecordo(minim(i))=[]
      primary(minim(i),:)=[]
    end
  end
endfunction

//s'il existe un "+"
function [vec_plus]=get_plus()
  vec_plus=[]
  vec_plus1=clkconnect(:,3)
  i=1
  while i<=size(vec_plus1,1)
    f=find(vec_plus1(i)==vec_plus1)
    if size(f,2)>1 then
      vec_plus=[vec_plus;vec_plus1(i)]
      vec_plus1(f)=[]
    else
      i=i+1
    end
  end
  vec_plus=gsort(vec_plus,'g','i')
endfunction

//mise à jour de vec_plus après modifications de liens
function [vec_plus]=update_vec_plus(vec_plus,blk)
  del=[]
  n_b=size(blk,1)
  for i=1:n_b
    f=find(clkconnect(:,3)==blk(i))
    if size(f,2)<2 then
      g=find(blk(i)==vec_plus)
      vec_plus(g')=[]
    else
      if find(vec_plus==blk(i))==[]
	vec_plus=[vec_plus;blk(i)]
      end
    end
  end
  vec_plus=gsort(vec_plus,'g','i')
endfunction

function [vec_plus]=get_plus2()
  nblock=size(typ_l,1)
  vec_plus=[]
  nblock=size(typ_l,1)
  for i=1:nblock
    f=find(clkconnect(:,3)==i)
    if size(f,2)>1 then
      n_in=cliptr(i+1)-cliptr(i)
      for j=1:n_in
	g=find(clkconnect(f,4)==j)
	if size(g,2)>1 then
	  h=find(i==vec_plus(:,1))
	  if h==[] | (h~=[] & vec_plus(h,2)~=j) then
	    vec_plus=[vec_plus;i,j]
	  end
	end
      end
    end
  end
endfunction

function [ordclk,ordptr,cord,ordoclk,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,..
          dep_uptr,corinv,clkptr,cliptr,critev,ok]=paksazi(typ_l,clkconnect,..
          connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev)

  ok=%t
  vec_clk=get_clocks(clkconnect,clkptr);
  vec_clk=gsort(vec_clk,'lr','i')
  vec_clk0=vec_clk(:,1)
  //suppression des liens inutiles
  [vec_plus]=get_plus2()
  if show_comment then
    disp('avant del_inutile')
  end
  [clkconnect,amaj]=find_del_inutile(clkconnect,vec_plus,typ_l)
  //on duplique les if du primary activés 2 fois, ayant pour ancetre une horloge commune
  done=%t
  while done
    [done,blk_duplicated,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,..
     corinv,clkptr,cliptr,critev,amaj]=if_double_activated(vec_clk0,typ_l,clkconnect,..
     connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev);
    if (blk_duplicated ~=[]) then
      vec_clk0=set_primary_clk(vec_clk0,blk_duplicated,vec_clk0($))
    end
  end
  if show_pause then 
    disp('fin de duplique les if activés 2 fois')
    pause
  end
  
  //On fait une boucle while, au cas où des doubles activations de bloc if seraient 
  //créées lors de la duplication

  //existe le temps continu?
  temps_continu=%f
  if vec_clk0(1)==0 then
    temps_continu=%t
  end
  
  [vec_plus]=get_plus()
  
  if show_pause & vec_plus~=[] then 
    disp('il existe un +')
    pause
  end
  
  if vec_plus~=[] then
    mapjplus=[]
    duplicated=%t
    counter=0
    [allpar,allparptr]=ini_allpar(clkconnect,vec_plus)
    while duplicated
      duplicated=%f
      counter=counter+1
      i=1
      while i<=size(vec_clk0,1)
	if vec_clk0(i)==0 then
	  [done,blk_duplicated,primary,wire,typ_l,clkconnect,connectmat,bllst,dep_t,..
           dep_u,dep_uptr,corinv,clkptr,cliptr,critev,vec_plus,allpar,allparptr,amaj,..
           oldvec,majplus]=get_wire2([],0,0,typ_l,clkconnect,connectmat,bllst,dep_t,..
           dep_u,dep_uptr,corinv,clkptr,cliptr,critev,vec_plus,allpar,allparptr);
	  if done==%t then
	    if blk_duplicated~=[] then
	      vec_clk0=set_primary_clk(vec_clk0,blk_duplicated,vec_clk0($))
	      [clkconnect]=cleanup(clkconnect)
	    else
	      [clkconnect]=cleanup(clkconnect)
	    end
	    vec_plus=update_vec_plus(vec_plus,majplus)
	    [allpar,allparptr]=maj_allpar(allpar,allparptr,clkconnect,oldvec,vec_plus,amaj)
	    duplicated=%t
	  end
	  vec_clk0=set_primary_clk(vec_clk0,primary,vec_clk0(i))
	else
	  n_out=clkptr(vec_clk0(i)+1)-clkptr(vec_clk0(i))
	  if n_out>0 then
	    j=1
	    while j<=n_out
	      //parcours des wire avec duplication éventuelle d'un if au primary
	      
	      [done,blk_duplicated,primary,wire,typ_l,clkconnect,connectmat,bllst,..
	       dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev,vec_plus,allpar,..
               allparptr,amaj,oldvec,majplus]=get_wire2([],vec_clk0(i),j,typ_l,..
               clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,corinv,clkptr,..
               cliptr,critev,vec_plus,allpar,allparptr);
	      
	      if done==%t then
		if blk_duplicated~=[] then
		  vec_clk0=set_primary_clk(vec_clk0,blk_duplicated,vec_clk0($))
		  [clkconnect]=cleanup(clkconnect)
		else
		  [clkconnect]=cleanup(clkconnect)
		end
		vec_plus=update_vec_plus(vec_plus,majplus)
		[allpar,allparptr]=maj_allpar(allpar,allparptr,clkconnect,..
					      oldvec,vec_plus,amaj)
		duplicated=%t
	      end
	      vec_clk0=set_primary_clk(vec_clk0,primary,vec_clk0(i))
	      if ~done then
		j=j+1
	      end
	    end
	  end
	end
	if ~done then
	  i=i+1
	end
      end
    end
  end

  if show_pause then 
    disp('fin de wire')
    pause
  end

  //On cherche les blocs à dupliquer et on ordonnance le 1er niveau
  nblock=size(typ_l,1)
  ordoclk=vec_clk
  if temps_continu then
    ordoclk(1,:)=[]
  end

  ordptr=1;
  ordclk=[];
  blocs_traites=[]
  cord=[];
  blk_duplicated=[]
  k=1
  if temps_continu then
    [bouclalg,vec,primary,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,..
     corinv,blk_duplicated,clkptr,cliptr,critev]=ini_ordo(0,0,clkconnect,connectmat,..
     bllst,typ_l,dep_t,dep_u,dep_uptr,corinv,clkptr,cliptr,critev);
    if bouclalg then
      message('Algebrique  loop detected; cannot be compiled.');
      ok=%f
      disp('activation traitée: bloc 0 - port0')
      return,
    end
    if blk_duplicated~=[] then
      //on met les blocs dupliqués dans l'ordre parent-enfants
      blk_duplicated=sort(blk_duplicated)
      blk_dupl=[]
      //on les ajoute à la liste des blocs à traiter
      n_bd=size(blk_duplicated,1)
      for k1=1:n_bd
	n_in=cliptr(blk_duplicated(k1)+1)-cliptr(blk_duplicated(k1))
	for k2=1:n_in
	  blk_dupl=[blk_dupl;blk_duplicated(k1),k2]
	end
      end
      ordoclk0=set_primary_clkport(ordoclk,blk_duplicated,size(ordoclk,1))
      duplicated=%t
    else
      duplicated=%f
    end
    primary=discardprimary(primary)
    primary=gsort(primary,'lr','i')
    vecordo=vec(primary(:,1))
    ordoclk0=ordo_vecport(primary,vecordo)
    cord=ordoclk0;
//    ordoclkcont=ordoclk0
    ordoclk=[ordoclk0;ordoclk]
    ordoclk=gsort(ordoclk,'lr','i')
  end
  
  while k<=size(ordoclk,1) 
    n_out=clkptr(ordoclk(k,1)+1)-clkptr(ordoclk(k,1))
    if n_out>0 then
      for j=1:n_out
	[bouclalg,vec,primary,typ_l,clkconnect,connectmat,bllst,dep_t,dep_u,dep_uptr,..
         corinv,blk_duplicated,clkptr,cliptr,critev]=ini_ordo(ordoclk(k,1),j,..
         clkconnect,connectmat,bllst,typ_l,dep_t,dep_u,dep_uptr,corinv,clkptr,..
         cliptr,critev);
	if bouclalg then
	  message('Algebrique  loop detected; cannot be compiled.');
	  ok=%f
	  disp('activation traitée: bloc '+string(ordoclk(k,1))+' - port '+string(j))
	  return,
	end
	if blk_duplicated~=[] then
	  //on met les blocs dupliqués dans l'ordre parent-enfants
	  blk_duplicated=sort(blk_duplicated)
	  blk_dupl=[]
	  //on les ajoute à la liste des blocs à traiter
	  n_bd=size(blk_duplicated,1)
	  for k1=1:n_bd
	    n_in=cliptr(blk_duplicated(k1)+1)-cliptr(blk_duplicated(k1))
	    for k2=1:n_in
	      blk_dupl=[blk_dupl;blk_duplicated(k1),k2]
	    end
	  end
	  ordoclk=set_primary_clkport(ordoclk,blk_dupl,size(ordoclk,1))
	  duplicated=%t
	else
	  duplicated=%f
	end
	primary=discardprimary(primary)
	primary=gsort(primary,'lr','i')
	vecordo=vec(primary(:,1))
	ordoclk0=ordo_vecport(primary,vecordo) 
       	
	[ordptr,ordclk,blocs_traites]=set_ordclk(ordclk,ordoclk0,ordoclk(k),j,..
	                                         ordptr,blocs_traites)
	ordoclk=set_primary_clkport(ordoclk,ordoclk0,k)
      end
    end
    k=k+1
  end
  
  if show_pause then
    disp('fin de paksazi')
    pause
  end
  
endfunction

function primary=discardprimary(primary)
// discard
  mma=maxi(primary(:,2))+1
  con=mma*primary(:,1)+primary(:,2)
  [junk,ind]=sort(-con);con=-junk
  primary=primary(ind,:)
  // discard duplicate calls to the same block port
  if size(con,'*')>=2 then
    primary(find(con(2:$)-con(1:$-1)==0),:)=[]
  end
  // group calls to different ports of the same block.
  primary=[primary(:,1),2^(primary(:,2)-ones(primary(:,2)))]
  primary=int(primary)
  con=primary(:,1) 
  clkconnectjj=[]     
  if size(con,'*')>=2 then 
    iini=[find(con(2:$)-con(1:$-1)<>0),size(primary,1)]
  else
    iini=1
  end
  for ii=iini
    clkconnectjj=[clkconnectjj;[primary(ii,1),..
		    mysum(primary(find(primary(:,1)==primary(ii,1)),2))]]
  end
  primary=clkconnectjj
endfunction

function [ordclk,iord,oord,zord,typ_z,ok]=scheduler(inpptr,outptr,clkptr,execlk_cons,..
	  outoin,outoinptr,evoutoin,evoutoinptr,typ_z,typ_x,typ_l,bexe,boptr,..
	  blnk,blptr,ordclk,ordptr,cord,dep_t,dep_u,dep_uptr);

  nblk=size(typ_x,1)
  //compute iord
  if execlk_cons<>[] then
    vec=-ones(1,nblk)
    no_tu_dep=execlk_cons(:,1)
    wec=zeros(1,nblk)
    wec(no_tu_dep')=execlk_cons(:,2)'
    vec(no_tu_dep)=0*no_tu_dep'
    [r,ok]=newc_tree2(vec,outoin,outoinptr,dep_u,dep_uptr)

    iord=[r,wec(r)']
  else
    iord=[]
  end
  //
  if ~ok then 
    message('Algebrique  loop detected; cannot be compiled.');
    iord=[],oord=[],zord=[],critev=[]
    return,
  end
  
  n=size(cord,1)
  vec=-ones(1,nblk);
  vec(cord(:,1))=0;
  typp=zeros(typ_l);typp(typ_l)=1

  ext_cord1=cord;
  j=1
  while %t
    ii=ext_cord1(j,1)
    if typ_l(ii)
      for i=[clkptr(ii):clkptr(ii+1)-1]
	ext_cord1=[ext_cord1;ordclk([ordptr(i):ordptr(i+1)-1],:)];
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
  //  if typ_l(i) then typ_z(i)=clkptr(i+1)-clkptr(i)-1;end
  //end  // adding zero crossing surfaces to cont. time synchros
  
  //a supprimer

  [ext_cord_old,ok]=newc_tree3(vec,dep_u,dep_uptr,typp);
 
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
  maX=max([ext_cord1(:,2);ordclk(:,2)])+1;
  cordX=ext_cord1(:,1)*maX+ext_cord1(:,2);

  // 1: important; 0:non
  //n=clkptr(nblk+1)-1 //nb d'evenement
  n=size(ordptr,'*')-1 //nb d'evenement
  
  //a priori tous les evenemets sont non-importants
  //critev=zeros(n,1)
  for i=1:n
    for hh=ordptr(i):ordptr(i+1)-1
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
	  message('algebrique loop detected');ok=%f;ord=[];return;
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

function [clkconnectj_cons]=discard(clkptr,cliptr,clkconnect,exe_cons)

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
endfunction

function a=mysum(b)
  if b<>[] then a=sum(b), else a=[], end
endfunction

function [lnkptr,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,xptr,zptr,typ_mod,rpptr,..
          ipptr,xc0,xcd0,xd0,rpar,ipar,typ_z,typ_x,typ_m,funs,funtyp,initexe,labels,..
	  bexe,boptr,blnk,blptr,ok]=extract_info(bllst,connectmat,clkconnect,typ_l)
  ok=%t
  nbl=length(bllst)
  clkptr=zeros(nbl+1,1);clkptr(1)=1
  cliptr=clkptr;inpptr=cliptr;outptr=inpptr;
  xptr=1;zptr=1;
  rpptr=clkptr;ipptr=clkptr;
  //
  xc0=[];xcd0=[];xd0=[];rpar=[];ipar=[];

  fff=ones(nbl,1)==1
  typ_z=zeros(nbl,1);typ_x=fff;typ_m=fff;typ_mod=zeros(nbl,1);

  initexe=[];
  funs=list();
  funtyp=zeros(typ_z)
  labels=[]
  
  [ok,bllst]=adjust_inout(bllst,connectmat)
  
  // placed here to make sure nzcross and nmode correctly updated
  if ~ok then 
    lnkptr=[],inplnk=[],outlnk=[],clkptr=[],cliptr=[],inpptr=[],outptr=[],..
	   xptr=[],zptr=[],rpptr=[],ipptr=[],xc0=[],xcd0=[],xd0=[],rpar=[],ipar=[],..
	   typ_z=[],typ_x=[],typ_m=[],funs=[],funtyp=[],initexe=[],labels=[],..
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
    typ_x(i)=ll.state<>[]|ll.blocktype=='x' // some blocks like delay
                                            // need to be in oord even
                                            // without state
				
    typ_m(i)=ll.blocktype=='m'
    //
    if ~typ_l(i)&ll.evtout<>[] then  
      ll11=ll.firing
      prt=find(ll11>=zeros(ll11))
      nprt=prod(size(prt))
      initexe=[initexe;[i*ones(nprt,1),matrix(prt,nprt,1),matrix(ll11(prt),nprt,1)]];
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
 
  blptr=1;
  blnk=[];

  for i=1:nbl
    r=connectmat(find(connectmat(:,1)==i),3:4);
    blnk=[blnk;r];
    blptr=[blptr;blptr($)+size(r,1)];
  end  
  //

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
  for j=unco
    m=maxi(find(outptr<=j))
    n=j-outptr(m)+1
    nm=bllst(m).out(n)
    if nm<1 then 
      under_connection(corinv(m),n,nm,-1,0,0),ok=%f,return,
    end
    lnkptr=[lnkptr;lnkptr($)+nm]
    outlnk(j)=maxi(outlnk)+1
  end
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

function [ord,ok]=tree2(vec,outoin,outoinptr,dep_ut)
//compute blocks execution tree
  ok=%t;

  nb=size(vec,'*');
  for j=1:nb+2
    fini=%t
    for i=1:nb
      if vec(i)==j-1 then 
	if j==nb+2 then 
	  message('algebrique loop detected');ok=%f;ord=[];return;
	end
	for k=outoinptr(i):outoinptr(i+1)-1
	  ii=outoin(k,1);
	  //indport=find(dep_u(dep_uptr(ii):dep_uptr(ii+1)-1)==1);
	  //if (indport ~=[] &vec(ii)>-1) then
	  if (dep_ut(ii) &vec(ii)>-1) then
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
					typ_r,typ_l,outoin,outoinptr,tblock,typ_cons,clkptr)

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
  [r,ok]=newc_tree4(vec,outoin,outoinptr,typ_r)
  
  exe_cons=[exe_cons;r]

  if show_trace then disp('c_pass4445:'+string(timer())),end

  [clkr,clkc]=size(clkconnect);
  mm=max(clkconnect(:,2))+1;
  cll=clkconnect(:,1)*mm+clkconnect(:,2);
  [cll,ind]=sort(-cll);
  clkconnect=clkconnect(ind,:);
  if cll<>[] then mcll=max(-cll)+1, else mcll=1;end
  cll=[-1;-cll;mcll];
  ii=find(cll(2:$)-cll(1:$-1)<>0)
  
  for k=1:size(ii,'*')-1
    oo=[ii(k):ii(k+1)-1]
    vec=-ones(1,nblk);
    vec(clkconnect(oo,3))=0
    [r,ok]=newc_tree4(vec,outoin,outoinptr,typ_r)
    m=size(r,1)
    r=[clkconnect(ii(k),1)*ones(m,1),clkconnect(ii(k),2)*ones(m,1),r]
    clkconnect=[clkconnect;r]
  end  
  // temporary fix to take care of conditional blocks inherting from
  // constants: make these blocks always active
  
  ind=setdiff(find(typ_l),clkconnect(:,3))
  ind=ind(:)
  for k=ind'
    clkconnect=[clkconnect;[all_out,ones(all_out)*[k,0;0,0]]]
  end
  // end of  temoprary fix 

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

function [bllst,inplnk,outlnk,clkptr,cliptr,inpptr,outptr,dep_u,dep_uptr,dep_t,..
          typ_l,typ_r,typ_m,tblock,typ_cons,typ_zx,ok]=mini_extract_info(bllst,..
          connectmat,clkconnect)
  ok=%t
  nbl=length(bllst)
  clkptr=zeros(nbl+1,1);clkptr(1)=1
  cliptr=clkptr;inpptr=cliptr;outptr=inpptr;
  fff=ones(nbl,1)==1
  typ_l=fff;typ_r=fff;typ_cons=fff;typ_m=fff;typ_zx=fff;
  dep_t=ones(nbl,1)==1;
  dep_u=[];dep_uptr=1;
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
      //ok=%f
      
      typ_r(i)=~ll.dep_ut($)
      tblock(i)=ll.dep_ut($)
      //if block depends on time but has no event input port
      if ~ll.dep_ut($) then 
	//inherits from the inputs
	cinpnum=ones(inpnum)
	bllst(i).evtin=cinpnum  //XXXXXXXXXXXXXXXXXXXXX
      end
      //
    else
      tblock(i)=ll.dep_ut($);typ_r(i)=%f
    end
    inpptr(i+1)=inpptr(i)+size(inpnum,'*')
    outptr(i+1)=outptr(i)+size(outnum,'*')
    cliptr(i+1)=cliptr(i)+size(cinpnum,'*')
    clkptr(i+1)=clkptr(i)+size(coutnum,'*')
    //

    typ_l(i)=ll.blocktype=='l';typ_m(i)=ll.blocktype=='m';
    typ_cons(i)=cinpnum==[]&inpnum==[]&~ll.dep_ut($)
//test of the dep_ut size
    sizenin=size(ll.in,'*');
    if (size(ll.dep_ut,'*') <> 2) then
      if ( size(ll.dep_ut(1:$-1),'*') <> sizenin) then
	x_message(['the dep_ut size of the '+ll.sim(1)+' block is not correct';
	           'it should be colonn of size '+string(sizenin+1)])     
	ok=%f;   
      end
    end

    dep_t(i)=ll.dep_ut($);  
    
    if (size(ll.dep_ut,'*') == 2) then
      if (sizenin == 1) then
	dep_u($+1)=ll.dep_ut(1);
	dep_uptr($+1)=dep_uptr($)+1;
      elseif (sizenin > 1) then
	dep_u=[dep_u;ones(sizenin,1)==1*ll.dep_ut(1)];
	dep_uptr($+1)=dep_uptr($)+sizenin;
      else
	dep_uptr($+1)=dep_uptr($);
      end
    else  
      dep_u_i=ll.dep_ut(1:$-1);
      dep_u=[dep_u;dep_u_i(:)];
      dep_uptr($+1)=dep_uptr($)+sizenin;
    end
  
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
  for unco=find(outlnk==0);
    outlnk(unco)=maxi(outlnk)+1  
  end
endfunction

function [evoutoin,evoutoinptr]=synch_clkconnect(typ_l,clkconnect)
  nblk=size(typ_l,'*')
  evoutoin=[];evoutoinptr=1
  for i=1:nblk
    if typ_l(i) then
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

//function mat=cleanup1(mat)
//  mm=maxi(mat)+1
//  cc=mat(:,1)*mm
//  [cc1,ind]=sort(-cc)
//  mat=mat(ind,:)
//  ind=find(cc1(2:$)-cc1(1:$-1)==0)
//  mat(ind,:)=[]
//endfunction

function vec=intersection(vec1,vec2)
  vec=[]
  for i=1:size(vec1,1)
    if find(vec1(i)==vec2)~=[] then
      vec=[vec;vec1(i)]
    end
  end
endfunction

function  [r,ok]=newc_tree2(vec,outoin,outoinptr,dep_u,dep_uptr)
  dd=zeros(dep_u);dd(dep_u)=1;
  [r,ok2]=ctree2(vec,outoin,outoinptr,dd,dep_uptr)
  ok=ok2==1
endfunction

function  [r,ok]=new_tree2(vec,outoin,outoinptr,dep_u,dep_uptr)
  dd=zeros(dep_u);dd(dep_u)=1;
  [r,ok2]=sci_tree2(vec,outoin,outoinptr,dd)
  ok=ok2==1
endfunction

function  [r,ok]=new_tree3(vec,dep_ut,typ_l)
  dd=zeros(dep_ut);dd(dep_ut)=1;
  [r2,ok2]=sci_tree3(vec,dd,typ_l,bexe,boptr,blnk,blptr)
  r=r2'
  ok=ok2==1
endfunction

function  [r,ok]=newc_tree3(vec,dep_u,dep_uptr,typ_l)
  dd=zeros(dep_u);dd(dep_u)=1;
  [r2,ok2]=ctree3(vec,dd,dep_uptr,typ_l,bexe,boptr,blnk,blptr)
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

function  [r,ok]=newc_tree4(vec,outoin,outoinptr,typ_r)
  nd=zeros(size(vec,'*'),(max(outoin(:,2))+1));
  ddd=zeros(typ_r);ddd(typ_r)=1; 
  [r1,r2]=ctree4(vec,outoin,outoinptr,nd,ddd)  
  r=[r1',r2']
  ok=%t
endfunction

function [critev]=critical_events(connectmat,clkconnect,dep_t,typ_r,..
				  typ_l,typ_zx,outoin,outoinptr,clkptr)

  typ_c=typ_l<>typ_l;
  typ_r=typ_r|dep_t
  
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
      [r,ok]=newc_tree4(vec,outoin,outoinptr,typ_r)
      
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

