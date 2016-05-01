/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.filebrowser;

import java.awt.AWTEvent;
import java.awt.Color;
import java.awt.KeyboardFocusManager;
import java.awt.Toolkit;
import java.awt.event.AWTEventListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.util.Collections;
import java.util.Comparator;
import java.util.TreeSet;

import javax.swing.JComboBox;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultEditorKit;
import javax.swing.text.JTextComponent;

import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabFileSelectorComboBox extends JComboBox implements DocumentListener {

    private static final FileFilter DIRFILTER = new FileFilter() {
        public boolean accept(File f) {
            return f.isDirectory();
        }
    };

    private static boolean isWindows = (OS.get() == OS.WINDOWS);

    private int lastSize = -1;
    private String oldName = "";
    private TreeSet<String> currentSet;
    private JTextComponent textComponent;
    private boolean disableUpdateCombo;
    private boolean disableShowPopup;
    private boolean isValidate;
    private CommonCallBack validation;
    private SwingScilabTreeTable stt;
    private Color defaultFg;

    /**
     * Default constructor
     * @param baseDir the base directory to open
     * @param validation the callback to execute when the user valids the directory
     */
    public ScilabFileSelectorComboBox() {
        super();
        setFocusTraversalKeys(KeyboardFocusManager.FORWARD_TRAVERSAL_KEYS, Collections.EMPTY_SET);
        textComponent = (JTextComponent) getEditor().getEditorComponent();
        textComponent.getDocument().addDocumentListener(this);
        defaultFg = textComponent.getForeground();

        getEditor().getEditorComponent().addKeyListener(new KeyAdapter() {
            public void keyReleased(KeyEvent e) {
                int code = e.getKeyCode();
                if (code != KeyEvent.VK_ESCAPE && code != KeyEvent.VK_ENTER) {
                    showPopup();
                } else if (code == KeyEvent.VK_ENTER) {
                    if (!isValidate) {
                        updateComboBoxLater();
                        if (!currentSet.isEmpty()) {
                            showPopup();
                        }
                    } else {
                        isValidate = false;
                    }
                }
            }

            public void keyPressed(KeyEvent e) {
                int code = e.getKeyCode();
                if (code == KeyEvent.VK_DOWN || code == KeyEvent.VK_KP_DOWN || code == KeyEvent.VK_PAGE_DOWN
                        || code == KeyEvent.VK_UP || code == KeyEvent.VK_KP_UP || code == KeyEvent.VK_PAGE_UP
                        || code == KeyEvent.VK_END || code == KeyEvent.VK_BEGIN) {
                    disableUpdateCombo = true;
                } else {
                    disableUpdateCombo = false;
                }
            }
        });

        /* Workaround to override the default behaviour of TAB key */
        Toolkit.getDefaultToolkit().addAWTEventListener(new AWTEventListener() {
            public void eventDispatched(AWTEvent e) {
                KeyEvent ke = (KeyEvent) e;
                if (ke.getKeyCode() == KeyEvent.VK_TAB && ke.getID() == KeyEvent.KEY_RELEASED && ke.getSource() == textComponent) {
                    updateComboBoxLater();
                    String cp = getCommonPart();
                    getEditor().setItem(cp);
                    showPopup();
                    ke.consume();
                }
            }
        }, AWTEvent.KEY_EVENT_MASK);

        setEditable(true);

        textComponent.setComponentPopupMenu(createPopup());
    }

    /**
     * Set the treetable linked with this combobox
     */
    public void setTreeTable(SwingScilabTreeTable stt) {
        this.stt = stt;
    }

    /**
     * Set the action as CallBack
     * @param validation the action to use when a path is validated
     */
    public void setAction(CommonCallBack validation) {
        this.validation = validation;
        textComponent.addKeyListener(new KeyAdapter() {
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    isValidate = true;
                    ScilabFileSelectorComboBox.this.validation.callBack();
                    hidePopup();
                    e.consume();
                }
            }
        });
    }

    /**
     * Set the base directory and validate
     * @param path the path to set
     */
    public void setBaseDirAndValidate(String path) {
        disableUpdateCombo = true;
        textComponent.getDocument().removeDocumentListener(this);
        getEditor().setItem(path);
        validation.callBack();
        disableUpdateCombo = false;
    }

    /**
     * @return the base directory
     */
    public void setBaseDir(String baseDir) {
        if (!baseDir.endsWith(File.separator)) {
            baseDir += File.separator;
        }
        disableShowPopup = true;
        textComponent.setText(baseDir);
        disableShowPopup = false;
    }

    /**
     * @return the base directory
     */
    public String getBaseDir() {
        return getTruePath(getText());
    }

    /**
     * {@inheritDoc}
     */
    public void changedUpdate(DocumentEvent e) { }

    /**
     * {@inheritDoc}
     */
    public void insertUpdate(DocumentEvent e) {
        testPathValidity();
    }

    /**
     * {@inheritDoc}
     */
    public void removeUpdate(DocumentEvent e) {
        testPathValidity();
    }

    /**
     * Test the path validity. If the path is invalid the color in the combo's textfield is set to RED
     */
    private void testPathValidity() {
        File f = new File(getText());
        if (f.exists() && f.isDirectory() && f.canRead()) {
            textComponent.setForeground(defaultFg);
            updateComboBox();
        } else {
            textComponent.setForeground(Color.RED);
        }
    }

    /**
     * @return the text in the textfield
     */
    private String getText() {
        int len = textComponent.getDocument().getLength();
        try {
            return textComponent.getDocument().getText(0, len);
        } catch (BadLocationException e) {
            return "";
        }
    }

    /**
     * Get the path where the jokers such as ~, SCI or TMPDIR are replaced by their values
     * @param path the path
     * @return the true path
     */
    private static final String getTruePath(String path) {
        if (path != null) {
            if (path.startsWith("SCI\\") || path.startsWith("SCI/") || path.startsWith("SCI")) {
                return path.replaceFirst("SCI", ScilabConstants.SCI.getAbsolutePath());
            }
            if (path.startsWith("~\\") || path.startsWith("~/") || path.startsWith("~")) {
                return path.replaceFirst("~", ScilabConstants.USERHOME);
            }
            if (path.startsWith("TMPDIR\\") || path.startsWith("TMPDIR/") || path.startsWith("TMPDIR")) {
                return path.replaceFirst("TMPDIR", ScilabConstants.TMPDIR.getAbsolutePath());
            }
        }

        boolean failed = true;
        for (int i = 0; i < 5 && failed; i++) {
            try {
                path = new File(path).getCanonicalPath();
                failed = false;
            } catch (IOException e) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException ee) { }
            }
        }

        return path;
    }


    /**
     * Get the path where the jokers such as ~, SCI or TMPDIR are replaced by their values
     * @param path the path
     * @param joker the joker
     * @return the true path
     */
    private static final String getTruePath(String path, String joker) {
        if (path != null && joker != null) {
            if (joker.equals("SCI")) {
                return path.replaceFirst("SCI", ScilabConstants.SCI.getAbsolutePath());
            }
            if (joker.equals("~")) {
                return path.replaceFirst("~", ScilabConstants.USERHOME);
            }
            if (joker.equals("TMPDIR")) {
                return path.replaceFirst("TMPDIR", ScilabConstants.TMPDIR.getAbsolutePath());
            }
        }

        return path;
    }

    /**
     * Get the joker, if one exists, in the path. Jokers can be ~, SCI or TMPDIR
     * @param path the path to analyze
     * @return the joker
     */
    private static final String getJoker(String path) {
        if (path != null) {
            if (path.startsWith("SCI\\") || path.startsWith("SCI/")) {
                return "SCI";
            }
            if (path.startsWith("~\\") || path.startsWith("~/")) {
                return "~";
            }
            if (path.startsWith("TMPDIR\\") || path.startsWith("TMPDIR/")) {
                return "TMPDIR";
            }
        }

        return null;
    }

    /**
     * Get a path where the value of SCI is replaced by the string "SCI" (or ~, TMPDIR)
     * @param path the path
     * @param joker the joker
     * @return the fake path
     */
    private static final String getFakePath(String path, String joker) {
        if (path != null && joker != null) {
            if (joker.equals("SCI")) {
                return path.replaceFirst(ScilabConstants.SCI.getAbsolutePath(), "SCI");
            }
            if (joker.equals("~")) {
                return path.replaceFirst(ScilabConstants.USERHOME, "~");
            }
            if (joker.equals("TMPDIR")) {
                return path.replaceFirst(ScilabConstants.TMPDIR.getAbsolutePath(), "TMPDIR");
            }
        }

        return path;
    }

    /**
     * Get the directory where to list the file
     * @param name the directory name
     * @return the directory
     */
    private static final File getFileDirectory(String name) {
        File file = new File(name);

        if (name.endsWith(File.separator) && file.isDirectory()) {
            return file;
        }

        File parent = file.getParentFile();
        if (parent != null && parent.isDirectory()) {
            return parent;
        }

        return null;
    }

    /**
     * Get the common part of the strings present in currentSet (in fine, the combobox content list)
     * @return the common part
     */
    private String getCommonPart() {
        if (currentSet != null && !currentSet.isEmpty()) {
            if (currentSet.size() == 1) {
                return currentSet.first();
            }

            String parent = new File(currentSet.first()).getParent();
            int lenParent = 0;
            if (parent != null) {
                lenParent = parent.length();
            }

            int min = Integer.MAX_VALUE;
            char[][] strs = new char[currentSet.size()][];
            int i = 0;

            for (String s : currentSet) {
                strs[i] = s.substring(lenParent).toCharArray();
                if (strs[i].length < min) {
                    min = strs[i].length;
                }
                i++;
            }

            /* Now we get the common part */
            int j = strs.length;
            for (i = 0; i < min && j == strs.length; i++) {
                if (isWindows) {
                    char c = Character.toLowerCase(strs[0][i]);
                    j = 1;
                    for (; j < strs.length && Character.toLowerCase(strs[j][i]) == c; j++) {
                        ;
                    }
                } else {
                    char c = strs[0][i];
                    j = 1;
                    for (; j < strs.length && strs[j][i] == c; j++) {
                        ;
                    }
                }
            }

            String common = new String(strs[0], 0, i - 1);
            if (isWindows) {
                int k = i - 1;
                for (j = 1; j < strs.length && k == i - 1; j++) {
                    for (k = 0; k < i - 1 && strs[j][k] == strs[0][k]; k++) {
                        ;
                    }
                }

                if (j != strs.length) {
                    common = common.toLowerCase();
                }
            }

            if (lenParent != 0) {
                return parent + common;
            }
            return common;
        }

        return getText();
    }

    /**
     * Update the combobox. Since this function is called from the DocumentListener and methods in JComboBox change the JTextComponent content
     * (so the listener would try to write under a writeLock()... see DocumentListener javadoc), the update is called outside the main thread.
     */
    private void updateComboBox() {
        final boolean b = disableShowPopup;
        textComponent.getDocument().removeDocumentListener(this);
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                if (!disableUpdateCombo) {
                    updateComboBoxLater(b);
                }
                textComponent.getDocument().addDocumentListener(ScilabFileSelectorComboBox.this);
            }
        });
    }

    /**
     * Update the comboBox with the possible completion of the name
     * entered in the comboBox.
     */
    private void updateComboBoxLater() {
        updateComboBoxLater(false);
    }

    /**
     * Update the comboBox with the possible completion of the name
     * entered in the comboBox.
     */
    private void updateComboBoxLater(final boolean hidepopup) {
        String trueName = getText();
        if (trueName != null && !trueName.equals(oldName)) {
            String joker = getJoker(trueName);
            String name = getTruePath(trueName, joker);
            oldName = trueName;

            TreeSet<String> set = null;
            File[] files;

            if (name.isEmpty()) {
                files = File.listRoots();
            } else {
                File parent = getFileDirectory(name);

                if (parent != null) {
                    files = parent.listFiles(DIRFILTER);
                } else {
                    files = File.listRoots();
                }
            }

            if (isWindows) {
                set = new TreeSet<String>(new Comparator<String>() {
                    public int compare(String s1, String s2) {
                        int diff = s1.compareToIgnoreCase(s2);
                        if (diff == 0) {
                            diff = s1.compareTo(s2);
                        }
                        return diff;
                    }

                    public boolean equals(Object obj) {
                        return false;
                    }
                });
            } else {
                set = new TreeSet<String>();
            }
            for (File f : files) {
                String extra = File.separator;
                if (f.getParent() == null) {
                    extra = "";
                }
                set.add(getFakePath(f.getAbsolutePath(), joker) + extra);
            }

            if (set != null) {
                String bound = "";
                int len = trueName.length();
                if (len != 0) {
                    if (len == 1) {
                        bound = new String(new char[] {(char) (trueName.charAt(0) + 1)});
                    } else {
                        bound = trueName.substring(0, len - 1);
                        bound += (char) (trueName.charAt(len - 1) + 1);
                    }
                    currentSet = (TreeSet) set.subSet(trueName, true, bound, false);
                } else {
                    currentSet = set;
                }

                if (currentSet != null && currentSet.size() != lastSize) {
                    hidePopup();
                    removeAllItems();
                    for (String f : currentSet) {
                        addItem(f);
                    }
                    lastSize = currentSet.size();
                }
            }

            setSelectedIndex(-1);
            getEditor().setItem(trueName);
            textComponent.setSelectionStart(textComponent.getSelectionEnd());

            if (isShowing()) {
                setPopupVisible(!hidepopup);
            }
        }
    }

    /**
     * @return the popup used in the combo's textfield
     */
    private JPopupMenu createPopup() {
        JPopupMenu popup = new JPopupMenu();

        JMenuItem item = new JMenuItem(UiDataMessages.CUT);
        item.addActionListener(new DefaultEditorKit.CutAction());
        popup.add(item);

        item = new JMenuItem(UiDataMessages.COPY);
        item.addActionListener(new DefaultEditorKit.CopyAction());
        popup.add(item);

        item = new JMenuItem(UiDataMessages.PASTE);
        item.addActionListener(new DefaultEditorKit.PasteAction());
        popup.add(item);

        item = new JMenuItem(UiDataMessages.CLEAR);
        item.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                textComponent.setText("");
            }
        });
        popup.add(item);

        return popup;
    }
}
