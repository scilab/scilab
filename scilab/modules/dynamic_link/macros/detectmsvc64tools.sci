//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================

function bOK = detectmsvc64tools()

 bOK = %F;
 if MSDOS & win64() then
   compiler = findmsvccompiler();
   if ( (compiler == 'msvc90pro') | ..
        (compiler == 'msvc90std') | ..
        (compiler == 'msvc90express') ) then
     select compiler,
       // Microsoft Visual 2008 Studio Professional
       case  'msvc90pro' then          
         MSVSPATH = winqueryreg('HKEY_LOCAL_MACHINE', ..
                'Software\Microsoft\VisualStudio\9.0\Setup\VS\Pro', ..
                'ProductDir');
         
       // Microsoft Visual 2008 Studio Standard 
       case  'msvc90std' then                
         MSVSPATH = winqueryreg('HKEY_LOCAL_MACHINE', ..
                'Software\Microsoft\VisualStudio\9.0\Setup\VS\Std', ..
                'ProductDir');
         
       // Microsoft Visual 2008 Express  
       case  'msvc90express' then      
         MSVSPATH = winqueryreg('HKEY_LOCAL_MACHINE', ..
                'Software\Microsoft\VCExpress\9.0\Setup\VS', ..
                'ProductDir');
     else
       TXT = gettext('Microsoft Visual Studio C 2008 Compiler not found.');
       warning(TXT);
       bOK = %F;
       return
     end
     
     MSVCBIN64PATH = MSVSPATH + 'VC\bin\amd64';
     if fileinfo(MSVCBIN64PATH) <> [] then
       bOK = %T;
     else
       TXT = gettext('Microsoft Visual Studio C 2008 x64 Compiler not installed.');
       warning(TXT);
       bOK = %F;
     end
     
   end
 end
  
endfunction
//==========================================

