// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// quick way to detect bug 7575

function detect_bug_7575()

  function WarningDriversNvidia()
    msg = ["Nvidia 260.xx detected.";
           "No Nvidia 3D profile for Scilab detected (bug #7575).";
           "Workaround: You need to create a 3D graphic profile for wscilex.exe.";
           "start Nvidia control panel --> 3d settings --> Manage 3D settings --> Add and select ""wscilex.exe""";
           "and disable (""off"") for ""Threaded optimization"" property"];
    warning(msg);
  endfunction

  function bOK = haveScilabProfile()
    // This part must be improved
    // Here we check IF WE HAVE A Scilab profile but NOT if it is a correct profile
    // TO DO: check that "Threaded optimization" have the good value
    bOK = %f;
    ierr = execstr("path = winqueryreg(""HKEY_CLASSES_ROOT"", ""VirtualStore\MACHINE\SOFTWARE\NVIDIA Corporation\Global\NVTweak\NvCplAppNamesStored"", ""wscilex.exe"")", "errcatch");
    if (ierr == 0) then
       if (path <> []) then
         bOK = %t;
       end
    end
  endfunction

  function bOK = isNvidia()
    bOK = %f;
    [dyninfo, statinfo] = getdebuginfo();
    videocard = dyninfo(grep(dyninfo, "Video card:"));
    videocard = strsubst(videocard, "Video card:", "");
    bOK = grep(convstr(videocard, "u"), "NVIDIA") <> [];
  endfunction

  function bOK = is260DriversBuggy()
    bOK = %f;
    [dyninfo, statinfo] = getdebuginfo();
    driversvideocard = dyninfo(grep(dyninfo, "Video card driver version:"));
    driversvideocard = strsubst(driversvideocard, "Video card driver version:", "");
    ierr = execstr("[nbparams, driverVer1, driverVer2, driverVer3, driverVer4] = msscanf(driversvideocard, ""%d.%d.%d.%d"");", "errcatch");
    VistaSeven_drivers = [8. 17. 12];
    XP_drivers = [6. 14. 12];
    if  ierr == 0 then
      if and([driverVer1, driverVer2, driverVer3] == XP_drivers) | and([driverVer1, driverVer2, driverVer3] == VistaSeven_drivers) then
          // 260.6306 and more work
          // tested on rev. 6306 and 6658
          if (driverVer4 >= 6089) & (driverVer4 < 6306) then 
            bOK = %t;
          end
      end
    end
    
  endfunction

  if getos() == "Windows" then
    if isNvidia() & is260DriversBuggy() then
      if ~haveScilabProfile() then
        WarningDriversNvidia();
      end
    end
  end
endfunction

detect_bug_7575();
clear detect_bug_7575;
