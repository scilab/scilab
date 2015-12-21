/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph.model;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

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