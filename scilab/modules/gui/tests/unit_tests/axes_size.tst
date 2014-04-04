// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

function f = createFigure(num, flags)
    g = gdf();
    if flags(5) == %f then
        f = figure("figure_id", num, "dockable", flags(1), "menubar_visible", flags(2), "toolbar_visible", flags(3), "infobar_visible", flags(4));
        assert_checkequal(f.menubar_visible == "on", flags(2));
        assert_checkequal(f.toolbar_visible == "on", flags(3));
        assert_checkequal(f.infobar_visible == "on", flags(4));
        assert_checkequal(f.axes_size, g.axes_size);
        f.axes_size = [800 800];
        assert_checkequal(f.axes_size, [800 800]);
    else
        f = figure("figure_id", num, "dockable", flags(1), "menubar_visible", flags(2), "toolbar_visible", flags(3), "infobar_visible", flags(4), "axes_size", [800, 800]);
        assert_checkequal(f.menubar_visible == "on", flags(2));
        assert_checkequal(f.toolbar_visible == "on", flags(3));
        assert_checkequal(f.infobar_visible == "on", flags(4));
        assert_checkequal(f.axes_size, [800 800]);
        f.axes_size = g.axes_size;
        assert_checkequal(f.axes_size, g.axes_size);
    end
    delete(f);
endfunction

function createFigureXML(num, flags)
    g = gdf();
    for i = 1:size(flags, "*")
        if flags(i) == %t then
            flagsStr(i) = "on";
        else
            flagsStr(i) = "off";
        end
    end

    if flags(5) == %f then
        str = [ ...
        "<scilabgui usedeprecatedskin=""off"" version=""1.0"">";
        "<figure dockable=""" + flagsStr(1) + """ menubar_visible=""" + flagsStr(2) + """ toolbar_visible=""" + flagsStr(3) + """ infobar_visible=""" + flagsStr(4) + """/>";
        "</scilabgui>"];

        doc = xmlReadStr(str);
        xmlWrite(doc, TMPDIR + "/figure.xml");
        f = loadGui(TMPDIR + "/figure.xml");
        assert_checkequal(f.menubar_visible == "on", flags(2));
        assert_checkequal(f.toolbar_visible == "on", flags(3));
        assert_checkequal(f.infobar_visible == "on", flags(4));
        assert_checkequal(f.axes_size, g.axes_size);
        f.axes_size = [800 800];
        assert_checkequal(f.axes_size, [800 800]);
    else
        str = [ ...
        "<scilabgui usedeprecatedskin=""off"" version=""1.0"">";
        "<figure dockable=""" + flagsStr(1) + """ menubar_visible=""" + flagsStr(2) + """ toolbar_visible=""" + flagsStr(3) + """ infobar_visible=""" + flagsStr(4) + """ axes_size=""800,800""/>";
        "</scilabgui>"];

        doc = xmlReadStr(str);
        xmlWrite(doc, TMPDIR + "/figure.xml");
        f = loadGui(TMPDIR + "/figure.xml");
        assert_checkequal(f.menubar_visible == "on", flags(2));
        assert_checkequal(f.toolbar_visible == "on", flags(3));
        assert_checkequal(f.infobar_visible == "on", flags(4));
        assert_checkequal(f.axes_size, [800 800]);
        f.axes_size = g.axes_size;
        assert_checkequal(f.axes_size, g.axes_size);
    end
    delete(f);
endfunction

flags = [
    %f %f %f %f %f;
    %f %f %f %f %t;
    %f %f %f %t %f;
    %f %f %f %t %t;
    %f %f %t %f %f;
    %f %f %t %f %t;
    %f %f %t %t %f;
    %f %f %t %t %t;
    %f %f %f %f %f;
    %f %t %f %f %t;
    %f %t %f %t %f;
    %f %t %f %t %t;
    %f %t %t %f %f;
    %f %t %t %f %t;
    %f %t %t %t %f;
    %f %t %t %t %t;
    %t %f %f %f %f;
    %t %f %f %f %t;
    %t %f %f %t %f;
    %t %f %f %t %t;
    %t %f %t %f %f;
    %t %f %t %f %t;
    %t %f %t %t %f;
    %t %f %t %t %t;
    %t %f %f %f %f;
    %t %t %f %f %t;
    %t %t %f %t %f;
    %t %t %f %t %t;
    %t %t %t %f %f;
    %t %t %t %f %t;
    %t %t %t %t %f;
    %t %t %t %t %t];

for i = 1:size(flags, "r")
    createFigure(i, flags(i, :));
end

for i = 1:size(flags, "r")
    createFigureXML(i, flags(i, :));
end
