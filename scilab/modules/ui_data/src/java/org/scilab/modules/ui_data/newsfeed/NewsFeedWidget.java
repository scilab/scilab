/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2016 - Scilab Enterprises - Simon Marchetto
 * Copyright (C) 2016 - Scilab Enterprises - Clement David
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

package org.scilab.modules.ui_data.newsfeed;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.utils.WebBrowser;

import javax.swing.JPanel;
import javax.swing.JEditorPane;
import javax.swing.JScrollPane;
import javax.swing.text.html.HTMLEditorKit;
import javax.swing.text.Document;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;

import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.io.File;
import java.text.SimpleDateFormat;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JButton;

/**
 * News feed widget Displays one news at a time.
 * Controlled by NewsFeedController
 * Displays news in HTML format (JEditorPane/HTMLEditorKit)
 * Loads styles from CSS newsfeed.css (in directory ui_data/etc or SCIHOME)
 */
public class NewsFeedWidget extends JPanel implements NewsFeedEventListener, HyperlinkListener {
    private static final String NEWS_DATE_HTML_ID = "news_date";
    private static final String NEWS_CONTENT_HTML_ID = "news_content";
    private static final String NEWS_LINK_HTML_ID = "news_link";
    private static final String NEWS_MEDIA_CONTENT_HTML_ID = "news_media_content";
    private static final String NEWS_DESCRIPTION_HTML_ID = "news_description";

    private final NewsFeedController newsFeedController;
    private final JEditorPane editorPane;
    private final JScrollPane scrollPane;
    private final JPanel headerPane;
    private final JButton headerButton;

    public NewsFeedWidget(NewsFeedController newsFeedController) {
        this.newsFeedController = newsFeedController;

        editorPane = new JEditorPane();
        editorPane.setEditable(false);
        editorPane.setContentType("text/html");

        // use current look & feel font
        editorPane.putClientProperty(JEditorPane.HONOR_DISPLAY_PROPERTIES, Boolean.TRUE);
        Font font = UIManager.getFont("Label.font");
        editorPane.setFont(font);

        HTMLEditorKit htmlEditorKit = new HTMLEditorKit();
        editorPane.setEditorKit(htmlEditorKit);
        Document doc = htmlEditorKit.createDefaultDocument();
        editorPane.setDocument(doc);

        htmlEditorKit.getStyleSheet().importStyleSheet(getStyleSheetURL());

        editorPane.addHyperlinkListener(this);

        headerButton = new JButton();
        headerPane = new JPanel(new BorderLayout());
        headerPane.add(new JButton(new AbstractAction(null, new ImageIcon(FindIconHelper.findIcon("go-previous"))) {
            @Override
            public void actionPerformed(ActionEvent e) {
                newsFeedController.previousNews();
            }
        }), BorderLayout.WEST);
        headerPane.add(headerButton, BorderLayout.CENTER);
        headerPane.add(new JButton(new AbstractAction(null, new ImageIcon(FindIconHelper.findIcon("go-next"))) {
            @Override
            public void actionPerformed(ActionEvent e) {
                newsFeedController.nextNews();
            }
        }), BorderLayout.EAST);

        scrollPane = new JScrollPane(editorPane);

        setLayout(new BorderLayout());
        add(headerPane, BorderLayout.NORTH);
        add(scrollPane, BorderLayout.CENTER);
    }

    public void newsFeedEventReceived(NewsFeedEvent evt) {
        switch (evt.getEventType()) {
            case NewsFeedEvent.NEWS_CHANGED: {
                displayNews(newsFeedController.getCurrentNews());
                break;
            }
            case NewsFeedEvent.NEWSFEED_ERROR: {
                displayError(((NewsFeedErrorEvent) evt).getErrorMessage());
                break;
            }
        }
    }

    public void hyperlinkUpdate(HyperlinkEvent event) {
        if (event.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
            URL url = event.getURL();
            if (url != null) {
                WebBrowser.openUrl(url, event.getDescription());
            }
        }
    }

    private void displayNews(News news) {
        StringBuilder newsHtmlBuilder = new StringBuilder();

        // Add media content (image) if exist
        NewsMediaContent mediaContent = news.getMediaContent();
        if (mediaContent != null) {
            newsHtmlBuilder.append(getDivHtml(getImageHtml(mediaContent.getURL(), mediaContent.getWidth(), mediaContent.getHeight()), NEWS_MEDIA_CONTENT_HTML_ID));
        }

        if (news.getContent() != null) {
            // if given, use RSS item content
            newsHtmlBuilder.append(getDivHtml(news.getContent(), NEWS_CONTENT_HTML_ID));
        } else {
            // otherwise get content from description
            newsHtmlBuilder.append(getDivHtml(news.getDescription(), NEWS_DESCRIPTION_HTML_ID));
        }

        // Add news link if exist
        if (news.getLink() != null) {
            newsHtmlBuilder.append(getDivHtml(getLinkHtml(news.getLink(), news.getLink()), NEWS_LINK_HTML_ID));
        }

        display(new AbstractAction(news.getTitle()) {
            @Override
            public void actionPerformed(ActionEvent e) {
                try {
                    WebBrowser.openUrl(new URL(news.getLink()), news.getLink());
                } catch (MalformedURLException ex) {
                    Logger.getLogger(NewsFeedWidget.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }, getHTML(newsHtmlBuilder));
    }

    private String getImageHtml(String url, String width, String height) {
        // Cannot setup border with CSS, limitation of CSS support of HTMLEditorKit
        return String.format("<img src='%s' border='0' width='%s', height='%s'/>", url, width, height);
    }

    private String getLinkHtml(String url, String description) {
        return String.format("<a href='%s'>%s</a>", url, description);
    }

    private String getSpanHtml(String spanContent, String spanId) {
        return String.format("<span id='%s'>%s</span>", spanId, spanContent);
    }

    private String getDivHtml(String divContent, String divId) {
        return String.format("<div id='%s'>%s</div>", divId, divContent);
    }

    private String getDivHtml(String divContent) {
        return String.format("<div>%s</div>", divContent);
    }

    private void displayError(String errorMessage) {
        StringBuilder errMsgHtmlBuilder = new StringBuilder(errorMessage);
        display(new AbstractAction("") {
            @Override
            public void actionPerformed(ActionEvent e) {
                URL url = newsFeedController.getNewsFetcher().getURL();
                WebBrowser.openUrl(url, url.toString());
            }
        }, getHTML(errMsgHtmlBuilder));
    }

    private String getHTML(StringBuilder htmlBuilder) {
        htmlBuilder.insert(0, "<html><body>");
        htmlBuilder.append("</body></html>");
        return htmlBuilder.toString();
    }

    private void display(final Action titleAction, final String htmlContent) {
        try	{
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    headerButton.setAction(titleAction);
                    headerButton.setToolTipText(titleAction.getValue(Action.NAME).toString());

                    editorPane.setText(htmlContent);
                    // reset position to the upper left
                    editorPane.setCaretPosition(0);
                }
            });
        } catch (Exception e) {
        }
    }

    private URL getStyleSheetURL() {
        File settingsFile = new File(ScilabConstants.SCIHOME + "/newsfeed.css");
        if (!settingsFile.exists()) {
            settingsFile = new File(ScilabConstants.SCI + "/modules/ui_data/etc/newsfeed.css");
        }
        if (settingsFile.exists()) {
            try {
                return settingsFile.toURI().toURL();
            } catch (MalformedURLException e) {
                return null;
            }
        } else {
            return null;
        }
    }
}
