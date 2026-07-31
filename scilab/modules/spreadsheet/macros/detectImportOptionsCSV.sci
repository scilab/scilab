function opts = detectImportOptionsCSV(f, delim, decimal, numHeaderLines)
    if nargin == 1 then
        delim = "";
        decimal = [];
        numHeaderLines = [];
    end

    opts = struct();

    // trim trailing empty lines (find last non-empty)
    totalLines = size(f, "r");
    lastNonEmpty = totalLines;
    while lastNonEmpty > 0 && f(lastNonEmpty) == ""
        lastNonEmpty = lastNonEmpty - 1;
    end
    totalLines = lastNonEmpty;

    if numHeaderLines <> [] then
        l = 1:numHeaderLines;
        header = f(l);
    else
        // detect header
        [header, c , l] = detectHeader(f);
    end

    // build datalines without copying the full vector
    nbheader = size(l, "*");
    if l <> [] then
        // datalines = all line indices except header lines
        datalines = setdiff(1:totalLines, l);
    else
        datalines = 1:totalLines;
    end

    // use a small sample for delimiter/type detection (avoid passing full f)
    dataStart = datalines(1);
    sampleEnd = min(dataStart + 49, totalLines);
    fsample = f(dataStart:sampleEnd);

    if delim == "" then
        if decimal == [] then
            [delim, decimal] = detectDelimiter(fsample);
        else
            delim = detectDelimiter(fsample);
        end
    else
        if decimal == [] then
            [a, decimal] = detectDelimiter(fsample, delim);
        end
    end

    // detect variable names and type
    test = csvTextScan(f(dataStart), delim, decimal);
    variableNames = [];
    index = [];
    hasheader = %f;

    if totalLines > 1 then
        if and(isnan(test)) then
            h = csvTextScan(f(dataStart), delim, decimal, "string");
            variableNames = h;
            index = find(variableNames == "");
            if index <> [] then
                variableNames(index) = "Var" + string(index);
            end
            datalines(1) = [];
            hasheader = %t;
        end
    end

    // csvTextScan on sample for type detection
    if hasheader then
        sampleStart = datalines(1);
    else
        sampleStart = dataStart;
    end
    sampleEnd2 = min(sampleStart + 49, totalLines);
    h = csvTextScan(f(sampleStart:sampleEnd2), delim, decimal, "string");

    variableTypes = emptystr(variableNames);
    inputFormat = [];

    for i = 1:size(h, 'c')
        // types managed : datetime, double, string
        if h(1, i) == "" then
            variableTypes(1,i) = "string";
            inputFormat(1,i) = "";
            mat = h(:, i);
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
            [infmt, _typ] = detectFormatDatetime(h(:, i))
            variableTypes(1,i) = _typ;
            inputFormat(1,i) = infmt
        end
    end

    opts.variableNames = variableNames;
    opts.variableTypes = variableTypes;
    opts.delimiter = delim;
    opts.decimal = decimal;
    opts.datalines = datalines;
    opts.header = header;
    opts.inputFormat = inputFormat;
    opts.emptyCol = index;
endfunction