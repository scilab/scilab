function tree=sci_axis(tree)
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab axis()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

// Insert %v0=gca()
a=gettempvar()
insert(Equal(list(a),Funcall("gca",1,list(),list())))

if rhs>0 then
  for krhs=1:rhs
    if tree.rhs(krhs).vtype==Double then
      
      // axis([xmin xmax ymin ymax])
      // axis([xmin xmax ymin ymax zmin zmax])
      if or(tree.rhs(krhs).dims(2)==[4,6]) then
	mat=Funcall("matrix",1,list(tree.rhs(krhs),Cste(2),Cste(-1)),list())
	LHS=Operation("ins",list(a,Cste("data_bounds")),list())
	insert(Equal(list(LHS),mat))
	
      // axis([xmin xmax ymin ymax zmin zmax cmin cmax]) 
      elseif tree.rhs(krhs).dims(2)==8 then
	no_equiv(expression2code(tree))
	return
	
      // Unknown column number for tree.rhs(krhs)
      else
	tree.name="mtlb_axis"
      end
    elseif tree.rhs(krhs).vtype==String then
      
      // Option is a character string
      if typeof(tree.rhs(krhs))=="cste" then
	
	// axis auto
	if tree.rhs(krhs).value=="auto" then
	  LHS=Operation("ins",list(a,Cste("auto_scale")),list())
	  insert(Equal(list(LHS),Cste("on")))
	  
	  // axis manual
	elseif tree.rhs(krhs).value=="manual" then
	  LHS=Operation("ins",list(a,Cste("auto_scale")),list())
	  insert(Equal(list(LHS),Cste("off")))
	  
	  // axis tight
	elseif tree.rhs(krhs).value=="tight" then
	  LHS=Operation("ins",list(a,Cste("tight_limits")),list())
	  insert(Equal(list(LHS),Cste("on")))
	  
	  // axis fill
	elseif tree.rhs(krhs).value=="fill" then
	  no_equiv(expression2code(tree))
	  return
	  
	  // axis ij
	elseif tree.rhs(krhs).value=="ij" then
	  LHS=Operation("ins",list(a,Cste("rotation_angles")),list())
	  mat=Operation("cc",list(Cste(180),Cste(270)),list())
	  insert(Equal(list(LHS),mat))
	  
	  // axis xy
	elseif tree.rhs(krhs).value=="xy" then
	  LHS=Operation("ins",list(a,Cste("rotation_angles")),list())
	  mat=Operation("cc",list(Cste(0),Cste(270)),list())
	  insert(Equal(list(LHS),mat))
	  
	  // axis equal
	elseif tree.rhs(krhs).value=="equal" then
	  LHS=Operation("ins",list(a,Cste("isoview")),list())
	  insert(Equal(list(LHS),Cste("on")))
	  
	  // axis image
	elseif tree.rhs(krhs).value=="image" then
	  no_equiv(expression2code(tree))
	  return
	  
	  // axis square
	elseif tree.rhs(krhs).value=="square" then
	  set_infos("cube_scaling only used in 3d mode",2);
	  LHS=Operation("ins",list(a,Cste("cube_scaling")),list())
	  insert(Equal(list(LHS),Cste("on")))
	  
	  // axis vis3d
	elseif tree.rhs(krhs).value=="vis3d" then
	  LHS=Operation("ins",list(a,Cste("view")),list())
	  insert(Equal(list(LHS),Cste("3d")))
	  
	  // axis normal
	elseif tree.rhs(krhs).value=="normal" then
	  no_equiv(expression2code(tree))
	  return
	  
	  // axis on
	elseif tree.rhs(krhs).value=="on" then
	  LHS=Operation("ins",list(a,Cste("axes_visible")),list())
	  insert(Equal(list(LHS),Cste("on")))
	  
	  // axis off
	elseif tree.rhs(krhs).value=="off" then
	  LHS=Operation("ins",list(a,Cste("visible")),list())
	  insert(Equal(list(LHS),Cste("off")))
	  
	// [mode,visibility,direction] = axis('state')
        elseif tree.rhs(krhs).value=="state" then
	  tree.name="mtlb_axis"
	  return
	  
	// Unknown character string
	else
	  tree.name="mtlb_axis"
	  return
	end
	
	// Option is a variable
      else
	tree.name="mtlb_axis"
	return
      end
      
    // axis(axes_handles,...)
    elseif tree.rhs(krhs).vtype==Handle then
      no_equiv(expression2code(tree))
      return
      
    // Unknown type for tree.rhs(krhs)
    else
      tree.name="mtlb_axis"
      return
    end
  end
  tree=list()
// v = axis
else
  tree=Operation("ins",list(a,Cste("data_bounds")),tree.lhs)
  tree.out(1).dims=list(2,Unknown)
  tree.out(1).type=Type(Double,Real)
end
endfunction
