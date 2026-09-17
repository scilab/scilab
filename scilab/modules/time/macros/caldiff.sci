// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function out = caldiff(dt, c, dim)
    // caldiff -  calendar successive differences
    arguments
        dt {mustBeA(dt, "datetime")}
        c {mustBeA(c, "string"), mustBeScalar} = "default"
        dim {mustBeA(dim, ["string", "double"]), mustBeScalar, mustBeMember(dim, {1, 2, "r", "c", "*", "m"})} = "*"
    end

    if c <> "default" then
        values = ["years", "months", "days", "time"];
        c = convstr(c);
        if and(c <> values) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in %s.\n"), "caldiff", 2, sci2exp(values)));
        end
    end
    
    
    select dim
    case "r"
        dim = 1;
    case "c"
        dim = 2;
    case "*"
        dim = 0;
    case "m"
        dim = find(size(dt) > 1, 1);
        if dim == [] then
            dim = 1;
        end
    end
    if dim == 0 then
        d1 = dt(1:$-1);
        d2 = dt(2:$);
    else
        nd = ndims(dt);            
        args=emptystr(1,nd)+":";
        arg1=args;arg1(dim)="1:$-1";
        arg2=args;arg2(dim)="2:$";
        execstr("d1=dt("+strcat(arg1,",")+")");
        execstr("d2=dt("+strcat(arg2,",")+")");
    end

    out = %caldiff(d1, d2, c)
    
endfunction
