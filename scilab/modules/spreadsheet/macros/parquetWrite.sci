// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Valentin MULLER
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
// For more information, see the COPYING file which you should have received
// along with this program.

function parquetWrite(filename, t)
    arguments
        filename {mustBeA(filename, "string"), mustBeScalar}
        t {mustBeA(t, ["table", "timeseries"])}
    end

    ext = fileext(filename);
    if and(ext ~= [".parquet", ".arrow"]) then
        error(msprintf(_("%s: Wrong extension for input argument #%d: Supported extensions are %s.\n"), "parquetWrite", 1, sci2exp(["parquet", "arrow"])));
    end

    columnNames = t.props.variableNames;
    cols = size(columnNames, "*");
    data = t.vars.data;
    if typeof(data) <> "list" then
        // if table has one column
        data = list(data);
    end

    columnTypes = emptystr(1, cols);
    for i = 1:cols
        col_data = data(i);
        col_type = type(col_data);
        select col_type
        case 1 then
            columnTypes(i) = "double"; // double : 0
        case 8 then
            columnTypes(i) = typeof(col_data);
        case 4 then
            columnTypes(i) = "bool"; // boolean : 10
        case 10 then
            columnTypes(i) = "string"; // string : 12
        case 17 then
            if isdatetime(col_data) then
                columnTypes(i) = "datetime"; // timestamp / datetime : 13
                data(i) = convert_datetime_column(col_data);
            elseif isduration(col_data) then
                columnTypes(i) = "duration";
                data(i) = col_data.duration;
            end
        else
            //columnTypes(i) = 16; // unknown
            error(msprintf("Warning: Column %d has unsupported type %d, writing as unknown type\n", i, col_type));
        end
    end

    %parquetWrite(filename, ext, data, columnNames, columnTypes);
endfunction

function col_double = convert_datetime_column(col_data)
    [r, c] = size(col_data);
    col_double = zeros(r, c);
    epoch_datenum = datenum(1970, 1, 1, 0, 0, 0);
    mask = ~isnat(col_data);
    
    if or(mask) then
        current_datenum = datenum(col_data(mask));
        days_diff = current_datenum - epoch_datenum;
        col_double(mask) = days_diff * 86400 * 1000000;
    end

    col_double(~mask) = %nan;
endfunction

