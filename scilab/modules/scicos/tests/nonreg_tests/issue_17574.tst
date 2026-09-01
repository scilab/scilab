// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17574 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17574
//
// <-- Short Description -->
// When creating an empty Xcos diagram, adding a block, and saving to SSP file,
// then opening the SSD file contained in the SSP file, the SystemGeometry has
// wrong y1 value (INT_MIN: -2147483648).
//

loadXcosLibs()

// Create a simple diagram with one block
blk = SUMMATION("define");
blk.graphics.pin = 2;
blk.graphics.pout = 1;

// Create a diagram with the block
scs_m = scicos_diagram(objs=list(blk));

// Save to a temporary SSP file
ssp_file = fullfile(TMPDIR, "test_issue_17574.ssp");
scicosDiagramToScilab(ssp_file, scs_m);

// SSP is a ZIP archive containing SSD files
ssd_file = fullfile(TMPDIR, "test_issue_17574.ssd");
movefile(decompress(ssp_file), ssd_file);

// Read the SSD file to check its content
content = xmlRead(ssd_file);

// Check that the SystemGeometry y1 attribute is NOT INT_MIN (-2147483648)
// The pattern should be: <ssd:SystemGeometry ... y1="..." ...>
// We need to extract the y1 value
node = xmlXPath(content, "//ssd:SystemGeometry", ["ssd", "http://ssp-standard.org/SSP1/SystemStructureDescription"]);

y1 =  evstr(node(1).attributes.y1);

// Check that y1 is not INT_MIN
assert_checktrue(y1 <> -2147483648);

// Also check that y1 is a reasonable value (not extremely large negative)
// For a diagram with a block, y1 should be negative or small positive, not -1.79769e+308
assert_checktrue(y1 >= -1e10);

// Clean up
mdelete(ssp_file);
mdelete(ssd_file);
