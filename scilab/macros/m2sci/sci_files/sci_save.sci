function [tree]=sci_save(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab save()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_save()
// V.C.

set_infos(["Variables will be saved in a format that MATLAB v4 can open"],2);

tree.name="mtlb_save"

// If no rhs, save Scilab environnement
if rhs<=0 then
  tree.rhs=Rhs("scilab.mat")
  return
end

if rhs>1 then
  k=2
  while k<=size(tree.rhs)
    // Suppress -v4 option if exists
    if tree.rhs(k).value=="-v4" then
      no_equiv("Option -v4: ignored");
      tree.rhs(k)=null()
    // Suppress -v4 option if exists
    elseif tree.rhs(k).value=="-mat" then
      no_equiv("Option -mat: ignored");
      tree.rhs(k)=null()  
    // Suppress -append option if exists because not handled
    elseif tree.rhs(k).value=="-append" then
      no_equiv("Option -append: ignored");
      tree.rhs(k)=null()  
    else
      k=k+1
    end
  end
end

endfunction
