//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JAVA NOT MANDATORY -->

// Load dynamic_link Internal lib if it's not already loaded
if ~ exists("dynamic_linkutilslib") then
  load("SCI/modules/dynamic_link/macros/utils/lib");
end

function checkAddPathToEnv(initialEnv, addedPaths, expectedEnv)
    setenv('myEnv', initialEnv);
    addPathToEnv('myEnv', addedPaths);
    myEnv = getenv('myEnv', '');
    assert_checkequal(myEnv, expectedEnv)
endfunction

if getos() == 'Windows' then

    checkAddPathToEnv('', 'C:\dummy', 'C:\dummy');
    checkAddPathToEnv('', 'C:\dummy;c:\dummy;c:/dummy;C:/dummy', 'C:\dummy');
    checkAddPathToEnv('', 'C:\dummy;D:\folder', 'C:\dummy;D:\folder');
    checkAddPathToEnv('', 'C:\dummy\;D:\folder\', 'C:\dummy;D:\folder');
    checkAddPathToEnv('C:\dummy', '', 'C:\dummy');
    checkAddPathToEnv('C:\dummy;', 'D:\folder', 'C:\dummy;D:\folder');
    checkAddPathToEnv('C:\dummy', 'D:\folder;D:\folder2', ..
        'C:\dummy;D:\folder;D:\folder2');
    checkAddPathToEnv('C:\dummy;D:\folder', 'D:\folder;D:\folder2', ..
        'C:\dummy;D:\folder;D:\folder2');
    checkAddPathToEnv('D:\folder;C:\dummy', 'C:\dummy;D:\folder;D:\folder2', ..
        'D:\folder;C:\dummy;D:\folder2');

else

    checkAddPathToEnv('', '/dummy', '/dummy');
    checkAddPathToEnv('', '/dummy:/folder', '/dummy:/folder');
    checkAddPathToEnv('', '/dummy/:/folder/', '/dummy:/folder');
    checkAddPathToEnv('/dummy', '', '/dummy');
    checkAddPathToEnv('/dummy:', '/folder', '/dummy:/folder');
    checkAddPathToEnv('/dummy', '/folder:/folder2', ..
        '/dummy:/folder:/folder2');
    checkAddPathToEnv('/dummy:/folder', '/folder:/folder2', ..
        '/dummy:/folder:/folder2');
    checkAddPathToEnv('/folder:/dummy', '/dummy:/folder:/folder2', ..
        '/folder:/dummy:/folder2');

end