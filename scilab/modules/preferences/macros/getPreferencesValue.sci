// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
// Copyright (C) 2017 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function values = getPreferencesValue(xpath, attributes, doc)

    warnobsolete("xmlGetValues", "6.1");
    rhs = argn(2);

    if (rhs ~= 2 & rhs ~= 3) then
        msg = _("%s: Wrong number of input arguments: %d or %d expected.\n")
        error(msprintf(msg, "getPreferencesValue", 2, 3));
    end

    if type(xpath) <> 10 then
        msg = _("%s: Wrong type for input argument #%d: String expected.\n")
        error(msprintf(msg, "getPreferencesValue", 1));
    end

    if type(attributes) <> 10 then
        msg = _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n")
        error(msprintf(msg, "getPreferencesValue", 2));
    end

    if rhs == 2 then
        doc = SCIHOME + "/XConfiguration.xml"
    end
    doc0 = doc
    delDoc = (rhs == 2 | type(doc0)==10)
    if type(doc)==10 then
        if isfile(doc)
            try
                doc = xmlRead(doc0);
            catch
                msg = _("%s: Invalid ""%s"" file.\n")
                error(msprintf(msg, "getPreferencesValue", tokens(doc0,["/" "\"])($)));
            end
        else
            msg = _("%s: The file ""%s"" does not exist.\n")
            error(msprintf(msg, "getPreferencesValue", doc));
        end
    elseif typeof(doc) ~= "XMLDoc" then
        msg = _("%s: Wrong type for input argument #%d: A XMLDoc expected.\n")
        error(msprintf(msg, "getPreferencesValue", 3));
    end

    try
        xp = xmlXPath(doc, xpath);
    catch
        if delDoc then
            xmlDelete(doc);
        end
        msg = gettext("%s: Invalid XPath request.\n")
        error(msprintf(msg, "getPreferencesValue"));
    end

    if xp.size == 0 then
        if delDoc then
            xmlDelete(doc);
        end
        msg = gettext("%s: Invalid XPath request.")
        error(msprintf(msg, "getPreferencesValue"));
    end

    values = [];
    for i = 1 : xp.size
        node = xp(i);
        if node.type ~= "XML_ELEMENT_NODE" then
            if delDoc then
                xmlDelete(doc);
            end
            msg = gettext("%s: Target node is not a XML_ELEMENT_NODE.")
            error(msprintf(msg, "getPreferencesValue"));
        end

        attr = node.attributes;
        for j = 1:size(attributes,"*")
            a = attributes(j)
            v = attr(a);
            if v ~= [] then
                values(i,j) = v;
            else
                if delDoc then
                    xmlDelete(doc);
                end
                msg = gettext("%s: Invalid attribute name: %s.")
                error(msprintf(msg, "getPreferencesValue", a));
            end
        end
    end
    if xp.size==1 then
        values = matrix(values, size(attributes));
    end
    if delDoc then
        xmlDelete(doc);
    end
endfunction
