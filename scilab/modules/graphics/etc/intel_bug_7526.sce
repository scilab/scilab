// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// quick way to detect bug 7526

function detect_bug_7526()

  function WarningDriversIntel()
    msg = ["Old Intel(R) HD Graphics drivers detected (bug #7526).";
           "Please update your video drivers:";
           "http://downloadcenter.intel.com/Detail_Desc.aspx?lang=eng&amp;changeLang=true&DwnldId=19758"];
    warning(msg);
  endfunction

  function bOK = isIntelHD()
    bOK = %f;
    [dyninfo, statinfo] = getdebuginfo();
    videocard = dyninfo(grep(dyninfo, "Video card:"));
    videocard = strsubst(videocard, "Video card:", "");
    bOK = grep(stripblanks(convstr(videocard, "u")), "INTEL(R) HD GRAPHICS") <> [];
  endfunction
  
  function bOK = isOldIntelHDDrivers()
    
    bOK = %f;
    [dyninfo, statinfo] = getdebuginfo();
    driversvideocard = dyninfo(grep(dyninfo, "Video card driver version:"));
    driversvideocard = strsubst(driversvideocard, "Video card driver version:", "");
    ierr = execstr("[nbparams, driverVer1, driverVer2, driverVer3, driverVer4] = msscanf(driversvideocard, ""%d.%d.%d.%d"");", "errcatch");
    if  ierr == 0 then
     // revision < to 2279 does not work
     if driverVer4 < 2279 then 
       bOK = %t;
     end
    end
  endfunction

  if getos() == "Windows" then
    if isIntelHD() & isOldIntelHDDrivers() then
      WarningDriversIntel();
    end
  end
endfunction

detect_bug_7526();
clear detect_bug_7526;
