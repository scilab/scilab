function [%state0,state,sim]=modipar(newparameters,%state0,state,sim,scs_m,cor)
//store modified parameters in compiled structure state,sim
//newparameters gives modified blocks numbers in original structure scs_m
//cor is the correspondance table from original structure to compiled one
// Copyright INRIA
xptr=sim('xptr')
zptr=sim('zptr')
izptr=sim('izptr')
rpptr=sim('rpptr')
ipptr=sim('ipptr')
ipar=sim('ipar')
rpar=sim('rpar')
[st,dst]=state(['x','z'])
[st0,dst0]=%state0(['x','z'])
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
  if prod(size(k))==1 then //parameter of a sImple block
    statekd=[]
    [fun,statek,dstatek,rpark,ipark]=scs_m(k)(3)([1 6:9]);
    if type(fun)==15 then
      if fun(2)==3 then 
        rpark=var2vec(rpark),
        dstatek=var2vec(dstatek),
      elseif fun(2)>10000 then
	statekd=statek($/2+1:$)
	statek=statek(1:$/2)
      end
    end
    kc=cor(k) //index of modified block in compiled structure
  else //parameter of a super block
    kc=get_tree_elt(cor,k); //index of the modified sub_block in compiled structure
    nk=size(k,2)
    o=scs_m(get_subobj_path(k))
    statekd=[]
    [fun,statek,dstatek,rpark,ipark]=o(3)([1 6:9])
    if type(fun)==15 then
      if fun(2)==3 then 
        rpark=var2vec(rpark),
        dstatek=var2vec(dstatek),
      elseif fun(2)>10000 then
	statekd=statek($/2+1:$)
	statek=statek(1:$/2)
      end
    end
  end
  
  if kc>0 then
    //Change continuous state
    nek=prod(size(statek))-(xptr(kc+1)-xptr(kc))
    if nek<>0&kc<>nb then
      st(nek+(xptr(kc+1)-1:xptr($)-1))=st((xptr(kc+1)-1:xptr($)-1))
      st0(nek+(xptr(kc+1)-1:xptr($)-1))=st0((xptr(kc+1)-1:xptr($)-1))
      if Impl then
	std(nek+(xptr(kc+1)-1:xptr($)-1))=std((xptr(kc+1)-1:xptr($)-1))
	std0(nek+(xptr(kc+1)-1:xptr($)-1))=std0((xptr(kc+1)-1:xptr($)-1))
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
    if nek<>0&kc<>nb then
      dst(nek+(zptr(kc+1)-1:zptr($)-1))=dst((zptr(kc+1)-1:zptr($)-1))
      dst0(nek+(zptr(kc+1)-1:zptr($)-1))=dst0((zptr(kc+1)-1:zptr($)-1))
    end
    zptr(kc+1:$)=zptr(kc+1:$)+nek
    dst(zptr(kc):zptr(kc+1)-1)=dstatek(:),
    dst0(zptr(kc):zptr(kc+1)-1)=dstatek(:),

    //Change real parameters
    nek=prod(size(rpark))-(rpptr(kc+1)-rpptr(kc))
    if nek<>0&kc<>nb then
      rpar(nek+(rpptr(kc+1)-1:rpptr($)-1))=rpar((rpptr(kc+1)-1:rpptr($)-1))
    end
    rpptr(kc+1:$)=rpptr(kc+1:$)+nek
    rpar(rpptr(kc):rpptr(kc+1)-1)=rpark,

    //Change integer parameters
    if type(ipark)==1 then   //scifunc
      nek=prod(size(ipark))-(ipptr(kc+1)-ipptr(kc))
      if nek<>0&kc<>nb then
	ipar(nek+(ipptr(kc+1)-1:ipptr($)-1))=ipar((ipptr(kc+1)-1:ipptr($)-1))
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
  end
end


sim('xptr')=xptr
sim('zptr')=zptr
sim('izptr')=izptr

sim('rpar')=rpar;
sim('rpptr')=rpptr;
if  type(ipark)==1 then sim('ipar')=ipar;end  //scifunc
sim('ipptr')=ipptr

if Impl then
  state('x')=[st;std]
else
  state('x')=st
end

state('z')=dst
if Impl then
  %state0.x=[st0;std0]
else
  %state0('x')=st0
end
%state0('z')=dst0
endfunction
