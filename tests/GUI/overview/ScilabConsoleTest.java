
/* Copyright INRIA 2007 */

package overview;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.UIManager;

import org.flexdock.docking.DockingManager;
import org.flexdock.util.SwingUtility;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.console.Console;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.frame.Frame;
import org.scilab.modules.gui.frame.ScilabFrame;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;

/**
 * Test class for Scilab 5.0 GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class ScilabConsoleTest {

	private static final int WIDTH = 200;
	private static final int HEIGHT = 200;
	
	/**
	 * Constructor
	 */
	protected ScilabConsoleTest() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}
	
	/**
	 * Launch main Scilab
	 * @param args not used
	 */
	public static void main(String[] args) {
		SwingUtility.setPlaf(UIManager.getSystemLookAndFeelClassName());

		DockingManager.setFloatingEnabled(true);

		EventQueue.invokeLater(new Runnable() {
			public void run() {
				createAndShowGui();
			}
		});
	}

	/**
	 * Creates and show Scilab main GUI
	 */
	private static void createAndShowGui() {
		System.out.println("-*- Starting Program -*-");

		Window mainView = ScilabWindow.createWindow();
		mainView.draw();

		Tab consoleTab = ScilabTab.createTab("Console");
		mainView.addTab(consoleTab);
		mainView.draw();
		System.out.println("Console Tab Title: " + consoleTab.getName());
		System.out.println("-*- Stoping Program -*-");

		/* Console creation */
		Frame consoleFrame = ScilabFrame.createFrame();

		Console sciConsole = ScilabConsole.createConsole();

		//JyConsole console2 = new JyConsole();

		consoleFrame.addMember(sciConsole);
		//ScilabBridge.addMember(consoleFrame, sciConsole);

		// Bruno
		//((JPanel) consoleFrame).add(console2);
		//((JPanel) consoleFrame).add(new JButton("Ca pue @"));
		//Console sick = new SwingScilabConsole();
		//((JPanel) consoleFrame).add(new SwingScilabConsole());

		//ScilabBridge.addMember(consoleTab, consoleFrame);
		consoleTab.addMember(consoleFrame);

		consoleFrame.draw();
		consoleTab.draw();
		mainView.draw();

		JFrame f = new JFrame();
		f.setSize(WIDTH, HEIGHT);
		SwingScilabConsole content = new SwingScilabConsole();
		f.add(content);
		f.setVisible(true);

		//		JyConsole console = new JyConsole();
//		JFrame f = new JFrame("JyConsole by Artenum");
//				//f.setIconImage(new ImageIcon(JyConsole.class.getResource("logo.gif")).getImage());
//		f.getContentPane().add(console, BorderLayout.CENTER);
//		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
//		f.setSize(600, 400);
//		f.setLocationRelativeTo(null);
//		f.setVisible(true);
	}
}
