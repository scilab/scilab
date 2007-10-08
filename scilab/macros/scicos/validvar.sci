function r=validvar(s)
//Serge Steer, 08/10/2007
//copyright INRIA

  //check if the string s is a valid identifier
    s=stripblanks(s)
    r=%f
    if size(s,'*')<>1 then return, end
    if s=='' then return, end

    //create a function with s as single statement
    if execstr('deff(''foo'',s)','errcatch') then return, end
    t=macr2tree(foo) //syntax tree
    instr=t.statements(2) //ignore leading and trailing EOL

    //lhs shoud be "ans"
    if size(instr.lhs)<>1 then return,end
    if instr.lhs(1).name<>'ans' then return,end

    //expression should be just a reference to s
    if instr.expression.operator<>'ext'  then return,end
    if size(instr.expression.operands)<>1 then return,end
    if instr.expression.operands(1).name<>s  then return,end

    r=%t
endfunction
