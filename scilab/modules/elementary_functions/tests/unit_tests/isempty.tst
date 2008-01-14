//==============================================================================
// Author : Pierre MARECHAL
// Scilab team
// Copyright INRIA
// Date : 14 janvier 2008
//==============================================================================

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
