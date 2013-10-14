/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.callback;

import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.gui.uiwidget.UIComponent;

public class UIURLCallback extends UICallback {

    public UIURLCallback(UIComponent uicomp, String code) {
        super(uicomp, code);
    }

    public UIURLCallback(String code) {
        super(code);
    }

    public void exec(final Object ... args) {
        WebBrowser.openUrl(code);
    }

    public void exec(final UIComponent comp, final Object ... args) {
        WebBrowser.openUrl(code);
    }
}
