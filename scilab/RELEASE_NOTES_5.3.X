                      Scilab 5.3.3 Release_Notes
                      ==========================


- The 5.3.3 release is a minor release to tackle a critical bug causing the 
  Japanese and Polish localizations to fail.

- New Nvidia drivers 260.xx can crash scilab (bug #7575)
  Workaround: You need to create a 3D graphic profile for wscilex.exe.
  start Nvidia control panel --> 3d settings --> Manage 3D settings --> Add and
  select  wscilex.exe" for Scilab and you disable ("off") for "Threaded 
  optimization".
  A warning on Scilab startup should be displayed.
  The lastest release of the Nvidia drivers fixes this issue.

- All portability issues of Scilab under Mac OS X (10.6) should be
  fixed. 
  However, if any problems is encounter, please:
  * Test the workaround described here:
    http://bugzilla.scilab.org/show_bug.cgi?id=7012#c10
  * Do not hesitate to report your problem as comment of the bug:
    http://bugzilla.scilab.org/show_bug.cgi?id=7012

- Some issues on startup under Mac OS X 10.6 have been reported.
  If you have "Audio Hijack" installed on your Mac OS X, it prevents some
  applications to work including Scilab. See bug #7012.
  Uninstall / disable Audio Hijack should fix this problem.

Besides these points, do not hesitate to report bugs on:
http://bugzilla.scilab.org/
