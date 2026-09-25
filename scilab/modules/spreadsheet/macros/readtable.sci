// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2022 - Dassault Systèmes S.E. - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function tt = readtable(varargin)

    fname = "readtable"
    rhs = nargin;
    names = "";
    readrownames = %f;
    sheet = 1;
    rangevalue = "";

    nargs = argn(2);
    if nargs < 1 then
        error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"), fname, 1));
    end

    filename = varargin(1);

    if rhs > 2 then
        for i = nargin-1:-2:2
            key = varargin(i);
            value = varargin(i+1);
            if type(key) <> 10 then
                break;
            end
            select convstr(key, "l")
            case "variablenames"
                if type(value) <> 10 then
                    error(msprintf(_("%s: Wrong type for %s argument: string expected.\n"), fname, key));
                end
                names = value;
            case "readrownames"
                if type(value) <> 4 then
                    error(msprintf(_("%s: Wrong type for %s argument: boolean expected.\n"), fname, key));
                end
                readrownames = value;
            case "sheet"
                if type(value) == 10 then
                    sheet = value;
                elseif type(value) == 1 then
                    if value <> int(value) | value < 1 then
                        error(msprintf(_("%s: Wrong value for ""%s"" argument: Must be a positive integer >=1.\n"), fname, key));
                    end
                    sheet = value;
                else
                    error(msprintf(_("%s: Wrong type for ""%s"" argument: A double or string expected.\n"), fname, key));
                end

            case "range"
                if type(value) == 10 then
                    rangevalue = value;
                elseif type(value) == 1 then
                    if and(size(value) == [2 2]) then
                        rangevalue = %matrix_to_range(value);
                    else
                        error(msprintf(_("%s: Wrong value for ""%s"" argument: Range matrix must be 2x2 [row1 col1; row2 col2].\n"), fname, key));
                    end
                else
                    error(msprintf(_("%s: Wrong type for ""%s"" argument: Must be a string or a 2x2 matrix.\n"), fname, key));
                end
            else
                error(msprintf(_("%s: Wrong value for input argument #%d: ''%s'' not allowed.\n"), fname, i, key));
            end
            
            rhs = rhs - 2;
        end
    end

    isxlsx = isXlsx(filename);
    if isxlsx then
        f = %xlsxRead(filename, sheet, rangevalue, "string");
    else
        f = mgetl(filename);
    end
    disp(f)

    if nargin == 2 || rhs >= 2 then
        opts = varargin(2);
    else
        if ~isxlsx then
            opts = detectImportOptionsCSV(f);
        else
            opts = detectImportOptionsXlsx(f);
        end
    end

    variableTypes = opts.variableTypes;
    onlyDouble = %f;

    if ~isxlsx then
        if and(variableTypes == "double") then
            mat = csvTextScan(f(opts.datalines), opts.delimiter, opts.decimal);
            onlyDouble = %t;
        else
            mat = csvTextScan(f(opts.datalines), opts.delimiter, opts.decimal, "string");//(:,_kk);
        end
    else
        mat = f(opts.datalines, :);
    end

    variableNames = opts.variableNames;
    disp(opts.variableNames)
    if variableNames == [] then
        variableNames = ["Var" + string(1:size(variableTypes, "*"))];
    end
    disp(variableNames)
    
    fmt = opts.inputFormat;

    if names <> "" then
        [nb, _kk] = members(names, variableNames);
        if and(nb == 0) then
            error(msprintf(_("%s: no matching VariableNames.\n"), "readtimeseries"));
        end
        variableNames = names;
        variableTypes = variableTypes(_kk);
        fmt = fmt(_kk);
    else
        _kk = 1:$;
    end

    idx = find(variableNames == "Row")
    if idx <> [] then
        variableNames(idx) = "Var" + string(idx);
    end

    mat = mat(:, _kk);
    disp(variableNames)

    if onlyDouble then
        tt = table(mat, "VariableNames", variableNames);
    else
        l = list();
        for j = 1:size(mat, 2)
            m = mat(:,j)
            select variableTypes(j)
            case "duration"
                d = duration(%nan) * ones(m);
                idx = m <> "";
                d(idx) = duration(m(idx), "InputFormat", fmt(j));
                l($+1) = d;
            case "datetime"
                d = NaT(m);
                idx = m <> "";
                d(idx) = datetime(m(idx), "InputFormat", fmt(j));
                l($+1) = d;
            case "double"
                l($+1) = strtod(m)
            case "boolean"
                m = convstr(m, "l");
                d = members(m, ["t", "true"]) == 1;
                l($+1) = d
            else
                l($+1) = m
            end
        end

        tt = table(l(:), "VariableNames", variableNames);
    end
    tt.props.variableDescriptions = variableNames;

    if readrownames then
        tt.Row = string(tt.vars(1).data);
        tt(:,1) = [];
    end
endfunction
