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

import org.scilab.modules.commons.ScilabConstants;

import java.io.File;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;

/**
 * Loads news feed settings like URL or update interval time
 * Settings stored in newsfeed.xml (in directory ui_data/etc or SCIHOME)
 */
public class NewsFeedSettings {

    public static String getStringSetting(String settingName) throws Exception {
        File settingsFile = getSettingsFile();

        DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
        Document doc = builder.parse(settingsFile);
        NodeList newsfeedNodes = doc.getElementsByTagName("newsfeed");
        return getItemValue((Element) (newsfeedNodes.item(0)), settingName);
    }

    public static int getIntSetting(String settingName) throws Exception {
        String strSetting = getStringSetting(settingName);
        return Integer.parseInt(strSetting);
    }

    private static File getSettingsFile() {
        File settingsFile = new File(ScilabConstants.SCIHOME + "/newsfeed.xml");
        if (!settingsFile.exists()) {
            settingsFile = new File(ScilabConstants.SCI + "/modules/ui_data/etc/newsfeed.xml");
        }
        if (settingsFile.exists()) {
            return settingsFile;
        } else {
            return null;
        }
    }

    private static String getItemValue(Element item, String nodeName) {
        NodeList nodeList = item.getElementsByTagName(nodeName);
        if (nodeList.getLength() > 0) {
            Node node = nodeList.item(0);
            if (node.hasChildNodes()) {
                return node.getFirstChild().getNodeValue();
            } else {
                return null;
            }
        } else {
            return null;
        }
    }

}
