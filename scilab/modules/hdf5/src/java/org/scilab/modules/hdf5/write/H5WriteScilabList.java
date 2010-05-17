/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

package org.scilab.modules.hdf5.write;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.H5ScilabConstant;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;

public class H5WriteScilabList extends H5WriteScilabCommonList {

    public static void writeInDataSet(int file_id, String dataSetName, ScilabList data) throws NullPointerException, HDF5Exception {
	writeInDataSet(file_id, dataSetName, data, H5ScilabConstant.SCILAB_CLASS_LIST);
    }

}
