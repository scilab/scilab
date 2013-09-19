// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Add ecj to classpath if needed
if getos()<>"Windows" then
    if isdir(SCI + "/thirdparty/") then
        javaclasspath(SCI + "/thirdparty/ecj.jar"); // Source version
    elseif isdir(SCI + "/../../thirdparty/") then
        javaclasspath(SCI + "/../../thirdparty/ecj.jar"); // Binary version
    end
end
