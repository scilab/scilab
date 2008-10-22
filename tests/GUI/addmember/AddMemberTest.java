
/* Copyright INRIA 2007 */

package addmember;

import static org.junit.Assert.assertTrue;

import org.junit.Test;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.ScilabFrame;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;

/**
 * Test class for Scilab 5.0 GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class AddMemberTest {

	
	private static final String TAB_DEFAULT_TITLE = "Tab";
	private static final String START_BANNER = "-*- Starting Program -*-";
	private static final String STOP_BANNER = "-*- Stoping Program -*-";
	
	/**
	 * 
	 */
	protected AddMemberTest() {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * We want to be able to add a Frame in a Tab
	 */
	@Test
	public void addFrameInTab() {
		System.out.println(START_BANNER);

		/* Tab Creation */
		Tab tab = ScilabTab.createTab(TAB_DEFAULT_TITLE);

		/* Frame creation */
		Frame frame = ScilabFrame.createFrame();

		// Try to add a Frame in a Tab
		assertTrue("_MUST_ be able to add a Frame in a Tab", tab.addMember(frame) != -1);
		assertTrue("_MUST_ be able to add a Frame in a Tab (Through the Bridge)", ScilabBridge.addMember(tab, frame) != -1);
		System.out.println(STOP_BANNER);
	}

	/**
	 * We DO NOT want to be able to add a Console in a Frame
	 */
	@Test
	public void addConsoleInFrame() {
		System.out.println(START_BANNER);

		/* Frame creation */
		Frame frame = ScilabFrame.createFrame();

		/* Console creation */
		Console console = ScilabConsole.createConsole();

		// Try to add a Console in a Frame
		assertTrue("_MUST_NOT_ be able to add a Console in a Frame", frame.addMember(console) == -1);
		assertTrue("_MUST_ be able to add a Console in a Frame (Through the Bridge)", ScilabBridge.addMember(frame, console) != -1);
		System.out.println(STOP_BANNER);
	}

	/**
	 * We want to be able to add a Console in a Tab
	 */
	@Test
	public void addConsoleInTab() {
		System.out.println(START_BANNER);

		/* Tab creation */
		Tab tab = ScilabTab.createTab(TAB_DEFAULT_TITLE);

		/* Console creation */
		Console console = ScilabConsole.createConsole();

		// Try to add a Console in a Tab
		assertTrue("_MUST_ be able to add a Console in a Tab", tab.addMember(console) != -1);
		assertTrue("_MUST_ be able to add a Console in a Tab(Through the Bridge)", ScilabBridge.addMember(tab, console) != -1);
		System.out.println(STOP_BANNER);
		}

}
