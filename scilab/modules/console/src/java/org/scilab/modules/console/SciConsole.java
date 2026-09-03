/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2025-2026 - Dassault Systèmes S.E. - Antoine ELIAS
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
import java.util.logging.Level;
import java.util.logging.Logger;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.KeyboardFocusManager;
import java.awt.MouseInfo;
import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.Semaphore;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.browser.CefMessageRouter;
import org.cef.browser.CefMessageRouter.CefMessageRouterConfig;
import org.cef.callback.CefQueryCallback;
import org.cef.handler.CefKeyboardHandlerAdapter;
import org.cef.handler.CefMessageRouterHandlerAdapter;
import org.cef.misc.EventFlags;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.ScilabCefPanel;
import org.scilab.modules.commons.utils.ScilabBrowser;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.completion.Completion;
import org.scilab.modules.history_manager.HistoryManagement;
import org.scilab.modules.localization.Messages;

/**
 * Main class for Scilab Console based on Generic Console from Artenum
 * @author Vincent COUVERT
 */
public abstract class SciConsole extends JPanel {

    private static final Logger log = Logger.getLogger("org.scilab.modules.console.SciConsole");

    private static final String CRLF = "\r\n";
    private static final String CR = "\r";
    private static final String LF = "\n";

    private static final String XPATH_CONSOLE_KEY = "//general/shortcuts/body/actions/action-folder[@name='Console']/action";

    private static Map<KeyStroke, String> actionKeys;

    /**
     * Value used to get one char from user input (when using [more y or n ?])
     */
    private int userInputValue = (int) 'n';

    /**
     * Protection for userInputValue variable R/W
     */
    private Semaphore canReadUserInputValue = new Semaphore(0);

    private int maxNumberOfLines;

    private static List<String> waitingQueue = new ArrayList<String>();
    private static BlockingQueue<String> queue = new LinkedBlockingQueue<String>();
    private static BlockingQueue<Boolean> displayQueue = new LinkedBlockingQueue<Boolean>();

    // JCEF
    private final ScilabCefPanel cefPanel;
    public boolean ready;

    //Input
    private boolean disableLock = false;
    private Thread concurrentThread = null;
    private String defaultPrompt;

    private String selectedText;

    private Color background = null;
    private Color foreground = null;

    private int linesRow = 0;
    private int linesCol = 80;

