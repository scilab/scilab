/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

import javax.swing.text.Element;
import javax.swing.text.AttributeSet;
import javax.swing.text.html.HTML;

import java.awt.BorderLayout;
import java.awt.Font;
import java.io.File;
import java.text.SimpleDateFormat;

import java.net.MalformedURLException;
import java.net.URL;

/**
 * News feed widget Displays one news at a time.
 * Controlled by NewsFeedController
 * Displays news in HTML format (JEditorPane/HTMLEditorKit)
 * Loads styles from CSS newsfeed.css (in directory ui_data/etc or SCIHOME)
 */
public class NewsFeedWidget extends JPanel implements NewsFeedEventListener, HyperlinkListener {
    private static final String NAVIGATION_HTML_ID = "navigation";
    private static final String PREVIOUS_HTML_ID = "previous";
    private static final String NEXT_HTML_ID = "next";
    private static final String NEWS_TITLE_HTML_ID = "news_title";
    private static final String NEWS_DATE_HTML_ID = "news_date";
    private static final String NEWS_LINK_HTML_ID = "news_link";
    private static final String NEWS_DESCRIPTION_HTML_ID = "news_description";

    private NewsFeedController newsFeedController;
    private JEditorPane editorPane;
    private JScrollPane scrollPane;

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

        scrollPane = new JScrollPane(editorPane);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

        setLayout(new BorderLayout());
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
            String name = null;
            Element element = event.getSourceElement();
            AttributeSet atts = element.getAttributes();
            AttributeSet linkAtts = (AttributeSet)atts.getAttribute(HTML.Tag.A);
            if (linkAtts != null) {
                name = (String)linkAtts.getAttribute(HTML.Attribute.NAME);
            }
            if (name != null) {
                if (name.equals("previous")) {
                    newsFeedController.previousNews();
                } else if (name.equals("next")) {
                    newsFeedController.nextNews();
                }
            } else {
                URL url = event.getURL();
                if (url != null) {
                    WebBrowser.openUrl(url, event.getDescription());
                }
            }
        }
    }

    private void displayNews(News news) {
        StringBuilder newsHtmlBuilder = new StringBuilder();

        // Add navigation links
        // Previous news icon link
        String previousIconUrl = getIconURL("go-previous");
        String previousIconHtml = getImageHtml("previous", previousIconUrl);
        String previousLinkHtml = getSpanHtml(getLinkHtml("previous", "previous", previousIconHtml), PREVIOUS_HTML_ID);
        // Next news icon link
        String nextIconUrl = getIconURL("go-next");
        String nextIconHtml = getImageHtml("next", nextIconUrl);
        String nextLinkHtml = getSpanHtml(getLinkHtml("next", "next", nextIconHtml), NEXT_HTML_ID);
        newsHtmlBuilder.append(getDivHtml(String.format("%s %s", previousLinkHtml, nextLinkHtml), NAVIGATION_HTML_ID));

        // Add news title & date
        String newsTitleHtml = getSpanHtml(news.getTitle(), NEWS_TITLE_HTML_ID);
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("EEE dd MMM yyyy HH:mm");
        String dateStr = simpleDateFormat.format(news.getDate());
        String newsDateHtml = getSpanHtml(dateStr, NEWS_DATE_HTML_ID);
        newsHtmlBuilder.append(getDivHtml(String.format("%s - %s", newsTitleHtml, newsDateHtml)));

        newsHtmlBuilder.append(getDivHtml(news.getDescription(), NEWS_DESCRIPTION_HTML_ID));

        // Add news link if exist
        if (news.getLink() != null) {
            newsHtmlBuilder.append(getDivHtml(getLinkHtml(news.getLink(), news.getLink()), NEWS_LINK_HTML_ID));
        }

        display(getHTML(newsHtmlBuilder));
    }

    private String getLinkHtml(String name, String url, String description) {
        return String.format("<a name='%s' href='%s'>%s</a>", name, url, description);
    }

    private String getLinkHtml(String url, String description) {
        return String.format("<a href='%s'>%s</a>", url, description);
    }

    private String getImageHtml(String name, String url) {
        // Cannot setup margin and border with CSS, bugs of HTMLEditorKit ?
        return String.format("<img name='%s' src='%s' hspace=2 border='0'/>", name, url);
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
        display(getHTML(errMsgHtmlBuilder));
    }

    private String getHTML(StringBuilder htmlBuilder) {
        htmlBuilder.insert(0, "<html><body>");
        htmlBuilder.append("</body></html>");
        return htmlBuilder.toString();
    }

    private void display(String htmlContent) {
        editorPane.setText(htmlContent);
        try	{
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    scrollPane.getHorizontalScrollBar().setValue(0);
                    scrollPane.getVerticalScrollBar().setValue(0);
                    repaint();
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
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
                e.printStackTrace();
                return null;
            }
        } else {
            return null;
        }
    }

    private String getIconURL(String iconName) {
        String iconFilePath = FindIconHelper.findIcon(iconName);
        File iconFile = new File(iconFilePath);
        if (iconFile.exists()) {
            try {
                return iconFile.toURI().toURL().toString();
            } catch (MalformedURLException e) {
                return null;
            }
        } else {
            return null;
        }
    }
}
