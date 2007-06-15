
/* Copyright INRIA 2007 */

package fr.scilab.test;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.UIManager;

import org.flexdock.docking.DockingManager;
import org.flexdock.util.SwingUtility;

import fr.scilab.console.Console;
import fr.scilab.console.ScilabConsole;
import fr.scilab.ihm.bridge.swing.SwingScilabConsole;
import fr.scilab.ihm.frame.Frame;
import fr.scilab.ihm.frame.ScilabFrame;
import fr.scilab.ihm.tab.ScilabTab;
import fr.scilab.ihm.tab.Tab;
import fr.scilab.ihm.window.ScilabWindow;
import fr.scilab.ihm.window.Window;

/**
 * Test class for Scilab 5.0 GUIs
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public class ScilabConsoleTest {

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
		System.out.println("-*- Stopping Program -*-");

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
		f.setSize(200,200);
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
