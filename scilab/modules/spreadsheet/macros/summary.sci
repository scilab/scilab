// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function varargout = summary(t, varargin)
    arguments
        t {mustBeA(t, ["double", "string", "int", "bool", "datetime", "duration", "table", "timeseries"])}
        varargin
    end

    if istable(t) | istimeseries(t) then
        [st, varnames, haveStatistics, statistics, idxNumeric, idxString, idxBoolean] = %summary(t, varargin(:));
        if nargout == 0 then
            // properties of table/timeseries
            props = t.props;
            descr = props.description;
            vardescr = props.variableDescriptions;
            varunits = props.variableUnits;
            start = istimeseries(t) + 1;

            // display summary
            // type and size
            str = typeof(t);
            out = [""; " [Size type]: " + %type_dims_outline(t, typeStr=str, forceDims=%t); ""];

            // display Description
            if descr <> "" then
                out = [out; " Description: " + descr; ""];
            end

            if str == "timeseries" then
                // istimeseries - row times
                out = [out; " RowTimes:"]
                name = varnames(1);
                varSt = st(name);
                out = [out; "    " + name + ": "+ varSt.Type];
                fields = fieldnames(varSt)';
                idx =  members(fields, ["Size", "Type"]);
                fields(idx == 1) = [];

                for f = fields
                    out = [out; "        "+f+": " + string(varSt(f))];              
                end

                out = [out; ""];
            end

            // display Variables
            out = [out; " Variables:"]

            for i = start:size(varnames, "*")
                name = varnames(i);
                varSt = st(name);
                out = [out; "     " + name + ": " + varSt.Type];
                if ~isempty(varSt.Description) then
                    out = [out; "        Description: " + varSt.Description];
                end
                if ~isempty(varSt.Units) then
                    out = [out; "        Units: " + varSt.Units];
                end

            end

            // display Statistics
            out = strcat(out, "", "c");
            mprintf("%s\n", out);

            if haveStatistics then
                l = lines();
                lines(0,0)

                groupIndices = list(idxNumeric, idxBoolean, idxString);
                sections = [" Statistics for numeric data:", " Statistics for booleans:", " Statistics for strings:"];
                fields = list(statistics, ["True", "False"], ["Values", "Counts"]);

                for i = 1:size(groupIndices)
                    index = groupIndices(i);
                    if ~isempty(index) then
                        mprintf("\n");
                        mprintf("%s\n", sections(i));

                        stat = fields(i);
                        mat = emptystr(size(stat, "*"), length(index));
                        
                        jdx = 1;
                        for idx = index
                            name = varnames(idx);
                            varSt = st(name);
                            typ = varSt.Type;
                            f = fieldnames(varSt)';
                            m =  members(f, ["Size", "Type", "Description", "Units"]);
                            f(m == 1) = [];
                            if typ == "string" then
                                for j = f
                                    mat(stat == j, jdx) = sci2exp(varSt(j));
                                end
                            else
                                for j = f
                                    mat(stat == j, jdx) = string(varSt(j));
                                end
                            end
                            jdx = jdx + 1;
                        end

                        tt = matrix2table(mat, "RowNames", stat, "VariableNames", varnames(index));
                        %table_p(tt)
                    end
                end

                lines(l(2), l(1));
            end
        else 
            varargout(1) = st;
        end
    else
        if ndims(t) > 2 then
            error(msprintf(_("%s: Wrong size for input argument #%d: A matrix expected.\n"), fname, 1));
        end
        [st, dim, haveStatistics, statistics] = %summaryMatrix(t, varargin(:));
        if nargout == 0 then

            // type and size
            str = typeof(t);
            out = [""; " [Size type]: " + %type_dims_outline(t, typeStr=str, forceDims=%t); ""];

            // display summary
            out = strcat(out, "", "c");
            mprintf("%s\n", out);

            if haveStatistics then
                l = lines();
                lines(0,0)

                select typeof(t)
                case {"constant", "duration", "datetime", "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64"}

                    fields = fieldnames(st)';
                    nFields = size(fields, "*");
                    n = length(st(fields(1)));

                    mat = emptystr(n, nFields);
                    for k = 1:nFields
                        mat(:, k) = string(matrix(st(fields(k)), -1, 1));
                    end

                    if dim == "r" then
                        mat = mat';
                        tt = matrix2table(mat, ...
                            "RowNames", fields, ...
                            "VariableNames", "Col" + string(1:n));
                    else
                        tt = matrix2table(mat, ...
                            "VariableNames", fields, ...
                            "RowNames", "Row" + string(1:n));
                    end

                case "string"
                    if isrow(st.Values) then
                        st.Values = st.Values';
                    end
                    if dim == "r" then
                        tt = matrix2table([st.Counts; st.NumMissing], ...
                            "RowNames", [st.Values; "<undefined>"], ...
                            "VariableNames", "Col" + string(1:size(t, 2)));
                    else
                        tt = matrix2table([st.Counts st.NumMissing], ...
                            "VariableNames", [st.Values; "<undefined>"]', ...
                            "RowNames", "Row" + string(1:size(t, 1)));
                    end

                case "boolean"
                    if dim == "r" then
                        tt = matrix2table([st.True; st.False], ...
                            "RowNames", ["True"; "False"], ...
                            "VariableNames", "Col" + string(1:size(t, 2)));
                    else
                        tt = matrix2table([st.True st.False], ...
                            "VariableNames", ["True" "False"], ...
                            "RowNames", "Row" + string(1:size(t, 1)));
                    end
                    
                end

                %table_p(tt)

                lines(l(2), l(1));
            end

        else
            // return struct
            varargout(1) = st;
        end

    end

