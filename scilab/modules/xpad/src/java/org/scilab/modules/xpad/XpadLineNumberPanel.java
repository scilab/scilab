/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

public class XpadLineNumberPanel extends JPanel implements CaretListener, DocumentListener, PropertyChangeListener,
MouseListener, MouseMotionListener, HighlightPainter	{

	// Alignment of the line index
	public final static float LEFT_JUSTIFY = 0.0f;
	public final static float CENTER_JUSTIFY = 0.5f;
	public final static float RIGHT_JUSTIFY = 1.0f;

	private final static Border OUTER = new MatteBorder(0, 0, 0, 2, Color.GRAY);

	private final static int HEIGHT = Integer.MAX_VALUE - 1000000;

	public final static int PANEL_GAP_SIZE = 10;

	private JTextPane textPane;

	private boolean updateFont;
	private int borderGap;
	private Color currentLineForeground;
	private boolean isHighlighted;
	private Color currentLineHighlightColor = new Color(228,233,244);;
	private float alignmentOfNumber;
	private int minimumNumbers;

	private int lastNumber;
	private int lastHeight;
	private int lastLine;

	private HashMap<String, FontMetrics> fonts;


	public XpadLineNumberPanel(JTextPane textPane) {
		// Here we have 3, it means that the LineNumber panel will 
		// allow a minimum of 3 numbers
		this(textPane, 3);
	}

	public XpadLineNumberPanel(JTextPane textPane, int minimumNumbers) {
		this.textPane = textPane;

		setFont(textPane.getFont());
		setBorderGap(PANEL_GAP_SIZE);
		setCurrentLineForeground(Color.RED);
		setNumberAlignment(CENTER_JUSTIFY);
		setMinimumNumbers(minimumNumbers);

		textPane.getDocument().addDocumentListener(this);
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

	public boolean getUpdateFont() {
		return updateFont;
	}

	public void setUpdateFont(boolean updateFont) {
		this.updateFont = updateFont;
	}

	public int getBorderGap() {
		return borderGap;
	}

	public void setBorderGap(int borderGap) {
		this.borderGap = borderGap;
		Border inner = new EmptyBorder(0, borderGap, 0, borderGap);
		setBorder(new CompoundBorder(OUTER, inner));
		lastNumber = 0;
		setPreferredWidth();
	}

	public Color getCurrentLineForeground() {
		return currentLineForeground == null ? getForeground() : currentLineForeground;
	}

	public void setCurrentLineForeground(Color currentLineForeground) {
		this.currentLineForeground = currentLineForeground;
	}

	public float getNumberAlignment() {
		return alignmentOfNumber;
	}

	public void setNumberAlignment(float numberAlignment) {
		this.alignmentOfNumber =
			numberAlignment > 1.0f ? 1.0f : numberAlignment < 0.0f ? -1.0f : numberAlignment;
	}

	public int getMinimumNumbers() {
		return minimumNumbers;
	}

	public void setMinimumNumbers(int minimumNumbers) {
		this.minimumNumbers = minimumNumbers;
		setPreferredWidth();
	}

	private void setPreferredWidth() {
		Element root = textPane.getDocument().getDefaultRootElement();
		int lines = root.getElementCount();
		int numbers = Math.max(String.valueOf(lines).length(), minimumNumbers);

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

	public void paintComponent(Graphics g) {
		super.paintComponent(g);

		FontMetrics fontMetrics = textPane.getFontMetrics(textPane.getFont());
		Insets insets = getInsets();
		int availableWidth = getSize().width - insets.left - insets.right;

		Rectangle clip = g.getClipBounds();
		int rowStartOffset = textPane.viewToModel(new Point(0, clip.y));
		int endOffset = textPane.viewToModel(new Point(0, clip.y + clip.height));

		while (rowStartOffset <= endOffset) {
			try {
				if (isCurrentLine(rowStartOffset))
					g.setColor(getCurrentLineForeground());
				else
					g.setColor(getForeground());

				String lineNumber = getTextLineNumber(rowStartOffset);
				int stringWidth = fontMetrics.stringWidth(lineNumber);
				int x = getOffsetX(availableWidth, stringWidth) + insets.left;
				int y = getOffsetY(rowStartOffset, fontMetrics);
				g.drawString(lineNumber, x, y);

				rowStartOffset = Utilities.getRowEnd(textPane, rowStartOffset) + 1;
			}
			catch(Exception ex) {
				ex.printStackTrace();
			}
		}
	}

	private boolean isCurrentLine(int rowStartOffset) {
		int caretPosition = textPane.getCaretPosition();
		Element root = textPane.getDocument().getDefaultRootElement();

		if (root.getElementIndex(rowStartOffset) == root.getElementIndex(caretPosition))
			return true;
		else
			return false;
	}

	protected String getTextLineNumber(int rowStartOffset) {
		Element root = textPane.getDocument().getDefaultRootElement();
		int index = root.getElementIndex(rowStartOffset);
		Element line = root.getElement(index);

		if (line.getStartOffset() == rowStartOffset)
			return String.valueOf(index + 1);
		else
			return "";
	}

	private int getOffsetX(int availableWidth, int stringWidth) {
		return (int)((availableWidth - stringWidth) * alignmentOfNumber);
	}

	private int getOffsetY(int rowStartOffset, FontMetrics fontMetrics) throws BadLocationException {

		Rectangle r = textPane.modelToView(rowStartOffset);
		int lineHeight = fontMetrics.getHeight();
		int y = r.y + r.height;
		int descent = 0;

		if (r.height == lineHeight) {
			descent = fontMetrics.getDescent();
		} else {
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

	public void caretUpdate(CaretEvent e) {

		int caretPosition = textPane.getCaretPosition();
		Element root = textPane.getDocument().getDefaultRootElement();
		int currentLine = root.getElementIndex(caretPosition);

		if (lastLine != currentLine) {
			repaint();
			lastLine = currentLine;
		}

		resetHighlight();
	}

	public void changedUpdate(DocumentEvent e) {
		documentChanged();
	}
	public void insertUpdate(DocumentEvent e) {
		documentChanged();
	}
	public void removeUpdate(DocumentEvent e) {
		documentChanged();
	}

	private void documentChanged() {

		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				int preferredHeight = textPane.getPreferredSize().height;

				if (lastHeight != preferredHeight) {
					setPreferredWidth();
					repaint();
					lastHeight = preferredHeight;
				}
			}
		});
	}

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

}
