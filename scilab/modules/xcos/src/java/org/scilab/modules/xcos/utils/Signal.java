/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.util.HashMap;
import java.util.Map;

public class Signal {

    private static Map<String, Object> waiters = new HashMap<String, Object>();

    public static void wait(String index) {
	Object data = new Object();
	waiters.put(index, data);

	synchronized (data) {
	    try {
		data.wait();
	    } catch (InterruptedException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    }
	}
    }

    public static void notify(String index) {
	Object data = waiters.get(index);
	if (data != null) {
	    synchronized (data) {
		data.notify();   
	    }
	    waiters.remove(index);
	}
    }

}
