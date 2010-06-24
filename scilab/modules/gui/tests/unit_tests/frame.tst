// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Unitary tests for Scilab Java frames

// Exec tests common to all uicontrols
clear uicontrol_generic_test
exec("SCI/modules/gui/tests/unit_tests/uicontrol_generic_test.sci");
if uicontrol_generic_test("frame") <> 0 then pause; end

// Default relief value
h = uicontrol("parent", scf(), "style", "frame");
if get(h, "relief") <> "ridge" then pause; end

// --- Value tests ---
// Default value
if ~isempty(get(h, "value")) then pause; end

set(h, "value", [1]);
if get(h, "value") <> 1 then pause; end

set(h, "value", [1 2 3]);
if ~and(get(h, "value") == [1 2 3]) then pause; end

set(h, "value", []);
if ~isempty(get(h, "value")) then pause; end
// TODO test with wrong values
