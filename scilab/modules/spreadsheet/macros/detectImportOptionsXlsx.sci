function opts = detectImportOptionsXlsx(f)
    opts = struct();
    // detect variable names and type
    test = strtod(f(1, :));
    datalines = [1:size(f, "r")];
    variableNames = [];
    index = [];

    if size(f, "r") > 1 then
        if and(isnan(test)) then
            variableNames = f(1,:);
            index = find(variableNames == "");
            if index <> [] then
                variableNames(index) = "Var" + string(index);
            end
            datalines(1) = [];
            hasheader = %t;
        end
        f(1,:) = [];
    end

    variableTypes = emptystr(variableNames);
    inputFormat = [];

    for i = 1:size(f, 'c')
        // types managed : datetime, double, string
        if f(1, i) == "" then
            variableTypes(1,i) = "string";
            inputFormat(1,i) = "";
            mat = f(:, i);
            idx = mat <> "";
            if or(idx) then
                [infmt, _typ] = detectFormatDatetime(mat(idx)(1))
                variableTypes(1,i) = _typ;
                inputFormat(1,i) = infmt;
            else
                // empty column
                if index <> i then
                    index = [index, i];
                end
            end
        else
            [infmt, _typ] = detectFormatDatetime(f(:, i))
            variableTypes(1,i) = _typ;
            inputFormat(1,i) = infmt
        end
    end

    opts.variableNames = variableNames;
    opts.variableTypes = variableTypes;
    opts.inputFormat = inputFormat;
    opts.datalines = datalines;
    opts.emptyCol = index;
endfunction