endfunction

function [haveStatistics, statistics] = defineStats(statistics)

    haveStatistics = %t;
    idx = find(statistics == "none");
    if or(idx <> []) then
        haveStatistics = %f;
    else
        idx = find(statistics == "allstats");
        if or(idx <> []) then
            statistics = ["NumMissing", "Min", "Median", "Max", "Q1", "Mean", "Q3", "Std", "Var", "Mode", "Range", "Sum", "NumUnique", "Nnz"];
        end

        idx = find(statistics == "default")
        if idx <> [] then
            fields = statistics(1:idx-1);
            fields = [fields, "NumMissing", "Min", "Median", "Max", "Mean", "Std"];
            fields = [fields, statistics(idx+1:$)];
            statistics = fields;
        end

        statistics = unique(statistics, "keepOrder");
        statistics = convstr(part(statistics, 1), "u") + part(statistics, 2:$);
    end

endfunction

function [outSt, dim, haveStatistics, statistics] = %summaryMatrix(t, dim, statistics)
    arguments
        t 
        dim {mustBeA(dim, ["double", "string"]), mustBeMember(dim, {1, 2, "r", "c"})} = 1
        statistics {mustBeA(statistics, "string")} = "default"
    end

    if dim == 1 then
        dim = "r";
    else
        dim = "c";
    end

    [haveStatistics, statistics] = defineStats(statistics);
    outSt = struct();
    
    if haveStatistics then
        // double
        select typeof(t)
        case {"constant", "duration", "datetime", "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64"}
            outSt = doubleSummary(outSt, t, statistics, dim)

        case "string"
            u = unique(t);
            s = size(u, "*");
            outSt.Values = u;
            select dim
            case "r"
                N = zeros(s, size(t,2));
                for i = 1:s
                    N(i,:) = sum(t == u(i), dim);
                end
            case "c"
                N = zeros(size(t,1), s);
                for i = 1:s
                    N(:,i) = sum(t == u(i), dim);
                end
            end
            outSt.Counts = N;
            outSt.NumMissing = sum(t == "<undefined>", dim);
        case "boolean"
            outSt.True = sum(t, dim);
            outSt.False = sum(t == %f, dim);
        end
    end

endfunction

function [outSt, varnames, haveStatistics, statistics, idxNumeric, idxString, idxBoolean] = %summary(t, statistics, datavars)
    arguments
        t 
        statistics {mustBeA(statistics, "string")} = "default"
        datavars {mustBeA(datavars, "string")} = "all"
    end

    // properties of table/timeseries
    props = t.props;
    descr = props.description;
    varnames = props.variableNames;
    vardescr = props.variableDescriptions;
    varunits = props.variableUnits;

    [haveStatistics, statistics] = defineStats(statistics);

    if datavars <> "all" then
        nb = members(datavars, varnames);
        if or(nb == 0) then
            error(msprintf(_("%s: Wrong value for input argument #%d: no valid variable name %s.\n"), "summary", sci2exp(datavars(nb ==0))));
        end
        varnames = datavars;
    end

    outSt = struct();
    start = 1;
    rownames = [];

    if istimeseries(t) then
        // manage rowtimes
        name = varnames(1);
        data = t.vars(1).data;
        typ = typeof(data);
        varSt = struct();
        varSt.Size = size(data);
        varSt.Type = typ;
        varSt.StartTime = props.startTime;
        varSt.SampleRate = props.sampleRate;
        varSt.TimeStep = props.timeStep;
        varSt = doubleSummary(varSt, data, statistics, "r");
        outSt(name) = varSt;
        start = 2;
    end

    idxNumeric = [];
    idxBoolean = [];
    idxString = [];

    for i = start:size(varnames, "*")
        name = varnames(i);
        data = t(name);
        typ = typeof(data);
        if typ == "constant" then
            typ = "double";
        end

        varSt = struct();
        varSt.Size = size(data);
        varSt.Type = typ;
        varSt.Description = vardescr(i);
        varSt.Units = varunits(i);

        if haveStatistics then
            // double
            select typ
            case {"double", "duration", "datetime", "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64"}
                varSt = doubleSummary(varSt, data, statistics, "r")
                idxNumeric(1, $+1) = i;

            case "boolean"
                [u, k, l, nb] = unique(data);
                varSt.True = nb(u == %t);
                varSt.False = nb(u == %f);
                idxBoolean(1, $+1) = i;

            case "string"
                [u, k, l, nb] = unique(data);
                varSt.NumMissing = sum(data == "<undefined>");
                varSt.Values = u;
                varSt.Counts = nb;
                idxString(1, $+1) = i;
            end
        end

        outSt(name) = varSt;
    end

