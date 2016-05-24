// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre Ledru
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

prot = funcprot();
funcprot(0);

function diazepam_demo()

    my_test_fig = figure(100001);
    my_test_fig.figure_name = "Display of a molecule of diazepam from a CML file (XML)";

    filename = "diazepam.dem.sce";
    demo_viewCode(filename);

    listDisplayed = struct();

    function displayAtom(atomName)
        if ~isfield(listDisplayed,atomName) then

            listDisplayed(atomName)=%t;
            [coords elementType] = getAtomInfos(atomName);

            rgbX = xmlXPath(xmlAtomsColors, "//atom[@name=''" + elementType + "'']/@*[name()=''R'' or name()=''G'' or name()=''B'']");
            rgb = xmlAsNumber(rgbX);

            xfarc(coords(1) - 0.5, coords(2) + 0.5, 1, 1, 0, 360 * 64);
            cercle = get("hdl");
            cercle.background = color(rgb(1), rgb(2), rgb(3));
            xstring(coords(1), coords(2) - 1, elementType);
        end
    endfunction

    function [coord, elemType] = getAtomInfos(atomName)
        xp = xmlXPath(xmlFile, "//cmldoc/molecule/atomArray/atom[@id=''" + atomName + "'']");
        atomX = xmlXPath(xp(1), "float");
        coord = xmlAsNumber(atomX);
        elemType = xmlXPath(xp(1), "string(string[@builtin=''elementType''])");
    endfunction

    xmlAtomsColors = xmlRead(SCI + "/modules/xml/demos/atoms_colors.xml");
    xmlFile = xmlRead(SCI + "/modules/xml/demos/diazepam.xml");

    bonds = xmlXPath(xmlFile, "//cmldoc/molecule/bondArray/bond");
    nbBonds = size(bonds);
    coords = [];
    drawlater();
    plot2d([], [], -1, "030", " ", [-6, -6, 6, 6]);
    f = gcf();
    f.background = color(255, 255, 255);
    f.anti_aliasing = "8x";

    for i = 1:nbBonds(2)
        bondCouple = xmlAsText(xmlXPath(bonds(i), "string[@builtin=''atomRef'']"));
        order = xmlAsNumber(xmlXPath(bonds(i), "string[@builtin=''order'']"));
        x1 = getAtomInfos(bondCouple(1))';
        x2 = getAtomInfos(bondCouple(2))';
        if order ~= 1 then
            u = x2 - x1;
            u = [0 1 ; -1 0] * u / norm(u);
        end

        if order == 1 then
            coords(:, $ + 1) = x1;
            coords(:, $ + 1) = x2;
        elseif order == 2 then
            coords(:, $ + 1) = x1 - 0.1 * u;
            coords(:, $ + 1) = x2 - 0.1 * u;
            coords(:, $ + 1) = x1 + 0.1 * u;
            coords(:, $ + 1) = x2 + 0.1 * u;
        elseif order == 3 then
            coords(:, $ + 1) = x1;
            coords(:, $ + 1) = x2;
            coords(:, $ + 1) = x1 - 0.1 * u;
            coords(:, $ + 1) = x2 - 0.1 * u;
            coords(:, $ + 1) = x1 + 0.1 * u;
            coords(:, $ + 1) = x2 + 0.1 * u;
        end
    end
    coords = coords(:, 2:$);

    xsegs(coords(1, :), coords(2, :));

    for i = 1:nbBonds(2)
        bondCouple = xmlAsText(xmlXPath(bonds(i), "string[@builtin=''atomRef'']"));
        displayAtom(bondCouple(1));
        displayAtom(bondCouple(2));
    end

    drawnow();

    xmlDelete(xmlAtomsColors, xmlFile);

endfunction

funcprot(prot);

diazepam_demo();
clear diazepam_demo;
