/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "H5DatasetsList.hxx"
#include "H5Group.hxx"

namespace org_modules_hdf5
{

H5DatasetsList::H5DatasetsList(H5Group & _parent) : H5NamedObjectsList<H5Dataset>(_parent, H5G_DATASET, "H5 Dataset")
{

}

H5DatasetsList::~H5DatasetsList()
{

}
}
