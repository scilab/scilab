package org.scilab.modules.xpad;

import java.util.EventListener;

public interface KeywordListener extends EventListener {

    void keywordClicked(KeywordEvent e);
}