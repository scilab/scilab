/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.Dimension;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;
import java.io.Writer;
import java.util.LinkedList;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyleContext;

import com.artenum.rosetta.interfaces.ui.OutputView;
import com.artenum.rosetta.util.StringConstants;
import com.artenum.rosetta.util.BufferedWriter;

/**
 * Scilab Console UI which contains the previous commands and their outputs
 * 
 * @author Vincent COUVERT
 */
public class SciOutputView extends JTextPane implements OutputView {
	private static final long serialVersionUID = 1L;

	private static final int TOP_BORDER = 0;

	private static final int BOTTOM_BORDER = 0;

	private static final int LEFT_BORDER = 0;

	private static final int RIGHT_BORDER = 0;

	private static final int BUFFER_SIZE = 10;

	private String activeStyle;

	private String lastAppendedStyle;

	private BlockingQueue<StringBuffer> bufferQueue;

	private LinkedList<String> styleQueue;

	private StringBuffer currentWorkingBuffer;

	private SciConsole console;
	
	private Thread thread;

	/**
	 * Constructor
	 */
	public SciOutputView() {
		setBorder(BorderFactory.createEmptyBorder(TOP_BORDER, LEFT_BORDER,
				BOTTOM_BORDER, RIGHT_BORDER));

		// Enabled Drag&Drop with this component
		this.setDragEnabled(true);

		activeStyle = StyleContext.DEFAULT_STYLE;
		bufferQueue = new ArrayBlockingQueue<StringBuffer>(BUFFER_SIZE);
		styleQueue = new LinkedList<String>();
		thread = new Thread() {
			public void run() {
				while (!bufferQueue.isEmpty() && console != null) {
					displayLineBuffer();
				}
				if (console != null && console.isWorkDone()) {
					console.waitForInput();
				}
			}
		};
		thread.setPriority(Thread.MIN_PRIORITY);
	}

	/**
	 * Thread used to display data in console
	 * 
	 * @see java.lang.Runnable#run()
	 */
	public void run() {
			while (!bufferQueue.isEmpty() && console != null) {
				displayLineBuffer();
			}
			if (console != null && console.isWorkDone()) {
				console.waitForInput();
			}
	}

