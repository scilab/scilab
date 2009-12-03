// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// help_from_sci

my_path = SCI+"/modules/helptools/tests/unit_tests/";
my_macros = "macrosExample.sci";
my_generated_xml = "macrosExample.xml";
target_file = TMPDIR+"/"+my_generated_xml;
help_from_sci(my_path + my_macros, TMPDIR);
if fileinfo(target_file) == [] then pause, end
mdelete(target_file);
