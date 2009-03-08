// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// unit tests for isempty function
// =============================================================================

// empty matrix
if ~isempty([]) then pause, end

// character string
if ~isempty('') then pause, end

// matrix of character string
if ~isempty([ '' , '' ; '' , '' ]) then pause, end
if isempty([ ' ' , '' ; '' , '' ]) then pause, end

// empty list
if ~isempty(list()) then pause, end

// mlist is never empty
if isempty(mlist('')) then pause, end
