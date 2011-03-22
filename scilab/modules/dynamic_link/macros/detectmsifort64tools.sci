// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bOK = detectmsifort64tools()

 bOK = %F;
 if getos() == "Windows" & win64() then
  IFORTPATH = "";
   ifortcomp = findmsifortcompiler();
   if ifortcomp <> "unknown" then
     select ifortcomp,
       case  "ifort12" then
         IFORTPATH = getenv("IFORT_COMPILER12","");

       case  "ifort11" then
         IFORTPATH = getenv("IFORT_COMPILER11","");

       case  "ifort10" then
         IFORTPATH = getenv("IFORT_COMPILER10","");

       case  "ifort9" then
         IFORTPATH = getenv("IFORT_COMPILER9","");

     else
       show = displayWarningmsifort();
       if show then
         TXT = gettext("Intel Fortran Compiler 9, 10, 11 or 12 Compiler not found.");
         warning(TXT);
         clear TXT;
         disableWarningmsifort();
       end
       bOK = %F;
       return
     end
   end

   select ifortcomp,
     case  "ifort12" then
       IFORTSPATHEMT64 = IFORTPATH + "compiler\lib\intel64";
     case  "ifort11" then
       IFORTSPATHEMT64 = IFORTPATH + "lib\intel64";
     else
       IFORTSPATHEMT64 = IFORTPATH + "em64t";
   end

   if isdir(IFORTSPATHEMT64) then
     bOK = %T;
   end
 end

endfunction
//=============================================================================
function show = displayWarningmsifort()
  settings_filename = "/.settings_warning_msif";
  settings_file = pathconvert(SCIHOME+settings_filename,%f,%t);
  w = fileinfo(settings_file);
  show = %t;
  if isfile(settings_file) then
    show = grep(mgetl(settings_file),"displayWarningIFx64=no")==[]
  end
endfunction
//=============================================================================
function disableWarningmsifort()
  settings_filename = "/.settings_warning_msif";
  settings_file = pathconvert(SCIHOME+settings_filename,%f,%t);
  mputl("displayWarningIFx64=no",settings_file);
endfunction
//=============================================================================
