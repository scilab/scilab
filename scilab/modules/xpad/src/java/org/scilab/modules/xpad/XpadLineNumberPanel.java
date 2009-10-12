/*
 * The base of this file is comming from http://tips4java.wordpress.com/about/
 * 
 * The license allows us to us for whatever is the reason. The actual license is
 * You are free to use and/or modify any or all code posted on the Java Tips Weblog without restriction. 
 * A credit in the code comments would be nice, but not in any way mandatory.
 * Author: Rob Camick
 * 
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Modifications:
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 *
 */
package org.scilab.modules.xpad;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.HashMap;

import javax.swing.JPanel;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.border.Border;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.border.MatteBorder;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;
import javax.swing.text.JTextComponent;
import javax.swing.text.StyleConstants;
import javax.swing.text.Utilities;
import javax.swing.text.Highlighter.HighlightPainter;

import org.scilab.modules.xpad.style.ScilabStyleDocument;

/**
 *  This class will display line numbers for a related text component. The text
 *  component must use the same line height for each line. TextLineNumber
 *  supports wrapped lines and will highlight the line number of the current
 *  line in the text component.
 *
 *  This class was designed to be used as a component added to the row header
 *  of a JScrollPane.
 */
public class XpadLineNumberPanel extends JPanel implements CaretListener, DocumentListener, PropertyChangeListener,
MouseListener, MouseMotionListener, HighlightPainter, KeyListener	{

	// Alignment of the line index
	public final static float LEFT_JUSTIFY = 0.0f;
	public final static float CENTER_JUSTIFY = 0.5f;
	public final static float RIGHT_JUSTIFY = 1.0f;

	private final static Border OUTER = new MatteBorder(0, 0, 0, 2, Color.GRAY);

	private final static int HEIGHT = Integer.MAX_VALUE - 1000000;

	public final static int PANEL_GAP_SIZE = 10;

	private JTextPane textPane;

	//  Properties that can be changed
	private boolean updateFont;
	private int borderGap;
	private Color currentLineForeground;
	private boolean isHighlighted;
	private Color currentLineHighlightColor = new Color(228,233,244);;
	private float alignmentOfNumber;
	private int minimumNumbers;

	//  Keep history information to reduce the number of times the component
	//  needs to be repainted
	private int lastNumber;
	private int lastHeight;
	private int lastLine;

	private HashMap<String, FontMetrics> fonts;

	/**
	 *	Create a line number component for a text component. This minimum
	 *  display width will be based on 3 digits.
	 *
	 *  @param textPane  the related text component
	 */
	public XpadLineNumberPanel(JTextPane textPane) {
		// Here we have 3, it means that the LineNumber panel will 
		// allow a minimum of 3 numbers
		this(textPane, 3);
	}

	/**
	 *	Create a line number component for a text component.
	 *
	 *  @param textPane the related text component
	 *  @param minimumNumbers  the number of digits used to calculate
	 *                               the minimum width of the component
	 */
	public XpadLineNumberPanel(JTextPane textPane, int minimumNumbers) {
		this.textPane = textPane;

		setFont(textPane.getFont());
		setBorderGap(PANEL_GAP_SIZE);
		setCurrentLineForeground(Color.RED);
		setNumberAlignment(CENTER_JUSTIFY);
		setMinimumNumbers(minimumNumbers);

		//textPane.getStyledDocument().addDocumentListener(this);
		//textPane.getDocument().addDocumentListener(this);
		textPane.addKeyListener(this);
		textPane.addCaretListener(this);
		textPane.addPropertyChangeListener("font", this);
		textPane.addMouseListener(this);
		textPane.addMouseMotionListener(this);

		try {
			textPane.getHighlighter().addHighlight(0, 0, this);
		} catch (BadLocationException ex) {
			ex.printStackTrace();
		}

	}

	/**
	 *  Gets the update font property
	 *
	 *  @return the update font property
	 */
	public boolean getUpdateFont() {
		return updateFont;
	}

	/**
	 *  Set the update font property. Indicates whether this Font should be
	 *  updated automatically when the Font of the related text component
	 *  is changed.
	 *
	 *  @param updateFont  when true update the Font and repaint the line
	 *                     numbers, otherwise just repaint the line numbers.
	 */
	public void setUpdateFont(boolean updateFont) {
		this.updateFont = updateFont;
	}

	/**
	 *  Gets the border gap
	 *
	 *  @return the border gap in pixels
	 */
	public int getBorderGap() {
		return borderGap;
	}

	/**
	 *  The border gap is used in calculating the left and right insets of the
	 *  border. Default value is 5.
	 *
	 *  @param borderGap  the gap in pixels
	 */
	public void setBorderGap(int borderGap) {
		this.borderGap = borderGap;
		Border inner = new EmptyBorder(0, borderGap, 0, borderGap);
		setBorder(new CompoundBorder(OUTER, inner));
		lastNumber = 0;
		setPreferredWidth();
	}

	/**
	 *  Gets the current line rendering Color
	 *
	 *  @return the Color used to render the current line number
	 */
	public Color getCurrentLineForeground() {
		return currentLineForeground == null ? getForeground() : currentLineForeground;
	}

	/**
	 *  The Color used to render the current line numbers. Default is Coolor.RED.
	 *
	 *  @param currentLineForeground  the Color used to render the current line
	 */
	public void setCurrentLineForeground(Color currentLineForeground) {
		this.currentLineForeground = currentLineForeground;
	}

	/**
	 *  Gets the number alignment
	 *
	 *  @return the alignment of the painted numbers
	 */
	public float getNumberAlignment() {
		return alignmentOfNumber;
	}

	/**
	 *  Specify the horizontal alignment of the numbers within the component.
	 *  Common values would be:
	 *  <ul>
	 *  <li>TextLineNumber.LEFT
	 *  <li>TextLineNumber.CENTER
	 *  <li>TextLineNumber.RIGHT (default)
	 *	</ul>
	 *  @param numberAlignment  the Color used to render the current line
	 */
	public void setNumberAlignment(float numberAlignment) {
		this.alignmentOfNumber =
			numberAlignment > 1.0f ? 1.0f : numberAlignment < 0.0f ? -1.0f : numberAlignment;
	}

	/**
	 *  Gets the minimum display numbers
	 *
	 *  @return the minimum display numbers
	 */
	public int getMinimumNumbers() {
		return minimumNumbers;
	}

	/**
	 *  Specify the mimimum number of digits used to calculate the preferred
	 *  width of the component. Default is 3.
	 *
	 *  @param minimumDisplayDigits  the number digits used in the preferred
	 *                               width calculation
	 */
	public void setMinimumNumbers(int minimumNumbers) {
		this.minimumNumbers = minimumNumbers;
		setPreferredWidth();
	}

	/**
	 *  Calculate the width needed to display the maximum line number
	 */
	private void setPreferredWidth() {
		Element root = textPane.getDocument().getDefaultRootElement();
		int lines = root.getElementCount();
		int numbers = Math.max(String.valueOf(lines).length(), minimumNumbers);

		//  Update sizes when number of digits in the line number changes

		if (lastNumber != numbers) {
			lastNumber = numbers;
			FontMetrics fontMetrics = getFontMetrics(getFont());
			int width = fontMetrics.charWidth('0') * numbers;
			Insets insets = getInsets();
			int preferredWidth = insets.left + insets.right + width;

			Dimension d = getPreferredSize();
			d.setSize(preferredWidth, HEIGHT);
			setPreferredSize( d );
			setSize( d );
		}
	}

	/**
	 *  Draw the line numbers
	 */
	public void paintComponent(Graphics g) {
		super.paintComponent(g);

		//	Determine the width of the space available to draw the line number
		FontMetrics fontMetrics = textPane.getFontMetrics(textPane.getFont());
		Insets insets = getInsets();
		int availableWidth = getSize().width - insets.left - insets.right;

		//  Determine the rows to draw within the clipped bounds.

		Rectangle clip = g.getClipBounds();
		int rowStartOffset = textPane.viewToModel(new Point(0, clip.y));
		int endOffset = textPane.viewToModel(new Point(0, clip.y + clip.height));

		while (rowStartOffset <= endOffset) {
			try {
				if (isCurrentLine(rowStartOffset))
				{
					g.setColor(getCurrentLineForeground());
				} else {
					g.setColor(getForeground());
				}

    			//  Get the line number as a string and then determine the
    			//  "X" and "Y" offsets for drawing the string.

				String lineNumber = getTextLineNumber(rowStartOffset);
				int stringWidth = fontMetrics.stringWidth(lineNumber);
				int x = getOffsetX(availableWidth, stringWidth) + insets.left;
				int y = getOffsetY(rowStartOffset, fontMetrics);
				g.drawString(lineNumber, x, y);

    			//  Move to the next row

				rowStartOffset = Utilities.getRowEnd(textPane, rowStartOffset) + 1;
			}
			catch(Exception ex) {
				ex.printStackTrace();
			}
		}
	}

	/*
	 *  We need to know if the caret is currently positioned on the line we
	 *  are about to paint so the line number can be highlighted.
	 */
	private boolean isCurrentLine(int rowStartOffset) {
		int caretPosition = textPane.getCaretPosition();
		Element root = textPane.getDocument().getDefaultRootElement();

		if (root.getElementIndex(rowStartOffset) == root.getElementIndex(caretPosition))
			return true;
		else
			return false;
	}

	/*
	 *	Get the line number to be drawn. The empty string will be returned
	 *  when a line of text has wrapped.
	 */
	protected String getTextLineNumber(int rowStartOffset) {
		Element root = textPane.getDocument().getDefaultRootElement();
		int index = root.getElementIndex(rowStartOffset);
		Element line = root.getElement(index);

		if (line.getStartOffset() == rowStartOffset)
			return String.valueOf(index + 1);
		else
			return "";
	}

	/*
	 *  Determine the X offset to properly align the line number when drawn
	 */
	private int getOffsetX(int availableWidth, int stringWidth) {
		return (int)((availableWidth - stringWidth) * alignmentOfNumber);
	}

	/*
	 *  Determine the Y offset for the current row
	 */
	private int getOffsetY(int rowStartOffset, FontMetrics fontMetrics) throws BadLocationException {

		Rectangle r = textPane.modelToView(rowStartOffset);
		int lineHeight = fontMetrics.getHeight();
		int y = r.y + r.height;
		int descent = 0;

		//  The text needs to be positioned above the bottom of the bounding
		//  rectangle based on the descent of the font(s) contained on the row.

		if (r.height == lineHeight) { // default font is being used

			descent = fontMetrics.getDescent();
		} else { // We need to check all the attributes for font changes

			if (fonts == null)
				fonts = new HashMap<String, FontMetrics>();

			Element root = textPane.getDocument().getDefaultRootElement();
			int index = root.getElementIndex(rowStartOffset);
			Element line = root.getElement(index);

			for (int i = 0; i < line.getElementCount(); i++) {
				Element child = line.getElement(i);
				AttributeSet as = child.getAttributes();
				String fontFamily = (String)as.getAttribute(StyleConstants.FontFamily);
				Integer fontSize = (Integer)as.getAttribute(StyleConstants.FontSize);
				String key = fontFamily + fontSize;

				FontMetrics fm = fonts.get(key);

				if (fm == null) {
					Font font = new Font(fontFamily, Font.PLAIN, fontSize);
					fm = textPane.getFontMetrics(font);
					fonts.put(key, fm);
				}
				descent = Math.max(descent, fm.getDescent());
			}
		}

		return y - descent;
	}

//
//  Implement CaretListener interface
//
	public void caretUpdate(CaretEvent e) {
		//  Get the line the caret is positioned on

		int caretPosition = textPane.getCaretPosition();
		Element root = textPane.getDocument().getDefaultRootElement();
		int currentLine = root.getElementIndex(caretPosition);

		//  Need to repaint so the correct line number can be highlighted

		if (lastLine != currentLine) {
			repaint();
			lastLine = currentLine;
		}

		textPane.repaint();
		resetHighlight();
	}

//
//  Implement DocumentListener interface
//
	public void changedUpdate(DocumentEvent e) {
		documentChanged();
		repaint();
	}
	public void insertUpdate(DocumentEvent e) {
		documentChanged();
		repaint();
	}
	public void removeUpdate(DocumentEvent e) {
		System.err.println("--- Calling LineNumberPanel.removeUpdate");
		documentChanged();
		repaint();
	}

	/*
	 *  A document change may affect the number of displayed lines of text.
	 *  Therefore the lines numbers will also change.
	 */
	private void documentChanged() {
		//  Preferred size of the component has not been updated at the time
		//  the DocumentEvent is fired

		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				int preferredHeight = textPane.getPreferredSize().height;
				//  Document change has caused a change in the number of lines.
				//  Repaint to reflect the new line numbers

				if (lastHeight != preferredHeight) {
					setPreferredWidth();
					repaint();
					lastHeight = preferredHeight;
				}
			}
		});
	}

