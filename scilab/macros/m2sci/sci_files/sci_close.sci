function [tree]=sci_close(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab close()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// Emulation function: mtlb_close()
// V.C.

global %graphicswindow

if tree.lhs(1).name<>"ans" then
  no_equiv(tree.lhs(1).name+" will be ignored at run time")
end

// close
if rhs==0 then
  if %graphicswindow==Handle then
    tree.name="delete"
    tree.rhs(1)=Funcall("gcf",1,list(),list())
  else
    // Nothing changed
  end
// close(h) - close('all') - close(name)
elseif rhs==1 then
  h=tree.rhs(1)
  if h.vtype==Handle then // close(h)
    tree.name="delete"
  elseif h.vtype==Double then // close(h)
    // Nothing changed
  elseif h.vtype==String then
    if typeof(h)=="cste" then
      if h.value=="all" then // close('all')
	tree.name="xdel"
	tree.rhs(1)=Funcall("winsid",1,list(),list())
      else // close(name)
	tree.name="mtlb_close"
      end
    else
      tree.name="mtlb_close"
    end
  else // Unknown type for h
    tree.name="mtlb_close"
  end
else // close('all','hidden')
  set_infos("All windows will be deleted",2);
  tree.name="xdel"
  tree.rhs(1)=Funcall("winsid",1,list(),list())
  tree.rhs(2)=null()
end
endfunction
