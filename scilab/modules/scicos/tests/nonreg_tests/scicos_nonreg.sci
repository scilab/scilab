// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function scicos_nonreg(varargin)

//  Description:
//
//    Launcher for Scicos non-regression tests
//
//  Input argument(s):
//
//    -
//
//  Output argument(s):
//
//    -
//
//  Usage:
//
//     cd SCI/modules/scicos/tests/nonreg_tests;
//     getf('scicos_nonreg.sci'); scicos_nonreg();
//
//  Algorithm: (grep "^\s*//--" scicos_nonreg.sci | awk -F "//-- " '{print "//  " $1 $2}')
//
//    Parse input arguments
//    Disable vertical paging
//    List and sort available tests (*.cos files)
//      For each available test
//        Launch test and display result
//
//  Notes:
//
//    - 
//
//  2008/04/22     Laurent VAYLET   Creation
//  <YYYY/MM/DD>   <AUTHOR>         Modification
//

  //-- Parse input arguments
  lhs = argn(1)
  rhs = argn(2)

  // Define default input arguments
  defaultArgs = list()
  inputArgs   = defaultArgs 
  
  // Define maximum number of input arguments
  MAX_ARGS  = length(inputArgs)
  
  // Check number of input arguments
  if rhs > MAX_ARGS
    error(mprintf('%s: Wrong number of input arguments: At most %d expected.\n', ..
      'scicos_nonreg', MAX_ARGS))
  elseif rhs > 0
    // Replace any default value with user-provided one
    for k = 1:rhs
      inputArgs(k) = varargin(k)
    end
  end
  
  //-- Disable vertical paging
  lines(0)

  //-- List and sort available tests (*.cos files)
  baseDir    = pwd()
  cosFiles   = gsort(basename(listfiles('*.cos')),'lr','i')
  nbCosFiles = size(cosFiles,'*')
  if nbCosFiles ~= 0
  
    //-- For each available test
    for k = 1:nbCosFiles
    
      //-- Launch test and display result
      launch_nonreg(baseDir,cosFiles(k))
      
    end
  else
    error(mprintf('%s: No test found in following directory: ''%s''', 'run.sce', baseDir))
  end

endfunction

// -----------------------------------------------------------------------------

function launch_nonreg(baseDir, testName)

