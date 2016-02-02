/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
