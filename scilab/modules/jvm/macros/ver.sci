//===============================
// Allan CORNET INRIA 2007
//===============================
//Version information for Scilab
//===============================
function ver()
  mprintf('\n');
  mprintf('-------------------------------------------------------------------------------\n');
  sciver=getversion('scilab');
  mprintf('Scilab Version %d.%d.%d.%d\n',sciver(1),sciver(2),sciver(3),sciver(4));
  mprintf('Operating System: %s %s\n',system_getproperty('os.name'),system_getproperty('os.version'));
  mprintf('Java VM Version: Java %s with %s\n%s %s\n', system_getproperty('java.vm.version'), ..
    system_getproperty('java.vm.specification.vendor'), ..
    system_getproperty('java.vm.name'), ..
    system_getproperty('java.vm.info'));
  mprintf('-------------------------------------------------------------------------------\n');
endfunction
//===============================
