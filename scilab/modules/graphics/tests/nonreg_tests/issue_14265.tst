// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2024 - Dassault Systèmes S.E. - Cédric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14265 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/14265
//
// <-- Short Description -->
// Display properties instead of the function
// when inserting in gcf()

// helper to execute a Scilab command after all pending AWT events have been processed
jimport org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement

ScilabInterpreterManagement.executeAndWaitForEDT("gcf();");
gcf().info_message = "Non-regression test for bug 14265.";

ScilabInterpreterManagement.executeAndWaitForEDT("figure();");
gcf().info_message = "Non-regression test for bug 14265."; // Use figure created by 'figure()'
