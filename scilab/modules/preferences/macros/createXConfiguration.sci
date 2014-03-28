// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// Create the XConfiguration.xml if it is not present in SCIHOME
// This function is useful when Scilab is launched in nwni mode for the first
// time (atoms could be used in this mode and need proxy settings which are
// stored in the prefs file).
//
function createXConfiguration()
    path = SCIHOME + "/XConfiguration.xml";
    if fileinfo(path) ~= [] then
        return
    end;

    if isdir(SCIHOME) then
        try
            fd = mopen(SCIHOME + "/test", "wb");
            mclose(fd);
            mdelete(SCIHOME + "/test")
        catch
            return
        end
    else
        return
    end

    doc = xmlRead(SCI + "/modules/preferences/etc/XConfiguration.xml");
    modules = ls(SCI + "/modules/*");
    xmls = [];
    for m = modules'
        xmls = [xmls ; ls(m + "/etc/XConfiguration-*.xml")];
    end

    children = doc.root.children;

    meta = "ctrl";
    if getos() == "Darwin" then
        meta = "meta";
    end

    for xml = xmls'
        xdoc = xmlRead(xml);
        if basename(xml) == "XConfiguration-general" then
            xp = xmlXPath(xdoc, "//shortcuts/body/actions/action-folder/action[contains(@key,''OSSCKEY'')]");
            for i = 1:xp.size
                e = xp(i);
                attr = e.attributes;
                attr.key = strsubst(attr.key, "OSSCKEY", meta);
            end
        end
        pos = xmlXPath(doc, "count(//" + xdoc.root.name + "/preceding-sibling::*)+1");
        if children(pos).name == xdoc.root.name then
            children(pos) = xdoc.root;
        end
        xmlDelete(xdoc);
    end

    xmlWrite(doc, path, %T);
    xmlDelete(doc);
endfunction