endfunction

function st = doubleSummary(st, data, fields, dim)

    hasNaNorNaT = %f;
    isDate = isdatetime(data);
    if isDate then
        hasNaNorNaT = isnat(data);
    else
        hasNaNorNaT = isnan(data);
    end

    [val, kind] = %summaryToDouble(data);

    for f = fields
        fl = convstr(f, "l")
        try
            select fl
            case "nummissing"
                r = sum(hasNaNorNaT, dim);
            case "numunique"
                r = %numunique(val, ~hasNaNorNaT, dim);
            case "min"
                if isDate then
                    [_, idx] = min(val, dim);
                    [m,n] = size(data);
                    if dim=="r" then
                        idx = idx + ((1:n)-1)*m;
                    else
                        idx = (1:m)' + (idx-1)*m;
                    end
                    r = data(idx);
                else
                    r = min(data, dim);
                end
            case "max"
                if isDate then
                    [_, idx] = max(val, dim);
                    [m,n] = size(data);
                    if dim=="r" then
                        idx = idx + ((1:n)-1)*m;
                    else
                        idx = (1:m)' + (idx-1)*m;
                    end
                    r = data(idx);
                else
                    r = max(data, dim);
                end
            case "median"
                if type(data) == 8 then
                    r = median(data, dim);
                else
                    r = %summaryFromDouble(nanmedian, val, dim, kind, data);
                end
            case "mean"
                r = %summaryFromDouble(nanmean, val, dim, kind, data);
            case {"std", "var"}
                r = nanstdev(val, dim);
                if isdatetime(data) then
                    r = seconds(r);
                elseif isduration(data) then
                    r = milliseconds(r);
                end

                if fl == "var" then
                    r = r.^2;
                end
            case "q1"
                quartData = %summaryFromDouble(quart, val, dim, kind, data);
                if dim == "r" then
                    r = quartData(1, :);
                else
                    r = quartData(:, 1);
                end

            case "q3"
                quartData = %summaryFromDouble(quart, val, dim, kind, data);
                if dim == "r" then
                    r = quartData(3, :);
                else
                    r = quartData(:, 3);
                end
    
            case "mode"
                r = %mode(data, ~hasNaNorNaT, dim);              
            case "range"
                r = strange(data, dim);
            case "sum"
                data(hasNaNorNaT) = 0;
                r = sum(data, dim);
            case "nnz"
                r = data <> 0 & ~hasNaNorNaT;
                r = sum(r, dim);
            end
            st(f) = r;
        catch
            lasterror();
        end
    end
endfunction

function [val, kind] = %summaryToDouble(data)
    if isduration(data) then
        kind = "duration";
        val = data.duration;
    elseif isdatetime(data) then
        kind = "datetime";
        val = data.date * 86400 + data.time;
        val(isnat(data)) = %nan;
    else
        kind = "other";
        val = data;
    end
endfunction

function r = %summaryFromDouble(fun, val, dim, kind, proto)

    val = fun(val, dim);
    select kind
    case "duration"
        r = milliseconds(val);
    case "datetime"
        r = proto;
        r.date = floor(val / 86400);
        r.time = modulo(val, 86400);
    else
        r = val;
    end
endfunction

function r = %mode(x, noHasNaNorNaT, orient)
    if or(size(x) == 1) then
        if (size(x, 1) <> 1 && orient == "c") || (size(x, 2) <> 1 && orient == "r")
            r = x;
        else
            [u, _, _, counts] = unique(x(noHasNaNorNaT), orient)
            maxCount = max(counts);
            r = u(find(counts == maxCount, 1));
        end
    else
        [row, col] = size(x);
        select orient
        case "r"
            r = zeros(1, col)
            for j = 1:col
                r(j) = %mode(x(:, j), noHasNaNorNaT(:,j), orient);
            end

        case "c"
            r = zeros(row, 1);
            for i = 1:row
                r(i) = %mode(x(i, :), noHasNaNorNaT(i,:), orient);
            end
        end
    end
endfunction

function r = %numunique(x, noNaNorNaT, orient)
    if or(size(x) == 1) then
        r = length(unique(x(noNaNorNaT), orient));
    else
        [row, col] = size(x);
        select orient
        case "r"
            r = zeros(1, col)
            for j = 1:col
                r(j) = %numunique(x(:, j), noNaNorNaT(:, j), orient);
            end

        case "c"
            r = zeros(row, 1);
            for i = 1:row
                r(i) = %numunique(x(i, :), noNaNorNaT(i, :), orient);
            end
        end
    end
endfunction
