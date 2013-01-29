function deff(varargin)
    if size(varargin) < 2 | size(varargin) > 3 then
        error(999, msprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"), "deff", 2, 3));
    end

    opt = [];
    if size(varargin) == 3 then
        opt = varargin(3);
    end

    //try to build execstr string
    str = "function " + varargin(1);
    str = [str ; varargin(2); "endfunction"];

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
