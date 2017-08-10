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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=EXPRESSION(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
        model=arg1.model;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        %scicos_context=%scicos_context;
        for ii=1:8,
            execstr("%scicos_context.u"+string(ii)+"=0"),
        end
        ieee(2)
        while %t do
            [ok,%nin,%exx,%usenz,exprs]=scicos_getvalue(..
            ["Give a scalar scilab expression using inputs u1, u2,...";
            "If only one input, input is vector [u1,u2,...] (max 8)";
            "ex: (dd*u1+sin(u2)>0)*u3";
            "Note that here dd must be defined in context"],..
            ["number of inputs";"scilab expression";"use zero-crossing (0: no, 1 yes)"],..
            list("vec",1,"vec",1,"vec",1),exprs)
            ieee(0)
            clear %scicos_context

            if ~ok then
                break,
            end
            %exx=strsubst(exprs(2)," ","")
            if %exx==emptystr() then
                %exx="0",
            end  //avoid empty
            //expression

            if %nin==1 then
                %nini=8,
            else
                %nini=%nin
            end
            %head="%foo("
            for %jji=1:%nini-1,
                %head=%head+"u"+string(%jji)+",",
            end
            %head=%head+"u"+string(%nini)+")"
            ok=execstr("deff(%head,%exx)","errcatch")==0
            if ~ok then
                message(["Erroneous expression";lasterror()])
            else
                if %nin>1 then
                    [model,graphics,ok]=check_io(model,graphics,ones(1,%nin),1,[],...
                    [])
                else
                    [model,graphics,ok]=check_io(model,graphics,-1,1,[],...
                    [])
                end
                if ok then

                    [ok,%ok1,ipar,rpar,%nz]=compiler_expression(%foo)

                    if ~ok then
                        message(["Erroneous expression";lasterror()])
                    else
                        if %ok1 then
                            model.rpar=rpar
                            model.ipar=ipar
                            if %usenz then
                                model.nzcross=%nz
                                model.nmode=%nz
                            else
                                model.nzcross=0
                                model.nmode=0
                            end
                            graphics.exprs=exprs
                            x.graphics=graphics
                            x.model=model
                            break
                        end
                    end
                end
            end
        end
    case "define" then
        in=[1;1]
        out=1
        txt="(u1>0)*sin(u2)^2"
        deff("%foo(u1,u2)",txt)
        [%ok1,ipar,rpar,nz]=compile_expr(%foo)
        model=scicos_model()
        model.sim=list("evaluate_expr",4)
        model.in=in
        model.out=out
        model.rpar=rpar
        model.ipar=ipar
        model.nzcross=nz
        model.nmode=nz
        model.dep_ut=[%t %f]
        exprs=[string(size(in,"*"));txt;"1"]
        gr_i=[]
        x=standard_define([5 2],model,exprs,gr_i)
    end
endfunction

function [ok,%ok1,ipar,rpar,%nz]=compiler_expression(%foo)
    ok=%t,%ok1=%f,ipar=[],rpar=[],%nz=[]
    if exists("%scicos_context") then
        %mm=getfield(1,%scicos_context)
        for %mi=%mm(3:$)
            if execstr(%mi+"=%scicos_context(%mi)","errcatch")<>0 then
                ok=%f
            end
        end
    end
    if ok then
        ok=execstr("[%ok1,ipar,rpar,%nz]=compile_expr(%foo)","errcatch")==0
    end
endfunction


function [ok,%ipar,%rpar,%nz]=compile_expr(%foo)
    ok=%t
    %tlst=macr2tree(%foo);
    %mm=macrovar(%foo);
    %MM=%mm(3);
    %FF=["sin";"cos";"tan";"exp";"log";
    "sinh";"cosh";"tanh";
    "int";"round";"ceil";"floor";
    "sign";"abs";"max";"min";
    "asin";"acos";"atan";"asinh";"acosh";"atanh";
    "atan2";
    "log10";
    ]; // ops above 100
    %num_arg=[1;1;1;1;1;
    1;1;1
    1;1;1;1;
    1;1;2;2;
    1;1;1;1;1;1;
    2;
    1;
    ];  //number of arguments
    %OP = emptystr(32, 1); // ops
    %OP(1) = "+";
    %OP(2) = "-";
    %OP(3) = "*";
    %OP(4) = ".*";
    %OP(5) = "*.";
    %OP(6) = ".*.";
    %OP(7) = "/";
    %OP(8) = "./";
    %OP(9) = "/.";
    %OP(10) = "./.";
    %OP(11) = "\";
    %OP(12) = ".\";
    %OP(13) = "\.";
    %OP(14) = ".\.";
    %OP(15) = "^";
    %OP(22) = ":";
    %OP(23) = "rc"; // "[,]"
    %OP(26) = "''";
    %OP(27) = "cc"; // "[;]"
    %OP(31) = ".^";
    %OP(32) = ".''";
    %ZCR = emptystr(116, 1);  // ops with zero-crossing
    %ZCR(16) = "==";
    %ZCR(17) = "<";
    %ZCR(18) = ">";
    %ZCR(19) = "<=";
    %ZCR(20) = ">=";
    %ZCR(21) = "<>"; // Also works for "~=" operator
    %ZCR(28) = "|";
    %ZCR(29) = "&";
    %ZCR(30) = "~";
    // FIXME: find what these ops correspond to in Scilab 5 and update decypher_operation & decypher_funcall accordingly
    //%ZCR(109) = "";
    //%ZCR(110) = "";
    //%ZCR(111) = "";
    //%ZCR(112) = "";
    //%ZCR(113) = "";
    //%ZCR(114) = "";
    //%ZCR(115) = "";
    //%ZCR(116) = "";
    // Only 24 and 25 are missing in the intersection of %OP and %ZCR indexes,
    // that is because they correspond to insertion and extraction:
    // insertion is not needed and extraction is handled locally.
    %UU=%mm(1)
    %ipar=[]
    %rpar=[]
    %nrpar=0
    %nz=0

    function [ok, rpar, nrpar, ipar] = decypher_variable(var, rpar, nrpar, ipar)
        ok = %t
        %indOut = find(var("name")==%MM)
        if %indOut <> [] then
            if ~exists(%MM(%indOut)) then
                message("Variable "+%MM(%indOut)+" is not defined.")
                ok = %f
                return
            end
            %var = evstr(%MM(%indOut))
            if size(%var,"*") <> 1 then
                message("Variable "+%MM(%indOut)+" is not scalar.")
                ok=%f
                return
            else
                nrpar = nrpar+1
                rpar(nrpar) = %var
                ipar = [ipar; 6; nrpar]
            end
        else
            %indIn = find(var("name")==%UU)
            if %indIn <> [] then
                ipar = [ipar; 2; %indIn]
            else
                // All defined variables that are not in the prototype (for instance, Scilab constants)
                ierr = execstr("localVar = evstr(var(""name""))", "errcatch");
                if ierr == 0 then
                    if size(localVar,"*") <> 1 then
                        message("Variable "+var("name")+" is not scalar.")
                        ok=%f
                        return
                    end
                    nrpar = nrpar+1
                    rpar(nrpar) = localVar
                    ipar = [ipar; 6; nrpar]
                else
                    message("Unknown variable "+var("name"))
                    ok = %f
                    return
                end
            end
            // ipar = [ipar; 2; evstr(strsubst(var("name"),'u',''))]
        end
    endfunction

    function [ok, rpar, nrpar, ipar, nz] = decypher_funcall(f, rpar, nrpar, ipar, nz)
        ok = %t
        // Check number of operands for concerned operation
        %indOperator = find(f("name")==%FF)
        if %indOperator <> [] then
            if length(f("rhs")) <> %num_arg(%indOperator) then
                message(f("name")+" must have "+string(%num_arg(%indOperator))+" arguments")
                ok = %f
                return
            end
        end

        for i=1:length(f("rhs"))
            oper = f("rhs")(i)
            select typeof(oper)
            case "operation"
                [ok, rpar, nrpar, ipar, nz] = decypher_operation(oper, rpar, nrpar, ipar, nz);
                if ~ok then
                    return
                end
            case "funcall"
                [ok, rpar, nrpar, ipar, nz] = decypher_funcall(oper, rpar, nrpar, ipar, nz);
                if ~ok then
                    return
                end
            case "variable"
                [ok, rpar, nrpar, ipar] = decypher_variable(oper, rpar, nrpar, ipar);
                if ~ok then
                    return
                end
            else // "cste"
                // ipar = [ipar; 6; evstr(op)]
                nrpar = nrpar+1
                if oper("value") == [] then
                    rpar(nrpar) = 0; // We are dealing with scalars so [] = 0
                else
                    rpar(nrpar) = oper("value")
                end
                ipar = [ipar; 6; nrpar]
            end
        end

        %indIn = find(f("name")==%UU) // Handling of extraction
        if %indIn <> [] then
            ipar = [ipar; 2; %indIn; 5; 25]
        elseif %indOperator <> [] then
            ipar = [ipar; 5; 100+%indOperator]
        else
            %indOperator = find(f("name")==%ZCR)
            if %indOperator <> [] then
                ipar = [ipar; 5; %indOperator]
                nz = nz+1
            else
                message("Function "+f("name")+" not supported.")
                ok = %f
                return
            end
        end
    endfunction

    function [ok, rpar, nrpar, ipar, nz] = decypher_operation(op, rpar, nrpar, ipar, nz)
        ok = %t
        // 'operations' are not "functions", not no need to look for theù in %FF here

        for i=1:length(op("operands"))
            oper = op("operands")(i)
            select typeof(oper)
            case "operation"
                [ok, rpar, nrpar, ipar, nz] = decypher_operation(oper, rpar, nrpar, ipar, nz);
                if ~ok then
                    return
                end
            case "funcall"
                [ok, rpar, nrpar, ipar, nz] = decypher_funcall(oper, rpar, nrpar, ipar, nz);
                if ~ok then
                    return
                end
            case "variable"
                [ok, rpar, nrpar, ipar] = decypher_variable(oper, rpar, nrpar, ipar);
                if ~ok then
                    return
                end
            else // "cste"
                // ipar = [ipar; 6; evstr(op)]
                nrpar = nrpar+1
                if oper("value") == [] then
                    rpar(nrpar) = 0; // We are dealing with scalars so [] = 0
                else
                    rpar(nrpar) = oper("value")
                end
                ipar = [ipar; 6; nrpar]
            end
        end

        if op("operator") == "-" & length(op("operands")) == 1 then
            ipar = [ipar; 5; 99] // case of - with one operand (-u1)
        else
            %indOperator = find(op("operator")==%OP)
            if %indOperator <> [] then
                ipar = [ipar; 5; %indOperator]
            else
                if op("operator") == "~=" then
                    op("operator") = "<>"
                end
                %indOperator = find(op("operator")==%ZCR)
                if %indOperator <> [] then
                    ipar = [ipar; 5; %indOperator]
                    nz = nz+1
                end
            end
        end
    endfunction

    // Check statements
    i = "statements"
    for j=1:length(%tlst(i))
        if typeof(%tlst(i)(j)) == "equal" then
            expr = %tlst(i)(j)("expression");
            select typeof(expr)
            case "operation"
                [ok, %rpar, %nrpar, %ipar, %nz] = decypher_operation(expr, %rpar, %nrpar, %ipar, %nz);
                if ~ok then
                    return
                end
            case "funcall"
                [ok, %rpar, %nrpar, %ipar, %nz] = decypher_funcall(expr, %rpar, %nrpar, %ipar, %nz);
                if ~ok then
                    return
                end
            case "variable"
                [ok, %rpar, %nrpar, %ipar] = decypher_variable(expr, %rpar, %nrpar, %ipar);
                if ~ok then
                    return
                end
            else // "cste"
                // %ipar = [%ipar; 6; evstr(expr)]
                %nrpar = %nrpar+1
                if expr("value") == [] then
                    %rpar(%nrpar) = 0; // We are dealing with scalars so [] = 0
                else
                    %rpar(%nrpar) = expr("value")
                end
                %ipar = [%ipar; 6; %nrpar]
            end
        end
    end
endfunction
