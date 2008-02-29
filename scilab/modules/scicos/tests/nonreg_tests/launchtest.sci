function launchtest(baseDir, testName)

// =============================================================================

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

// =============================================================================

txt = [ "// Set display settings";
        "mode(3)";
        "clear";
        "lines(28,72)";
        "lines(0)";
        "";
        "// Start logging output";
        "diary(''" + diaryFilename + "'')";
        "";
        "// Load and launch simulation, displaying some debug data in the mean time";
        "load(''" + modelFilename + "'')";
        "disp(scs_m)";
        "// Info = list()";
        "// Force compilation";
        "//          %tcur  %cpr    alreadyran  needstart  needcompile  %state0";
        "Info = list(0,     list(), %f,         %t,        4,           list())";
        "try";
        "  Info = scicos_simulate(scs_m,Info,[],''nw'')";
        "catch";
        "  disp(msprintf(''%-25s: ERROR while launching simulation'',''" + testName + "''))";
        "end";
        "disp(Info)";
        "";
        "// Stop logging output";
        "diary(0)";
        "";
        "exit"];

mputl(txt, testFilename);

cmd = '(' + SCI + '/bin/scilab -nw -nb -args -nouserstartup -f ' + testFilename + ' > ' + resFilename + ') 2> ' + errFilename;

host(cmd)

// =============================================================================

if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'

  // Rename file.out -> file.out.ref
  mdelete(outRefFilename)
  [status, msg] = copyfile(outFilename, outRefFilename)
  mdelete(outFilename)

  if status
	  disp(msprintf('%-25s: Reference file successfully generated',testName))
  else
	  disp(msprintf('%-25s: WARNING: Reference file not generated',testName))
  end

elseif getversion() == 'scilab-trunk-SVN' | getversion() == 'scilab-5.0'

  // Compare output data with reference data
  // ---
  // Read output data
  try
    fidOut = mopen(outFilename, 'r');
    out = mgetl(fidOut);
    mclose(fidOut);
  catch
    disp(msprintf('%-25s: ERROR: Cannot read output data from file ' + outFilename, testName))
    return // go on to next test
  end

  // Read reference
  try
    fidRef = mopen(outRefFilename, 'r');
    ref = mgetl(fidRef);
    mclose(fidRef);
  catch
    disp(msprintf('%-25s: ERROR: Cannot read reference data from file ' + outRefFilename, testName))
    return // go on to next test
  end

  // Compare (%F means identical)
  if or(out<>ref)
    status.msg     = 'ERROR: Output and reference are not equal';
    status.details = msprintf('  Compare the following files for more details: %s and %s', outFilename, outRefFilename);
    status.id      = 1; // KO
  else
    status.msg     = 'OK';
    status.details = '';
    status.id      = 0; // OK
  end

  disp(msprintf('%-25s: %s', testName, status.msg))
  if status.id == 1 // KO
    disp(msprintf('%s', status.details))
  end  

else
  disp(msprintf('%s: Warning: Unknown Scilab version, did nothing more than launching the simulation...'), testName)
end

endfunction