    /**
     * Constructor
     */
    public SciConsole() {
        super(new BorderLayout(0, 0));

        ready = false;

        String url = "file:///SCI/modules/console/src/web/console.html";
        cefPanel = new ScilabCefPanel(url);
        if (log.isLoggable(Level.FINE))
        {
            cefPanel.setDebug(true);
        }

        addFocusListener(new FocusListener() {
            public void focusGained(FocusEvent e) {
                if (e.isTemporary())
                {
                    return;
                }
                log.info(() -> "SciConsole focusGained " + Thread.currentThread());

                sendToHTML("focus");
            }

            public void focusLost(FocusEvent e) {
                if (e.isTemporary())
                {
                    return;
                }
                log.info(() -> "SciConsole focusLost " + Thread.currentThread());
            }
        });

        cefPanel.addKeyboardHandler(new CefKeyboardHandlerAdapter() {
            @Override
            public boolean onKeyEvent(CefBrowser browser, final CefKeyEvent event) {
                boolean isMac = OS.get() == OS.MAC;
                boolean isCTRL = isMac == false && event.modifiers == EventFlags.EVENTFLAG_CONTROL_DOWN;
                boolean isCOMMAND = isMac && event.modifiers == EventFlags.EVENTFLAG_COMMAND_DOWN;

                log.info("CefKeyboardHandlerAdapter " + event);

                if (event.type == CefKeyEvent.EventType.KEYEVENT_RAWKEYDOWN && (isCTRL || isCOMMAND))
                {
                    SwingUtilities.invokeLater(() -> {
                        KeyEvent ke = new KeyEvent(getMe(), KeyEvent.KEY_PRESSED, System.currentTimeMillis(), isMac ? InputEvent.META_DOWN_MASK : InputEvent.CTRL_DOWN_MASK , event.windows_key_code, event.unmodified_character);
                        KeyboardFocusManager.getCurrentKeyboardFocusManager().dispatchKeyEvent(ke);
                    });
                    return true;
                }

                return false;
            }
        });

        // add listener from javascript
        cefPanel.getClient().addMessageRouter(CefMessageRouter.create(new CefMessageRouterConfig("Scilab", "ScilabAbort"), new CefMessageRouterHandlerAdapter() {
            @Override
            public boolean onQuery(CefBrowser browser_, CefFrame frame, long queryId, String request, boolean persistent, CefQueryCallback callback) {
                if (log.isLoggable(Level.INFO))
                {
                    log.info("onQuery: " + request);
                }
                String[] req = request.split("::", -1);
                switch(req[0]) {
                    case "ready": {
                        ready = true;
                        // UNCOMMENT FOR DEBUGGING PURPOSES
                        // openDevTools();
                        updateKeys();
                        background = Color.getColor(req[1]);
                        foreground = Color.getColor(req[1]);
                        sendQueue();

                        // Ensure the CefBrowser gets focus when console is ready
                        SwingUtilities.invokeLater(() -> {
                            cefPanel.requestBrowserFocus();
                        });
                        return true;
                    }
                    case "contextmenu":
                        SwingUtilities.invokeLater(() -> {
                            browser_.getUIComponent().requestFocusInWindow();
                            getComponentPopupMenu().setLocation(MouseInfo.getPointerInfo().getLocation());
                            getComponentPopupMenu().setVisible(true);
                        });
                        return true;
                    case "reload":
                        browser_.reloadIgnoreCache();
                        return true;
                    case "focus":
                        SwingUtilities.invokeLater(() -> {
                            cefPanel.requestBrowserFocus();
                        });
                        return true;
                    case "copy":
                        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(new StringSelection(req[1]), null);
                        return true;
                    case "paste":
                        return pasteAction();
                    case "devtools":
                        cefPanel.openDevTools();
                        return true;
                    case "size":
                        SwingUtilities.invokeLater(() -> {
                            int w = Integer.parseInt(req[1]);
                            int h = Integer.parseInt(req[2]);
                            setPreferredSize(new Dimension(w, h));
                            linesCol = Integer.parseInt(req[3]);
                            linesRow = Integer.parseInt(req[4]);
                            scilabLinesUpdate();
                        });
                        return true;
                    case "input": {
                        String command = null;
                        if (req.length > 2) {
                            command = String.join("::", Arrays.copyOfRange(req, 1, req.length));
                        } else {
                            command = req[1];
                        }
                        // If we are waiting for a Y/N char
                        if (command.length() > 0 && setUserInputValue(command.charAt(0))) {
                            return true;
                        }

                        sendCommandsToScilab(command, true, true);
                        return true;
                    }
                    case "input_char": {
                        String inputChar = null;
                        if (req.length > 2) {
                            return false;
                        } else {
                            inputChar = req[1];
                        }
                        // If we are waiting for a Y/N char
                        if (inputChar.length() > 0 && setUserInputValue(inputChar.charAt(0))) {
                            return true;
                        }
                        return false;
                    }
                    case "previous": {
                        String res = historyPrevious(req[1], Boolean.parseBoolean(req[2]));
                        callback.success(res);
                        return true;
                    }
                    case "next": {
                        String res = historyNext(req[1], Boolean.parseBoolean(req[2]));
                        callback.success(res);
                        return true;
                    }
                    case "completion": {
                        String res = completion(req[1], Integer.parseInt(req[2]));
                        callback.success(res);
                        return true;
                    }
                    case "completeline" : {
                        boolean isFile = req[3].equals(Messages.gettext("File")) || req[3].equals(Messages.gettext("Directory"));
                        String newLine = Completion.completelineforjava(req[1], req[2], isFile, req[4]);
                        callback.success(newLine);
                        return true;
                    }
                    case "file": {
                        DropFiles.dropFiles(new String[] { req[1] });
                        return true;
                    }
                    case "interrupt": {
                        InterpreterManagement.requestScilabExec("pause;");
                        return true;
                    }
                    case "interruptcommand": {
                        InterpreterManagement.interruptScilab();

                        String command = null;
                        if (req.length > 2) {
                            command = String.join("::", Arrays.copyOfRange(req, 1, req.length));
                        } else {
                            command = req[1];
                        }

                        append(defaultPrompt + command + LF);
                        sendCommandsToScilab("", false, false);
                        return true;
                    }
                    case "select": {
                        selectedText = req[1];
                        return true;
                    }
                    case "clear": {
                        clear();
                        return true;
                    }
                    case "cleared": {
                        lockOutput--;
                        if (lockOutput == 0) {
                            for (String s : outputList) {
                                append(s);
                            }
                            outputList.clear();
                        }
                        return true;
                    }
                    case "help": {
                        selectedText = req[1];
                        helpOnTheKeyword();
                        return false;
                    }
                    case "version": {
                        String version = String.format("chrome: %s\njcef  : %s\ncef   : %s", ScilabBrowser.getChromeVersion(), ScilabBrowser.getJcefVersion(), ScilabBrowser.getCefVersion());
                        callback.success(version);
                        return true;
                    }
                    case "log": {
                        Level level = Level.INFO;
                        if (req.length > 1) {
                            // with a level prefix
                            try {
                                level = Level.parse(req[1]);
                            } catch (IllegalArgumentException e) {
                                log.warning("Invalid log level: " + req[1]);
                            }
                        }
                        String msg = String.join("::", Arrays.copyOfRange(req, 2, req.length));
                        if (log.isLoggable(level))
                        {
                            log.log(level, msg);
                        }
                        return true;
                    }
                    case "consolebox": {
                        InterpreterManagement.requestScilabExec("consolebox " + req[1]);
                        return true;
                    }
                    default:
                        System.err.println("not managed: " + request);
                        return false;
                }
            }
        }));

        setMaxSize(10000);
        setPreferredSize(new Dimension(100, 10));

        setBorder(BorderFactory.createLineBorder(Color.GRAY));
        add(cefPanel, BorderLayout.CENTER);

        XConfiguration.addXConfigurationListener(new org.scilab.modules.console.ConsoleConfiguration(this));

        setForeground(ConsoleOptions.getConsoleColor().foreground);
        setBackground(ConsoleOptions.getConsoleColor().background);
        setFont(ConsoleOptions.getConsoleFont().font);
        setCaretColor(ConsoleOptions.getConsoleColor().cursor);
    }

