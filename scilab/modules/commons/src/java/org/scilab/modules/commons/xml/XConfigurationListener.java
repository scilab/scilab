/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons.xml;

import java.util.EventListener;

public interface XConfigurationListener extends EventListener {

    /**
     * Prevent the listener that configuration has changed
     *
     * @param e the event
     */
    public void configurationChanged(XConfigurationEvent e);

}
