// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018, 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// INTERNAL function to be called by plot(), bar(), barh(), nicholschart(),
// hallchart(), etc..
function colors = iscolor(C, acceptedFormats)
    // C: scalar, vector, matrix.. of colors specifications:
    //    - integers: => is a color if -2,-1, or > 0 and <= cmapSize
    //                       default colormap used = gcf().color_map
    //    - strings: can mix predefined colors names and "#RRGGBB" specifications.
    //               Shortcuts from the standard list "red" "green" "blue"
    //               "cyan" "magenta" "yellow" "black" "white" "k" are supported,
    //               like "ma" for magenta or "b" for blue.
    //    - (Nx3) [r g b] matrix, with 0 =< r,g,b <= 1 as in the color map.
    // acceptedFormats: single string (optional) made of 0 to 4 chars in any
    //      order among
    //     "1" : indices accepted
    //     "." : [r g b] expected
    //     "#" : "#RRGGBB" accepted (case insensitive)
    //     "a" : colors names accepted
    //   Default = "1.#a"  (all)
    //
    // colors:
    //    - if C are indices: column of indices
    //    - else: (Nx3) matrix of 0 <= decimal numbers <= 1,
    //        with N = numbers of colors defined through C.
    //    If C(i) or C(i,:) is not a known color specification,
    //    colors(i,:) = %nan
    //    Warning: the output format can't be forced by specifying a
    //     unique type of input. For instance, iscolor(4,"a") will
    //     return %nan (because 4 is recognized as an index),
    //     NOT [%nan %nan %nan] despite only "a" is accepted.

    fname = "iscolor"

    // CHECKING INPUT ARGUMENTS
    // ------------------------
    rhs = argn(2)
    if rhs<1 | rhs>3 then
        msg = _("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, fname, 1, 3))
    end
    if ~or(type(C)==[1 10]) then
        msg = _("%s: Argument #%d: Numbers or text expected.\n")
        error(msprintf(msg, fname, 1, 3))
    end
    if type(C)==1 then
        if ~isreal(C,0) then
            msg = _("%s: Argument #%d: Decimal number(s) expected.\n")
            error(msprintf(msg, fname, 1))
        else
            C = real(C)
        end
    end
    if isdef("acceptedFormats", "l") then
        if type(acceptedFormats)~= 10
            msg = _("%s: Argument #%d: Text(s) expected.\n")
            error(msprintf(msg, fname, 2))
        end
        acceptedFormats = convstr(acceptedFormats(1))
        if grep(acceptedFormats,"/^[a.#1]+$/","r")==[]
            msg = _("%s: Argument #%d: Characters in {a.#1} expected.\n")
            error(msprintf(msg, fname, 2))
        end
    else
        acceptedFormats = "1.#a";
    end

    // PROCESSING
    // ----------
    //tmp = strsubst(strsubst(acceptedFormats, "a",""),"#","");
    if type(C)==10 // | tmp=="" then
        colors = %nan * ones(size(C,"*"), 3);
        C = stripblanks(convstr(C));

        // "#RRGGBB" cases
        if grep(acceptedFormats, "#")~=[]
            k = grep(C, "/^#[0-9a-f]{6}$/", "r");
            if k~=[]
                s = strcat(part(C(k), 2:7));
                s = matrix(strsplit(s, 2:2:length(s)-1), -1,3);
                colors(k,:) = hex2dec(s)/255;
            end
        end

        // Predefined named colors
        if grep(acceptedFormats, "a")~=[]
            shortList = ["red" "green" "blue" "cyan" "magenta" "yellow" "black" "white"]
            k = grep(C, "/^[a-z0-9 ]+$/i", "r");
            for i = k
                r = name2rgb(C(i));
                if r~=[]
                    colors(i,:) = r / 255;
                else
                    // shortcuts in the standard short list
                    tmp = grep(shortList,"/^"+C(i)+"/","r")
                    if tmp <> []
                        colors(i,:) = name2rgb(shortList(tmp(1))) / 255
                    elseif C(i)=="k"
                        colors(i,:) = [0 0 0]
                    end
                end
            end
        end
    else
        C = real(C)

        if size(C,"c") <> 3 | (or(C<0 | C>1) & and(C==int(C))) then
            // Colors indices
            colors = %nan * ones(size(C, "*"), 1);
            if grep(acceptedFormats, "1")~=[]
                if winsid()==[]
                    cmap = gdf().color_map;
                else
                    cmap = gcf().color_map;
                end
                k = find(C==int(C) & C>-3 & C<>0 & C<=size(cmap,"r"))
                colors(k) = C(k)
            end
        else
            // [r g b] case
            colors = %nan * ones(C);
            if grep(acceptedFormats, ".")~=[]
                k = find(and(C>=0 & C<=1, "c"));
                colors(k,:) = C(k,:);
            end
        end
    end
endfunction
