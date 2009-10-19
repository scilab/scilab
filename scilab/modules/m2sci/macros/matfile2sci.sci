// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Serge STEER 
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function matfile2sci(mat_file_path,result_file_path)
// Translate a Matlab MAT file into a Scilab file
// 
// mat_file_path : path of the Matlab MAT file
// result_file_path : path of the generated Scilab file  
//
//This function has been developped following the "MAT-File Format" description:
//www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf 

if ~with_module("matio") then
  error(msprintf(gettext("%s: %s module is not installed.\n"), "matfile2sci", "matio"));
end

[lhs,rhs]=argn(0)
if rhs<>2 then
    error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"matfile2sci",2)),
end

//--file opening
fdi=matfile_open(pathconvert(mat_file_path, %F, %T), "r");
fdo=mopen(pathconvert(result_file_path, %F, %T),"w") 

//-- Read first variable
ierr = execstr("[Name, Matrix, Class] = matfile_varreadnext(fdi);", "errcatch");
ierrsave = 0;

//--loop on the stored variables
while Name<>"" & ierr==0 & ierrsave==0
  ierrsave = execstr(Name + " = Matrix; save(fdo,"+Name+")", "errcatch")
  if ierrsave==0 then
    //-- Read next variable
    ierr = execstr("[Name, Matrix, Class] = matfile_varreadnext(fdi);", "errcatch");
  end
end

//--file closing
matfile_close(fdi);
mclose(fdo)
endfunction

