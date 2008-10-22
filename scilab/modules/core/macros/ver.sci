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
function res = ver()
  
  sciver = getversion('scilab');
  res(1,1) = gettext('Scilab Version: ');
  res(1,2) = sprintf('%d.%d.%d.%d',sciver(1),sciver(2),sciver(3),sciver(4));
  
  if ( with_module('jvm') )  & ( getscilabmode()<> 'NWNI' ) then
    res(2,1) = gettext('Operating System: ');
    res(2,2) = sprintf('%s %s',system_getproperty('os.name'), ..
          system_getproperty('os.version'));
          
    res(3,1) = gettext('Java version :');
    res(3,2) = system_getproperty('java.version');
  
    res(4,1) = gettext('Java runtime information :');
    res(4,2) = sprintf('%s (build %s)\n',system_getproperty('java.runtime.name'), ..
          system_getproperty('java.runtime.version'));
          
    res(5,1) = gettext('Java vm information :');
    res(5,2) = sprintf('%s (build %s, %s)\n',system_getproperty('java.vm.name'), ..
          system_getproperty('java.vm.version'), ..
          system_getproperty('java.vm.info'));
          
    res(6,1) = gettext('Vendor specification: ');          
    res(6,2) = system_getproperty('java.vm.specification.vendor');
  end
endfunction
//===============================
