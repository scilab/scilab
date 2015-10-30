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

import java.net.URL;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

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

    public boolean isOK() {
        return rssURL != null;
    }

    public List<News> fetchNews() throws Exception {
        List<News> newsList = new ArrayList<News>();
        DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
        Document doc = builder.parse(rssURL.openStream());
        NodeList items = doc.getElementsByTagName("item");

        SimpleDateFormat formatter = new SimpleDateFormat("EEE, dd MMM yyyy HH:mm:ss z", Locale.US);

        for (int i = 0; i < items.getLength(); i++) {
            Element item = (Element) items.item(i);
            String title = getItemValue(item, "title");
            String dateStr = getItemValue(item, "pubDate");
            Date date = formatter.parse(dateStr);
            String description = getItemValue(item, "description");
            String link = getItemValue(item, "link");
            newsList.add(new News(title, date, description, link));
        }
        return newsList;
    }

    private String getItemValue(Element item, String nodeName) {
        NodeList nodeList = item.getElementsByTagName(nodeName);
        if (nodeList.getLength() > 0) {
            return nodeList.item(0).getFirstChild().getNodeValue();
        } else {
            return null;
        }
    }
}