//  Algorithm: (grep "^\s*//--" scicos_nonreg.sci | awk -F "//-- " '{print "//  " $1 $2}')
//
//    Initializations
//    Build a script whose purpose is to launch simulation and log console output   
//    Launch script using a background Scilab
//    Non-regression tests launched under Scilab 4.X ?  
//      Rename file.out -> file.out.ref
//    Non-regression tests launched under Scilab 5.X ?  
//      Compare output data with reference data:
//      Read output data
//      Read reference data
//      Compare (%F meaning identical) and update status
//      Display result

  //-- Initializations

  // testDir        = fullfile(baseDir, testName)
  // mkdir(testDir) // create a subfolder for test results (NOT outputs)
  
  currentScilabFamily = get_scilab_family();
  
  testFilename   = fullfile(baseDir, testName + '.test')
  modelFilename  = fullfile(baseDir, testName + '.cos')
  
  select currentScilabFamily
  
  case "4" then
    diaryFilename  = fullfile(baseDir, testName + '_v4.log')
    resFilename    = fullfile(baseDir, testName + '_v4.res')
    errFilename    = fullfile(baseDir, testName + '_v4.err')
    
  case "5" then
    diaryFilename  = fullfile(baseDir, testName + '_v5.log')
    resFilename    = fullfile(baseDir, testName + '_v5.res')
    errFilename    = fullfile(baseDir, testName + '_v5.err')
    
  else
    disp(msprintf('%-25s: ERROR: Currently using unknown Scilab version (%s)', testName, getversion()))
    return
    
  end
  
  outFilename    = fullfile(baseDir, testName + '.out')
  outRefFilename = outFilename + '.ref'
  
  
  //-- Build a script whose purpose is to launch simulation and log console output   
  txt = [ '// Set display settings';
          'mode(3)';
          'clear';
          'lines(28,72)';
          'lines(0)';
          '';
          'cd(''' + baseDir + ''')';
          '';
          '// Start logging output';
          'diary(''' + diaryFilename + ''')';
          '';
          '// Load some helper functions (findIOblocks, renameIO, setW2Fformat, ...)';
          'getd(''./utils'')';
          '';
          '// Load and launch simulation, displaying some debug data in the mean time';
          'load(''' + modelFilename + ''')';
          '';
          '//-- Rename output file to match variant name';
          '//-- Override any existing format with predefined one (''(7(e22.15,1x))'')'; 
          '// This modification is not saved, it only exists during simulation';
          '[idxWrite, idxRead] = findIOblocks(scs_m)';
          'if ~isempty(idxWrite) & size(idxWrite,''*'') == 1';
          '  scs_m = renameIO(scs_m, idxWrite, ''' + testName + '.out'')';
          '  scs_m = setW2Fformat(scs_m, idxWrite, ''(7(e22.15,1x))'')';
          'end';
          '';
          '//-- Rename input file (if present) to match variant name';
          '// This modification is not saved, it only exists during simulation';
          'if ~isempty(idxRead) & size(idxRead,''*'') == 1';
          '  scs_m = renameIO(scs_m, idxRead, ''' + testName + '.in'')';
          'end';
          '';
          'disp(scs_m)';
          '// Info = list()';
          '// Force compilation';
          '//          %tcur  %cpr    alreadyran  needstart  needcompile  %state0';
          'Info = list(0,     list(), %f,         %t,        4,           list())';
          'try';
          '  Info = scicos_simulate(scs_m,Info,[],''nw'')';
          'catch';
          '  disp(msprintf(''%-25s: ERROR while simulating '',''' + testName + '''))';
          'end';
          'disp(Info)';
          '';
          '// Stop logging output';
          'diary(0)';
          '';
          '// Quit background Scilab session';
          'exit'];
  mputl(txt, testFilename);

  //-- Launch script using a background Scilab
  // Binary or source version ?
	if (~MSDOS) & isempty(fileinfo(SCI + '/bin/scilab')) then
		SCI_BIN = strsubst(SCI, '/share/scilab', '');
	else
		SCI_BIN = SCI;
	end
  // Launch previous script inside a NW Scilab and redirect both standard and error output to files
  if MSDOS then
		cmd = '(""' + SCI_BIN + '\bin\scilex.exe"" -nw -nb -args -nouserstartup -f ""' + testFilename + '"" > ""' + resFilename + '"") 2> ""' + errFilename + '""'
	else
		cmd = '(' + SCI_BIN + '/bin/scilab -nw -nb -args -nouserstartup -f ' + testFilename + ' > ' + resFilename + ') 2> ' + errFilename
	end
  host(cmd)

  //-- Which version of Scilab was used ?
  select currentScilabFamily
    
  //-- Non-regression tests launched under Scilab 4.X ?  
  case "4" then
  
    //-- Rename file.out -> file.out.ref
    mdelete(outRefFilename)
    [status, msg] = copyfile(outFilename, outRefFilename)
    mdelete(outFilename)
  
    if status
      disp(msprintf('%-25s: Reference file successfully generated',testName))
    else
      disp(msprintf('%-25s: WARNING: Reference file not generated',testName))
    end
  
  //-- Non-regression tests launched under Scilab 5.X ?  
  case "5" then

    //-- Compare output data with reference data:
    
    //-- Read output data
    try
      fidOut = mopen(outFilename, 'r')
      out = mgetl(fidOut)
      mclose(fidOut)
    catch
      disp(msprintf('%-25s: ERROR: Cannot read output data from file ' + outFilename, testName))
      return // go on to next test
    end
  
    //-- Read reference data
    try
      fidRef = mopen(outRefFilename, 'r')
      ref = mgetl(fidRef)
      mclose(fidRef)
    catch
      mprintf('%-25s: ERROR: Cannot read reference data from file ''%s''', testName, outRefFilename)
      return // go on to next test
    end
  
    //-- Compare (%F meaning identical) and update status
    if or(out<>ref)
      status.msg     = 'ERROR: Output and reference are not equal'
      status.details = msprintf('  Compare the following files for more details: %s and %s', outFilename, outRefFilename)
      status.ok      = %f
    else
      status.msg     = 'OK'
      status.details = ''
      status.ok      = %t
    end
  
    //-- Display result
    disp(msprintf('%-25s: %s', testName, status.msg))
    if ~status.ok
      disp(msprintf('%s', status.details))
    end  
  end  
endfunction

// -----------------------------------------------------------------------------

function family = get_scilab_family()
// Get family (major version) of currently running Scilab

  //-- Initialize output to [] <=> unknow version of Scilab
  family = [];

  //-- Get complete version name
  version = getversion();
  
  //-- Extract family from a known pattern found in version name
  if ~isempty(grep(getversion(),'scilab-4')) ..
    | ~isempty(grep(getversion(), 'Scilab-4')) ..
    | ~isempty(grep(getversion(), 'scicos_work'))

    // 4.X version
    family = "4";
    
  elseif ~isempty(grep(getversion(), 'trunk')) ..
    | ~isempty(grep(getversion(), 'scilab-5'))
    
    // 5.X version
    family = "5";
    
  end

endfunction

// -----------------------------------------------------------------------------

