                      Scilab 5.1 Release_Notes
                      ========================
					  
					  
- Graphics:
	* Errors occuring when getting or setting graphic handles properties are now teated as any other Scilab errors.
	As a consequence, these kinds of errors are now interrupting Scilab scripts when occuring. Although, this makes easier to find graphic errors,
	it might break some script containing such errors.
	
- fileio:
	* mputl returns %t or %f to check if function has correctly written on the file.	