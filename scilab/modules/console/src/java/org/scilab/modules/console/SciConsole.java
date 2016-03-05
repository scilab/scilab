/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.console;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Map;
import java.util.concurrent.Semaphore;

import javax.swing.BoundedRangeModel;
import javax.swing.DefaultBoundedRangeModel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JEditorPane;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.text.JTextComponent;
import javax.swing.KeyStroke;
import javax.xml.parsers.ParserConfigurationException;
import org.scilab.modules.localization.Messages;

import org.xml.sax.SAXException;

import com.artenum.rosetta.interfaces.core.ConsoleAction;
import com.artenum.rosetta.interfaces.core.ConsoleConfiguration;
import com.artenum.rosetta.interfaces.core.InputParsingManager;
import com.artenum.rosetta.interfaces.ui.InputCommandView;
import com.artenum.rosetta.interfaces.ui.OutputView;
import com.artenum.rosetta.interfaces.ui.PromptView;
import com.artenum.rosetta.ui.Console;
import com.artenum.rosetta.util.ConfigurationBuilder;
import com.artenum.rosetta.util.ConsoleBuilder;
import com.artenum.rosetta.util.StringConstants;

import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.commons.xml.XConfiguration;

/**
 * Main class for Scilab Console based on Generic Console from Artenum
 * @author Vincent COUVERT
 */
public abstract class SciConsole extends JPanel {

    private static final long serialVersionUID = 1L;

    private static final int LINE_NUMBER_IN_PROMPT = 2;

    private static final String BACKSLASH_R = "\r";

    private static final String XPATH_CONSOLE_KEY = "//general/shortcuts/body/actions/action-folder[@name='Console']/action";
    private static final String XPATH_CONSOLE_ACTION = "/map/console/entry";

    /**
     * Maximum length of a command send to Scilab
     */
    private static final int MAX_CMD_LENGTH = 4096;

    private static Map<KeyStroke, String> actionKeys;
    private static final Map<String, String> actionToName;

    static {
        org.w3c.dom.Document doc = ScilabXMLUtilities.readDocument(System.getenv("SCI") + "/modules/console/etc/Actions-Configuration.xml");
        actionToName = XConfiguration.get(doc, "name", String.class, "action", String.class, XPATH_CONSOLE_ACTION);
    }

    /**
     * Configuration associated to the console oject
     */
    private ConsoleConfiguration config;

    /**
     * Scroll Pane used in Scilab Console
     */
    private JScrollPane jSP;

    /**
     * Generic console object
     */
    private Console sciConsole;

    /**
     * Value used to get one char from user input (when using [more y or n ?])
     */
    private int userInputValue;

    /**
     * Protection for userInputValue variable R/W
     */
    private Semaphore canReadUserInputValue = new Semaphore(1);

    /**
     * Boolean flag used to store the state of Scilab (true is all works done)
     */
    private boolean workDone;

    private boolean atBottom;

    private boolean isToHome;

    private Object searchField;

