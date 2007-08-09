
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.awt.Dimension;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;

import javax.swing.JPanel;
import javax.swing.JTextPane;

import com.artenum.console.ui.ConsoleTextPane;
import com.artenum.console.util.StringConstants;

/**
 * Scilab Console UI which contains the previous commands and their outputs
 * @author Vincent COUVERT
 *
 */
public class SciOutputView extends ConsoleTextPane {
	
	private static final long serialVersionUID = 1L;
	
	private SciConsole console;
	
	/**
	 * Constructor
	 */
	public SciOutputView() {
		super();
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
		/* - if a carriage return has been appened */
		if (console != null && console.getInputCommandViewSizeForced() && lines.length > 0) {
			JTextPane outputView = ((JTextPane) console.getConfiguration().getOutputView());
			
			/* Height of a text line in the ouput view */
			int charHeight = outputView.getFontMetrics(outputView.getFont()).getHeight();

			JPanel promptView = ((JPanel) console.getConfiguration().getPromptView());
			
			/* Input command view dimensions */
			int height = ((JPanel) promptView.getParent()).getPreferredSize().height;
			int width = ((JPanel) promptView.getParent()).getPreferredSize().width;

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
			((JPanel) promptView.getParent()).setPreferredSize(newDim);
			((JPanel) promptView.getParent()).invalidate();
			((JPanel) promptView.getParent()).doLayout();
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
	}

	/**
	 * Gets the console object containing this output view
	 * @return the console associated 
	 */
	public SciConsole getConsole() {
		return console;
	}

}
