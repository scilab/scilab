//===============================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//===============================
//Version information for Scilab
//===============================
function ver()
  mprintf('\n');
  mprintf('-------------------------------------------------------------------------------\n');
  
  sciver = getversion('scilab');
  
  mprintf('Scilab Version %d.%d.%d.%d\n',sciver(1),sciver(2),sciver(3),sciver(4));
  
  mprintf('Operating System: %s %s\n',system_getproperty('os.name'), ..
          system_getproperty('os.version'));
          
  mprintf('Java version ""%s""\n', system_getproperty('java.version'));
  
  mprintf('%s (build %s)\n',system_getproperty('java.runtime.name'), ..
          system_getproperty('java.runtime.version'));
          
  mprintf('%s (build %s, %s)\n',system_getproperty('java.vm.name'), ..
          system_getproperty('java.vm.version'), ..
          system_getproperty('java.vm.info'));
          
  mprintf('Vendor specification: %s\n', ..
          system_getproperty('java.vm.specification.vendor'));
          
  mprintf('-------------------------------------------------------------------------------\n');
  
endfunction
//===============================
