function [tree]=sci_sort(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab sort()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_sort()
// V.C.

// B = sort(A) or [B,IND] = sort(A) 
if rhs==1 then
  A = getrhs(tree)
  
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
  
  if or(A.vtype==[String,Unknown]) then
    tree.name="mtlb_sort"
  elseif A.vtype==Boolean then
    tree.name="gsort"
    if is_a_vector(A) then
      A = Funcall("bool2s",1,Rhs(A),list())
      tree.rhs=Rhs(A,"g","i")
    elseif not_a_vector(A) then
      A = Funcall("bool2s",1,Rhs(A),list())
      tree.rhs=Rhs(A,"r","i")
    else
      tree.name="mtlb_sort"
    end
  else
    tree.name="gsort"
    if is_a_vector(A) then
      tree.rhs=Rhs(A,"g","i")
    elseif not_a_vector(A) then
      tree.rhs=Rhs(A,"r","i")
    else
      tree.name="mtlb_sort"
    end
  end
  
  if lhs==2 then
    tree.lhs(2).dims=A.dims
    tree.lhs(2).type=Type(Double,Real)
  end
// B = sort(A,dim) or [B,IND] = sort(A,dim) 
elseif rhs==2
  [A,dim] = getrhs(tree)
  dim = convert2double(dim)
  tree.rhs=Rhs(A,dim)
  
  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type
  
  if size(A.dims)>2 then
    set_infos("Scilab sort() and gsort() do not work with multidimensional arrays !")
  else
  name="gsort"
    if typeof(dim)=="cste" then
      if dim.value==1 then
	dim=Cste("r")
      elseif dim.value==2 then
	dim=Cste("c")
      else
        name="mtlb_sort"
      end
    else
        name="mtlb_sort"
    end
    
    if or(A.vtype==[String,Unknown]) then
      name="mtlb_sort"
    elseif A.vtype==Boolean then
      A = Funcall("bool2s",1,Rhs(A),list())
      tree.rhs=Rhs(A,dim,"i")
    else
      tree.rhs=Rhs(A,dim,"i")
    end
    tree.name=name
    
    if lhs==2 then
      tree.lhs(2).dims=A.dims
      tree.lhs(2).type=Type(Double,Real)
    end
  end    
elseif rhs==3 
  [A,dim,txt] = getrhs(tree)
  dim = convert2double(dim)
  tree.rhs=Rhs(A,dim,txt)

  tree.lhs(1).dims=A.dims
  tree.lhs(1).type=A.type

  if size(A.dims)>2 then
    set_infos("Scilab sort() and gsort() do not work with multidimensional arrays !")
  else
    name="gsort"
    if typeof(dim)=="cste" then
      if dim.value==1 then
	dim=Cste("r")
      elseif dim.value==2 then
	dim=Cste("c")
      else 
        name="mtlb_sort"
      end
    else 
      name="mtlb_sort"
    end
    if or(A.vtype==[String,Unknown]) then
      name="mtlb_sort"
    elseif A.vtype==Boolean then
      A = Funcall("bool2s",1,Rhs(A),list())
      tree.rhs(1)=A
    end
    
    if txt.vtype==String & txt.value=="ascend"
      tree.rhs=Rhs(A,dim,"i")
    elseif txt.vtype==String & txt.value=="descend"
      tree.rhs=Rhs(A,dim,"d")
    else
      name="mtlb_sort" 
    end	
    tree.name=name
    
    if lhs==2 then
      tree.lhs(2).dims=A.dims
      tree.lhs(2).type=Type(Double,Real)
    end
  end      
end

endfunction
