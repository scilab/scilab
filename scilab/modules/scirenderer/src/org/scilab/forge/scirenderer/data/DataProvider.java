/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.data;

/**
 * @author Pierre Lando
 * @param <DataUserType> The type of data user.
 */
public interface DataProvider<DataUserType extends DataUser> {

    /**
     * Remove a data user.
     * @param dataUser the data user to remove.
     */
    void removeDataUser(DataUserType dataUser);

    /**
     * Add a data user.
     * @param texture the data user to add.
     */
    void addDataUser(DataUserType texture);

    /**
     * Data provider validity getter.
     * @return the validity if this data provider.
     */
    boolean isValid();
}
