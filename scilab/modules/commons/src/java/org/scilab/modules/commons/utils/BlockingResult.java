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
public class BlockingResult<BlockingResultType> {

    private BlockingResultType theResult = null;
    private final Object lock = new Object();

    /**
     * Get the user input (wait until it)
     *
     * @return the user input
     */
    public BlockingResultType getResult() {
        synchronized (lock) {
            try {
                lock.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        return theResult;
    }

    /**
     * Set the result for this BlockingResult and notify
     *
     * @param theResult
     *            the user input to set
     */
    public void setResult(BlockingResultType theResult) {
        this.theResult = theResult;
        synchronized (lock) {
            lock.notify();
        }
    }

}