    /**
     * Constructor
     * @param configFilePath the configuration file to use
     */
    public SciConsole(String configFilePath) {
        super(new BorderLayout());

        try {
            config = ConfigurationBuilder.buildConfiguration(configFilePath);
            config.setActiveProfile("scilab");
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        }

        sciConsole = ConsoleBuilder.buildConsole(config, this);

        try {
            Class hsf = Class.forName("org.scilab.modules.gui.utils.HelpSearchField");
            Constructor constructor = hsf.getConstructor(JPanel.class, JTextComponent.class);
            searchField = constructor.newInstance(this, (SciOutputView) config.getOutputView());
        } catch (Exception e) { }

        XConfiguration.addXConfigurationListener(new org.scilab.modules.console.ConsoleConfiguration(this));
        sciConsole.setForeground(ConsoleOptions.getConsoleColor().foreground);
        sciConsole.setBackground(ConsoleOptions.getConsoleColor().background);
        setCaretColor(ConsoleOptions.getConsoleColor().cursor);
        sciConsole.setFont(ConsoleOptions.getConsoleFont().font);
        setKeyStrokeAction();

        jSP = new JScrollPane(sciConsole);

        BoundedRangeModel model = jSP.getVerticalScrollBar().getModel();
        jSP.getVerticalScrollBar().setModel(new DefaultBoundedRangeModel(model.getValue(), model.getExtent(), model.getMinimum(), model.getMaximum()) {
            public void setRangeProperties(int newValue, int newExtent, int newMin, int newMax, boolean adjusting) {
                // This method is overriden to keep the knob at the bottom during viewport resize
                // and to keep the knob at an other place if the user decided it.
                if (newMax != getMaximum()) {
                    if (!adjusting) {
                        if (atBottom) {
                            super.setRangeProperties(newMax - newExtent, newExtent, newMin, newMax, false);
                        } else {
                            super.setRangeProperties(newValue, newExtent, newMin, newMax, false);
                        }
                    } else {
                        double percent = (double) Math.abs(newMax - newValue - newExtent) / (double) newMax;
                        if (atBottom && percent <= 0.03) {
                            super.setRangeProperties(newMax - newExtent, newExtent, newMin, newMax, true);
                        } else {
                            super.setRangeProperties(newValue, newExtent, newMin, newMax, true);
                            atBottom = percent <= 0.01;
                        }
                    }
                } else {
                    super.setRangeProperties(newValue, newExtent, newMin, newMax, adjusting);
                }
            }
        });

        this.add(jSP, BorderLayout.CENTER);

        // The console is given to the outputView so that updateScrollPosition is is accessible
        ((SciOutputView) config.getOutputView()).setConsole(this);

        // The console is given to the outputView so that Drag&Drop can work
        ((SciInputCommandView) config.getInputCommandView()).setConsole(this);

        // The console is given to the CompletionWindow
        ((SciCompletionWindow) config.getCompletionWindow()).setConsole(this);
        ((SciCompletionWindow) config.getCompletionWindow()).setGraphicalContext(this);

        // The promptview is given to the Parsing Manager
        // Used to get the position of the CompletionWindow
        ((SciInputParsingManager) config.getInputParsingManager()).setPromptView(this.getConfiguration().getPromptView());

        // Reset history settings - bug 3612
        ((SciHistoryManager)config.getHistoryManager()).setInHistory(false);

        // Bug 8055 : update the lines/columns only when the console is resized
        addComponentListener(new ComponentAdapter() {
            public void componentResized(ComponentEvent evt) {
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        scilabLinesUpdate();
                        jSP.getVerticalScrollBar().setBlockIncrement(jSP.getViewport().getExtentSize().height);
                        jSP.getHorizontalScrollBar().setBlockIncrement(jSP.getViewport().getExtentSize().width);
                    }
                });
            }
        });

        sciConsole.invalidate();
        sciConsole.doLayout();
    }

    public void configurationChanged(org.scilab.modules.console.ConsoleConfiguration.Conf conf) {
        if (conf.color) {
            sciConsole.setForeground(ConsoleOptions.getConsoleColor().foreground);
            sciConsole.setBackground(ConsoleOptions.getConsoleColor().background);
            setCaretColor(ConsoleOptions.getConsoleColor().cursor);
        }
        if (conf.font) {
            sciConsole.setFont(ConsoleOptions.getConsoleFont().font);
        }
        if (conf.display) {
            ((SciOutputView) config.getOutputView()).setMaxSize(ConsoleOptions.getConsoleDisplay().maxOutputLines);
            scilabLinesUpdate();
        }
        if (conf.keymap) {
            actionKeys = null;
            setKeyStrokeAction();
        }
    }

    public static Map<KeyStroke, String> getActionKeys() {
        if (actionKeys == null) {
            org.w3c.dom.Document doc = XConfiguration.getXConfigurationDocument();
            actionKeys = XConfiguration.get(doc, "key", KeyStroke.class, "name", String.class, XPATH_CONSOLE_KEY);
        }

        return actionKeys;
    }

    public static Map<String, String> getActionName() {
        return actionToName;
    }

    /**
     * Set the shortcuts in the pane relatively to the file
     * keysConfiguration.xml
     *
     * @param sep
     *            the textpane
     * @param ed
     *            the SciNotes editor
     */
    private void setKeyStrokeAction() {
        Map<KeyStroke, String> map = getActionKeys();
        ClassLoader loader = ClassLoader.getSystemClassLoader();

        for (Map.Entry<KeyStroke, String> entry : map.entrySet()) {
            KeyStroke key = entry.getKey();
            String actionName = entry.getValue();
            if (actionName.equals("console-search-field") && searchField != null) {
                try {
                    Method sks = searchField.getClass().getMethod("setKeyStroke", KeyStroke.class);
                    sks.invoke(searchField, key);
                } catch (Exception e) { }
            }
            String action = actionToName.get(actionName);
            if (action != null) {
                try {
                    Class clazz = loader.loadClass(action);
                    Constructor constructor = clazz.getConstructor(new Class[0]);
                    Object act = constructor.newInstance(new Object[0]);
                    ((ConsoleAction) act).setConfiguration(config);
                    ((SciInputCommandView) config.getInputCommandView()).getInputMap().put(key, act);
                } catch (ClassNotFoundException e) {
                    System.err.println("No action: " + action);
                } catch (InstantiationException e) {
                    System.err.println("Problem to instantiate in action: " + action);
                } catch (NoSuchMethodException e) {
                    System.err.println("No valid constructor in action: " + action);
                } catch (IllegalAccessException e) {
                    System.err.println("The constructor must be public: " + action);
                } catch (InvocationTargetException e) {
                    System.err.println("The constructor in " + action + " threw an exception :");
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * Gets the configuration associated to the console
     * @return the configuration
     */
    public ConsoleConfiguration getConfiguration() {
        return config;
    }

    /**
     * Sets the configuration associated to the console
     * @param newConfig the new config to set
     */
    public void setConfiguration(ConsoleConfiguration newConfig) {
        config = newConfig;
    }

    /**
     * Updates Scilab internal variables containing the size of the console
     * These variables are used to format data before displaying it
     */
    public void scilabLinesUpdate() {
        if (ConsoleOptions.getConsoleDisplay().adaptToDisplay) {
            // Size of the console
            int outputViewWidth = jSP.getViewport().getExtentSize().width;

            // Size of a char
            OutputView outputView = this.getConfiguration().getOutputView();
            int[] charsWidth = ((JEditorPane) outputView).getFontMetrics(((JEditorPane) outputView).getFont()).getWidths();

            // This loop is not needed for monospaced fonts !
            int maxCharWidth = charsWidth[33];
            // The range 33--126 corresponds to the usual characters in ASCII
            for (int i = 34; i < 126; i++) {
                if (charsWidth[i] > maxCharWidth) {
                    maxCharWidth = charsWidth[i];
                }
            }

            int numberOfLines = getNumberOfLines();
            int promptWidth = ((JPanel) this.getConfiguration().getPromptView()).getPreferredSize().width;

            int numberOfColumns = (outputViewWidth - promptWidth) / maxCharWidth - 1;
            /* -1 because of the margin between text prompt and command line text */

            if (numberOfLines > 0 && numberOfColumns > 0) {
                GuiManagement.setScilabLines(Math.max(1, numberOfLines), numberOfColumns);
            }
        } else {
            GuiManagement.forceScilabLines(ConsoleOptions.getConsoleDisplay().nbLines, ConsoleOptions.getConsoleDisplay().nbColumns);
        }
    }

    /**
     * Get the number of lines that can be displayed in the visible part of the console
     * @return the number of lines
     */
    public int getNumberOfLines() {
        // Size of the console
        int outputViewHeight = jSP.getViewport().getExtentSize().height;

        // Size of a char
        OutputView outputView = this.getConfiguration().getOutputView();
        int charHeight = ((JEditorPane) outputView).getFontMetrics(((JEditorPane) outputView).getFont()).getHeight();
        int[] charsWidth = ((JEditorPane) outputView).getFontMetrics(((JEditorPane) outputView).getFont()).getWidths();

        // This loop is not needed for monospaced fonts !
        int maxCharWidth = charsWidth[0];
        for (int i = 1; i < charsWidth.length; i++) {
            if (charsWidth[i] > maxCharWidth) {
                maxCharWidth = charsWidth[i];
            }
        }

        return outputViewHeight / charHeight - 1; /* -1 because of the size of the InputCommandLine */
    }

    /**
     * Updates the scroll bars according to the contents
     */
    public void updateScrollPosition() {
        // Update the scrollbar properties
        jSP.getVerticalScrollBar().setBlockIncrement(jSP.getViewport().getExtentSize().height);
        jSP.getHorizontalScrollBar().setBlockIncrement(jSP.getViewport().getExtentSize().width);
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                jSP.getVerticalScrollBar().getModel().setValue(jSP.getVerticalScrollBar().getModel().getMaximum() - jSP.getVerticalScrollBar().getModel().getExtent());
            }
        });
    }

    /**
     * Clears the console and the output view
     */
    public void clear() {
        if (sciConsole.isPreferredSizeSet()) {
            sciConsole.setPreferredSize(null);
            sciConsole.invalidate();
            sciConsole.doLayout();
        }

        try {
            config.getInputCommandViewStyledDocument().remove(0, config.getInputCommandViewStyledDocument().getLength());
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
        config.getOutputView().reset();
        /* Bug 4014 */
        /* We add a space to add a line */
        /* clc , F2 and menus have same position */
        config.getOutputView().append(" ");
    }

    /**
     * Clears lines from the end of the output view
     * If nbLines == -1 ==> Called from SwingScilabConsole.getCharWithoutOutput() ([more y or n ?])
     * If nbLines == 0 ==> Clear the InputCommandLine
     * @param nbLines the number of lines to be deleted
     */
    public void clear(int nbLines) {
        if (sciConsole.isPreferredSizeSet()) {
            sciConsole.setPreferredSize(null);
            sciConsole.invalidate();
            sciConsole.doLayout();
        }

        if (nbLines == 0) {
            // Clear the prompt
            config.getInputCommandView().reset();
        } else {
            // Clear lines in output command view
            try {
                // We have to remove the command entered by the user
                int totalNumberOfLines = nbLines + LINE_NUMBER_IN_PROMPT;

                Document outputDoc = ((JEditorPane) config.getOutputView()).getDocument();
                String outputTxt =  outputDoc.getText(0, outputDoc.getLength());

                // Are there enough lines in the output view ?
                String[] allLines = outputTxt.split(StringConstants.NEW_LINE);
                if (allLines.length < totalNumberOfLines) {
                    // Delete lines
                    config.getOutputView().reset();
                    config.getOutputView().append(Messages.gettext("Out of Screen"));
                } else {
                    // Delete lines
                    int lastEOL;
                    for (int i = 0; i < totalNumberOfLines; i++) {
                        outputTxt = outputDoc.getText(0, outputDoc.getLength());
                        lastEOL = outputTxt.lastIndexOf(StringConstants.NEW_LINE);
                        if (lastEOL != -1) {
                            outputDoc.remove(lastEOL, outputDoc.getLength() - lastEOL);
                        }
                    }
                }
            } catch (BadLocationException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Puts the prompt in the top left corner of the console
     */
    public void toHome() {
        isToHome = true;
    }

    public void setToHome() {
        if (isToHome) {
            Dimension jSPExtSize = jSP.getViewport().getExtentSize();
            int caretH = ((SciInputCommandView) config.getInputCommandView()).getCaretHeight();
            int height = jSPExtSize.height + ((SciPromptView) config.getPromptView()).getParent().getBounds().y - caretH;
            Dimension newDim = new Dimension(sciConsole.getSize().width, height);
            sciConsole.setPreferredSize(newDim);
            sciConsole.invalidate();
            sciConsole.doLayout();
            ((SciOutputView) config.getOutputView()).addComponentListener(new ComponentAdapter() {
                public void componentResized(ComponentEvent evt) {
                    if (evt.getComponent().getSize().height >= sciConsole.getSize().height) {
                        evt.getComponent().removeComponentListener(this);
                        sciConsole.setPreferredSize(null);
                        sciConsole.invalidate();
                        sciConsole.doLayout();
                    }
                }
            });

            isToHome = false;
            jSP.getVerticalScrollBar().getModel().setValue(jSP.getVerticalScrollBar().getModel().getMaximum() - jSP.getVerticalScrollBar().getModel().getExtent());
        }
    }

    /**
     * Gets the user input value
     * @return the value entered by the used
     */
    public int getUserInputValue() {
        try {
            canReadUserInputValue.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return userInputValue;
    }

    /**
     * Sets the value entered by the user
     * @param userInputValue new value
     */
    public void setUserInputValue(int userInputValue) {
        this.userInputValue = userInputValue;
        canReadUserInputValue.release();
    }

    /**
     * Gets the semaphore protection so that it can be acquired
     * @return the semaphore
     */
    public Semaphore getCanReadUserInputValue() {
        return canReadUserInputValue;
    }

    /**
     * Send commands to be executed by Scilab (after a copy/paste or drag&drop...)
     * @param textToExec all text lines to executed
     * @param displayCmdInOutput flag indicating if the input command has to be displayed in the output view
     * @param storeInHistory flag indicating if the input command has to be stored in the history
     */
    public void sendCommandsToScilab(String textToExec, boolean displayCmdInOutput, boolean storeInHistory) {
        String[] linesToExec = textToExec.split(StringConstants.NEW_LINE);
        int nbStatements = 0;

        atBottom = true;
        this.setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

        while (nbStatements < linesToExec.length) {
            // This loop contains code very similar to the code of ValidationAction.java
            InputParsingManager inputParsingManager = config.getInputParsingManager();
            PromptView promptView = config.getPromptView();

            // Reset command line
            inputParsingManager.reset();

            // Reset history settings
            config.getHistoryManager().setInHistory(false);

            // Hide the prompt and command line
            config.getInputCommandView().setEditable(false);
            ((SciInputCommandView) config.getInputCommandView()).getCaret().setVisible(false);

            config.getPromptView().setVisible(false);

            // Remove the prompt if present at the beginning of the text to execute
            // Bug 3002 fix: this "functionality" has been removed because:
            // - Remove the --> even if not from paste action
            // - Does not remove pause prompts

            // Store the command in the buffer so that Scilab can read it
            if (linesToExec[nbStatements].length() > MAX_CMD_LENGTH) {
                config.getOutputView().append(String.format(Messages.gettext("Command is too long (more than %d characters long): could not send it to Scilab\n"), MAX_CMD_LENGTH));
                ((SciInputCommandView) config.getInputCommandView()).setCmdBuffer("", false);
                return;
            }

            ((SciInputCommandView) config.getInputCommandView())
            .setCmdBuffer(linesToExec[nbStatements].replace(BACKSLASH_R, ""), displayCmdInOutput);
            if (storeInHistory) {
                ((SciHistoryManager) config.getHistoryManager()).addEntry(linesToExec[nbStatements].replace(BACKSLASH_R, ""));
            }
            nbStatements++;
        }

    }

    /**
     * Get the JScrollPane associated to the console
     * @return the JScrollPane associated to the console
     */
    public JScrollPane getJScrollPane() {
        return jSP;
    }

    /**
     * Get the Console object associated to the console
     * @return the Console object associated to the console
     */
    public Console getSciConsole() {
        return sciConsole;
    }

    /**
     * Get the current status of the console
     * If the prompt view is visible, Scilab is waiting for commands
     * @return true is Scilab is waiting for commands
     */
    public boolean isWaitingForInput() {
        return ((JTextPane) config.getInputCommandView()).isEditable();
    }

    /**
     * This methods is used by Scilab to get a new command to execute
     * @return the command to execute
     */
    public String readLine() {

        InputCommandView inputCmdView = this.getConfiguration().getInputCommandView();

        getConfiguration().getOutputView().setCaretPositionToEnd();

        displayPrompt();

        // Display Cursor to show Scilab is available.
        this.setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));

        // Reads the buffer
        return ((SciInputCommandView) inputCmdView).getCmdBuffer();
    }

    /**
     * This method is used to display the prompt
     */
    public abstract void displayPrompt();

    /**
     * Does Scilab have finished its work ?
     * @return true if Scilab is waiting for new commands
     */
    public boolean isWorkDone() {
        return workDone;
    }

    /**
     * Set the font of the Console
     * @param font the font to set
     */
    public void setFont(Font font) {
        if (sciConsole != null && !font.equals(getFont())) {
            sciConsole.setFont(font);

            /* Have to update the output view contents with new font */
            String txt;
            try {
                Document outputDoc = ((JEditorPane) config.getOutputView()).getDocument();
                txt = outputDoc.getText(0, outputDoc.getLength());
                outputDoc.remove(0, outputDoc.getLength());
                config.getOutputView().append(txt);
            } catch (BadLocationException e) {
                System.err.println(Messages.gettext("Could not change the Console Font."));
                return;
            }

            /* Update the prompt */
            ((SciPromptView) config.getPromptView()).getPromptUI().setFont(font);
            config.getPromptView().updatePrompt();
            scilabLinesUpdate();
        }
    }

    /**
     * Get the font of the Console
     * @return the font
     */
    public Font getFont() {
        if (sciConsole != null) {
            return ((SciPromptView) config.getPromptView()).getPromptUI().getFont();
        } else {
            return null;
        }
    }

    /**
     * Get the Foreground Color of the Console
     * @return the Foreground Color
     */
    public Color getForeground() {
        if (sciConsole != null) {
            return sciConsole.getForeground();
        } else {
            return null;
        }
    }

    /**
     * Get the Background Color of the Console
     * @return the Background Color
     */
    public Color getBackground() {
        if (sciConsole != null) {
            return sciConsole.getBackground();
        } else {
            return null;
        }
    }

    /**
     * Set the Foreground Color of the Console
     * @param color the Foreground Color
     */
    public void setForeground(Color color) {
        if (sciConsole != null) {
            sciConsole.setForeground(color);

            /* Have to update the output view contents with new Foreground */
            String txt;
            try {
                Document outputDoc = ((JEditorPane) config.getOutputView()).getDocument();
                txt = outputDoc.getText(0, outputDoc.getLength());
                outputDoc.remove(0, outputDoc.getLength());
                config.getOutputView().append(txt);
            } catch (BadLocationException e) {
                System.out.println(Messages.gettext("Could not change the Console Foreground."));
                return;
            }

            /* Update the prompt */
            ((SciPromptView) config.getPromptView()).getPromptUI().setForeground(color);
            config.getPromptView().updatePrompt();
        }
    }

    /**
     * Set the Background Color of the Console
     * @param color the Background Color
     */
    public void setBackground(Color color) {
        if (sciConsole != null) {
            sciConsole.setBackground(color);
        }
    }

    /**
     * Set the Background Color of the Console
     * @param color the Background Color
     */
    public void setCaretColor(Color color) {
        if (sciConsole != null) {
            ((SciInputCommandView) config.getInputCommandView()).setCaretColor(color);
        }
    }
}
