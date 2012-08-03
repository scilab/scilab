/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
