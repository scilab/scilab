// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// Copyright (C) ???? - INRIA - Serge STEER 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function loadmatfile(varargin)
// Loads variables in a Matlab binary or ASCII file into Scilab
// This function has been developped following the 'MAT-File Format' description:
// www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf 

// Verify that all inputs are character strings
for k=1:size(varargin)
  if type(varargin(k))<>10 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),"loadmatfile"));
  end
end

[lhs,rhs]=argn(0)

fil=[]
bin=[]
varnames=[]

if rhs==1 then // Compatibility with old loadmatfile version
  bin=%T
  fil=varargin(1)
else // Try to find type binary or ASCII ?
  // Filename is the first parameter: loadmatfile(filename[,opts])
  // or the second parameter: loadmatfile(filetype,filename[,opts]) with filetype equal to -ascii or -mat
  // filetype can also be included in opts
  k=1
  while k<=lstsize(varargin)
    select varargin(k)
    case "-mat"
      bin=%T
      k=k+1
    case "-ascii"
      bin=%F
      k=k+1
    case "-regexp"
      warning(msprintf(gettext("%s: This feature has not been implemented: %s."),"loadmatfile","-regexp"));
      while k<=lstsize(varargin) & and(varargin(k)<>["-mat","-ascii"])
      	k=k+1
      end
    else 
      if isempty(fil) then // Filename
      	fil=pathconvert(varargin(k),%f,%t);
      	if fileparts(fil,"extension")==".mat" & isempty(bin) then // extension .mat and bin not already fixed by options
      	  bin=%T
      	elseif isempty(bin) then
      	  bin=%F
      	end
      else // Variable names
      	varnames=[varnames;varargin(k)]
      end
      k=k+1
    end
  end
  if isempty(bin) then
    bin=%T
  end
end

if fileparts(fil,"extension")=="" then
  fil=fil+".mat"
end

// --- BINARY FILE ---
if bin then // Uses MATIO interface
  
  Name='';
  Names=[];
  Matrices=list()
  
  //-- Try to open the file
  fd = matfile_open(pathconvert(fil, %F, %T), "r");
  if fd==-1 then
    error(msprintf(gettext("%s: Cannot open file %s.\n"),"loadmatfile",fil));
  end
  
  //-- Read first variable
  [Name, Matrix, Class] = matfile_varreadnext(fd);
  
  //-- Loop on the stored variables
  while Name<>""
    
    // Old version compatibility | Name has been given
    if isempty(varnames) | or(Name==varnames) then
      Names=[Names,Name];Matrices($+1)=Matrix
    end
    
    [Name, Matrix, Class] = matfile_varreadnext(fd);
    
  end
  
  //-- Close the file
  matfile_close(fd)
  
  //-- Return variables in the calling context
  execstr('['+strcat(Names,',')+']=resume(Matrices(:))')
   
// --- ASCII FILE (Copy/Paste from mtlb_load.sci) ---
else
  ke=strindex(fil,'.')
  if ke==[] then
    ke=length(fil)
  else
    ke=ke($)-1
  end
  kp=strindex(fil,['/','\'])
  if kp==[] then
    kp=1
  else
    kp=kp($)+1
  end
  name=part(fil,kp:ke)
  mat=evstr(mgetl(fil))
  execstr(name+'= resume(mat)')
end
endfunction

