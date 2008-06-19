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
//  Algorithm: (grep '^\s*//--' scicos_nonreg.sci | awk -F '//-- ' '{print '//  ' $1 $2}')
//
//    Parse input arguments
//    Disable vertical paging
//    Keep track of number of successful and failed tests  
//    List and sort available tests (*.cos files)
//      For each available test
//        Launch test and display result
//        Update lists of successful and failed tests
//      Display overall report
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

  //-- Keep track of number of successful and failed tests  
  listSuccessful  = list() // list of successful tests
  listFailed      = list() // list of failed tests
  listSkipped     = list() // list of skipped tests
  failureDetails  = ''     // details about every failed test

  //-- List and sort available tests (*.cos files)
  baseDir    = pwd()
  cosFiles   = gsort(basename(listfiles('*.cos')),'lr','i')
  nbTests = size(cosFiles,'*')
  if nbTests ~= 0
  
    //-- For each available test
    for k = 1:nbTests
    
      //-- Print some info about current test
			printf('   %03d/%03d - ', k, nbTests)
			printf('[%s] %s','scicos',cosFiles(k))
			for j = length('scicos' + cosFiles(k)):50
				printf('.')
			end
    
      //-- Launch test and display result
      try
        status = launch_nonreg(baseDir,cosFiles(k))
      catch
        // Set status.ok to false in case of error in launch_nonreg
        status.ok = %f
        status.msg = ''
        status.details = ''
      end
      printf('%s \n', status.msg)      
      
      //-- Update lists of successful and failed tests
      if status.ok
        listSuccessful($+1) = cosFiles(k)
      else
        listFailed($+1)     = cosFiles(k)
				failureDetails = [ failureDetails ; sprintf('   TEST : [%s] %s','scicos',cosFiles(k)) ]
				failureDetails = [ failureDetails ; sprintf('     %s', status.msg) ]
				failureDetails = [ failureDetails ; status.details ]
				failureDetails = [ failureDetails ; '' ]
      end
    end
    
    //-- Display overall report
    nbPassed  = length(listSuccessful)
    nbFailed  = length(listFailed)
    nbSkipped = length(listSkipped)

		printf('\n')
		printf('   --------------------------------------------------------------------------\n')
		printf('   Summary\n\n')
		printf('   tests                     %4d - 100.0 %% \n', nbTests)
		printf('   passed                    %4d - %5.1f %% \n', nbPassed,  nbPassed/nbTests*100)
		printf('   failed                    %4d - %5.1f %% \n', nbFailed,  nbFailed/nbTests*100)
		printf('   skipped                   %4d - %5.1f %% \n', nbSkipped, nbSkipped/nbTests*100)
		printf('   --------------------------------------------------------------------------\n')

		if nbFailed > 0 then
			printf('   Details\n\n')
			printf('%s\n', failureDetails)
			printf('\n')
			printf('   --------------------------------------------------------------------------\n')
		end
    
  else
    error(mprintf('%s: No test found in following directory: ''%s''', 'run.sce', baseDir))
  end

endfunction

// -----------------------------------------------------------------------------

function status = launch_nonreg(baseDir, testName)

