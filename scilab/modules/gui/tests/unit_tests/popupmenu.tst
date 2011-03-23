// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Unitary tests for Scilab Java popupmenu

// Exec tests common to all uicontrols
clear uicontrol_generic_test
exec("SCI/modules/gui/tests/unit_tests/uicontrol_generic_test.sci");
if uicontrol_generic_test("popupmenu") <> 0 then pause; end

// Default relief value
h = uicontrol("parent", scf(), "style", "popupmenu");
if get(h, "relief") <> "flat" then pause; end

// --- Value tests ---
// Default value
if ~isempty(get(h, "value")) then pause; end

set(h, "string", "Item1|Item2"); // Item has to be set before selecting it
set(h, "value", 1);
if get(h, "value") <> 1 then pause; end

// TODO return an error
//set(h, "value", [1 2 3]);
//if ~and(get(h, "value") == [1 2 3]) then pause; end

//set(h, "value", []);
//if ~isempty(get(h, "value")) then pause; end

// TODO test with wrong values

// Tests for multiline String property
set(h, "string", "test|for|multiline|string");
if or(get(h, "string") <> ["test","for","multiline","string"]) then pause; end

// Row vector
set(h, "string", ["a" "row" "vector" "string" "property"]);
if or(get(h, "string") <> ["a" "row" "vector" "string" "property"]) then pause; end

// Column vector
set(h, "string", ["a";"column";"vector";"string";"property"]);
if or(get(h, "string") <> ["a" "column" "vector" "string" "property"]) then pause; end

// TODO test with a matrix should fail