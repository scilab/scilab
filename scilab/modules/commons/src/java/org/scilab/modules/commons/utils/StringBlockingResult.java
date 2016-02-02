/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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

package org.scilab.modules.commons.utils;

/**
 * Class use for dialogs that wait for a user input
 *
 * @author Bruno JOFRET
 */
public final class StringBlockingResult extends BlockingResult<String> {

    private static StringBlockingResult me;

    /**
     * Get the current instance of BlockingResult
     * @return this instance
     */
    public static StringBlockingResult getInstance() {
        if (me == null) {
            me = new StringBlockingResult();
        }
        return me;
    }
}
