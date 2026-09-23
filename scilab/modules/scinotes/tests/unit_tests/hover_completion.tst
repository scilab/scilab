// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Documented hover, help links, and completion.
//
// 1. Hover linspace, sin, mean, plot, gcf, and color.
// 2. Each documented symbol must show its Scilab help.
// 3. Follow several links from the help: they must use Scilab "doc" links,
//    display readable labels/URIs.
// 4. Right-click inside a hover must not open the SciNotes context menu.
// 5. Hover a documented macro such as plot, Ctrl+click it to open its source,
//    then return here and hover it again: the complete help must remain visible
//    and the source filename must be appended after it.
//    In the opened source, hover an internal call to the same macro: it must
//    also keep the complete help instead of showing only the local prototype.

angles = linspace(0, 2 * %pi, 20);
values = sin(angles);
average = mean(values);

// 6. Request completion after a partially typed "plot".
// 7. Without executing the file, type "angl" on a new line: angles must be
//    proposed as a variable from the current document, without duplicates.
// 8. Execute the file, then request completion after "figure.": public fields
//    and methods of the instantiated object must be proposed.
plot(angles, values);
figure = gcf();
figure.background = color("white");

plot()
