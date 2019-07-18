// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function xmlSetValues(xpath, kv, doc)
	// Sets attribute=values pairs for a given tag in a XML document.
	// - xpath is something like "//web/body/proxy", the target must be a single node/tag
	// - kv is a matrix of strings 2xN: keys are in the first row and values in the
	//   seconde one.
	// - doc (optional): the prefs xml document where to set the values
	//                   (take care: in this case xmlWrite is not called)
	//
    rhs = argn(2);
	fname = "xmlSetValues"

    if (rhs ~= 2 & rhs ~= 3) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), fname, 2, 3));
    end

    if type(xpath) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1));
    end

    if type(kv) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 2));
    end

    if (size(kv, "r") ~= 2) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: a 2xN matrix expected.\n"), fname, 2));
    end

    if rhs == 2 then
        try
            doc = xmlRead(SCIHOME + "/XConfiguration.xml");
        catch
            error(msprintf(gettext("%s: Invalid XConfiguration.xml file.\n"), fname));
        end
    elseif typeof(doc) ~= "XMLDoc" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A XMLDoc expected.\n"), fname, 3));
    end

    try
        xp = xmlXPath(doc, xpath);
    catch
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Invalid XPath request.\n"), fname));
    end

    if xp.size ~= 1 then
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Invalid XPath request."), fname));
    end

    node = xp(1);
    if node.type ~= "XML_ELEMENT_NODE" then
        if rhs == 2 then
            xmlDelete(doc);
        end
        error(msprintf(gettext("%s: Target node is not a XML_ELEMENT_NODE."), fname));
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
            error(msprintf(gettext("%s: Invalid attribute name: %s."), fname, kv(1, i)));
        end
    end

    if rhs == 2 then
        xmlWrite(doc);
        xmlDelete(doc);
    end
endfunction
