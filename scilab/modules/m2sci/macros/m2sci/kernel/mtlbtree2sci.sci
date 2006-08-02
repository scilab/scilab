function [scitree,crp]=mtlbtree2sci(mtlbtree,prettyprint)
// Copyright INRIA
// Scilab Project - V. Couvert
// Conversion of a Matlab function tree to Scilab (and code generation)
// Input arguments:
//  - mtlbtree: tree (returned by macr2tree) representing Matlab function compiled code
//  - prettyprint: boolean flag for pretty printed output file if TRUE
// Output arguments:
//  - scitree: Scilab equivalent for mtlbtree
//  - crp: Scilab equivalent function code (function body)

// Global variables for M2SCI
global("m2sci_to_insert_b") // To insert before current instruction
global("m2sci_to_insert_a") // To insert after current instruction
global("tmpvarnb") // Index for temporary variables
m2sci_to_insert_b=list()
m2sci_to_insert_a=list()
tmpvarnb=0

if typeof(mtlbtree)<>"program" then
  error("mtlbtree2sci(): wrong type of input !")
end

// Init Scilab tree
scitree=tlist(["program","name","outputs","inputs","statements"],mtlbtree.name,mtlbtree.outputs,mtlbtree.inputs,list())

ninstr=1 // Index of Matlab tree
if batch then // defined in m2sci.sci
  nblines=0
else
  nblines=1 // Number of converted lines
end

m2sci_info("Conversion of M-tree...",-1);

// Default value
rhs = argn(2);
if rhs<2 then
  prettyprint=%F
end

crp=""

// Function prototype
lhsstr=[]
rhsstr=[]
if scitree.name<>"" then // Not a batch file
  for k=1:size(scitree.outputs)
    lhsstr=[lhsstr,expression2code(scitree.outputs(k))]
  end
  lhsstr="["+strcat(lhsstr,",")+"]"
  
  for k=1:size(scitree.inputs)
    rhsstr=[rhsstr,expression2code(scitree.inputs(k))]
  end
  rhsstr="("+strcat(rhsstr,",")+")"
  
  crp=lhsstr+" = "+scitree.name+rhsstr;
end

// Convert Matlab instruction tree to Scilab
while ninstr<=size(mtlbtree.statements)-3
  //Case : sup_equal instruction
  // Add converted tree to scitree and also inserted instructions
  if typeof(mtlbtree.statements(ninstr))=="sup_equal"

    sci_stat=list()
    for i=1:size(mtlbtree.statements(ninstr).sup_instr) 
      [converted_tree,nblines]=instruction2sci(mtlbtree.statements(ninstr).sup_instr(i),nblines)
      
      sci_stat=update_instr_list(sci_stat,converted_tree)
    end
    
    scitree.statements($+1)=tlist(["sup_equal","sup_instr","nb_opr"],sci_stat,mtlbtree.statements(ninstr).nb_opr)  
  else
    
    [converted_tree,nblines]=instruction2sci(mtlbtree.statements(ninstr),nblines)
    
    // Add converted tree to scitree and also inserted instructions
    
    scitree.statements=update_instr_list(scitree.statements,converted_tree)
    
    // Generate code corresponding to scitree.statements
  end    
  for k=1:size(scitree.statements)
    if k<size(scitree.statements)
      crp = cat_code(crp,instruction2code(scitree.statements(k),prettyprint));  
      crp = format_txt(crp,scitree.statements(k),prettyprint,scitree.statements(k+1));
    end
  end
  
  scitree.statements=list(scitree.statements($))
  
  // Disp percentage of conversion done
  mprintf(margin+"Line "+string(nblines)+" out of "+string(mtlbtree.nblines)+"...\r")
  ninstr=ninstr+1
  tmpvarnb=0
end

if scitree.statements(1)<>list("EOL") then
  crp = cat_code(crp,instruction2code(scitree.statements(1),prettyprint));
  crp = format_txt(crp,scitree.statements(1),prettyprint,list("EOL"));
end

if scitree.name<>"" then // Not a batch file
  crp=cat_code(crp,"");
  crp=cat_code(crp,"endfunction"); // Replace last return
  crp=cat_code(crp,"");
end

m2sci_info("Conversion of M-tree: Done",-1);


clearglobal("m2sci_to_insert_b")
clearglobal("m2sci_to_insert_a")
clearglobal("tmpvarnb")

endfunction
