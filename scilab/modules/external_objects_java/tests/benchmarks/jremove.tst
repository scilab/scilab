// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- BENCH NB RUN : 1 -->

// <-- BENCH START -->
exec(fullfile("SCI", "modules", "external_objects_java", "tests", "unit_tests", "jremove.tst"));

// plot(times'); // this should look like GC-cycles
M = double(max(times, "c")) / 1e6 // peak heap used per loop, in MB
// <-- BENCH END -->
