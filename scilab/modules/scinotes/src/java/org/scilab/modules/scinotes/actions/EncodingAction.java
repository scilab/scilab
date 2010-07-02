/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
import java.util.List;
import java.util.Iterator;
import java.util.Set;
import java.util.SortedMap;
import java.util.Map;
import java.util.HashMap;

import javax.swing.JRadioButtonMenuItem;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;

import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
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

    private static Map<String, String> encodings = new HashMap();
    private static Map<String, List<String>> language = new HashMap();

    static {
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
        encodings.put("iso-2022-jp", "Japanese");
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
     * getEncodings
     * @return Map : Language -> {enc1, enc2, ...}
     */
    public static Map<String, List<String>> getEncodings() {
        Set<String> keys = encodings.keySet();
        Iterator<String> iterator = keys.iterator();
        while (iterator.hasNext()) {
            String enc = iterator.next();
            try {
                Charset.forName(enc);
                String lang = encodings.get(enc);
                if (!language.containsKey(lang)) {
                    language.put(lang, new ArrayList());
                }

                language.get(lang).add(enc);
            } catch (IllegalCharsetNameException e) {
                encodings.remove(enc);
            } catch (UnsupportedCharsetException e) {
                encodings.remove(enc);
            }
        }

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
                    getEditor().getSciNotesGUI().updateEncodingMenu(styleDocument);
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
        ConfigSciNotesManager.saveDefaultEncoding(encoding);

        // If file associated then reload
        EditorKit editorKit = getEditor().getEditorKit();
        String fileName = getEditor().getTextPane().getName();

        try {
            if (fileName != null) {
                File file = new File(getEditor().getTextPane().getName());
                if (file.exists()) {
                    if (styleDocument.getLength() > 0) {
                        styleDocument.getUndoManager().discardAllEdits();
                        styleDocument.disableUndoManager();
                        styleDocument.remove(0, styleDocument.getLength());
                        editorKit.read(new BufferedReader(new InputStreamReader(new FileInputStream(file), encoding)), styleDocument, 0);
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
}
