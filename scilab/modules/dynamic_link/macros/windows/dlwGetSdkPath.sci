// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//=============================================================================
function SDKpath = dlwGetSdkPath()

    SDKpath = [];

    compiler = findmsvccompiler();
    entries = [];

    //detect sdk 8 only for VS >= 2012
    if find(compiler == ["msvc120express";"msvc120pro";"msvc110express";"msvc110pro"]) <> [] then;
        entries = [entries ;
            "Software\Microsoft\Microsoft SDKs\Windows\v8.0A" "InstallationFolder" ; .. // Windows 8 A
            "Software\Microsoft\Microsoft SDKs\Windows\v8.0" "InstallationFolder" .. // Windows 8
        ];
    end

    entries = [entries;
        "Software\Microsoft\Microsoft SDKs\Windows\v7.0" "InstallationFolder"; .. // Microsoft Windows SDK 7.0
        "Software\Microsoft\Microsoft SDKs\Windows\v7.0A" "InstallationFolder"; .. // Microsoft Windows SDK 7.0A
        "Software\Microsoft\Microsoft SDKs\Windows\v7.1" "InstallationFolder"; .. // Microsoft Windows SDK 7.1
        "Software\Microsoft\Microsoft SDKs\Windows" "CurrentInstallFolder" ; .. // Vista & Seven SDK
        "Software\Microsoft\MicrosoftSDK\InstalledSDKs\D2FF9F89-8AA2-4373-8A31-C838BF4DBBE1" "Install Dir" ; .. // Windows 2003 R2 SDK
        "Software\Microsoft\MicrosoftSDK\InstalledSDKs\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3" "Install Dir" .. // Windows 2003 SDK
    ];

    for i = 1:size(entries(:,1),"*")
        err = execstr("SDKpath = winqueryreg(""HKEY_LOCAL_MACHINE"", entries(i,1), entries(i,2))", "errcatch");
        // remove last file separator if it exists
        if err == 0 & SDKpath <> [] then
            SDKpath = pathconvert(SDKpath, %f, %t);
            // We check that returned path exists
            // case: install and uninstall sdk, uninstaller does not remove registry key
            if isdir(SDKpath) then
                break;
            else
                SDKpath = [];
            end
        else
            errclear();
        end
    end
endfunction
//=============================================================================
