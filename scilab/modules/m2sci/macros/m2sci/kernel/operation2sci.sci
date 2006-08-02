function [sci_equiv]=operation2sci(mtlb_expr)
// Copyright INRIA
// M2SCI function
// Convert a function call in an instruction or in an expression from Matlab to Scilab
// Input:
// - mtlb_instr: Matlab instr or expression to convert
// Output:
// - sci_instr: Scilab equivalent for mtlb_instr
// V.C.

sci_equiv=mtlb_expr

nboperands=size(mtlb_expr.operands)

// Special case for extraction
if mtlb_expr.operator=="ext" then
  [bval,index]=isdefinedvar(mtlb_expr.operands(1))
  
  // Variable is unknown
  if ~bval then
    // Try to find what is this variable
    [tmp]=getvar2sci(mtlb_expr.operands(1),lhslist)
    if typeof(tmp)=="funcall" then
      newname=mtlb_expr.operands(1).name
      mtlb_expr.operands(1)=null()
      mtlb_expr=Funcall(newname,size(lhslist),mtlb_expr.operands,lhslist)
      [sci_equiv]=expression2sci(mtlb_expr,lhslist)
      return
    else
    end
  end
end

opind=1
while opind<=nboperands
  [sci_operand]=expression2sci(mtlb_expr.operands(opind))
  mtlb_expr.operands(opind)=sci_operand
  opind=opind+1
  // Special case for insertion, is this case, last operand (right expression) is not converted
  if mtlb_expr.operator=="ins" & opind==nboperands then
    break
  end
  
end

// Function find is not used because does not work with strings
ind=-1
for kop=1:size(ops,1)
  if ops(kop,1)==mtlb_expr.operator then
    ind=kop
    break
  end
end
if ind==-1 then
  error("operation2sci(): operator "+mtlb_expr.operator+" not found !")
end
rhs=size(mtlb_expr.operands)
execstr("[sci_equiv]=%"+ops(ind,2)+"2sci(mtlb_expr)");

// Update lhsnb if Scilab equivalent is a function call
if typeof(sci_equiv)=="funcall" then
  sci_equiv.lhsnb=size(sci_equiv.lhs)
end

endfunction
