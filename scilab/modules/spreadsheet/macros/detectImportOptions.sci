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

function opts = detectImportOptions(filename, varargin)
    arguments
        filename {mustBeA(filename, "string"), mustBeScalar}
        varargin
    end

    fname = "detectImportOptions";

    if ~isfile(filename) then
        error(msprintf(_("%s: Wrong value for input argument #%d: File ""%s"" does not exist.\n"), fname, 1, filename));
    end

    if fileext(filename) == ".xlsx" then
        sheet = 1;
        rangevalue = "";

        if nargin > 2 then
            if modulo(nargin-1, 2) <> 0 then
                error(msprintf(_("%s: Wrong number of input arguments"), fname));
            end

            for i = nargin-2:-2:1
                key = varargin(i);
                if type(key) <> 10 || (type(key) == 10 && ~isscalar(key)) then
                    error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, i));
                end
                value = varargin(i+1);

                select convstr(key, "l")
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
                    if typeValue == 10 then
                        rangevalue = value;
                    elseif typeValue == 1 then
                        if and(size(value) == [2 2]) then
                            rangevalue = %matrix_to_range(value);
                        else
                            error(msprintf(_("%s: Wrong value for ""%s"" argument: Range matrix must be 2x2 [row1 col1; row2 col2].\n"), fname, key));
                        end
                    else
                        error(msprintf(_("%s: Wrong type for ""%s"" argument: Must be a string or a 2x2 matrix.\n"), fname, key));
                    end
                else
                    error(msprintf(_("%s: Unknown option ""%s"". Valid options to xlsx format are %s.\n"), fname, key, sci2exp(["sheet", "range"])));
                end
            end
        end

        s = xlsxInfo(filename);
        f = %xlsxRead(filename, sheet, rangevalue, "string");
        opts = detectImportOptionsXlsx(f);
        if type(sheet) == 10 then
            opts.sheet = sheet;
        else
            opts.sheet = s.sheet_names(sheet);
        end
        opts.range = rangevalue;    
        
    else
        delim = "";
        decimal = [];
        numHeaderLines = [];
        if nargin > 2 then
            if modulo(nargin-1, 2) <> 0 then
                error(msprintf(_("%s: Wrong number of input arguments"), fname));
            end

            for i = nargin-2:-2:1
                if type(varargin(i)) <> 10 || (type(varargin(i)) == 10 && ~isscalar(varargin(i))) then
                    error(msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), fname, i));
                end

                select convstr(varargin(i), "l")
                case "delimiter"
                    delim = varargin(i+1);
                    if type(delim) <> 10 then
                        error(msprintf(_("%s: Wrong type for %s argument #%d: A string expected.\n"), fname, "Delimiter", i+1));
                    end
                    if delim == "" then
                        error(msprintf(_("%s: Wrong value for %s argument #%d: A non-empty string expected.\n"), fname, "Delimiter", i+1));
                    end

                case "decimal"
                    decimal = varargin(i+1);
                    if type(decimal) <> 10 then
                        error(msprintf(_("%s: Wrong type for %s argument #%d: A string expected.\n"), fname, "Decimal", i+1));
                    end
                    if decimal == "" then
                        error(msprintf(_("%s: Wrong value for %s argument #%d: A non-empty string expected.\n"), fname, "Decimal", i+1));
                    end

                case "numheaderlines"
                    numHeaderLines = varargin(i+1);
                    if type(numHeaderLines) <> 1 then
                        error(msprintf(_("%s: Wrong type for %s argument #%d: A double expected.\n"), fname, "NumHeaderLines", i+1));
                    end
                    if size(numHeaderLines, "*") > 1 then
                        error(msprintf(_("%s: Wrong size for %s argument #%d: A non-empty value expected.\n"), fname, "NumHeaderLines", i+1));
                    end
                end
            end
        end

        f = mgetl(filename);
        opts = detectImportOptionsCSV(f, delim, decimal, numHeaderLines);
    end

endfunction
