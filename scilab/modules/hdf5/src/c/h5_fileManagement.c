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

#include <hdf5.h>
#include "h5_fileManagement.h"

int createHDF5File(char *name) 
{
  hid_t       file;
  /*
   * Create a new file using the default properties.
   */
  file = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  
  return file;
}

int openHDF5File(char *name) 
{
  hid_t           file;
  
  /*
   * Open file.
   */
  file = H5Fopen (name, H5F_ACC_RDONLY, H5P_DEFAULT);

  return file;
}

void closeHDF5File(int file)
{
	H5Fflush(file, H5F_SCOPE_GLOBAL);
  H5Fclose(file);
}
