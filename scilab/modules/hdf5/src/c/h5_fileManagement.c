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
#include "FileExist.h"
#include "deleteafile.h"

int createHDF5File(char *name) 
{
  hid_t file;
	herr_t status			= 0;
	
	hid_t fileAccess = H5Pcreate(H5P_FILE_ACCESS);
	status = H5Pset_fclose_degree(fileAccess, H5F_CLOSE_STRONG);
	if(status < 0)
	{
		return status;
	}

	if(FileExist(name))
	{
		deleteafile(name);
	}
	/*
   * Create a new file using the default properties.
   */

	file = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT, fileAccess);
	H5Pclose(fileAccess);
  
  return file;
}

int openHDF5File(char *name) 
{
  hid_t file;
  /*
   * Open file.
   */
	file = H5Fopen (name, H5F_ACC_RDONLY, H5P_DEFAULT);

  return file;
}

void closeHDF5File(int file)
{
	herr_t status					= 0;

  status = H5Fclose(file);
	if(status < 0)
	{
		fprintf(stderr, "%s", "failed to close file");
	}
}
