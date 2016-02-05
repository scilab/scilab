/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.xcos;

/**
 * Dummy abstract implementation of an XcosView listener.
 */
public abstract class XcosViewListener {
    public void objectCreated(long uid, Kind kind) {};
    public void objectDeleted(long uid, Kind kind) {};
    public void objectReferenced(long uid, Kind kind, long refCount) {};
    public void objectUnreferenced(long uid, Kind kind, long refCount) {};
    public void propertyUpdated(long uid, Kind kind, ObjectProperties property, UpdateStatus status) {};
}