//  Algorithm: (grep '^\s*//--' scicos_nonreg.sci | awk -F '//-- ' '{print '//  ' $1 $2}')
//
//    Initializations
//    Build a script whose purpose is to launch simulation and log console output   
//    Launch script using a background Scilab
//    Which version of Scilab was used ?
//    Non-regression tests launched under Scilab 4.X ?  
//      Rename file.out -> file.out.ref
//    Non-regression tests launched under Scilab 5.X ?  
//      Compare output data with reference data:
//      Read output data
//      Read reference data
//      Compare (%F meaning identical) and update status
//      Display result

  //-- Initializations
  
  status.ok      = %f
  status.msg     = []
  status.details = []

  // Define some aliases
  currentScilabFamily = get_scilab_family()
  
  testFilename   = fullfile(baseDir, testName + '.test')
  modelFilename  = fullfile(baseDir, testName + '.cos')
  
  select currentScilabFamily
  
  case '4' then
    diaryFilename  = fullfile(baseDir, testName + '_v4.log')
    resFilename    = fullfile(baseDir, testName + '_v4.res')
    errFilename    = fullfile(baseDir, testName + '_v4.err')
    
  case '5' then
    diaryFilename  = fullfile(baseDir, testName + '_v5.log')
    resFilename    = fullfile(baseDir, testName + '_v5.res')
    errFilename    = fullfile(baseDir, testName + '_v5.err')
    
  else
    disp(sprintf('%-25s: ERROR: Currently using unknown Scilab version (%s)', testName, getversion()))
    return
    
  end
  
  // Keep separate references and results for Windows and Linux, as results given
  // by the two versions have always been different (though it's not logical)
  if MSDOS
    outFilename = fullfile(baseDir, testName + '.win.out') // foo.win.out
  else
    outFilename = fullfile(baseDir, testName + '.unix.out') // foo.unix.out
  end
  outRefFilename = outFilename + '.ref' // foo.win.out.ref or foo.unix.out.ref
  
  // Define format used in 'Write to File' blocks to log output (Fortran syntax)
  outputFormat = '(7(e22.15,1x))'
  
  
  //-- Build a script whose purpose is to launch simulation and log console output   
  txt = [ '// Set display settings';
          'mode(3)';
          'clear';
          'lines(28,72)';
          'lines(0)';
          '';
          '// Go to folder containing the diagram to test';
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
          '// Rename output file to match variant name';
          '// Override any existing format with predefined one (''' + outputFormat + ''')'; 
          '// This modification is not saved, it only exists during simulation';
          '[idxWrite, idxRead] = findIOblocks(scs_m)';
          'if ~isempty(idxWrite) & size(idxWrite,''*'') == 1';
          '  if MSDOS';
          '    scs_m = renameIO(scs_m, idxWrite, ''' + testName + '.out.win'')';
          '  else';
          '    scs_m = renameIO(scs_m, idxWrite, ''' + testName + '.out.gnu'')';
          '  end';
          '  scs_m = setW2Fformat(scs_m, idxWrite, ''' + outputFormat + ''')';
          'end';
          '';
          '// Rename input file (if present) to match variant name';
          '// This modification is only valid during simulation, thus not saved';
          'if ~isempty(idxRead) & size(idxRead,''*'') == 1';
          '  scs_m = renameIO(scs_m, idxRead, ''' + testName + '.in'')';
          'end';
          '';
          'disp(scs_m)';
          '';
          '// Force compilation';
          '//          %tcur  %cpr    alreadyran  needstart  needcompile  %state0';
          'Info = list(0,     list(), %f,         %t,        4,           list())';
          'try';
          '  Info = scicos_simulate(scs_m,Info,[],''nw'')';
          'catch';
          '  disp(sprintf(''%-25s: ERROR while simulating '',''' + testName + '''))';
          'end';
          '';
          'disp(Info)';
          '';
          '// Stop logging output';
          'diary(0)';
          '';
          '// Quit background Scilab session';
          'exit'];
  mputl(txt, testFilename)

  //-- Launch script using a background Scilab
  // Binary or source version ?
	if (~MSDOS) & isempty(fileinfo(SCI + '/bin/scilab')) then
		SCI_BIN = strsubst(SCI, '/share/scilab', '')
	else
		SCI_BIN = SCI
	end
  // Launch previous script inside a NW Scilab and redirect both standard and error output to files
  if MSDOS then
		cmd = '(''' + SCI_BIN + '\bin\scilex.exe'' -nw -nb -args -nouserstartup -f ''' + testFilename + ''' > ''' + resFilename + ''') 2> ''' + errFilename + ''''
	else
		cmd = '(' + SCI_BIN + '/bin/scilab -nw -nb -args -nouserstartup -f ' + testFilename + ' > ' + resFilename + ') 2> ' + errFilename
	end
  host(cmd)

  //-- Which version of Scilab was used ?
  select currentScilabFamily
    
  //-- Non-regression tests launched under Scilab 4.X ?  
  case '4' then
  
    //-- Rename file.out -> file.out.ref
    mdelete(outRefFilename)
    [status.ok, msg] = copyfile(outFilename, outRefFilename)
    mdelete(outFilename)
  
    // Status determines if copy succeeded or failed
    // A failure might indicate that simulation failed and did not produce any output
    if status.ok
      status.msg = 'passed  : Reference file successfully generated'
      status.details = ''
    else
      status.msg = 'failed  : Reference file NOT generated'
      status.details = 'It might indicate a failure during simulation'
    end
  
  //-- Non-regression tests launched under Scilab 5.X ?  
  case '5' then

    //-- Compare output data with reference data:
    
    //-- Read output data
    try
      fidOut = mopen(outFilename, 'r')
      out = mgetl(fidOut)
      mclose(fidOut)
    catch
      status.ok = %f
      status.msg = 'failed  : Cannot read output data'
      status.details = sprintf('Cannot read output data from file ''%s''', outFilename)
      return // go on to next test
    end
  
    //-- Read reference data
    try
      fidRef = mopen(outRefFilename, 'r')
      ref = mgetl(fidRef)
      mclose(fidRef)
    catch
      status.ok = %f
      status.msg = 'failed  : Cannot read reference data'
      status.details = sprintf('   Cannot read reference data from file ''%s''', outRefFilename)
      return // go on to next test
    end
  
    //-- Compare (%F meaning identical) and update status
    if or(out<>ref)
      status.ok      = %f
      status.msg     = 'failed  : Output and reference are NOT equal'
      status.details = sprintf('     Compare the following files for more details:')
      status.details = [ status.details ; sprintf('     - %s', outFilename) ]
      status.details = [ status.details ; sprintf('     - %s', outRefFilename) ]
      return
    else
      status.ok      = %t
      status.msg     = 'passed  : Output and reference are equal'
      status.details = ''
      return
    end
  end  
endfunction

// -----------------------------------------------------------------------------

function family = get_scilab_family()
// Get family (major version) of currently running Scilab
//
// Algorithm: (grep '^\s*//--' scicos_nonreg.sci | awk -F '//-- ' '{print '//  ' $1 $2}')
//
//    Initialize output to [] <=> unknow version of Scilab
//    Get complete version name
//    Extract family from a known pattern found in version name


  //-- Initialize output to [] <=> unknow version of Scilab
  family = []

  //-- Get complete version name
  version = getversion()
  
  //-- Extract family from a known pattern found in version name
  if ~isempty(grep(getversion(),'scilab-4')) ..
    | ~isempty(grep(getversion(), 'Scilab-4')) ..
    | ~isempty(grep(getversion(), 'scicos_work'))

    // 4.X version
    family = '4'
    
  elseif ~isempty(grep(getversion(), 'trunk')) ..
    | ~isempty(grep(getversion(), 'scilab-5'))
    
    // 5.X version
    family = '5'
    
  end

endfunction

// -----------------------------------------------------------------------------

