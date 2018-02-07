// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// INTERNAL function to be called by plot(), bar(), barh(), nicholschart(),
// hallchart(), etc..
function rgb = iscolor(C, acceptedFormats)
    // C: scalar, vector, matrix.. of colors specifications:
    //    - integers: => % T if > 0 and <= cmapSize
    //                       default colormap used = gcf().color_map
    //    - strings: can mix predefined colors names and "#RRGGBB" specifications
    //    - (Nx3) [r g b] matrix, with 0 =< r,g,b <= 1 as in the color map.
    // acceptedFormats: single string (optional) made of 0 to 4 chars in any
    //      order among
    //     "1" : indices accepted
    //     "." : [r g b] expected
    //     "#" : "#RRGGBB" accepted (case insensitive)
    //     "a" : colors names accepted
    //   Default = "1.#a"  (all)
    //
    // rgb: (Nx3) matrix of 0 <= decimal numbers <= 1, with N = size(C, "*")
    //      If C(i) is not a known color specification, rgb(i,:) = [-1 -1 -1]

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
        acceptedFormats = acceptedFormats(1)
    else
        acceptedFormats = "1.#a";
    end

    // PROCESSING
    // ----------
    if type(C)==10 then
        rgb = -ones(size(C,"*"), 3);
        C = stripblanks(C);

        // "#RRGGBB" cases
        if grep(acceptedFormats, "#")~=[]
            k = grep(C, "/^#[0-9a-f]{6}$/i", "r");
            if k~=[]
                s = strcat(part(C(k), 2:7));
                s = matrix(strsplit(s, 2:2:length(s)-1), -1,3);
                rgb(k,:) = hex2dec(s)/255;
            end
        end
        // Predefined named colors
        if grep(acceptedFormats, "a")~=[]
            k = grep(C, "/^[a-z0-9 ]+$/i", "r");
            for i = k
                r = name2rgb(C(i));
                if r~=[]
                    rgb(i,:) = r / 255;
                end
            end
        end
    else
        C = real(C)

        // [r g b] case
        if size(C,"c")==3 & grep(acceptedFormats, ".")~=[]
            rgb = -ones(C);
            k = find(and(C>=0 & C<=1, "c"));
            rgb(k,:) = C(k,:);
        else
            // Colors indices
            rgb = -ones(size(C, "*"), 3);
            if grep(acceptedFormats, "1")~=[]
                if winsid()==[]
                    cmap = gdf().color_map;
                else
                    cmap = gcf().color_map;
                end
                k = find(C==int(C) & C>0 & C<=size(cmap,"r"))
                rgb(k,:) = cmap(C(k),:);
            end
        end
    end
endfunction
