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
extern "C" {
#include "stack-c.h"
#include "gw_hdf5.h"
#include "Scierror.h"
#include "localization.h"
}

int gw_hdf5()
{
  Scierror(999, _("HDF5 is not installed"));
  return 0;
}

int sci_export_to_hdf5(char *fname,unsigned long fname_len)
{
  Scierror(999, _("HDF5 is not installed"));
  PutLhsVar();
  return 0;
}

int sci_import_from_hdf5(char *fname,unsigned long fname_len)
{
  Scierror(999, _("HDF5 is not installed"));
  PutLhsVar();
  return 0;
}
