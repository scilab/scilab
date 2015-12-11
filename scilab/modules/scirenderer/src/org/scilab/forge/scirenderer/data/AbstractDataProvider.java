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

import java.util.Collections;
import java.util.HashSet;
import java.util.Set;

/**
 * @author Pierre Lando
 * @param <DataUserType> The type of data user.
 */
public abstract class AbstractDataProvider<DataUserType extends DataUser> implements DataProvider<DataUserType> {

    private final Set<DataUserType> users = Collections.synchronizedSet(new HashSet<DataUserType>());

    @Override
    public final void removeDataUser(DataUserType dataUser) {
        users.remove(dataUser);
    }

    @Override
    public final void addDataUser(DataUserType dataUser) {
        users.add(dataUser);
    }

    /**
     * Notify all registered data users for a data update.
     */
    protected final void fireUpdate() {
        for (DataUserType dataUser : users) {
            dataUser.dataUpdated();
        }
    }

    /**
     * Convert given value to a byte.
     * double in [0, 1] are mapped to [0x00, 0xFF].
     * @param value the given value.
     * @return the byte corresponding to the given value.
     */
    protected byte toByte(double value) {
        return (byte) (((int) (value * 255)) & 0xFF);
    }

    /**
     * Convert given value to a byte.
     * double in [0, 1] are mapped to [0x00, 0xFF].
     * @param values the given value.
     * @return the byte corresponding to the given value.
     */
    protected byte[] toByte(float[] values) {
        byte bytes[] = new byte[values.length];
        for (int i = 0; i < values.length; i++) {
            bytes[i] = toByte(values[i]);
        }
        return bytes;
    }
}