    public SciConsole getMe() {
        return this;
    }

    public Container getUIComponent() {
        // Safely cast, this is only a wrapper around the Canvas of the CefBrowser
        return this;
    }

    public void configurationChanged(org.scilab.modules.console.ConsoleConfiguration.Conf conf) {
        if (conf.color) {
            log.info("configurationChanged color");
            setForeground(ConsoleOptions.getConsoleColor().foreground);
            setBackground(ConsoleOptions.getConsoleColor().background);
            setCaretColor(ConsoleOptions.getConsoleColor().cursor);
        }
        if (conf.font) {
            log.info("configurationChanged font");
            setFont(ConsoleOptions.getConsoleFont().font);
        }

        if (conf.display) {
            log.info("configurationChanged lines");
            setMaxSize(ConsoleOptions.getConsoleDisplay().maxOutputLines);
            scilabLinesUpdate();
        }

        if (conf.keymap) {
            log.info("configurationChanged keymap");
            updateKeys();
        }
    }

    public void updateKeys() {

        //clear all shorctuts
        sendToHTML("clear-shortcut");
        for (Map.Entry<KeyStroke, String> entry : getActionKeys().entrySet()) {
            if (entry.getKey() == null) {
                //action declared without default binding (key="")
                continue;
            }

            if (log.isLoggable(Level.INFO))
            {
                log.info(entry.getKey().toString() + " -> " + entry.getValue());
            }

            int keyCode = entry.getKey().getKeyCode();
            boolean isCTRL = (entry.getKey().getModifiers() & InputEvent.CTRL_DOWN_MASK) == InputEvent.CTRL_DOWN_MASK;
            isCTRL |= (entry.getKey().getModifiers() & InputEvent.META_DOWN_MASK) == InputEvent.META_DOWN_MASK;

            boolean isALT  = (entry.getKey().getModifiers() & InputEvent.ALT_DOWN_MASK) == InputEvent.ALT_DOWN_MASK;
            String action = entry.getValue();

            sendToHTML("shortcut", action, keyCode, isCTRL, isALT);
        }
        sendToHTML("sort-shortcut");
    }

