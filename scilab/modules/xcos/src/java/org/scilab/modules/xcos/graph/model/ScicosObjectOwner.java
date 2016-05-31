/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.graph.model;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

/**
 * Helper class to own a Scicos MVC object from the Java world.
 */
public class ScicosObjectOwner {
    final long uid;
    final Kind kind;

    public ScicosObjectOwner(long uid, Kind kind) {
        this(new JavaController(), uid, kind);
    }

    public ScicosObjectOwner(final JavaController controller, long uid, Kind kind) {
        // defensive programming
        if (uid == 0) {
            throw new IllegalStateException();
        }

        this.uid = uid;
        this.kind = kind;

        controller.referenceObject(uid);
    }

    public long getUID() {
        return uid;
    }

    public Kind getKind() {
        return kind;
    }

    @Override
    public String toString() {
        return "[ " + uid + " , " + kind.name() + " ]";
    }

    @Override
    protected void finalize() throws Throwable {
        JavaController controller = new JavaController();

        controller.deleteObject(uid);
    }
}