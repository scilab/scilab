function [sci_instr]=equal2sci(mtlb_instr)
// Copyright INRIA
// Convertion of a Matlab instruction or expression to Scilab
// Input:
// - mtlb_instr: Matlab instr or expression to convert
// Output:
// - sci_instr: Scilab equivalent for mtlb_instr
// V.C.

// Trees to insert in converted function tree
global("m2sci_to_insert_b")
m2sci_to_insert_b=list()
global("m2sci_to_insert_a")
m2sci_to_insert_a=list()
global("varslist")

sci_instr=mtlb_instr

// Add inference data to lhs
lhslist=list()

// Inference field added to each lhs argument
// Get infos in varslist to init
for k=1:size(mtlb_instr.lhs)
  if typeof(mtlb_instr.lhs(k))=="variable" then
    [bval,index]=isdefinedvar(mtlb_instr.lhs(k))
    if bval then
      INFER=varslist(index).infer
    else
      INFER=Infer()
    end
    lhslist($+1)=Variable(mtlb_instr.lhs(k).name,INFER)
  elseif typeof(mtlb_instr.lhs(k))=="operation" then
    if mtlb_instr.lhs(k).operator<>"ins" then
      error("equal2sci(): lhs cannot be a "+mtlb_instr.lhs(k).operator+" operation")
    end
    
    [bval,index]=isdefinedvar(mtlb_instr.lhs(k).operands(1))
    if bval then
      INFER=varslist(index).infer
    else
      INFER=Infer()
    end
    
    lhslist($+1)=Operation(mtlb_instr.lhs(k).operator,..
	mtlb_instr.lhs(k).operands,..
	list(Variable(mtlb_instr.lhs(k).operands(1).name,INFER)))
  else
    error("equal2sci(): lhs cannot be a "+typeof(mtlb_instr.lhs(k)))
  end
end

// Convert expression

[sci_expr]=expression2sci(mtlb_instr.expression,lhslist);

if sci_expr==list() then // Conversion made by inserted instructions or 'm2scideclare'
  sci_instr=list()
else

  sci_instr.expression=sci_expr;
  
  // Update lhs of instruction
  select typeof(sci_instr.expression)
  case "operation" then
    sci_instr.lhs=sci_expr.out;
  case "funcall" then
    sci_instr.lhs=sci_instr.expression.lhs
    if typeof(mtlb_instr.expression)=="funcall" then
      sci_instr.lhs=sci_expr.lhs;
    end
  case "cste" then
    sci_instr.lhs=lhslist;
    sci_instr.lhs(1).dims=sci_expr.dims;
    sci_instr.lhs(1).type=sci_expr.type;
  case "variable" then
    sci_instr.lhs=lhslist;
    sci_instr.lhs(1).dims=sci_expr.dims;
    sci_instr.lhs(1).type=sci_expr.type;
  else
    error("equal2sci: "+typeof(sci_instr.expression)+" is not implemented !");
  end
 
  // If lhs are insertion operation, they also have to be converted
  for k=1:size(sci_instr.lhs)
    if typeof(sci_instr.lhs(k))=="operation" then
      sci_instr.lhs(k).operands($+1)=sci_instr.expression
      // Keep just one inference field in sci_instr.expression (if is a funcall) so that inference can be made in operation2sci()
      if typeof(sci_instr.lhs(k).operands($))=="funcall" then
	for l=1:size(sci_instr.lhs(k).operands($).lhs)
	  if l<>k then
	    sci_instr.lhs(k).operands($).lhs(l)=list()
	  end
	end
	l=1
	while l<=size(sci_instr.lhs(k).operands($).lhs)
	  if sci_instr.lhs(k).operands($).lhs(l)==list() then
	    sci_instr.lhs(k).operands($).lhs(l)=null()	
	  else
	    l=l+1
	  end
	end
	// Verify that there is just one lhs kept
	if size(sci_instr.lhs(k).operands($).lhs)<>1 then pause;end
      end
      // If insertion made in an unknown variable, I add it to varslist
      inservar=sci_instr.lhs(k).operands(1)
      [bval,index]=isdefinedvar(inservar)
      if ~bval then
	// Variable added to varslist before insertion
	if funptr(inservar.name)<>0 then
	  matname="%"+inservar.name
	else
	  matname=inservar.name
	end
	if sci_instr.expression.vtype==Struct then
	  // Variable is initialized to struct() in converted script is does not already exist
	  varslist($+1)=M2scivar(matname,inservar.name,Infer(list(0,0),Type(Struct,Unknown)))
	  //m2sci_to_insert_b($+1)=Equal(list(inservar),Funcall("struct",1,list(),list()))
	elseif sci_instr.expression.vtype==Cell then
	  // Variable is initialized to cell() in converted script is does not already exist
	  varslist($+1)=M2scivar(matname,inservar.name,Infer(list(0,0),Type(Cell,Unknown)))
	  //m2sci_to_insert_b($+1)=Equal(list(inservar),Funcall("cell",1,list(),list()))
	else
	  // Variable is initialized to [] in converted script is does not already exist
	  varslist($+1)=M2scivar(matname,inservar.name,Infer(list(0,0),Type(Double,Real)))
	  //m2sci_to_insert_b($+1)=Equal(list(inservar),Cste([]))
	end
	sci_instr.lhs(k).out(1).infer=varslist($).infer
      else
      	sci_instr.lhs(k).out(1).infer=varslist(index).infer
      end
      [sci_instr.lhs(k)]=operation2sci(sci_instr.lhs(k))
      if typeof(sci_instr.lhs(k))=="operation" then
	if or(sci_instr.lhs(k).operands($)<>sci_instr.expression) then // Update expression if has been modified while converting lhs
	  sci_instr.expression=sci_instr.lhs(k).operands($)
	end
	
	sci_instr.lhs(k).operands($)=null()
	updatevarslist(sci_instr.lhs(k).out)
      else
	// Insertion done by inserted instruction
	sci_instr=list()
	return
      end
    end
  end
  // Update varslist
  updatevarslist(sci_instr.lhs);
end
endfunction
