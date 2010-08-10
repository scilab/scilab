/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Jerzy ZAGORSKI
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.xcos.simulink;

import java.lang.reflect.InvocationTargetException;

import javax.swing.SwingUtilities;

import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BasicBlock.SimulationFunctionType;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.types.ScilabDouble;

import org.testng.annotations.*;


/**
 * Test of the {@link ConstantTest} block importation.
 */
public class ConstantTest {

	/**
	 * Initialization of diagram needed by tests
	 * @throws InvocationTargetException 
	 * @throws InterruptedException 
	 */
	@BeforeClass
	public void setUp() throws InterruptedException, InvocationTargetException {
		
		String XcosTest =  ScilabConstants.SCI + "/modules/xcos/tests/java/org/scilab/tests/modules/xcos/simulink/";
		Xcos.simulinkImport(XcosTest + "ConstantTest.mdl");
		
		// perform assert on the EDT Thread and after all events
		SwingUtilities.invokeAndWait(new Runnable() {
			@Override
			public void run() {
				assert PaletteManager.isVisible();
				assert Xcos.getInstance().getDiagrams().size() == 1;
			}
		});	
	}
	
	@Test
	public void checkSimulationFunctionName() {
		XcosDiagram xcosDiagram = Xcos.getInstance().getDiagrams().get(0);
		BasicBlock toVerify = (BasicBlock) xcosDiagram.getModel().getChildAt(xcosDiagram.getDefaultParent(), 0);

		assert toVerify.getSimulationFunctionName().equals("cstblk4");
	}
	@Test
	public void checkSimulationFunctionType() {
		XcosDiagram xcosDiagram = Xcos.getInstance().getDiagrams().get(0);
		BasicBlock toVerify = (BasicBlock) xcosDiagram.getModel().getChildAt(xcosDiagram.getDefaultParent(), 0);
		
		assert toVerify.getSimulationFunctionType() == SimulationFunctionType.C_OR_FORTRAN;
	}
	@Test
	public void checkGraphics() {
		
	}
	@Test
	public void checkExprs() {
		
	}
	@Test
	public void checkState() {
		
	}
	@Test
	public void checkDState() {
		
	}
	@Test
	public void checkODState() {
		
	}
	@Test
	public void checkRealParameters() {
		XcosDiagram xcosDiagram = Xcos.getInstance().getDiagrams().get(0);
		BasicBlock toVerify = (BasicBlock) xcosDiagram.getModel().getChildAt(xcosDiagram.getDefaultParent(), 0);
		
		assert toVerify.getRealParameters().equals(new ScilabDouble(1));
	}
	@Test
	public void checkIntegerParameters() {
		
	}
	@Test
	public void checkObjectParameters() {
		
	}
	@Test
	public void checkDependsOnT() {
		
	}
	@Test
	public void checkNbZeroCrossing() {
		
	}
	@Test
	public void checkNmode() {
		
	}
	@Test
	public void checkEquations() {
		
	}
	@Test
	public void checkInterfaceFunctionName() {
		XcosDiagram xcosDiagram = Xcos.getInstance().getDiagrams().get(0);
		BasicBlock toVerify = (BasicBlock) xcosDiagram.getModel().getChildAt(xcosDiagram.getDefaultParent(), 0);
		
		assert toVerify.getInterfaceFunctionName().equals("CONST_m");
	}
	
	/**
	 * Clean up after tests are launched, and test if Xcos diagrams were closed correctly
	 * @throws InvocationTargetException 
	 * @throws InterruptedException 
	 */
	@AfterClass
	public void cleanUp() throws InterruptedException, InvocationTargetException {
		SwingUtilities.invokeAndWait(new Runnable() {
			@Override
			public void run() {
				// set the diagrm not modified before closing (avoid popup message)
				Xcos.getInstance().getDiagrams().get(0).setModified(false);
			}
		});
		
		Xcos.closeXcosFromScilab();
		
		// perform assert on the EDT Thread and after all events
		SwingUtilities.invokeAndWait(new Runnable() {
			@Override
			public void run() {
				assert !PaletteManager.isVisible();
				assert Xcos.getInstance().getDiagrams().size() == 0;
			}
		});
	}
}
