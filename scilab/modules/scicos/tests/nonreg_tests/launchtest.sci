
function launchtest(testName)

repName = ''; // '/home/vaylet/dev/scilab-5.0/modules/scicos/tests/nonreg_tests/'
modelFilename = repName + testName + '.cos'
outFilename = repName + testName + '.out'
outRefFilename = outFilename + '.ref'

load(modelFilename)
// %tcur,%cpr,%alreadyran,needstart,needcompile,%state0
Info = list(0,list(),%f,%t,4,list())
Info = scicos_simulate(scs_m,Info)

if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'

  // Rename file.out -> file.out.ref
  mdelete(outRefFilename)
  copyfile(outFilename, outRefFilename)
  mdelete(outFilename)
  
  disp(msprintf("%-15s: Reference file successfully generated",testName))

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
    status.msg     = 'ERROR: Test failed: output and reference are not equal';
    status.details = msprintf('  Compare the following files for more details: %s and %s', outFilename, outRefFilename);
    status.id      = 1; // KO
  else
    status.msg     = 'Test passed: output and reference are equal';
    status.details = '';
    status.id      = 0; // OK
  end

  disp(msprintf('%-15s: %s', testName, status.msg))
  if status.id == 1
    disp(msprintf('%s', status.details))
  end  
  
else
  disp(msprintf("%s: Warning: Unknown Scilab version, did nothing more than launching the simulation..."),testName);
end

