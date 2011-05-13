/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Desktop;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;

import javax.swing.JOptionPane;

import org.scilab.modules.localization.Messages;

/**
 * Class used to launch a Web Browser from Scilab
 * @author Vincent COUVERT
 */
public final class WebBrowser {

    private static final String ERROR_MSG = Messages.gettext("Could not open: ");

    /**
     * Constructor
     */
    private WebBrowser() {
        throw new UnsupportedOperationException();
    }

    /**
     * Open the Browser/Mailer with Given URL
     * @param url the URL to open
     * @param descrition the link as String
     */
    public static void openUrl(URL url, String description) {
        if (url == null) {
            JOptionPane.showMessageDialog(null, ERROR_MSG + description);
        } else {
            try {
                Desktop.getDesktop().browse(url.toURI());
            } catch (IOException e) {
                JOptionPane.showMessageDialog(null, ERROR_MSG + url);
            } catch (URISyntaxException e) {
                JOptionPane.showMessageDialog(null, ERROR_MSG + url);
            }
        }
    }

    /**
     * Open the Browser/Mailer with Given URL
     * @param url the URL to open
     */
    public static void openUrl(String url) {
        if (url == null || url.length() == 0) {
            return;
        }

        try {
            if (url.charAt(0) == 'h' || url.charAt(0) == 'f') {
                // We have something like http://... or file://
                Desktop.getDesktop().browse(new URI(url));
            } else {
                // We have <pierre.marechal@scilab.org>
                String mail = "mailto:" + url.substring(1, url.length() - 1);
                Desktop.getDesktop().mail(new URI(mail));
            }
        } catch (IOException e) {
            JOptionPane.showMessageDialog(null, ERROR_MSG + url);
        } catch (URISyntaxException e) {
            JOptionPane.showMessageDialog(null, ERROR_MSG + url);
        }
    }
}
