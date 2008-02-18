
// Disable vertical scrolling
lines(0)

// Load launchtest
exec('launchtest.sci');

// Set base directory for non-regression tests
if getversion() == 'scilab-4.1.2' | getversion() == 'Scilab-4.1.2-SVN'
 // TODO: do not depend on username and Scilab 5 path
 baseDir = '/home/vaylet/dev/scilab-5.0/modules/scicos/tests/nonreg_tests'
elseif getversion() == 'scilab-trunk-SVN' | getversion() == 'scilab-5.0'
  baseDir = SCI + '/modules/scicos/tests/nonreg_tests'
end

// Launch any available test (except skeleton.cos)
cosFiles = dir(fullfile(baseDir, '*.cos'));
cosFiles = sort(basename(cosFiles.name));
for k = 1:size(cosFiles,'*')
  currentTestName = cosFiles(k);
  launchtest(baseDir, currentTestName);
end

