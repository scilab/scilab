function deff(varargin)
    if size(varargin) <> 2  then
        error(999, msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "deff", 2));
    end

    //try to build execstr string
    str = "function " + varargin(1);
    str = [str ; varargin(2)(:); "endfunction"];

    execstr(str);
    funcs = funclist();

    if size(funcs, "*") <= 0 then
        return;
    elseif size(funcs, "*") <> 1 then
        return;
    else
        execstr(funcs + "= return(" + funcs + ")");
    end

endfunction
