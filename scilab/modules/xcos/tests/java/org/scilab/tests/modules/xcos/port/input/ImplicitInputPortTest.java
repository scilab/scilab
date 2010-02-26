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

package org.scilab.tests.modules.xcos.port.input;

import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.BasicPort.Type;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.testng.annotations.Test;

/**
 * Unit test for {@link ImplicitInputPort}
 */
public class ImplicitInputPortTest {
	public static final int DEFAULT_PORTSIZE = 8;
	
	@Test
	public void checkType() {
		ImplicitInputPort port = new ImplicitInputPort();
		assert port.getType() == Type.IMPLICIT;
	}
	
	@Test
	public void checkDefaultOrientation() {
		ImplicitInputPort port = new ImplicitInputPort();
		assert port.getOrientation() == Orientation.WEST;
	}
	
	@Test
	public void checkStyle() {
		ImplicitInputPort port = new ImplicitInputPort();
		assert port.getStyle().contains("ImplicitInputPort");
	}
	
	@Test
	public void checkTypeName() {
		ImplicitInputPort port = new ImplicitInputPort();
		assert port.getTypeName().equals("ImplicitInputPort");
	}
	
	@Test
	public void checkClassHierarchy() {
		ImplicitInputPort port = new ImplicitInputPort();
		assert port instanceof InputPort;
		assert port instanceof BasicPort;
	}
	
	@Test
	public void checkDefaultGeometry() {
		ImplicitInputPort port = new ImplicitInputPort();
		assert port.getGeometry().getWidth() == DEFAULT_PORTSIZE;
		assert port.getGeometry().getHeight() == DEFAULT_PORTSIZE;
	}
}
