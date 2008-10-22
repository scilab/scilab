//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================

function bOK = detectmsifort64tools()

 bOK = %F;
 if MSDOS & win64() then
   IFORTPATH = '';
   ifortcomp = findmsifortcompiler();
   if ifortcomp <> 'unknown' then
     select ifortcomp,
     
       case  'ifort10' then          
         IFORTPATH = getenv('IFORT_COMPILER10','');
     
       case  'ifort9' then                
         IFORTPATH = getenv('IFORT_COMPILER9','');
     
     else
       TXT = gettext('Intel Fortran Comiler 9 or 10 Compiler not found.');
       warning(TXT);
       bOK = %F;
       return
     end
   end
   
   IFORTSPATHEMT64 = IFORTPATH + 'em64t';
   
   if isdir(IFORTSPATHEMT64) then
     bOK = %T;
   end
 end
  
endfunction
//==========================================