	/**
	 * Display a buffer entry in the console
	 */
	private void displayLineBuffer() {
		
		StringBuffer buffer = null;
		try {
			buffer = bufferQueue.take();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String style = styleQueue.poll();

		/*
		 * Temporary variables created to avoid to long line (checkstyle
		 */
		String dispBuffer = buffer.toString();
		int sDocLength = getStyledDocument().getLength();
		try {
			getStyledDocument().insertString(sDocLength, dispBuffer,
					getStyledDocument().getStyle(style));
		} catch (BadLocationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		/* Special case for Scilab when clc or tohome have been used */
		String[] lines = buffer.toString().split(
				StringConstants.NEW_LINE);
		/* Change the size of the input command view if necessary */
		/* - if the console size has been forced to a value */
		/* - if a carriage return has been appended */
		if (console != null
				&& console.getInputCommandViewSizeForced()
				&& lines.length > 0) {

			JTextPane outputView = ((JTextPane) console
					.getConfiguration().getOutputView());

			// Get JScrollPane viewport size to adapt input command
			// view size
			JScrollPane jSP = console.getJScrollPane();
			Dimension jSPExtSize = jSP.getViewport()
			.getExtentSize();

			/* Height of a text line in the ouput view */
			int charHeight = outputView.getFontMetrics(
					outputView.getFont()).getHeight();

			JPanel promptView = ((JPanel) console
					.getConfiguration().getPromptView());

			/* Input command view dimensions */
			JTextPane inputCmdView = ((JTextPane) console
					.getConfiguration().getInputCommandView());
			int height = inputCmdView.getPreferredSize().height;
			int width = inputCmdView.getPreferredSize().width
			- jSPExtSize.width;

			int promptViewHeight = promptView.getPreferredSize().height;

			/* New dimension for the input command view */
			/*
			 * -1 because last EOL removed in
			 * SwingScilabConsole.readline
			 */
			int newHeight = height - (lines.length - 1)
			* charHeight;
			Dimension newDim = null;

			if (newHeight > promptViewHeight) {
				/*
				 * If the input command view is bigger than the
				 * promptUI
				 */
				/*
				 * It's height is descreased according to line
				 * number of lines added to output view
				 */
				newDim = new Dimension(width, newHeight);
			} else {
				/*
				 * If the input command view is smaller than the
				 * promptUI
				 */
				/* It's height adapted to the promptUI height */
				newDim = new Dimension(width, promptViewHeight);
				console.setInputCommandViewSizeForced(false);
			}
			/* Change the input command view size */
			((JTextPane) console.getConfiguration()
					.getInputCommandView())
					.setPreferredSize(newDim);
			((JTextPane) console.getConfiguration()
					.getInputCommandView()).invalidate();
			((JTextPane) console.getConfiguration()
					.getInputCommandView()).doLayout();
		}
		/* Update scroll only if console has been set */
		if (console != null) {
			console.updateScrollPosition();
		}
		
	}
	
	/**
	 * Adds text to the output view and change the size of others components if
	 * necessary
	 * 
	 * @param content
	 *            text to add
	 */
	public void append(String content) {
		append(content, activeStyle);
	}

	/**
	 * Adds text to the output view and change the size of others components if
	 * necessary
	 * 
	 * @param content
	 *            text to add
	 * @param styleName
	 *            style to set for content
	 */
	public void append(String content, String styleName) {
		if (styleName.equals(lastAppendedStyle) && bufferQueue.size() > 1) {
			currentWorkingBuffer.append(content);
		} else {
			lastAppendedStyle = styleName;
			styleQueue.add(lastAppendedStyle);
			try {
				currentWorkingBuffer = new StringBuffer(content);
				bufferQueue.put(currentWorkingBuffer);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		if (!thread.isAlive()) {
			thread.run();
		}
	}

	/**
	 * Gets the error writer
	 * 
	 * @return the error writer
	 * @see com.artenum.rosetta.interfaces.ui.OutputView#getErrorWriter()
	 */
	public Writer getErrorWriter() {
		return new BufferedWriter(StyleContext.DEFAULT_STYLE, bufferQueue,
				styleQueue);
	}

	/**
	 * Gets the writer
	 * 
	 * @return the writer
	 * @see com.artenum.rosetta.interfaces.ui.OutputView#getWriter()
	 */
	public Writer getWriter() {
		return new BufferedWriter(StyleContext.DEFAULT_STYLE, bufferQueue,
				styleQueue);
	}

	/**
	 * Resets the output view (remove text)
	 * 
	 * @see com.artenum.rosetta.interfaces.ui.OutputView#reset()
	 */
	public void reset() {
		try {
			getStyledDocument().remove(0, getStyledDocument().getLength());
		} catch (BadLocationException e) {
			e.printStackTrace();
		}
		setCaretPosition(0);
	}

	/**
	 * Move the caret to the beginning of the styled document
	 * 
	 * @see com.artenum.rosetta.interfaces.ui.OutputView#setCaretPositionToBeginning()
	 */
	public void setCaretPositionToBeginning() {
		setCaretPosition(0);
	}

	/**
	 * Move the caret to the end of the styled document
	 * 
	 * @see com.artenum.rosetta.interfaces.ui.OutputView#setCaretPositionToEnd()
	 */
	public void setCaretPositionToEnd() {
		setCaretPosition(getStyledDocument().getLength());
	}

	/**
	 * Set the style for current text
	 * 
	 * @param styleName
	 *            the style to set
	 * @see com.artenum.rosetta.interfaces.ui.OutputView#setStyleName(java.lang.String)
	 */
	public void setStyleName(String styleName) {
		activeStyle = styleName;
	}

	/**
	 * Sets the console object containing this output view
	 * 
	 * @param c
	 *            the console associated
	 */
	public void setConsole(SciConsole c) {
		console = c;

		// Drag n' Drop handling
		this.setDropTarget(new DropTarget(this,
				DnDConstants.ACTION_COPY_OR_MOVE, new SciDropTargetListener(
						console)));

		FocusMouseListener focusGrabber = new FocusMouseListener(console);
		this.addMouseListener(focusGrabber);
	}

	/**
	 * Gets the console object containing this output view
	 * 
	 * @return the console associated
	 */
	public SciConsole getConsole() {
		return console;
	}
	
	/**
	 * Get the current thread used to display 
	 * @return the thread
	 */
	public Thread getThread() {
		return thread;
	}
}