    public static Map<KeyStroke, String> getActionKeys() {
        if (actionKeys == null) {
            org.w3c.dom.Document doc = XConfiguration.getXConfigurationDocument();
            actionKeys = XConfiguration.get(doc, "key", KeyStroke.class, "name", String.class, XPATH_CONSOLE_KEY);
        }

        return actionKeys;
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
    /**
     * Updates Scilab internal variables containing the size of the console
     * These variables are used to format data before displaying it
     */
    public void scilabLinesUpdate() {
        if (ConsoleOptions.getConsoleDisplay().adaptToDisplay) {
            GuiManagement.setScilabLines(Math.max(1, linesRow), linesCol);
        } else {
            GuiManagement.forceScilabLines(ConsoleOptions.getConsoleDisplay().nbLines, ConsoleOptions.getConsoleDisplay().nbColumns);
        }
    }

    /*
     * Get the number of lines that can be displayed in the visible part of the console
     * @return the number of lines
     */
    public int getNumberOfLines() {
        return 0;
    }

    private int lockOutput = 0;
    /**
     * Clears the console and the output view
     */
    public void clear() {
        lockOutput++;
        outputList.clear();
        sendToHTML("clear");
    }

    /**
     * Clears lines from the end of the output view or input command line based on the specified count.
     * <ul>
     * <li><code>nbLines == 0</code>: Clear</li>
     * <li><code>nbLines == 1</code>: Called from SwingScilabConsole.getCharWithoutOutput() (continue Y/N prompt)</li>
     * <li><code>nbLines > 0</code>: Clears the specified number of lines from the output view</li>
     * </ul>
     * @param nbLines the number of lines to be deleted, or special values: -1 for more prompt, 0 for input line
     */
    public void clear(int nbLines) {
        lockOutput++;
        outputList.clear();
        sendToHTML("clear", nbLines);
    }

    /**
     * Gets the user input value
     * @return the value entered by the used
     */
    public int getUserInputValue() {
        try {
            canReadUserInputValue.acquire();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            e.printStackTrace();
        }
        return userInputValue;
    }

    /**
     * Sets the value entered by the user
     * @param userInputValue new value
     * @return true if the console was waiting for a user input value, false otherwise
     */
    public boolean setUserInputValue(int userInputValue) {
        this.userInputValue = userInputValue;

        // be safe: if not waiting, execute a regular scilab command
        if (!canReadUserInputValue.hasQueuedThreads()) {
            return false;
        }

        canReadUserInputValue.release();
        return true;
    }

    /**
     * @return true if the console is waiting for a user input value, false otherwise
     */
    public boolean canReadUserInputValue() {
        return canReadUserInputValue.hasQueuedThreads();
    }

    /**
     * Unblock the console if needed
     */
    public void unblock() {
        if (canReadUserInputValue()) {
            setUserInputValue(Messages.gettext("nN").codePointAt(0));
        }
        canReadUserInputValue.release(Integer.MAX_VALUE);


        log.info("unblock");
        // interrupt any mscanf call (input, halt and so on)
        interruptAndDisable();
    }

    /**
     * Send commands to be executed by Scilab (after a copy/paste or drag&drop...)
     * @param textToExec all text lines to executed
     * @param displayCmdInOutput flag indicating if the input command has to be displayed in the output view
     * @param storeInHistory flag indicating if the input command has to be stored in the history
     */
    public void sendCommandsToScilab(String textToExec, boolean displayCmdInOutput, boolean storeInHistory) {

        sendToHTML("lock-input");
        String[] linesToExec = textToExec.split("\n");

        for (int i = 0; i < linesToExec.length; ++i) {
            String line = linesToExec[i];

            setInHistory(false);
            if (storeInHistory) {
                HistoryManagement.appendLineToScilabHistory(line);
            }

            setCmdBuffer(linesToExec[i].replace(CR, ""), displayCmdInOutput);
        }
    }

    public void setCmdBuffer(String command, boolean displayFlag) {
        try {
            queue.put(command);
            displayQueue.put(displayFlag);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * Get the current status of the console
     * If the prompt view is visible, Scilab is waiting for commands
     * @return true is Scilab is waiting for commands
     */
    public boolean isWaitingForInput() {
        return queue.isEmpty() && !canReadUserInputValue();
    }

    public boolean pasteAction() {
        try {
            Transferable t = Toolkit.getDefaultToolkit().getSystemClipboard().getContents(null);
            Object data = t.getTransferData(DataFlavor.stringFlavor);
            sendToHTML("paste", data.toString());
        } catch (Exception e) {
            log.warning("Failed to paste from clipboard: " + e.getMessage());
            return false;
        }
        return true;
    }

    /**
     * This methods is used by Scilab to get a new command to execute
     * @return the command to execute
     */
    public String readLine() {
        sendToHTML("unlock-input");

        String command = null;
        synchronized(this)
        {
            if (disableLock)
            {
                return "";
            }
        }

        try {
            if (concurrentThread == null) {
                concurrentThread = Thread.currentThread();
            } else {
                concurrentThread.interrupt();
            }

            command = queue.take();
            if (displayQueue.take()) {
                append(defaultPrompt + command + LF);
            }
        } catch (InterruptedException e) {
            /*
             * If we have concurrent access let's interrupt the first one, then allow
             * the second to return the command.
             */
            concurrentThread = null;
            return "";
        }
        concurrentThread = null;
        if (log.isLoggable(Level.INFO))
        {
            log.info(String.format("command: |%s|", command));
        }
        return command;
    }

    final public void interruptAndDisable() {
        synchronized(this)
        {
            disableLock = true;
        }

        if (concurrentThread != null) {
            concurrentThread.interrupt();
        }
    }

    /**
     * Set the font of the Console
     * @param font the font to set
     */
    public void setFont(Font font) {
        sendToHTML("font", font.getFontName(), font.getFamily(), font.getSize(), font.isBold());

        if (log.isLoggable(Level.INFO))
        {
            log.info(font.getFontName());
            log.info(font.getFamily());
            log.info(font.getPSName());
        }
    }

    /**
     * Get the font of the Console
     * @return the font
     */
    public Font getFont() {
        return null;
    }

    /**
     * Get the Foreground Color of the Console
     * @return the Foreground Color
     */
    public Color getForeground() {
        return foreground;
    }

    /**
     * Get the Background Color of the Console
     * @return the Background Color
     */
    public Color getBackground() {
        return background;
    }

    /**
     * Set the Foreground Color of the Console
     * @param color the Foreground Color
     */
    public void setForeground(Color color) {
        if (log.isLoggable(Level.INFO))
        {
            log.info("setForeground: " + color);
        }

        String hexColor = String.format("#%06x", Integer.valueOf(color.getRGB() & 0x00FFFFFF));
        sendToHTML("fgcolor", hexColor);
    }

    /**
     * Set the Background Color of the Console
     * @param color the Background Color
     */
    public void setBackground(Color color) {
        if (log.isLoggable(Level.INFO))
        {
            log.info("setBackground: " + color);
        }

        String hexColor = String.format("#%06x", Integer.valueOf(color.getRGB() & 0x00FFFFFF));
        sendToHTML("bgcolor", hexColor);
        super.setBackground(Color.CYAN);
    }

    /**
     * Set the Background Color of the Console
     * @param color the Background Color
     */
    public void setCaretColor(Color color) {
        if (log.isLoggable(Level.INFO))
        {
            log.info("setCaretColor: " + color);
        }
        String hexColor = String.format("#%06x", Integer.valueOf(color.getRGB() & 0x00FFFFFF));
        sendToHTML("caret", hexColor);
    }

    public void closeConsole() {
        interruptAndDisable();
    }

    private List<String> outputList = new ArrayList<String>();
    public void append(String data) {
        if (lockOutput != 0) {
            outputList.add(data);
        } else {
            data = data.replaceAll(CRLF, LF);
            sendToHTML("output", data, "monospace");
        }
    }

    public void setInputEditable(boolean status) {
        log.info("setInputEditable");
    }

    public void setPromptVisible(boolean status) {
        log.info("setPromptVisible");
    }

    public void setPrompt(String prompt) {
        defaultPrompt = prompt;
        sendToHTML("prompt", prompt);
    }

    public String getSelectedText() {
        return selectedText;
    }

    public void selectAll() {
        sendToHTML("selectall");
    }

    public void clearHistory() {
        HistoryManagement.resetScilabHistory();
    }

    public void setMaxSize(int number) {
        maxNumberOfLines = Math.max(1, number);
        if (log.isLoggable(Level.INFO))
        {
            log.info("maxNumberOfLines: " + maxNumberOfLines);
        }
    }

    private void sendQueue() {
        for (int i = 0; i < waitingQueue.size(); ++i) {
            cefPanel.executeJS(waitingQueue.get(i), null);
        }

        waitingQueue.clear();
    }

    //history manager
    public String getTmpEntry() {
        return HistoryManagement.getSearchedTokenInScilabHistory();
    }

    public void setTmpEntry(String currentCommandLine) {
        if (currentCommandLine != null && currentCommandLine.trim().equals("")) {
            HistoryManagement.resetSearchedTokenInScilabHistory();
            HistoryManagement.setSearchedTokenInScilabHistory("");
        } else {
            HistoryManagement.resetSearchedTokenInScilabHistory();
            HistoryManagement.setSearchedTokenInScilabHistory(currentCommandLine);
        }
    }

    private boolean isInHistory = false;
    public boolean isInHistory() {
        /* Nothing to do in because Scilab core does the work */
        return isInHistory;
    }

    public void setInHistory(boolean status) {
        /* Nothing to do in because Scilab core does the work */
        isInHistory = status;
    }

    public String getNextEntry(String beg) {
        if (beg != null && !beg.equals(HistoryManagement.getSearchedTokenInScilabHistory())) {
            HistoryManagement.setSearchedTokenInScilabHistory(beg);
        }
        return HistoryManagement.getNextLineInScilabHistory();
    }

    private String getPreviousEntry(String beg) {
        if (beg != null && !beg.equals(HistoryManagement.getSearchedTokenInScilabHistory())) {
            HistoryManagement.setSearchedTokenInScilabHistory(beg);
        }

        return HistoryManagement.getPreviousLineInScilabHistory();
    }

    private String historyNext(String beg, boolean forceReset) {

        if (forceReset) {
            setInHistory(false);
        }

        if (isInHistory() == false) {
            setInHistory(true);
            setTmpEntry(beg);
        }

        String history = getNextEntry(getTmpEntry());
        if (history == null) {
            history = getTmpEntry();
            setTmpEntry(null);
            setInHistory(false);
        }

        return history;
    }

    private String historyPrevious(String beg, boolean forceReset) {

        if (forceReset) {
            setInHistory(false);
        }

        if (isInHistory() == false) {
            setInHistory(true);
            setTmpEntry(beg);
        }

        String history = getPreviousEntry(getTmpEntry());
        if (history == null) {
            history = getTmpEntry();
            setTmpEntry(null);
            setInHistory(false);
        }

        return history;
    }

    public String completion(String entry, int caret) {
        CompletionEntries entries = null;

        String filePart = getFilePart(entry, caret);
        String[] scilabFilesDictionary = Completion.searchFilesDictionary(filePart);
        if (scilabFilesDictionary != null) {
            entries = new CompletionEntries(filePart);
            ArrayList<String> files = new ArrayList<String>();
            ArrayList<String> directories = new ArrayList<String>();
            separateFilesDirectories(scilabFilesDictionary, files, directories);
            entries.add(Messages.gettext("File"), files);
            entries.add(Messages.gettext("Directory"), directories);
        } else {
            String searchedPattern = getPart(entry, caret);
            entries = new CompletionEntries(searchedPattern);
            String[] scilabFieldsDictionary = Completion.searchFieldsDictionary(entry, searchedPattern);
            if (scilabFieldsDictionary != null) {
                entries.add("field", scilabFieldsDictionary);
            } else {
                entries.add(Messages.gettext("command"), Completion.searchCommandsDictionary(searchedPattern));
                entries.add(Messages.gettext("function"), Completion.searchFunctionsDictionary(searchedPattern));
                entries.add(Messages.gettext("handle property"), Completion.searchHandleGraphicsPropertiesDictionary(searchedPattern));
                entries.add(Messages.gettext("macro"), Completion.searchMacrosDictionary(searchedPattern));
                entries.add(Messages.gettext("variable"), Completion.searchVariablesDictionary(searchedPattern));
                entries.add(Messages.gettext("arguments validator"), Completion.searchMustBeDictionary(searchedPattern));
            }
        }

        return entries.toJson();
    }

    public void separateFilesDirectories(String[] scilabFilesDictionary, ArrayList<String> filesDictionary, ArrayList<String> directoriesDictionary) {
        String fileSep = System.getProperty("file.separator");
        for (String word : scilabFilesDictionary) {
            if (word.endsWith(fileSep)) {
                directoriesDictionary.add(new String(word));
            } else {
                filesDictionary.add(new String(word));
            }
        }
    }

    public String getFilePart(String command, int caret) {
        String returnLine = null;

        String lineToParse = command.substring(0, caret);
        if (lineToParse.length() > 0) {
            returnLine = Completion.getFilePartLevel(lineToParse);
        }

        return returnLine;
    }

    public String getPart(String command, int caret) {
        String returnLine = "";

        String lineToParse = command.substring(0, caret);
        if (lineToParse.length() > 0) {
            returnLine = Completion.getPartLevel(lineToParse);
        }

        return returnLine;
    }

    public <T, U, V, W> void sendToHTML(String type) {
        if (log.isLoggable(Level.FINE))
        {
            log.fine("sendToHTML: " + type);
        }
        sendToHTML(type, null, null, null, null);
    }

    public <T, U, V, W> void sendToHTML(String type, T data1) {
        if (log.isLoggable(Level.FINE))
        {
            log.fine("sendToHTML: " + type + " " + data1);
        }
        sendToHTML(type, data1, null, null, null);
    }

    public <T, U, V, W> void sendToHTML(String type, T data1, U data2) {
        if (log.isLoggable(Level.FINE))
        {
            log.fine("sendToHTML: " + type + " " + data1 + " " + data2);
        }
        sendToHTML(type, data1, data2, null, null);
    }

    public <T, U, V, W> void sendToHTML(String type, T data1, U data2, V data3) {
        if (log.isLoggable(Level.FINE))
        {
            log.fine("sendToHTML: " + type + " " + data1 + " " + data2 + " " + data3);
        }
        sendToHTML(type, data1, data2, data3, null);
    }

    public <T, U, V, W> void sendToHTML(String type, T data1, U data2, V data3, W data4) {
        if (log.isLoggable(Level.FINE))
        {
            log.fine("sendToHTML: " + type + " " + data1 + " " + data2 + " " + data3 + " " + data4);
        }
        ConsoleMessage<T, U, V, W> msg = new ConsoleMessage<T, U, V, W>(type, data1, data2, data3, data4);
        String code = String.format("fromJava(%s)", msg.toJson());
        if (ready) {
            cefPanel.executeJS(code, null);
        } else {
            waitingQueue.add(code);
        }
    }

    public abstract void helpOnTheKeyword();
}
