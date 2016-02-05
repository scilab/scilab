/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.scinotes.actions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.nio.charset.IllegalCharsetNameException;
import java.nio.charset.UnsupportedCharsetException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * EncodingAction Class
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 *
 */
public class EncodingAction extends DefaultCheckAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -5421313717126859924L;

    private static final String CHECKICON = ScilabConstants.SCI.getPath() + "/modules/gui/images/icons/check-icon.png";

    private static Map<String, String> encodings = new HashMap<String, String>();
    private static Map<String, List<String>> language = new HashMap<String, List<String>>();
    private static JRadioButtonMenuItem[] radioTypes;
    private static Menu[] menuLang;

    static {
        encodings.put("x-MacArabic", "Arabic");
        encodings.put("x-MacCentralEurope", "Central European");
        encodings.put("x-MacCroatian", "Central European");
        encodings.put("x-MacCyrillic", "Cyrillic");
        encodings.put("x-MacGreek", "Greek");
        encodings.put("x-MacHebrew", "Hebrew");
        encodings.put("x-MacIceland", "Western European");
        encodings.put("x-MacRoman", "Western European");
        encodings.put("x-MacRomania", "Central European");
        encodings.put("x-MacThai", "Thai");
        encodings.put("x-MacTurkish", "Turkish");
        encodings.put("x-MacUkraine", "Cyrillic");
        encodings.put("ASMO-708", "Arabic");
        encodings.put("cp866", "Cyrillic");
        encodings.put("windows-874", "Thai");
        encodings.put("shift_jis", "Japanese");
        encodings.put("gb2312", "Chinese Simplified");
        encodings.put("ks_c_5601-1987", "Korean");
        encodings.put("big5", "Chinese Traditional");
        encodings.put("utf-16", "Unicode");
        encodings.put("windows-1250", "Central European");
        encodings.put("windows-1251", "Cyrillic");
        encodings.put("windows-1252", "Western European");
        encodings.put("windows-1253", "Greek");
        encodings.put("windows-1254", "Turkish");
        encodings.put("windows-1255", "Hebrew");
        encodings.put("windows-1256", "Arabic");
        encodings.put("windows-1257", "Baltic");
        encodings.put("windows-1258", "Vietnamese");
        encodings.put("Johab", "Korean");
        encodings.put("utf-32", "Unicode");
        encodings.put("utf-32BE", "Unicode");
        encodings.put("us-ascii", "US-ASCII");
        encodings.put("koi8-r", "Cyrillic");
        encodings.put("EUC-JP", "Japanese");
        encodings.put("koi8-u", "Cyrillic");
        encodings.put("iso-8859-1", "Western European");
        encodings.put("iso-8859-2", "Central European");
        encodings.put("iso-8859-3", "Latin");
        encodings.put("iso-8859-4", "Baltic");
        encodings.put("iso-8859-5", "Cyrillic");
        encodings.put("iso-8859-6", "Arabic");
        encodings.put("iso-8859-7", "Greek");
        encodings.put("iso-8859-8", "Hebrew");
        encodings.put("iso-8859-9", "Turkish");
        encodings.put("iso-8859-13", "Estonian");
        encodings.put("iso-8859-15", "Latin");
        encodings.put("iso-2022-jp", "Japanese");
        encodings.put("csISO2022JP", "Japanese");
        encodings.put("iso-2022-kr", "Korean");
        encodings.put("euc-jp", "Japanese");
        encodings.put("EUC-CN", "Chinese Simplified");
        encodings.put("euc-kr", "Korean");
        encodings.put("GB18030", "Chinese Simplified");
        encodings.put("utf-8", "Unicode");
    }

    private String encoding;

    /**
     * Constructor
     * @param encodingName Encoding Name
     * @param editor SciNotes
     */
    public EncodingAction(String encodingName, SciNotes editor) {
        super(encodingName, editor);
        encoding = encodingName;
    }

    /**
     * Clean
     */
    public static void close() {
        radioTypes = null;
        menuLang = null;
    }

    /**
     * createRadioButtonMenuItem
     * @param editor SciNotes
     * @return JRadioButtonMenuItem
     */
    public JRadioButtonMenuItem createRadioButtonMenuItem(SciNotes editor) {
        JRadioButtonMenuItem radio = new JRadioButtonMenuItem(encoding);
        radio.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
                doAction();
            }
        });

        return radio;
    }

    /**
     * createEncodingSubMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return a Menu
     */
    public static Menu createMenu(String label, SciNotes editor, KeyStroke key) {
        Menu encodingTypeMenu = ScilabMenu.createMenu();
        encodingTypeMenu.setText(label);

        Map<String, List<String>> languages = getEncodings();
        Iterator<String> iter = languages.keySet().iterator();
        int size = 0;
        while (iter.hasNext()) {
            size += languages.get(iter.next()).size();
        }

        if (radioTypes == null) {
            radioTypes = new JRadioButtonMenuItem[size];
            menuLang = new Menu[languages.size()];
        }

        ButtonGroup group = new ButtonGroup();

        iter = languages.keySet().iterator();
        int psize = 0;
        int k = 0;
        while (iter.hasNext()) {
            String lang = iter.next();
            List<String> encodingList = languages.get(lang);
            menuLang[k] = ScilabMenu.createMenu();
            menuLang[k].setText(lang);
            encodingTypeMenu.add(menuLang[k]);
            for (int i = 0; i < encodingList.size(); i++) {
                radioTypes[psize + i] = (new EncodingAction(encodingList.get(i), editor)).createRadioButtonMenuItem(editor);
                group.add(radioTypes[psize + i]);
                ((JMenu) menuLang[k].getAsSimpleMenu()).add(radioTypes[psize + i]);

                if (encodingList.get(i).equalsIgnoreCase(Charset.defaultCharset().toString())) {
                    radioTypes[psize + i].setSelected(true);
                }
            }
            psize += encodingList.size();
            k++;
        }

        return encodingTypeMenu;
    }

    public static Set<String> getSupportedEncodings() {
        return encodings.keySet();
    }

    /**
     * Update the selected item in the encoding pull down menu of the document.
     * @param scilabDocument the document for which the encoding menu should
     * be updated
     */
    public static void updateEncodingMenu(ScilabDocument scilabDocument) {
        if (radioTypes != null) {
            for (int i = 0; i < radioTypes.length; i++) {
                if (scilabDocument.getEncoding().equalsIgnoreCase(radioTypes[i].getText())) {
                    radioTypes[i].setSelected(true);
                    updateIcon(radioTypes[i].getText());
                    return;
                }
            }
        }
    }

    /**
     * getEncodings
     * @return Map : Language -&gt; {enc1, enc2, ...}
     */
    public static Map<String, List<String>> getEncodings() {
        if (!language.isEmpty()) {
            return language;
        }

        Set<String> keys = encodings.keySet();
        Iterator<String> iterator = keys.iterator();
        while (iterator.hasNext()) {
            String enc = iterator.next();
            try {
                Charset.forName(enc);
                String lang = encodings.get(enc);
                if (!language.containsKey(lang)) {
                    language.put(lang, new ArrayList<String>());
                }

                language.get(lang).add(enc);
            } catch (IllegalCharsetNameException e) {
                encodings.remove(enc);
            } catch (UnsupportedCharsetException e) {
                encodings.remove(enc);
            }
        }

        language = new TreeMap<String, List<String>>(language);
        return language;
    }

    /**
     * doAction
     */
    public void doAction() {
        boolean isSuccess = false;

        ScilabDocument styleDocument = ((ScilabDocument) getEditor().getTextPane().getDocument());

        if (styleDocument.isContentModified()) {
            /* File modified */
            if (getEditor().getTextPane().getName() != null) {
                /* Not untitled */
                switch (ScilabModalDialog.show(getEditor(),
                                               SciNotesMessages.MODIFICATIONS_WILL_BE_LOST, SciNotesMessages.CONTINUE,
                                               IconType.QUESTION_ICON, ButtonType.YES_NO)) {
                    case YES_OPTION : //Yes, continue
                        break;
                    case NO_OPTION ://No, exit
                        //Back to previous menu checked
                        updateEncodingMenu(styleDocument);
                        return;
                    default:
                        break;
                }
            }
        }

        // Avoid modifications to be saved
        styleDocument.setUpdater(false);
        boolean indentMode = styleDocument.getAutoIndent();
        styleDocument.setAutoIndent(false);

        styleDocument.setEncoding(encoding);
        //ConfigSciNotesManager.saveDefaultEncoding(encoding);

        //Update the menu
        updateIcon(encoding);

        // If file associated then reload
        EditorKit editorKit = getEditor().getEditorKit();
        String fileName = getEditor().getTextPane().getName();

        FileInputStream fis = null;
        InputStreamReader isr = null;
        BufferedReader br = null;

        try {
            if (fileName != null) {
                File file = new File(getEditor().getTextPane().getName());
                if (file.exists()) {
                    if (styleDocument.getLength() > 0) {
                        styleDocument.getUndoManager().discardAllEdits();
                        styleDocument.disableUndoManager();
                        styleDocument.remove(0, styleDocument.getLength());

                        fis = new FileInputStream(file);
                        isr = new InputStreamReader(fis, encoding);
                        br = new BufferedReader(isr);
                        editorKit.read(br, styleDocument, 0);

                        styleDocument.enableUndoManager();
                    }
                }
            }
            isSuccess = true;
        } catch (UnsupportedEncodingException e) {
            isSuccess = false;
        } catch (FileNotFoundException e) {
            isSuccess = false;
        } catch (IOException e) {
            isSuccess = false;
        } catch (BadLocationException e) {
            isSuccess = false;
        } finally {
            try {
                if (fis != null) {
                    fis.close();
                }
                if (isr != null) {
                    isr.close();
                }
                if (br != null) {
                    br.close();
                }
            } catch (IOException e) { }
        }

        /* Allow changes to be saved */
        styleDocument.setAutoIndent(indentMode);
        styleDocument.setUpdater(true);

        styleDocument.setContentModified(false);
        // changing such a property dicards undo
        styleDocument.getUndoManager().discardAllEdits();
        if (!isSuccess) {
            ScilabModalDialog.show(getEditor(), SciNotesMessages.COULD_NOT_CONVERT_FILE,
                                   SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
        }
    }

    /**
     * Add a check-icon near the current used language
     * @param enc the actual encoding
     */
    private static void updateIcon(String enc) {
        String lang = encodings.get(enc);
        for (int i = 0; i < menuLang.length; i++) {
            if (((JMenu) menuLang[i].getAsSimpleMenu()).getIcon() != null) {
                ((JMenu) menuLang[i].getAsSimpleMenu()).setIcon(null);
            }
            if (menuLang[i].getText().equals(lang)) {
                ((JMenu) menuLang[i].getAsSimpleMenu()).setIcon(new ImageIcon(CHECKICON));
            }
        }
    }
}
