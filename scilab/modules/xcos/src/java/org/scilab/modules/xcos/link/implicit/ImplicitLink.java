/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.link.implicit;

import org.scilab.modules.xcos.link.BasicLink;

/**
 * An implicit link connect an
 * {@link org.scilab.modules.xcos.port.output.ImplicitOutputPort} to an
 * {@link org.scilab.modules.xcos.port.input.ImplicitInputPort}.
 */
public class ImplicitLink extends BasicLink {

    private static final long serialVersionUID = 1006434636942664464L;
    // 1 means red color
    // 2 means implicit link
    private static final double[][] COLOR_AND_TYPE = { { 1.0, 2.0 } };

    /** Default constructor */
    public ImplicitLink() {
        super("ImplicitLink");
    }

    /** @return A red colored implicit link identifier */
    @Override
    public double[][] getColorAndType() {
        return COLOR_AND_TYPE;
    }
}
