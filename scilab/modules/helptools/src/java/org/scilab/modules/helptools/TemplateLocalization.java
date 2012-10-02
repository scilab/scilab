/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools;

import java.util.HashMap;
import java.util.Map;

/**
 * Class to handle a template localization
 * @author Calixte DENIZET
 */
public final class TemplateLocalization {

    private static final Map<String, Map<String, String>> map = new HashMap<String, Map<String, String>>();

    static {
        Map<String, String> report_an_issue = new HashMap<String, String>();
        report_an_issue.put("fr_FR", "Signaler un problème");
        report_an_issue.put("pt_BR", "Reportero a bugo");
        report_an_issue.put("es_ES", "Raportare el bugo");
        report_an_issue.put("ja_JP", "問題を報告");
        report_an_issue.put("ru_RU", "Сообщить об ошибке");

        map.put("Report an issue", report_an_issue);
    }

    public static final String getLocalized(final String language, final String str) {
        if (language.equals("en_US")) {
            return str;
        }

        Map<String, String> m = map.get(str);
        if (m == null) {
            return str;
        }

        String s = m.get(language);
        if (s == null) {
            return str;
        }

        return s;
    }
}

