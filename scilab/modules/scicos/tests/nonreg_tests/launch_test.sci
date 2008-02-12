function launch_test(baseDir, testName)

// =============================================================================

testFilename   = fullfile(baseDir, testName + '.test')
modelFilename  = fullfile(baseDir, testName + '.cos')
outFilename    = fullfile(baseDir, testName + '.out')
outRefFilename = outFilename + '.ref'

// =============================================================================

txt = [ "// Set display settings";
        "mode(3)";
        "clear";
        "lines(28,72)";
        "lines(0)";
        "";
        "// Set name of log file (used by ''diary'' below)";
        "if getversion() == ''scilab-4.1.2'' | getversion() == ''Scilab-4.1.2-SVN''";
        "  diaryName = fullfile(''" + baseDir + "'', ''" + testName + "'' + ''_v4.log'')";
        "elseif getversion() == ''scilab-trunk-SVN'' | getversion() == ''scilab-5.0''";
        "  diaryName = fullfile(''" + baseDir + "'', ''" + testName + "'' + ''_v5.log'')";
        "else";
        "  diaryName = fullfile(''" + baseDir + "'', ''" + testName + "'' + ''_diary.log'')";
        "end";
        "";
        "// Start logging output";
        "diary(diaryName);";
        "";
        "// Load and launch simulation, displaying some debug data in the mean time";
        "load(fullfile(''" + baseDir + "'', ''" + testName + "'' + ''.cos''))";
        "disp(scs_m)";
        "Info = list()";
        "Info = scicos_simulate(scs_m,Info)";
        "disp(Info)";
        "";
        "// Stop logging output";
        "diary(0)";
        "";
        "exit"]

mputl(txt, testFilename);

cmd = SCI + '/bin/scilab -nw -nb -f ' + testFilename;

host(cmd)

// =============================================================================

if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'

  // Rename file.out -> file.out.ref
  mdelete(outRefFilename)
  copyfile(outFilename, outRefFilename)
  mdelete(outFilename)
  
  disp(msprintf('%-15s: Reference file successfully generated',testName))

elseif getversion() == 'scilab-trunk-SVN' | getversion() == 'scilab-5.0'

  // Compare output data with reference data
  // ---
  // Read output data
  try
    fidOut = mopen(outFilename, 'r');
  catch
    error('ERROR: Cannot read output data from file ''' + outFilename + '''');
  end
  out = mgetl(fidOut);
  mclose(fidOut);
  
  // Read reference
  try
    fidRef = mopen(outRefFilename, 'r');
  catch
    error('ERROR: Cannot read reference data from file ''' + outRefFilename + '''');
  end
  ref = mgetl(fidRef);
  mclose(fidRef);

  // Compare (%F means identical)
  if or(out<>ref)
    status.msg     = 'ERROR: Output and reference are not equal';
    status.details = msprintf('  Compare the following files for more details: %s and %s', outFilename, outRefFilename);
    status.id      = 1; // KO
  else
    status.msg     = 'OK: Output and reference are equal';
    status.details = '';
    status.id      = 0; // OK
  end

  disp(msprintf('%-15s: %s', testName, status.msg))
  if status.id == 1
    disp(msprintf('%s', status.details))
  end  
  
else
  disp(msprintf('%s: Warning: Unknown Scilab version, did nothing more than launching the simulation...'),testName);
end

