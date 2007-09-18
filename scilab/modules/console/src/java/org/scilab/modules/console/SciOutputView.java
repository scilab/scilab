
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.Dimension;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;

import com.artenum.rosetta.ui.ConsoleTextPane;
import com.artenum.rosetta.util.StringConstants;

/**
 * Scilab Console UI which contains the previous commands and their outputs
 * @author Vincent COUVERT
 *
 */
public class SciOutputView extends ConsoleTextPane {
	
	private static final long serialVersionUID = 1L;
	
	private static final int TOP_BORDER = 0;
	private static final int BOTTOM_BORDER = 0;
	private static final int LEFT_BORDER = 0;
	private static final int RIGHT_BORDER = 0;

	private SciConsole console;
	
	/**
	 * Constructor
	 */
	public SciOutputView() {
		super();
		setBorder(BorderFactory.createEmptyBorder(TOP_BORDER, LEFT_BORDER, BOTTOM_BORDER, RIGHT_BORDER));
	}
	
	/**
	 * Adds text to the output view and change the size of others components if necessary
	 * @param entry text to add
	 */
	public void append(String entry) {

		/* Add the data to the output view */
		super.append(entry);

		/* Find the number of lines added */
		String[] lines = entry.split(StringConstants.NEW_LINE);

		/* Change the size of the input command view if necessary */
		/* - if the console size has been forced to a value */
		/* - if a carriage return has been appended */
		if (console != null && console.getInputCommandViewSizeForced() && lines.length > 0) {

			JTextPane outputView = ((JTextPane) console.getConfiguration().getOutputView());
			
			// Get JScrollPane viewport size to adapt input command view size
			JScrollPane jSP = console.getJScrollPane();
			Dimension jSPExtSize = jSP.getViewport().getExtentSize();

			/* Height of a text line in the ouput view */
			int charHeight = outputView.getFontMetrics(outputView.getFont()).getHeight();

			JPanel promptView = ((JPanel) console.getConfiguration().getPromptView());
			
			/* Input command view dimensions */
			int height = ((JTextPane) console.getConfiguration().getInputCommandView()).getPreferredSize().height;
			int width = ((JTextPane) console.getConfiguration().getInputCommandView()).getPreferredSize().width - jSPExtSize.width;

			int promptViewHeight = promptView.getPreferredSize().height;

			/* New dimension for the input command view */
			int newHeight = height - lines.length * (charHeight + 1); 
			Dimension newDim = null;
			
			if (newHeight > promptViewHeight) {
				/* If the input command view is bigger than the promptUI */
				/* It's height is descreased according to line number of lines added to output view */
				newDim = new Dimension(width, newHeight);
			} else {
				/* If the input command view is smaller than the promptUI */
				/* It's height adapted to the promptUI height */
				newDim = new Dimension(width, promptViewHeight);
				console.setInputCommandViewSizeForced(false);
			}
			/* Change the input command view size */
	       	((JTextPane) console.getConfiguration().getInputCommandView()).setPreferredSize(newDim);
        	((JTextPane) console.getConfiguration().getInputCommandView()).invalidate();
	    	((JTextPane) console.getConfiguration().getInputCommandView()).doLayout();
			
		}			
		
		if (console != null) {
			console.updateScrollPosition();
		}
		
	}
	/**
	 * Gets the minimum size of this component
	 * @return the minimum size
	 * @see javax.swing.JComponent#getMinimumSize()
	 */
	@Override
	public Dimension getMinimumSize() {
		return SMALL;
	}
	
	/**
	 * Sets the console object containing this output view
	 * @param c the console associated 
	 */
	public void setConsole(SciConsole c) {
		console = c;
		
		// Drag n' Drop handling
		this.setDropTarget(new DropTarget(this, DnDConstants.ACTION_COPY_OR_MOVE, new SciDropTargetListener(console)));
		
		FocusMouseListener focusGrabber = new FocusMouseListener(console);
		this.addMouseListener(focusGrabber);
	}

	/**
	 * Gets the console object containing this output view
	 * @return the console associated 
	 */
	public SciConsole getConsole() {
		return console;
	}

}
