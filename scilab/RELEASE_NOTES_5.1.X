                      Scilab 5.1 Release_Notes
                      ========================
					  
					  
- Graphics:
	* Errors occuring when getting or setting graphic handles properties are now teated as any other Scilab errors.
	As a consequence, these kinds of errors are now interrupting Scilab scripts when occuring. Although, this makes easier to find graphic errors,
	it might break some script containing such errors.
	
- fileio:
	* mputl returns %t or %f to check if function has correctly written on the file.	

- Mac OS X port:
There are some know issues with the Mac OS X version:

      	* For now, Tcl/Tk has been disabled. The technical reason is because 
	both Swing and Tk need to be driven by the Core Foundation 
	(CFRunLoopRun) and it is tricky to handle both at the sametime.
	* As a consequence, scipad, GED and the variable editor are not working

	* On a non-up-to-date Mac OS X, an error saying that the Dock Icon 
	cannot be set can occur. Please update your Mac OS X Java.

	* Because of a Mac OS X bug, Scilab is not using the typical 
	Mac OS X Screen Menu Bar.
	See: http://bugzilla.scilab.org/show_bug.cgi?id=4074
	If you want to use it, uncomment the line about useScreenMenuBar in 
	modules/jvm/etc/jvm_options.xml
Patches are welcome!
