                      Scilab 5.3.3 Release_Notes
                      ==========================


- The 5.3.3 release is a minor release to tackle two critical bugs causing:
  * the Japanese and Polish localizations to fail,
  * the Mac OS X version to fail under Mac OS X 10.6.8 following Apple last update.

- New Nvidia drivers 260.xx could crash Scilab (bug #7575)
  Workaround: You need to create a 3-D graphic profile for wscilex.exe.
  start Nvidia control panel --> 3-D settings --> Manage 3-D settings --> Add and
  select Wscilex.exe" for Scilab and you disable ("off") for "Threaded
  optimization".
  A warning on Scilab startup should be displayed.
  The latest release of the Nvidia drivers fixes this issue.

- All portability issues of Scilab under Mac OS X (10.6.7 and previous 10.6
  versions) should be fixed.
  However, if any problem is encountered, please:
  * Test the workaround described here:
    http://bugzilla.scilab.org/show_bug.cgi?id=7012#c10
  * Do not hesitate to report your problem as comment of the bug:
    http://bugzilla.scilab.org/show_bug.cgi?id=7012

- Some issues on startup under Mac OS X 10.6.7 (and previous 10.6 versions)
  have been reported.
  If you have "Audio Hijack" installed on your Mac OS X, it prevents some
  applications to work including Scilab. See bug #7012.
  Uninstall / disable Audio Hijack should fix this problem.

Besides these points, do not hesitate to report bugs on:
http://bugzilla.scilab.org/
