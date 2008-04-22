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
//    baseDir - OPTIONAL - string
//      Base directory of non-regression tests (where COS files can be found)
//        DEFAULT: fullfile(SCI,"modules","scicos","tests","nonreg_tests")
//
//  Output argument(s):
//
//    -
//
//  Usage:
//
//     scicos_nonreg()
//     scicos_nonreg('/home/<USER>/scilab-5.0/modules/scicos/tests/nonreg_tests')
//
//  Algorithm: (grep "^\s*//--" scicos_nonreg.sci | awk -F "//-- " '{print "//  " $1 $2}')
//
//    Parse input arguments
//    Disable vertical scrolling
//    Load launchtest.sci
//    Base directory exists ?
//      List and sort available tests (*.cos files)
//      For each available test
//          Launch and display result
//
//  Notes:
//
//    baseDir is optional when launching tests from inside Scilab 5 but becomes
//      mandatory when using Scilab 4 to generate reference data. Otherwise,
//      Scilab 4 has no way to know where COS files reside. 
//
//  2008/04/22     Laurent VAYLET   Creation
//  <YYYY/MM/DD>   <AUTHOR>         Modification
//

  //-- Parse input arguments
  lhs = argn(1)
  rhs = argn(2)

  // Define default input arguments
  defaultArgs = list(fullfile(SCI,'modules','scicos','tests','nonreg_tests'))
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
  
  // Set final values of input arguments
  baseDir = inputArgs(1)

  //-- Disable vertical scrolling
  lines(0)

  //-- Load launchtest.sci
  // TODO: rename launchtest.sci (launch_nonreg.sci?) and put in 'macros/scicos_utils' folder
  //exec('launchtest.sci')

  //-- Base directory exists ?
  [x,ierr] = fileinfo(baseDir)
  if ierr ~= -1 & ~isempty(x)
  
    //-- List and sort available tests (*.cos files)
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
  else
    error(mprintf('%s: Base directory for non-regression tests does not exist: ''%s''\n', 'run.sce', baseDir))
  end

endfunction

// -----------------------------------------------------------------------------

function launch_nonreg(baseDir, testName)

  //-- Initializations

  // testDir        = fullfile(baseDir, testName)
  // mkdir(testDir) // create a subfolder for test results (NOT outputs)
  
  testFilename   = fullfile(baseDir, testName + '.test')
  modelFilename  = fullfile(baseDir, testName + '.cos')
  
  if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'
    diaryFilename  = fullfile(baseDir, testName + '_v4.log')
    resFilename    = fullfile(baseDir, testName + '_v4.res')
    errFilename    = fullfile(baseDir, testName + '_v4.err')
  elseif getversion() == 'scilab-trunk-SVN' | getversion() == 'scilab-5.0'
    diaryFilename  = fullfile(baseDir, testName + '_v5.log')
    resFilename    = fullfile(baseDir, testName + '_v5.res')
    errFilename    = fullfile(baseDir, testName + '_v5.err')
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
          '// Start logging output';
          'diary(''' + diaryFilename + ''')';
          '';
          '// Load and launch simulation, displaying some debug data in the mean time';
          'load(''' + modelFilename + ''')';
          'disp(scs_m)';
          '// Info = list()';
          '// Force compilation';
          '//          %tcur  %cpr    alreadyran  needstart  needcompile  %state0';
          'Info = list(0,     list(), %f,         %t,        4,           list())';
          'try';
          '  Info = scicos_simulate(scs_m,Info,[],''nw'')';
          'catch';
          '  disp(msprintf(''%-25s: ERROR while launching simulation'',''' + testName + '''))';
          'end';
          'disp(Info)';
          '';
          '// Stop logging output';
          'diary(0)';
          '';
          'exit'];
  mputl(txt, testFilename);

  //-- Launch script using a background Scilab  
  cmd = '(' + SCI + '/bin/scilab -nw -nb -args -nouserstartup -f ' + testFilename + ' > ' + resFilename + ') 2> ' + errFilename
  host(cmd)

  //-- Non-regression tests launched under Scilab 4.X ?  
  if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'
  
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
  elseif getversion() == 'scilab-trunk-SVN' | getversion() == 'scilab-5.0'
  
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
  
  else
    mprintf('%s: Warning: Unknown Scilab version, did nothing more than launching the simulation...', testName)
  end
  
endfunction

// -----------------------------------------------------------------------------

