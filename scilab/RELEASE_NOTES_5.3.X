                      Scilab 5.3.0 beta 5 Release_Notes
                      =================================

- New Nvidia drivers 260.xx can crash scilab (bug #7575)
  Workaround: You need to create a 3D graphic profile for wscilex.exe.
  start Nvidia control panel --> 3d settings --> Manage 3D settings --> Add and 
  select  wscilex.exe" for Scilab and you disable ("off") for "Threaded optimization"

- Some issues on startup under Mac OS X have been reported and should be fixed
  in Scilab 5.3.0:
  Bug #7012: Error in the detection of the version of Java available.
  Bug #7433: A segmentation fault on Scilab startup on some Mac OS X 
  installations.

- The accuracy of inversion of most distribution functions is increased: see
  bug #7569 for details.

Besides these points, do not hesitate to report bugs on:
http://bugzilla.scilab.org/
