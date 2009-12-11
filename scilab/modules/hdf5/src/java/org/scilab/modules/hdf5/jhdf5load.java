/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.hdf5;
import ncsa.hdf.hdf5lib.H5;

/**
 * This is a FAKE class called by import_from_hdf5 & export_to_hdf5
 * to make sure that the Java HDF5 is called before the native one.
 * See bug #5481
 *
 */
public class jhdf5load  {

	public static void forceLoad(){
		try {
			int[] vers = new int[3];
			H5.H5get_libversion(vers);
		}catch(Exception e){
			System.err.println("An error in the fake load of the java library has been triggered: "+e.getLocalizedMessage());
			e.printStackTrace();
		}
	}

    
    
}
