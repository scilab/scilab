// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Jérôme PICARD
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
// return the Modelica directory path modelica_path
// used by Xcos diagram containing Modelica blocks

function [modelica_path,modelica_directory] = getModelicaPath()

  lhs = argn(1);

  if (lhs > 2) then
    error(msprintf(gettext("%s: Wrong number of output argument(s): %d or %d expected.\n"), "getModelicaPath",1,2));
    return
    
  else
    
    modelica_path = [];
    modelica_directory = [];
    
    // path for generic modelica blocks
    // modelica_directory = pathconvert(TMPDIR+'/modelica/',%f,%t);
    
    // for the standard electrical and hydraulical components 
    modelica_path = 'SCI/modules/scicos_blocks/macros/' + ['Electrical','Hydraulics'];
    
    // add TMPDIR/modelica for generic modelica blocks
    // needed by modelicat to compile every modelica file
    
    // create modelica directory if it doesn't exist
    [status_exists,messages_exists] = mkdir(TMPDIR,'modelica');    
    
    if (status_exists==1 | status_exists==2)  then 
      modelica_path = [modelica_path,TMPDIR + '/modelica'];
    else
      error(msprintf(gettext("%s \n" ),messages_exists));
      return
    end    
    
  end

endfunction