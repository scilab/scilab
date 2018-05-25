// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 11583 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11583
//
// <-- Short Description -->
// In Scinotes preferences, when the default header is erased, available variables
// {$current-year} and {$current-date} were no longer documented

preferences();

// * Go to the Scinotes => Header section
// * Below the Textarea where the header can be written, a reminder
//   "Available variables: {$current-date}, {$current-year}"
//   must be displayed.
