                      Scilab 5.2.0 Release_Notes
                      ==========================

Known incompatibilities:

- Function "figure": when setting the size of the figure using the "Position" 
  property with a matrix such as [x y width height], the values of width and 
  height no more set the window size itself but the size of the virtual graphics
  window (the part of the figure which contains uicontrols and graphics) as if
  the user sets the axes_size property of a figure.

- 'sort' is obsolete. see 'gsort'.

- 'getf' is obsolete. see 'exec'.

- Removed functions, previously declared obsoletes:
   'mtlb_eig' to replace by 'spec'.
   'set_posfig_dim' to replace by 'figure_size' property.
   'xclip' to replace by 'clip_state' and 'clip_box' properties.
   'xgetfile' to replace by 'uigetfile'.
   'tk_getfile' to replace by 'uigetfile'.
   'x_message' to replace by 'messagebox'.

- Mac OS X port:
  * For now, Tcl/Tk has been disabled. The technical reason is because
    both Swing and Tk need to be driven by the Core Foundation
   (CFRunLoopRun) and it is tricky to handle both at the sametime.
	* As a consequence, TclSci (Scilab / Tcl interface) Scipad, GED
	and the variable editor are not working.

- Scipad has been removed from the Scilab distribution and replaced by an other
Scilab text editor.
This was due to the Tcl/Tk problem under Mac OS X and UI consistency.
Scipad remains available through ATOMS:
atomsInstall("scipad");
atomsLoad("scipad");
scipad();

- Metanet has been removed from the Scilab distribution.
Metanet remains available and maintained through ATOMS:
atomsInstall("metanet");
atomsLoad("metanet");

- Graphic export with accented chars will fail with Scilab based on the OpenJDK
  this works with the Sun proprietary JVM.
  See bug 4578.

- While PDF export are 10 time slower, PS and EPS exports are 4 time bigger
(with a better quality). This would be fixed in a next version of Scilab.
