// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Get preferences values
// - xpath is something like "//web/body/proxy", the target must be a single node
// - attributes is a matrix of strings containing the attributes names
// - doc (optional): the prefs xml document where to get the values
//
function values = getPreferencesValue(xpath, attributes, doc)
    rhs = argn(2);

    if (rhs ~= 2 & rhs ~= 3) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "getPreferencesValue", 2, 3));
    end

    if type(xpath) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "getPreferencesValue", 1));
    end

    if type(attributes) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "getPreferencesValue", 2));
    end

    if rhs == 2 then
        try
            doc = xmlRead(SCIHOME + "/XConfiguration.xml");
        catch
            error(msprintf(gettext("%s: Invalid XConfiguration.xml file.\n"), "getPreferencesValue"));
        end
    elseif typeof(doc) ~= "XMLDoc" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A XMLDoc expected.\n"), "getPreferencesValue", 3));
    end

    try
        xp = xmlXPath(doc, xpath);
    catch
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Invalid XPath request.\n"), "getPreferencesValue"));
    end

    if xp.size ~= 1 then
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Invalid XPath request."), "getPreferencesValue"));
    end

    node = xp(1);
    if node.type ~= "XML_ELEMENT_NODE" then
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Target node is not a XML_ELEMENT_NODE."), "getPreferencesValue"));
    end

    attr = node.attributes;
    values = [];
    for a = attributes(:)'
        v = attr(a);
        if v ~= [] then
            values = [values v];
        else
            if rhs == 2 then
                xmlDelete(doc);
            end
            error(msprintf(gettext("%s: Invalid attribute name: %s."), "getPreferencesValue", a));
        end
    end

    if rhs == 2 then
        xmlDelete(doc);
    end
    values = matrix(values, size(attributes));
endfunction
