
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.Component;
import java.awt.EventQueue;
import java.awt.FontMetrics;
import java.awt.Point;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.BadLocationException;

import org.scilab.modules.action_binding.InterpreterManagement;

import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.ui.ConsoleTextPane;
import com.artenum.rosetta.util.StringConstants;

/**
 * Scilab UI that contains the line edited by th euser
 * @author Vincent COUVERT
 */
public class SciInputCommandView extends ConsoleTextPane implements InputCommandView {
	
	private static final long serialVersionUID = 1L;
	private static final String END_LINE = "\n";
	private static final Point ERROR_POINT = new Point(0, 0);
	private static final int TOP_BORDER = 1;
	private static final int BOTTOM_BORDER = 2;

	private SciConsole console;

	/**
	 * Variable used to store the command entered by the user 
	 */
	private String cmdBuffer = "";
	
	/**
	 * Protection for safe reading
	 */
	private Semaphore canReadBuffer;
	
	/**
	 * Constructor
	 */
	public SciInputCommandView() {
		super();
		setBorder(BorderFactory.createEmptyBorder(TOP_BORDER, 0, BOTTOM_BORDER, 0));
		
		// Input command line is not editable when created
		this.setEditable(false);
		canReadBuffer = new Semaphore(1);
	}

	/**
	 * Gets the location of the caret in the UI
	 * @return the location as a Point object
	 * @see com.artenum.rosetta.interfaces.ui.InputCommandView#getCaretLocation()
	 */
	public Point getCaretLocation() {
		FontMetrics fontMetric = getFontMetrics(getFont());
		String[] lines = null;
		try {
			lines = getStyledDocument().getText(0, getCaretPosition()).split(END_LINE);
		} catch (BadLocationException e1) {
			e1.printStackTrace();
			return ERROR_POINT;
		}

		Point result = new Point(fontMetric.stringWidth(lines[lines.length - 1]), (lines.length * fontMetric.getHeight()));

		// Translate for absolute coordinates
		Component currentComponent = this;
		while (currentComponent != null) {
			result.translate(currentComponent.getLocation().x, currentComponent.getLocation().y);
			currentComponent = currentComponent.getParent();
			if (currentComponent instanceof JPanel) {
				return result;
			}
		}
		return result;
	}

	/**
	 * Gets the command buffer
	 * @return the command buffer
	 */
	public String getCmdBuffer() {
		this.setEditable(true);
		this.setFocusable(true);
		this.grabFocus();
		// Have to be allowed to write...
		try {
			// Try to acquire semaphore, if not, executes Scilab event loop for callbacks
			while (!canReadBuffer.tryAcquire(1, TimeUnit.MILLISECONDS)) {
				// TCl/TK event loop
				InterpreterManagement.execScilabEventLoop();
				// Callback events handling
				if (InterpreterManagement.haveCommandsInTheQueue()) {
					// Leave the buffer allowed
					canReadBuffer.release();
					return "";
				}
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		// Store command in an temp 
		String tmp = cmdBuffer;
		
		// Leave the buffer allowed
		canReadBuffer.release();
		
		return tmp;
	}
	
	/**
	 * Sets the command buffer after a user input in input command view
	 * @param command the string to set to the buffer
	 */
	public void setCmdBuffer(String command) {
		// The console do not wait to be allowed to write
		// Sure to have right to read
		// See SwingScilabConsole.readLine
		cmdBuffer = command;
		
		// Leave the buffer allowed
		canReadBuffer.release();
	}
	
	/**
	 * Sets a protection on the buffer to forbid reading it
	 */
	public void setBufferProtected() {
		try {
			canReadBuffer.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Sets the console object containing this input view
	 * @param c the console associated 
	 */
	public void setConsole(SciConsole c) {
		console = c;
		
		// Drag n' Drop handling
		this.setDropTarget(new DropTarget(this, DnDConstants.ACTION_COPY_OR_MOVE, new SciDropTargetListener(console)));
		
		// BUG 2510 fix: automatic validation of pasted lines
		this.getDocument().addDocumentListener(new DocumentListener() {
			public void changedUpdate(DocumentEvent e) {
				// Nothing to do in Scilab
			}

			public void insertUpdate(DocumentEvent e) {
				// Validates commands if followed by a carriage return
				String wholeTxt = console.getConfiguration().getInputParsingManager().getCommandLine();
				if ((e.getLength()) > 1 && (wholeTxt.lastIndexOf(StringConstants.NEW_LINE) == (wholeTxt.length() - 1))) {
					EventQueue.invokeLater(new Runnable() {
						public void run() {
							String wholeTxt = console.getConfiguration().getInputParsingManager().getCommandLine();
							console.sendCommandsToScilab(wholeTxt, true);
						};
					});
				}
			}

			public void removeUpdate(DocumentEvent e) {
				// Nothing to do in Scilab
			}
		});
	}
}
