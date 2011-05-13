/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2010-2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.FontMetrics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.BadLocationException;

import org.scilab.modules.commons.gui.ScilabCaret;
import org.scilab.modules.console.utils.ScilabLaTeXViewer;

import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.interfaces.ui.OutputView;
import com.artenum.rosetta.interfaces.ui.PromptView;
import com.artenum.rosetta.ui.ConsoleTextPane;
import com.artenum.rosetta.util.StringConstants;

/**
 * Scilab UI that contains the line edited by the user
 * @author Vincent COUVERT
 * @author Calixte DENIZET
 */
public class SciInputCommandView extends ConsoleTextPane implements InputCommandView, CaretListener {

    private static final long serialVersionUID = 1L;
    private static final String END_LINE = "\n";
    private static final Point ERROR_POINT = new Point(0, 0);
    private static final int TOP_BORDER = 1;
    private static final int BOTTOM_BORDER = 2;

    // A clearest pattern: ['"] \$ [^\\\$'"]* (( [\\].? | ['"]{2} ) [^\\\$'"]* )+
    private static final Pattern latexPattern = Pattern.compile("[\'\"]\\$[^\\\\\\$\'\"]*(?:(?:[\\\\].?|[\'\"]{2})[^\\\\\\$\'\"]*)*");

    private static BlockingQueue<String> queue = new LinkedBlockingQueue<String>();
    private static BlockingQueue<Boolean> displayQueue = new LinkedBlockingQueue<Boolean>();

    private Thread concurrentThread = null;

    private SciConsole console;
    private int height = -1;
    private int width = -1;
    private boolean isLatex;

    /**
     * Constructor
     */
    public SciInputCommandView() {
        super();
        setBorder(BorderFactory.createEmptyBorder(TOP_BORDER, 0, BOTTOM_BORDER, 0));

        // Input command line is not editable when created
        this.setEditable(false);
        ScilabCaret caret = new ScilabCaret(this);
        caret.setBlinkRate(getCaret().getBlinkRate());
        setCaret(caret);
        addCaretListener(this);
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
        String command = null;
        try {
            if (concurrentThread == null) {
                concurrentThread = Thread.currentThread();
            }
            else {
                concurrentThread.interrupt();
            }
            command = queue.take();
            if (displayQueue.take()) {
                OutputView outputView = console.getConfiguration().getOutputView();
                PromptView promptView = console.getConfiguration().getPromptView();
                outputView.append(StringConstants.NEW_LINE + promptView.getDefaultPrompt() + command + StringConstants.NEW_LINE);
            }
        } catch (InterruptedException e) {
            /*
             * If we have concurrent access let's interrupt the first one, then allow
             * the second to return the command.
             */
            return "";
        }
        concurrentThread = null;
        return command;
    }

    /**
     * Sets the command buffer after a user input in input command view
     * @param command the string to set to the buffer
     * @param displayFlag boolean indicating if the command has to be displayed
     */
    public void setCmdBuffer(String command, boolean displayFlag) {
        try {
            queue.put(command);
            displayQueue.put(displayFlag);
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
                                    console.sendCommandsToScilab(wholeTxt, true, true);
                                };
                            });
                    }
                }

                public void removeUpdate(DocumentEvent e) {
                    // Nothing to do in Scilab
                }
            });

        this.addKeyListener(new KeyListener() {
                public void keyPressed (KeyEvent e) {
                    if (e.getKeyCode()==KeyEvent.VK_BACK_SPACE) {
                        if (console.getConfiguration().getHistoryManager().isInHistory()) {
                            //console.getConfiguration().getInputParsingManager().reset();
                            //console.getConfiguration().getInputParsingManager().append(console.getConfiguration().getHistoryManager().getTmpEntry());
                            console.getConfiguration().getHistoryManager().setInHistory(false);
                        }
                    }
                }

                public void keyReleased (KeyEvent e) {
                    // Nothing to do in Scilab
                }

                public void keyTyped (KeyEvent e) {
                    // Nothing to do in Scilab
                }
            });
    }

    /**
     * This class listens to the caret event
     * @param e event
     */
    public void caretUpdate(CaretEvent e) {
        String str = getText().substring(0, e.getDot());
        int lastPos = str.lastIndexOf("\"$");
        if (lastPos != -1) {
            str = str.substring(lastPos);
        }

        Matcher matcher = latexPattern.matcher(str);
        if (matcher.find() && matcher.end() == str.length()) {
            String latex = matcher.group().replace("\'\'", "\'").replace("\"\"", "\"");
            latex = latex.substring(2, latex.length());
            int hl = ScilabLaTeXViewer.displayExpression(this, Integer.MAX_VALUE, latex, lastPos, e.getDot()) + 2;
            int y = 0;
            try {
                Rectangle rect = modelToView(getCaretPosition());
                y = (int) (rect.height + rect.y + 1);
            } catch (BadLocationException ex) { }

            int sheight = height;
            height = y + hl;
            if (height != sheight) {
                validate();
                console.updateScrollPosition();
            }
            isLatex = true;
        } else {
            if (isLatex) {
                ScilabLaTeXViewer.removeLaTeXViewer(this);
                if (height != -1) {
                    height = -1;
                    validate();
                }
                isLatex = false;
            }
        }
    }

    /**
     * Set the height of this text component.
     * @param height the height, -1 to have the natural preferred height
     */
    public void setPreferredHeight(int height) {
        this.height = height;
    }

    /**
     * Set the width of this text component.
     * @param width the width, -1 to have the natural preferred width
     */
    public void setPreferredWidth(int width) {
        this.width = width;
    }

    /**
     * {@inheritDoc}
     */
    public Dimension getPreferredSize() {
        Dimension dim;
        try {
            dim = super.getPreferredSize();
        } catch (Exception e) {
            // workaround bug 9442
            // Should be removed with JDK 7
            dim = new Dimension(0, 0);
        }

        if (height != -1) {
            dim.height = Math.max(height, dim.height);
        }
        if (width != -1) {
            dim.width = Math.max(width, dim.width);
        }
        return dim;
    }
}
