
// Load launch_test
exec('launch_test.sci');

// Set base directory for non-regression tests
if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'
 // TODO: do not depend on username and Scilab 5 path
 baseDir = '/home/vaylet/dev/scilab-5.0/modules/scicos/tests/nonreg_tests'
elseif getversion() == 'scilab-trunk-SVN' | getversion() == 'scilab-5.0'
  baseDir = SCI + '/modules/scicos/tests/nonreg_tests'
end

// Launch any available test
launch_test(baseDir, 'constant');
launch_test(baseDir, 'gensin');
launch_test(baseDir, 'gain');
launch_test(baseDir, 'sigbuild');
launch_test(baseDir, 'transferfcn');

