/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.graph.utils;

import java.util.Timer;
import java.util.TimerTask;

import org.scilab.modules.action_binding.utils.Signal;
import org.testng.annotations.Test;

/**
 * Test for the {@link Signal} class.
 */
public class SignalTest {
	/**
	 * Check that the following simple sequence is valid :
	 * <pre>
	 * wait
	 * notify
	 * </pre>
	 */
	@Test
	public void simpleNotify() {
		final String index = "1";

		new Timer().schedule((new TimerTask() {
			@Override
			public void run() {
				Signal.notify(index);
			}
		}), 1000);

		Signal.wait(index);
	}

	/**
	 * Check that the inverted sequence is valid :
	 * <pre>
	 * notify
	 * wait
	 * </pre>
	 */
	@Test
	public void invertedNotify() {
		final String index = "1";

		new Timer().schedule((new TimerTask() {
			@Override
			public void run() {
				Signal.wait(index);
			}
		}), 1000);

		Signal.notify(index);
	}
}
