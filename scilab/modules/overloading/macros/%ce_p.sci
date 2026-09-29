// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %ce_p(ce)
    function idx = computeIndex(dims)
        function c = combine(x, y)
            cx = size(x, "c")
            cy = size(y, "c")
            c = [x .*. ones(1, cy); ones(1, cx) .*. y]
        endfunction

        idx = 1:dims(3);
        for i = 4:size(dims, "*")
            idx = combine(1:dims(i), idx);
        end
        idx = idx';
    endfunction

    function printMatrix(str)
        s = max(length(str), "r");

        // pad each column to its max width for alignment
        for j = 1:size(str, 2)
            f = sprintf("%%%ds", -s(j));
            for i = 1:size(str, 1)
                str(i, j) = sprintf(f, str(i, j));
            end
        end

        //disp following lines limit
        maxL = lines()(1);

        //[12 14 15 18 21 8 43]
        listcol = list(1);
        nb = 2

        for i = 1:size(s, "*")
            if s(i) + nb > maxL then
                if i - 1 > listcol($) then
                    // at least two columns fit in the closing group
                    listcol($+1) = i-1;
                    nb = 2 + s(i) + 4;
                elseif i > listcol($) then
                    // exactly one column fits in the closing group
                    listcol($+1) = i;
                    nb = 2 + s(i) + 4;
                else
                    // column i alone already exceeds the console width: it must be
                    // its own group, there is nothing to close before it
                    listcol($+1) = i+1;
                    nb = 2;
                end
            else
                nb = nb + s(i) + 4;
            end
        end

        if listcol($) <> size(s, "*") + 1 then
            listcol($+1) = size(s, "*") + 1;
        end

        if length(listcol) > 2 then
            for i = 2:length(listcol)
                if listcol(i-1) == listcol(i)-1 then
                    printf("\n         column %d\n\n", listcol(i)-1);
                else
                    printf("         column %d to %d\n\n", listcol(i-1), listcol(i)-1);
                end

                tmp = "  " + strcat(str(:, listcol(i-1):listcol(i)-1), "    ", "c");
                printf("%s\n", tmp);
                printf("\n");
            end
        else
            tmp = "  " + strcat(str, "    ", "c");
            printf("%s\n", tmp);
        end
    endfunction

    str = %ce_string(ce);

    // Add quotes around scalar string elements for display
    for i = 1:size(ce, "*")
        if type(ce{i}) == 10 && size(ce{i}, "*") == 1 then
            str(i) = """" + str(i) + """";
        end
    end

    if ndims(str) > 2 then
        tuples = computeIndex(size(str));
        for i = 1:size(tuples, "r")
            t = tuples(i, :)($:-1:1);
            printf("(" + strcat([":", ":" string(t)], ",") + ")\n\n");
            d = vec2list(t, ones(size(t, "c"), 2))
            printMatrix(str(:, :, d(:)));
        end
    else
        printMatrix(str);
    end

endfunction
