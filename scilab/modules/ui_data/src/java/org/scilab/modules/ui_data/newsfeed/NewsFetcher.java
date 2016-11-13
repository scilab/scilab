/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
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

import java.net.URL;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;

import java.util.Date;
import java.text.SimpleDateFormat;
import java.util.Locale;

import java.util.List;
import java.util.ArrayList;

/**
 * Fetches news through a RSS feed
 * The RSS URL is configurable in the settings file
 */
public class NewsFetcher {
    private URL rssURL = null;

    public NewsFetcher() {
    }

    public void readSettings() throws Exception {
        rssURL = null;
        String urlSetting = NewsFeedSettings.getStringSetting("url");
        rssURL = new URL(urlSetting);
    }

    public URL getURL() {
        return rssURL;
    }


    public boolean isOK() {
        return rssURL != null;
    }

    public List<News> fetchNews() throws Exception {
        List<News> newsList = new ArrayList<News>();
        DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
        Document doc = builder.parse(rssURL.openStream());

        NodeList items = doc.getElementsByTagName("item");
        if ((items == null) || (items.getLength() == 0)) {
            throw new Exception("The fetched document has no 'item' element, please check it is a RSS feed.");
        }

        SimpleDateFormat formatter = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss Z", Locale.US);

        for (int i = 0; i < items.getLength(); i++) {
            Element item = (Element) items.item(i);
            String title = getItemValue(item, "title");
            String dateStr = getItemValue(item, "pubDate");
            Date date = formatter.parse(dateStr);
            String description = getItemValue(item, "description");
            String content = getItemValue(item, "content:encoded");
            // media content
            NewsMediaContent mediaContent = null;
            Node mediaContentNode = getItemNode(item, "media:content");
            if (mediaContentNode != null) {
                String url = mediaContentNode.getAttributes().getNamedItem("url").getNodeValue();
                String width = mediaContentNode.getAttributes().getNamedItem("width").getNodeValue();
                String height = mediaContentNode.getAttributes().getNamedItem("height").getNodeValue();
                mediaContent = new NewsMediaContent(url, width, height);
            }
            String link = getItemValue(item, "link");
            newsList.add(new News(title, date, description, content, mediaContent, link));
        }
        return newsList;
    }

    private Node getItemNode(Element item, String nodeName) {
        NodeList nodeList = item.getElementsByTagName(nodeName);
        if (nodeList.getLength() > 0) {
            return nodeList.item(0);
        } else {
            return null;
        }
    }

    private String getItemValue(Element item, String nodeName) {
        Node node = getItemNode(item, nodeName);
        if (node != null) {
            if (node.hasChildNodes()) {
                return node.getFirstChild().getNodeValue();
            }
        }
        return null;
    }

}
