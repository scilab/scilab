//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [mod,scs_m]=do_label(%pt,scs_m)
// do_block - edit a block label
//**
//** This code must be upgrated to NEW graphics 
//** 
  mod = %f
  win = %win;
    K=find(Select(:,2)==curwin);
  if size(K,'*')>1 then
    messagebox("Only one block can be selected in current window for this operation.","modal")
    Cmenu=[];ok=%f;return
  end
  
  if K==[] then
    xc = %pt(1); yc=%pt(2); %pt=[]
    K  = getblock(scs_m,[xc;yc])
    if K==[] then Cmenu=[];ok=%f;return,end
  else
    K=Select(K,1)
  end
  
  o = scs_m.objs(K)
  // avoid error with links
  if typeof(o)<>'Block' then 
    messagebox("No label can be placed on Links.","modal")  
    return,
  end
  model = o.model
  lab = model.label
  [ok,lab] = scicos_getvalue('Give block label','label',list('str',1),lab)
  
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
