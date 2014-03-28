// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Set preferences values
// - xpath is something like "//web/body/proxy", the target must be a single node
// - kv is a matrix of strings 2xN: keys are in the first row and values in the
//   seconde one.
// - doc (optional): the prefs xml document where to set the values
//                   (take care: in this case xmlWrite is not called)
//
function setPreferencesValue(xpath, kv, doc)
    rhs = argn(2);

    if (rhs ~= 2 & rhs ~= 3) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "setPreferencesValue", 2, 3));
    end

    if type(xpath) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "setPreferencesValue", 1));
    end

    if type(kv) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "setPreferencesValue", 2));
    end

    if (size(kv, "r") ~= 2) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: a 2xN matrix expected.\n"), "setPreferencesValue", 2));
    end

    if rhs == 2 then
        try
            doc = xmlRead(SCIHOME + "/XConfiguration.xml");
        catch
            error(msprintf(gettext("%s: Invalid XConfiguration.xml file.\n"), "setPreferencesValue"));
        end
    elseif typeof(doc) ~= "XMLDoc" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A XMLDoc expected.\n"), "setPreferencesValue", 3));
    end

    try
        xp = xmlXPath(doc, xpath);
    catch
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Invalid XPath request.\n"), "setPreferencesValue"));
    end

    if xp.size ~= 1 then
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Invalid XPath request."), "setPreferencesValue"));
    end

    node = xp(1);
    if node.type ~= "XML_ELEMENT_NODE" then
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Target node is not a XML_ELEMENT_NODE."), "setPreferencesValue"));
    end

    attr = node.attributes;
    for i = 1:size(kv, "c")
        v = attr(kv(1, i));
        if v ~= [] then
            attr(kv(1, i)) = kv(2, i);
        else
            if rhs == 2 then
                xmlDelete(doc);
            end
            error(msprintf(gettext("%s: Invalid attribute name: %s."), "setPreferencesValue", kv(1, i)));
        end
    end

    if rhs == 2 then
        xmlWrite(doc);
        xmlDelete(doc);
    end
endfunction
