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
import java.awt.Component;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JPanel;
import javax.swing.JEditorPane;
import javax.swing.border.Border;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.border.MatteBorder;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.Element;
import javax.swing.text.Document;
import javax.swing.text.JTextComponent;

/**
 *  This class will display line numbers for a related text component. The text
 *  component must use the same line height for each line. TextLineNumber
 *  supports wrapped lines and will highlight the line number of the current
 *  line in the text component.
 *
 *  This class was designed to be used as a component added to the row header
 *  of a JScrollPane.
 */
public class XpadLineNumberPanel extends JPanel implements CaretListener, DocumentListener, PropertyChangeListener {

    private final static Border OUTER = new MatteBorder(0, 0, 0, 2, Color.GRAY);
        
    private final static int HEIGHT = Integer.MAX_VALUE - 1000000;
    
    public final static int PANEL_GAP_SIZE = 10;
    
    private JEditorPane textPane;
    
    //  Properties that can be changed
    private boolean updateFont;
    private int borderGap;
    private Color currentLineForeground;
    private boolean isHighlighted;
    private Color currentLineHighlightColor = new Color(228,233,244);;
    
    //  Keep history information to reduce the number of times the component
    //  needs to be repainted
    private int numbers = 0;
    private int lastLine;
    
    private ScilabDocument doc;
    private FontMetrics metrics;
    private int ascent;
    private int availableWidth;

    /**
     *	Create a line number component for a text component.
     *
     *  @param textPane the related text component
     *  @param minimumNumbers  the number of digits used to calculate
     *                               the minimum width of the component
     */
    public XpadLineNumberPanel(JEditorPane textPane) {
	this.textPane = textPane;
	this.doc = (ScilabDocument) textPane.getDocument();
	this.metrics = textPane.getFontMetrics(textPane.getFont());
	this.ascent = metrics.getAscent();
	setOpaque(true);
	setFont(textPane.getFont());
	setBorderGap(PANEL_GAP_SIZE);
	updateWidth();
	setCurrentLineForeground(Color.RED);
	doc.addDocumentListener(this);
	textPane.addCaretListener(this);
	textPane.addPropertyChangeListener("font", this);
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
    
    private void updateWidth() {
	++numbers;
	Insets insets = getInsets();
	int width = metrics.charWidth('0') * numbers;
	availableWidth = width;
	Dimension d = getPreferredSize();
	d.setSize(availableWidth, HEIGHT);
	availableWidth = width;
	setPreferredSize(d);
	setSize(d);
    }

    /**
     *  Draw the line numbers
     */
    public void paintComponent(Graphics g) {
	super.paintComponent(g);
	
	synchronized (doc) {
	    Element root = doc.getDefaultRootElement();
	    ScilabView view = doc.getView();
	    Rectangle clip = g.getClipBounds();
	    Insets insets = getInsets();
	    Point pt = new Point(0, clip.y);
	    int rowStartOffset = textPane.viewToModel(pt);
	    pt.y += clip.height;
	    int endOffset = textPane.viewToModel(pt);
	    root = doc.getDefaultRootElement();
	    int line = root.getElementIndex(rowStartOffset);
	    int lineEnd = root.getElementIndex(endOffset);
	   
	    for (; line <= lineEnd; line++) {
		if (line != lastLine) {
		    g.setColor(getForeground());
		} else {
		    g.setColor(getCurrentLineForeground());
		}
		
		String str = Integer.toString(line + 1);
		int diff = (availableWidth - metrics.stringWidth(str)) / 2;
		if (diff <= 0) {
		    updateWidth();
		    diff = (availableWidth - metrics.stringWidth(str)) / 2;
		}

		g.drawString(str, diff, view.getLineAllocation(line) + ascent);
	    }
	}
    }
    
    //
    //  Implement CaretListener interface
    //
    public void caretUpdate(CaretEvent e) {
	//  Get the line the caret is positioned on

	Element root = doc.getDefaultRootElement();
	int currentLine = root.getElementIndex(textPane.getCaretPosition());

	if (lastLine != currentLine) {
	    lastLine = currentLine;
	    repaint();
	}
    }
    
    //
    //  Implement DocumentListener interface
    //
    public void changedUpdate(DocumentEvent e) { }

    public void insertUpdate(DocumentEvent e) {
	repaint();
    }
    public void removeUpdate(DocumentEvent e) {
	repaint();
    }
       
    //
    //  Implement PropertyChangeListener interface
    //
    public void propertyChange(PropertyChangeEvent evt) {
	if (evt.getNewValue() instanceof Font) {
	    if (updateFont) {
		Font newFont = (Font) evt.getNewValue();//System.out.println("changement font");
		setFont(newFont);
		updateWidth();
		metrics = textPane.getFontMetrics(newFont);
		ascent = metrics.getAscent();
	    } else {
		repaint();
	    }
	}
    }
}
