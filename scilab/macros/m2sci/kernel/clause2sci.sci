function [sci_clause,nblines]=clause2sci(mtlb_clause,nblines)
// Copyright INRIA
// M2SCI function
// V.C.

// Global variables for M2SCI
global("m2sci_to_insert_b")
global("varslist")

// Increment level of clause indentation
level(1)=level(1)+1

// Temp variable used to store instructions to insert before clause
to_insert=list()
select typeof(mtlb_clause)
  
// --- IF ---
case "ifthenelse"
  level(2)=1  
  // Convert expression
  [sci_expr]=expression2sci(mtlb_clause.expression)
  // Get instructions to insert if there are
  if m2sci_to_insert_b<>list() then
    to_insert=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end
  
  // Convert then statements
  sci_then=list()
  for k=1:size(mtlb_clause.then)
    [instr,nblines]=instruction2sci(mtlb_clause.then(k),nblines)
    sci_then=update_instr_list(sci_then,instr)
  end
  
  // Convert elseifs
  sci_elseifs=list()
  for k=1:size(mtlb_clause.elseifs)
    level(2)=level(2)+1
    
    // Convert expression
    [sci_exprn]=expression2sci(mtlb_clause.elseifs(k).expression)
    // Get instructions to insert if there are
    if m2sci_to_insert_b<>list() then
      to_insert=m2sci_to_insert_b
      m2sci_to_insert_b=list()
    end
    
    // Convert statements
    sci_stat=list()
    for l=1:size(mtlb_clause.elseifs(k).then)
      [instr,nblines]=instruction2sci(mtlb_clause.elseifs(k).then(l),nblines)
      sci_stat=update_instr_list(sci_stat,instr)
    end
    
    sci_elseifs($+1)=tlist(["elseif","expression","then"],sci_exprn,sci_stat)
  end
  
  // Convert else
  sci_else=list()
  level(2)=level(2)+1 
  for k=1:size(mtlb_clause.else)
    [instr,nblines]=instruction2sci(mtlb_clause.else(k),nblines)
    sci_else=update_instr_list(sci_else,instr)
  end
  
  // Create Scilab ifthenelse
  sci_clause=tlist(["ifthenelse","expression","then","elseifs","else"],sci_expr,sci_then,sci_elseifs,sci_else)
  updatevarslist("END OF CLAUSE")
  level(1)=level(1)-1

// --- SELECT ---
case "selectcase"
  level(2)=1  
  // Convert expression
  sci_expr=list()
  [sci_expr(1)]=expression2sci(mtlb_clause.expression(1))
  if size(mtlb_clause.expression)==2 then
    sci_expr(2)=mtlb_clause.expression(2) // EOL
  end
  // Get instructions to insert if there are
  if m2sci_to_insert_b<>list() then
    to_insert=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end
  
  // Convert cases
  sci_cases=list()
  k=0
  while k<size(mtlb_clause.cases)
    k=k+1
    level(2)=level(2)+1
    // Convert expression
    if typeof(mtlb_clause.cases(k).expression)=="funcall" then
      if mtlb_clause.cases(k).expression.name=="makecell" then
	// Insert new cases
	for nbcas=size(mtlb_clause.cases):-1:k+1
	  mtlb_clause.cases(nbcas+size(mtlb_clause.cases(k).expression.rhs))=mtlb_clause.cases(nbcas)
	end
	for nbrhs=1:size(mtlb_clause.cases(k).expression.rhs)
	  mtlb_clause.cases(nbrhs+k)=tlist(["case","expression","then"],mtlb_clause.cases(k).expression.rhs(nbrhs),mtlb_clause.cases(k).then)
	end
	mtlb_clause.cases(k)=null()
      end
    end
    [sci_exprn]=expression2sci(mtlb_clause.cases(k).expression)
    // Get instructions to insert if there are
    if m2sci_to_insert_b<>list() then
      to_insert=m2sci_to_insert_b
      m2sci_to_insert_b=list()
    end
    
    // Convert statements
    sci_stat=list()
    for l=1:size(mtlb_clause.cases(k).then)
      [instr,nblines]=instruction2sci(mtlb_clause.cases(k).then(l),nblines)
      sci_stat=update_instr_list(sci_stat,instr)
    end
    
    sci_cases($+1)=tlist(["case","expression","then"],sci_exprn,sci_stat)
  end
  
  // Convert else
  sci_else=list()
  level(2)=level(2)+1 
  for k=1:size(mtlb_clause.else)
    [instr,nblines]=instruction2sci(mtlb_clause.else(k),nblines)
    sci_else=update_instr_list(sci_else,instr)
  end
  
  // Create Scilab selectcase
  sci_clause=tlist(["selectcase","expression","cases","else"],sci_expr,sci_cases,sci_else)
  updatevarslist("END OF CLAUSE")
  level(1)=level(1)-1
  
// --- WHILE ---
case "while"
  level(2)=1  
  sci_do=list()
  // Convert expression
  [sci_expr]=expression2sci(mtlb_clause.expression)
  // If there are instructions to insert, while is modified so that inserted instruction is evaluated in each loop
  if m2sci_to_insert_b<>list() then
    newif=tlist(["ifthenelse","expression","then","elseifs","else"],sci_expr,list(Funcall("break",1,list(),list())),list(),list())
    m2sci_to_insert_b($+1)=newif
    sci_expr=Cste(%T)
    sci_do=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end

  // Convert all do instructions
  for k=1:size(mtlb_clause.statements)
    [instr,nblines]=instruction2sci(mtlb_clause.statements(k),nblines)
    sci_do=update_instr_list(sci_do,instr)
  end

  // Create Scilab while
  sci_clause=tlist(["while","expression","statements"],sci_expr,sci_do)

// --- FOR ---
case "for"
  level(2)=1  
  // Convert expression
  [sci_expr,nblines]=instruction2sci(mtlb_clause.expression,nblines)
  // Get instructions to insert if there are
  if m2sci_to_insert_b<>list() then
    to_insert=m2sci_to_insert_b
    m2sci_to_insert_b=list()
  end

  sci_instr=list()
  // Convert all do instructions
  for k=1:size(mtlb_clause.statements)
    [instr,nblines]=instruction2sci(mtlb_clause.statements(k),nblines)
    sci_instr=update_instr_list(sci_instr,instr)
  end

  // Create Scilab while
  sci_clause=tlist(["for","expression","statements"],sci_expr,sci_instr)
  
else
  error("clause2sci(): unknown clause type: "+typeof(mtlb_clause))
end

m2sci_to_insert_b=to_insert
endfunction
