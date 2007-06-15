
/* Copyright INRIA 2007 */

package fr.scilab.test.window.size;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import fr.scilab.ihm.utils.Size;
import fr.scilab.ihm.window.ScilabWindow;
import fr.scilab.ihm.window.Window;

/**
 * Unitary test for window size modification in Scilab
 * @author Bruno Jofret
 */
public class SizeTest {

	private static final int DEFAULTWIDTH = 100;
	private static final int DEFAULTHEIGHT = 100;
	private static final int NEWWIDTH = 500;
	private static final int NEWHEIGHT = 800;

	/**
	 * Constructor for class SizeTest
	 */
	public SizeTest() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	};

	/**
	 * Default window size test
	 */
	@Test
	public void windowDefaultSize() {

		Window mainView = ScilabWindow.createWindow();
		Size windowSize = mainView.getDims();
		/* Checking Default Size */
		assertEquals(DEFAULTWIDTH, windowSize.getWidth());
		assertEquals(DEFAULTHEIGHT, windowSize.getHeight());
	}


	/**
	 * Test the change of width
	 */
	@Test
	public void windowChangeWidth() {
		int newWidth = NEWWIDTH;
		Window mainView = ScilabWindow.createWindow();
		Size windowSize = mainView.getDims();
		/* Checking Default Size */
		assertEquals(DEFAULTWIDTH, windowSize.getWidth());
		assertEquals(DEFAULTHEIGHT, windowSize.getHeight());

		/* Changing Width */
		Size newWindowSize = new Size(newWidth, windowSize.getHeight());
		mainView.setDims(newWindowSize);
		/* Checking Updated Size */
		assertEquals(newWidth, mainView.getDims().getWidth());
		assertEquals(DEFAULTHEIGHT, mainView.getDims().getHeight());
	}

	/**
	 * Test the change of height
	 */
	@Test
	public void windowChangeHeight() {
		int newHeight = NEWHEIGHT;
		Window mainView = ScilabWindow.createWindow();
		Size windowSize = mainView.getDims();
		/* Checking Default Size */
		assertEquals(DEFAULTWIDTH, windowSize.getWidth());
		assertEquals(DEFAULTHEIGHT, windowSize.getHeight());

		/* Changing Width */
		Size newWindowSize = new Size(windowSize.getWidth(), newHeight);
		mainView.setDims(newWindowSize);
		/* Checking Updated Size */
		assertEquals(DEFAULTWIDTH, mainView.getDims().getWidth());
		assertEquals(newHeight, mainView.getDims().getHeight());
	}

}
