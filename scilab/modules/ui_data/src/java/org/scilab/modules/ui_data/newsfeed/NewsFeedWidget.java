/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015-2016 - Scilab Enterprises - Simon Marchetto
 * Copyright (C) 2016 - Scilab Enterprises - Clement David
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
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
import org.scilab.modules.commons.utils.WebBrowser;

import com.google.gson.Gson;

import org.scilab.modules.commons.gui.ScilabCefPanel;

import javax.swing.JPanel;
import javax.swing.Timer;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.io.File;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.handler.CefLoadHandlerAdapter;

/**
 * News feed widget Displays one news at a time.
 * Controlled by NewsFeedController
 * Displays news in HTML format using JCEF browser
 */
public final class NewsFeedWidget extends JPanel implements NewsFeedEventListener {
    private static final long serialVersionUID = 1L;
    private static final String NEWS_DATE_HTML_ID = "news_date";
    private static final String NEWS_CONTENT_HTML_ID = "news_content";
    private static final String NEWS_LINK_HTML_ID = "news_link";
    private static final String NEWS_MEDIA_CONTENT_HTML_ID = "news_media_content";
    private static final String NEWS_DESCRIPTION_HTML_ID = "news_description";

    private final transient NewsFeedController newsFeedController;
    private final transient JPanel headerPane;
    private final JButton headerButton;
    private final ScilabCefPanel cefPanel;

    public NewsFeedWidget(NewsFeedController newsFeedController) {
        this.newsFeedController = newsFeedController;

        cefPanel = new ScilabCefPanel(getURL());

        // Report errors when loading the news feed page, and display an error message in this case
        cefPanel.getClient().addLoadHandler(new CefLoadHandlerAdapter() 
        {   
            @Override
            public void onLoadError(CefBrowser browser, CefFrame frame, ErrorCode errorCode, String errorText, String failedUrl) {
                Logger.getLogger(NewsFeedWidget.class.getName()).log(Level.SEVERE, "Failed to load news feed page: {0} ({1})", new Object[]{errorText, failedUrl});
                displayError(NewsFeedUIMessages.NEWS_FEED_UNAVAILABLE_ERROR);
            }
        });

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

        setLayout(new BorderLayout());
        add(headerPane, BorderLayout.NORTH);
        add(cefPanel, BorderLayout.CENTER);
    }

    private static final String getURL() {
        String url;
        try {
            url = new File(ScilabConstants.SCI, "/modules/ui_data/src/web/newsfeed.html").toURI().toURL().toExternalForm();
        } catch (MalformedURLException e) {
            url = "about:blank";
        }
        return url;
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
        } else if (news.getDescription() != null) {
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
                String link = news.getLink();
                if (link == null)
                {
                    link = "https://www.scilab.org";
                }

                try {
                    WebBrowser.openUrl(URI.create(link).toURL(), link);
                } catch (MalformedURLException | IllegalArgumentException ex) {
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
        return htmlBuilder.toString();
    }

    private void display(final Action titleAction, final String htmlContent) {
        // Load HTML content into JCEF browser
        if (htmlContent == null || htmlContent.isEmpty()) {
            return;
        }

        if (!cefPanel.getBrowser().hasDocument())
        {
            // wait for the loading and then display the content
            Timer t = new Timer(2000, (ActionEvent e) -> display(titleAction, htmlContent));
            t.setRepeats(false);
            t.start();
            return;
        }

        // load content using javascript
        headerButton.setAction(titleAction);
        headerButton.setToolTipText(titleAction.getValue(Action.NAME).toString());

        StringBuilder jsStr = new StringBuilder("document.body.innerHTML = ");
        new Gson().toJson(htmlContent, jsStr);
        cefPanel.executeJS(jsStr.toString(), null);
    }
}
