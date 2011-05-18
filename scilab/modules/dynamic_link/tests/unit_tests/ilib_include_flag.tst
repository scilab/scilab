// =============================================================================                                                                                                                                  
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab                                                                                                                                                                                                                                                                                                                              
// Copyright (C) 2010 - DIGITEO - Vincent LEJEUNE
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Check failure case
if ( execstr('ilib_include_flag(3)','errcatch') == 0 ) then pause, end

// not a dir, should fail

if ( execstr('ilib_include_flag(/usr/include/stdlib.h)','errcatch')  == 0 ) then pause, end

 // should not exist, should fail
if ( execstr('ilib_include_flag(/usr/my_exotic_dir/);','errcatch')  == 0 ) then pause, end

cd TMPDIR;
mkdir("myincludespath");

if getos() == "Windows" then 
  ref_flag = " -I""" + WSCI + "/bin"" " + "-I""" + WSCI + "/modules"" -I""" + getlongpathname(TMPDIR) + filesep() + "myincludespath""";
  ref_flag = strsubst(ref_flag, "/", filesep());
else
  ref_flag = " -I" + SCI + "/bin " + "-I" + SCI + "/modules -I" + TMPDIR + filesep() + "myincludespath";
end 

includes_path = [ "bin" , "modules" , TMPDIR + filesep() + "myincludespath"];
cd SCI;
output_flag = ilib_include_flag(includes_path);

if (ref_flag ~= output_flag) then pause, end
