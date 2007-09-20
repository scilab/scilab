function [mod,scs_m]=do_label(%pt,scs_m)
// do_block - edit a block label
// Copyright INRIA
//**
//** This code must be upgrated to NEW graphics 
//** 
  mod = %f
  win = %win;
  
  if Select==[] then
    xc = %pt(1); yc=%pt(2); %pt=[]
    K  = getblock(scs_m,[xc;yc])
    if K==[] then Cmenu=[];return,end
  else
    K=Select(:,1)';%pt=[]
    if size(K,'*')>1 | curwin<>Select(1,2) then
      message("Only one block can be selected in current window for this o"+...
	      "peration.")
      Cmenu=[];return
    end
  end
  
  o = scs_m.objs(K)
  model = o.model
  lab = model.label
  [ok,lab] = getvalue('Give block label','label',list('str',1),lab)
  
  //** Output 
  if ok then
    
    //** drawblock(o); //** delete the block XOR mode 
    
    lab = stripblanks(lab)
    
    if length(lab)==0 then lab=' ',end
    
    model.label = lab ;
    
    o.model = model ;
    
    scs_m.objs(K) = o ;
    
    mod = %t ; 
    
    //** drawblock(o); //** draw the update block 
  
  end
endfunction
