/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

package org.scilab.modules.gui.utils;

import java.awt.Desktop;
import java.net.URI;
import java.net.URL;
import java.util.Set;

import javax.swing.JOptionPane;

import org.w3c.dom.Document;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import org.scilab.modules.localization.Messages;


/**
 * Class used to launch a Web Browser from Scilab
 * @author Vincent COUVERT
 */
public final class WebBrowser implements XConfigurationListener {

    private static final String ERROR_MSG = Messages.gettext("Could not open: ");
    public static final String WEBPATH = "//web/body/web";

    private static WebPreferences webprefs;
    private static Document doc;

    static {
        XConfiguration.addXConfigurationListener(new WebBrowser());
    }

    /**
     * Constructor
     */
    private WebBrowser() { }

    /**
     * Prevent the listener that configuration has changed
     *
     * @param e the event
     */
    public void configurationChanged(XConfigurationEvent e) {
        Set<String> path = e.getModifiedPaths();
        if (path.contains("ALL") || path.contains(WEBPATH)) {
            webprefs = null;
            doc = null;
        }
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
            openUrl(url.toString());
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

        if (webprefs == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            webprefs = XConfiguration.get(WebBrowser.WebPreferences.class, doc, WEBPATH)[0];
        }

        try {
            String protocol = getProtocol(url);
            if (protocol == null) {
                if (url.charAt(0) == '<') {
                    // We have <pierre.marechal@scilab.org>
                    String mail = "mailto:" + url.substring(1, url.length() - 1);
                    if (webprefs.defaultMailer) {
                        Desktop.getDesktop().mail(new URI(mail));
                    } else {
                        Runtime.getRuntime().exec(webprefs.cmdMailer + " " + new URI(mail).toString());
                    }
                }
            } else if (protocol.equals("mailto")) {
                if (webprefs.defaultMailer) {
                    Desktop.getDesktop().mail(new URI(url));
                } else {
                    Runtime.getRuntime().exec(webprefs.cmdMailer + " " + new URI(url).toString());
                }
            } else {
                if (webprefs.defaultBrowser) {
                    // Under Windows, ShellExecute is called with the URI and under Linux it is gnome_url_show.
                    // So to handle different protocol in URI, user must config its OS to handle them.
                    Desktop.getDesktop().browse(new URI(url));
                } else {
                    Runtime.getRuntime().exec(webprefs.cmdBrowser + " " + new URI(url).toString());
                }
            }
        } catch (Exception e) {
            JOptionPane.showMessageDialog(null, ERROR_MSG + url);
        }
    }

    private static String getProtocol(String url) {
        int colon = url.indexOf(':');
        if (colon != -1) {
            return url.substring(0, colon);
        } else {
            return null;
        }
    }

    /* web */
    @XConfAttribute
    static class WebPreferences {

        boolean defaultBrowser;
        boolean defaultMailer;
        String cmdBrowser;
        String cmdMailer;

        private WebPreferences() { }

        @XConfAttribute(tag = "web", attributes = {"default-browser", "default-mailer", "command-browser", "command-mailer"})
        private void set(boolean defaultBrowser, boolean defaultMailer, String cmdBrowser, String cmdMailer) {
            this.defaultBrowser = defaultBrowser;
            this.defaultMailer = defaultMailer;
            this.cmdBrowser = cmdBrowser;
            this.cmdMailer = cmdMailer;
        }
    }
}
