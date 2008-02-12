
exec('launch_test.sci');

baseDir = '/home/vaylet/dev/scilab-5.0/modules/scicos/tests/nonreg_tests'

launch_test(baseDir, 'constant');
launch_test(baseDir, 'gensin');
launch_test(baseDir, 'gain');