//
//  Implement PropertyChangeListener interface
//
	public void propertyChange(PropertyChangeEvent evt) {
		if (evt.getNewValue() instanceof Font) {
			if (updateFont) {
				Font newFont = (Font) evt.getNewValue();
				setFont(newFont);
				lastNumber = 0;
				setPreferredWidth();
			} else {
				repaint();
			}
		}
	}

	private void resetHighlight() {
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				try {
					Element root = textPane.getDocument().getDefaultRootElement();
					int offset =  textPane.getCaretPosition();
					int currentLine = root.getElementIndex( offset );

					if (lastLine != currentLine) {
						Element lineElement = root.getElement( lastLine );

						if (lineElement != null) {
							int start = lineElement.getStartOffset();
							Rectangle r = textPane.modelToView(start);
							textPane.repaint(0, r.y, textPane.getWidth(), r.height);
						}

						lastLine = currentLine;
					}
				}
				catch(BadLocationException ex) {
					ex.printStackTrace();
				}
			}
		});
	}

	public void mouseClicked(MouseEvent arg0) {}
	public void mouseEntered(MouseEvent arg0) {}
	public void mouseExited(MouseEvent arg0) {}
	public void mousePressed(MouseEvent arg0) {
		resetHighlight();
		repaint();
	}
	public void mouseReleased(MouseEvent arg0) {}
	public void mouseDragged(MouseEvent arg0) {
		resetHighlight();
	}
	public void mouseMoved(MouseEvent arg0) {}

	public void paint(Graphics g, int p0, int p1, Shape bounds, JTextComponent textPane) {
		try {
			Rectangle r = textPane.modelToView(textPane.getCaretPosition());
			if (isHighlighted == true) {
				g.setColor(currentLineHighlightColor);
			} else {
				g.setColor(textPane.getBackground());
			}
			g.fillRect(0, r.y, textPane.getWidth(), r.height);
		}
		catch(BadLocationException ex) {
			ex.printStackTrace();
		}
	}

	public boolean getCurrentLineHighlightColor() {
		return isHighlighted;
	}

	public void setCurrentLineHighlightColor(boolean highlight) {
		this.isHighlighted = highlight;
	}

	public void keyPressed(KeyEvent arg0) {
		repaint();
	}

	public void keyReleased(KeyEvent arg0) {
		
	}

	public void keyTyped(KeyEvent arg0) {
		repaint();
	}
	
	public void highlightLine(int lineNumber) {
		if (lineNumber > 1 && lineNumber <= textPane.getStyledDocument().getDefaultRootElement().getElementCount()) {
			textPane.setCaretPosition(textPane.getStyledDocument().getDefaultRootElement().getElement(lineNumber - 1).getStartOffset());
			this.isHighlighted = true;
			repaint();
		}
	}

}
