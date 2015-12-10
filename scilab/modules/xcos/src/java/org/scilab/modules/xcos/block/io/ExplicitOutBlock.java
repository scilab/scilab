/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS <antoine.elias@scilab.org>
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID <clement.david@scilab.org>
 * Copyright (C) 2010-2015 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block.io;

import com.mxgraph.model.mxGeometry;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;

/**
 * Implement an explicit output point of a
 * {@link org.scilab.modules.xcos.block.SuperBlock}.
 */
public final class ExplicitOutBlock extends ContextUpdate {

    private static final long serialVersionUID = -3423053321045811400L;

    /**
     * Constructor
     */
    public ExplicitOutBlock(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);
    }
}
