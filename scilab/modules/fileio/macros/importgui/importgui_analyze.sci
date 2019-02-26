function delim = importgui_analyze(f, n)

    if ~exists("n", "local") then
        n = 10;
    end
    
    head = mgetl(f, n);
    n = size(head, "r");
    
    delimiters = get("importgui_delim", "userdata");
    count = zeros(delimiters);
    
    for i = 1:n
        l = head(i);
        for j = 1:size(delimiters, "*")
            r = strindex(l, delimiters(j));
            if size(r, "*") > 1 then
                count(j) = count(j) + 1;
            end
        end
    end
    
    [m, k] = max(count);

    if m == n then
        delim = k;
    else
        //unable to find good delimiter.
        delim = 0;
        return;
    end
endfunction
