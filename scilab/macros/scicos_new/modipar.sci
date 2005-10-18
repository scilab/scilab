function [%state0,state,sim]=modipar(newparameters,%state0,state,sim,scs_m,cor)
//store modified parameters in compiled structure state,sim
//newparameters gives modified blocks numbers in original structure scs_m
//cor is the correspondance table from original structure to compiled one
// Copyright INRIA
  xptr=sim.xptr
  zptr=sim.zptr
  rpptr=sim.rpptr
  ipptr=sim.ipptr
  ipar=sim.ipar
  rpar=sim.rpar
  ztyp=sim.ztyp
  labels=sim.labels
  st=state.x
  dst=state.z
  st0=%state0.x
  dst0=%state0.z

  Impl=%f
  if xptr($)-1 < size(st,'*') then
    std=st($/2+1:$)
    st=st(1:$/2)
    std0=st0($/2+1:$)
    st0=st0(1:$/2)
    Impl=%t
  end

  nb=prod(size(rpptr))-1


  for k=newparameters
    if prod(size(k))==1 then //parameter of a simple block
      kc=cor(k) //index of modified block in compiled structure
      o=scs_m.objs(k)
    else
      kc=get_tree_elt(cor,k); 
      o=scs_m(get_subobj_path(k))
    end
    
    if is_modelica_block(o) then
      parameters=o.model.equations.parameters
      rpark=[];for p=parameters(2),rpark=[rpark;p(:)];end
      ipark=ipar(ipptr(kc):ipptr(kc+1)-1)
      corinvm=%cpr.corinv(kc);
      J=[];
      for jj=1:size(corinvm)
	if and(corinvm(jj)==k)  then J=jj;break,end
      end
      nek=size(rpark,'*')-(ipark(J+1)-ipark(J))
      if nek<>0 then
	warning('variying number of parameter for modelica block is not yet implemented')
      end
      rpar(rpptr(kc)+ipark(J)-1:rpptr(kc)+ipark(J+1)-2)=rpark
    else
      statekd=[]
      om=o.model
      [fun,statek,dstatek,rpark,ipark]=(om.sim,om.state,om.dstate,om.rpar,om.ipar);
      if type(fun)==15 then
	if (fun(2)==3 | fun(2)==5 |  fun(2)==10005) then 
	  if rpark<>[] then rpark=var2vec(rpark); end
	  if dstatek<>[] then dstatek=var2vec(dstatek),end
	end
	if fun(2)>10000 then
	  statekd=statek($/2+1:$)
	  statek=statek(1:$/2)
	end
      end
      if kc>0 then
	//Change continuous state
	nek=prod(size(statek))-(xptr(kc+1)-xptr(kc))
	sel=xptr(kc+1):xptr($)-1
	if nek<>0&sel<>[] then
	  st(nek+sel)=st(sel)
	  st0(nek+sel)=st0(sel)
	  if Impl then
	    std(nek+sel)=std(sel)
	    std0(nek+sel)=std0(sel)
	  end
	end
	xptr(kc+1:$)=xptr(kc+1:$)+nek
	st(xptr(kc):xptr(kc+1)-1)=statek(:),
	st0(xptr(kc):xptr(kc+1)-1)=statek(:),
	if Impl then
	  if statekd==[] then statekd=0*statek,end
	  std(xptr(kc):xptr(kc+1)-1)=statekd(:),
	  std0(xptr(kc):xptr(kc+1)-1)=statekd(:),
	end
	//Change discrete  state
	nek=prod(size(dstatek))-(zptr(kc+1)-zptr(kc))
	sel=zptr(kc+1):zptr($)-1
	if nek<>0&sel<>[] then
	  dst(nek+sel)=dst(sel)
	  dst0(nek+sel)=dst0(sel)
	end
	zptr(kc+1:$)=zptr(kc+1:$)+nek
	dst(zptr(kc):zptr(kc+1)-1)=dstatek(:),
	dst0(zptr(kc):zptr(kc+1)-1)=dstatek(:),

	
	//Change real parameters
	nek=prod(size(rpark))-(rpptr(kc+1)-rpptr(kc))
	sel=rpptr(kc+1):rpptr($)-1
	if nek<>0&sel<>[] then
	  rpar(nek+sel)=rpar(sel)
	end
	rpptr(kc+1:$)=rpptr(kc+1:$)+nek
	rpar(rpptr(kc):rpptr(kc+1)-1)=rpark,

	//Change integer parameters
	if type(ipark)==1 then   //scifunc
	  nek=prod(size(ipark))-(ipptr(kc+1)-ipptr(kc))
	  sel=ipptr(kc+1):ipptr($)-1
	  if nek<>0&sel<>[] then
	    ipar(nek+sel)=ipar(sel)
	  end
	  ipptr(kc+1:$)=ipptr(kc+1:$)+nek
	  ipar(ipptr(kc):ipptr(kc+1)-1)=ipark,
	end
	
	//Change simulation routine
	if type(sim('funs')(kc))<>13 then   //scifunc
	  sim('funs')(kc)=fun(1);
	  if prod(size(fun))> 1 then 
	    sim('funtyp')(kc)=fun(2);
	  else 
	    sim('funtyp')(kc)==0;
	  end
	end
	//Change label
	labels(kc)=o.model.label
      end
    end
  end

  sim.xptr=xptr
  sim.zptr=zptr
  sim.rpar=rpar;
  sim.rpptr=rpptr;
  sim.ipar=ipar;
  sim.ipptr=ipptr
  sim.labels=labels

  if Impl then
    state.x=[st;std]
  else
    state.x=st
  end

  state.z=dst
  if Impl then
    %state0.x=[st0;std0]
  else
    %state0.x=st0
  end
  %state0.z=dst0

endfunction
