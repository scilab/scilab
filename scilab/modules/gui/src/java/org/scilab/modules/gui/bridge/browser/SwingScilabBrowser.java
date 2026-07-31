/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.browser;

import org.cef.CefClient;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefMessageRouter;
import org.cef.browser.CefMessageRouter.CefMessageRouterConfig;
import org.cef.handler.CefMessageRouterHandlerAdapter;
import org.cef.handler.CefContextMenuHandlerAdapter;
import org.cef.handler.CefKeyboardHandlerAdapter;
import org.cef.handler.CefLifeSpanHandlerAdapter;
import org.cef.handler.CefLoadHandlerAdapter;
import org.cef.network.CefCookie;
import org.cef.network.CefCookieManager;
import org.cef.network.CefRequest.TransitionType;
import org.cef.browser.CefFrame;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefCookieVisitor;
import org.cef.callback.CefMenuModel;
import org.cef.callback.CefQueryCallback;
import org.cef.misc.BoolRef;
import org.cef.misc.EventFlags;

import com.google.gson.Gson;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.net.URI;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.UUID;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JPanel;
import javax.swing.BorderFactory;

import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.widget.Widget;
import org.scilab.modules.gui.SwingViewWidget;
import org.scilab.modules.gui.utils.PositionConverter;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.graphic_objects.graphicModel.GraphicModel;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.ScilabCefPanel;
import org.scilab.modules.commons.utils.ScilabBrowser;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_DEBUG__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_DATA__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;

public class SwingScilabBrowser extends JPanel implements SwingViewObject, Widget {

    private String callback;
    private Integer callbackType;
    private Integer uid;
    private ScilabCefPanel cefPanel;
    private CefBrowser browser_;
    private Boolean debug = false;
    private String url = "";
    private String helpers;

    public SwingScilabBrowser() {
        cefPanel = new ScilabCefPanel(url);
        browser_ = cefPanel.getBrowser();

        cefPanel.getClient().addMessageRouter(CefMessageRouter.create(new CefMessageRouterConfig("Scilab", "ScilabAbort"), new CefMessageRouterHandlerAdapter() {
            public boolean onQuery(CefBrowser browser, CefFrame frame, long queryId, String request, boolean persistent, CefQueryCallback callback) {
                if (browser_ == browser) {
                    callback.success(String.valueOf(queryId));
                    return ExecuteCallBack(request, queryId) == 0;
                }

                return false;
            }
        }));

        // Through the panel, not the client: CefClient.addLifeSpanHandler is a
        // no-op once one is registered, and the panel registers its own
        cefPanel.addLifeSpanHandler(new CefLifeSpanHandlerAdapter() {
            @Override
            public boolean onBeforePopup(CefBrowser browser, CefFrame frame, String target_url, String target_frame_name) {
                // Open external links in the user's default system browser
                if (browser == browser_ && target_url != null && !target_url.isEmpty()) {
                    try {
                        Desktop.getDesktop().browse(new URI(target_url));
                    } catch (Exception e) {
                        // ignore
                    }
                    return true; // cancel the popup in JCEF
                }
                return false;
            }

            @Override
            public void onAfterCreated(CefBrowser browser) {
                if (browser == browser_ && url != "") {
                    browser.loadURL(url);
                    url = "";
                }
            }
        });

        cefPanel.getClient().addLoadHandler(new CefLoadHandlerAdapter() {
            @Override
            public void onLoadStart(CefBrowser browser, CefFrame frame, TransitionType transitionType) {
                if (browser == browser_) {
                    browser.executeJavaScript(helpers, "localhost", 1);
                }
            }

            @Override
            public void onLoadEnd(CefBrowser browser, CefFrame frame, int httpStatusCode) {
                if (browser == browser_) {
                    updateCookies();
                    String[] text = new String[1];
                    text[0] = browser.getURL();

                    GraphicModel.getModel().setProperty(uid, __GO_UI_STRING__, text);
                    ExecuteCallBack("\"loaded\"", -1);
                }
            }

            @Override
            public void onLoadError(CefBrowser browser, CefFrame frame, ErrorCode errorCode, String errorText, String failedUrl) {
                if (browser == browser_) {
                    try {
                        Path src = Paths.get(ScilabConstants.SCI.getAbsolutePath() + "/modules/gui/etc/error.html");
                        Path dst = Paths.get(ScilabConstants.TMPDIR.getAbsolutePath() + "/" + UUID.randomUUID().toString() + ".html");

                        //patch error file with specific error
                        String contenu = new String(Files.readAllBytes(src), StandardCharsets.UTF_8);
                        contenu = contenu.replace("<!--TAG ERROR-->", errorText);
                        Files.write(dst, contenu.getBytes(StandardCharsets.UTF_8));

                        //copy puffin file in tmpdir to display image
                        Path imgSrc = Paths.get(ScilabConstants.SCI.getAbsolutePath() + "/modules/gui/images/icons/256x256/apps/puffin.png");
                        Path imgDst = Paths.get(ScilabConstants.TMPDIR.getAbsolutePath() + "/puffin.png");
                        Files.copy(imgSrc, imgDst, StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.COPY_ATTRIBUTES);

                        browser_.loadURL("file:///" + dst);
                    } catch (IOException e) {
                    }

                }
            }
        });

        try {
            helpers = new String(Files.readAllBytes(Paths.get(ScilabConstants.SCI.getCanonicalPath(), "/modules/gui/etc/ScilabBrowser.js")));
        } catch (IOException e) {
            e.printStackTrace();
        }

        invalidate();

        setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));
        setLayout(new BorderLayout(0, 0));
        add(cefPanel, BorderLayout.CENTER);

        validate();
    }

    @Override
    public void addToolBar(ToolBar toolBarToAdd) {
        throw new UnsupportedOperationException("Unimplemented method 'addToolBar'");
    }

    @Override
    public void addMenuBar(MenuBar menuBarToAdd) {
        throw new UnsupportedOperationException("Unimplemented method 'addMenuBar'");
    }

    @Override
    public void addInfoBar(TextBox infoBarToAdd) {
        throw new UnsupportedOperationException("Unimplemented method 'addInfoBar'");
    }

    @Override
    public Size getDims() {
        return new Size(super.getSize().width, super.getSize().height);
    }

    @Override
    public void setDims(Size newSize) {
        setSize(newSize.getWidth(), newSize.getHeight());
    }

    @Override
    public Position getPosition() {
        return PositionConverter.javaToScilab(getLocation(), getSize(), getParent());
    }

    @Override
    public void setPosition(Position newPosition) {
        Position javaPosition = PositionConverter.scilabToJava(newPosition, getDims(), getParent());
        setLocation(javaPosition.getX(), javaPosition.getY());
        //browser_.getUIComponent().setLocation(0, 0);
    }

    @Override
    public void draw() {
        throw new UnsupportedOperationException("Unimplemented method 'draw'");
    }

    @Override
    public MenuBar getMenuBar() {
        throw new UnsupportedOperationException("Unimplemented method 'getMenuBar'");
    }

    @Override
    public ToolBar getToolBar() {
        throw new UnsupportedOperationException("Unimplemented method 'getToolBar'");
    }

    @Override
    public TextBox getInfoBar() {
        throw new UnsupportedOperationException("Unimplemented method 'getInfoBar'");
    }

    @Override
    public void resetBackground() {
        throw new UnsupportedOperationException("Unimplemented method 'resetBackground'");
    }

    @Override
    public void resetForeground() {
        throw new UnsupportedOperationException("Unimplemented method 'resetForeground'");
    }

    @Override
    public void setText(String text) {
        throw new UnsupportedOperationException("Unimplemented method 'setText'");
    }

    @Override
    public void setEmptyText() {
        throw new UnsupportedOperationException("Unimplemented method 'setEmptyText'");
    }

    @Override
    public String getText() {
        throw new UnsupportedOperationException("Unimplemented method 'getText'");
    }

    @Override
    public void setCallback(CommonCallBack callback) {
    }

    @Override
    public void setHorizontalAlignment(String alignment) {
        throw new UnsupportedOperationException("Unimplemented method 'setHorizontalAlignment'");
    }

    @Override
    public void setVerticalAlignment(String alignment) {
        throw new UnsupportedOperationException("Unimplemented method 'setVerticalAlignment'");
    }

    @Override
    public void setRelief(String reliefType) {
        throw new UnsupportedOperationException("Unimplemented method 'setRelief'");
    }

    @Override
    public void destroy() {
        // will call removeNotify() on ScilabCefPanel
        ScilabSwingUtilities.removeFromParent(this);
    }

    @Override
    public void setId(Integer id) {
        uid = id;
    }

    @Override
    public Integer getId() {
        return uid;
    }

    @Override
    public void update(int property, Object value) {

        switch (property) {
            case __GO_UI_STRING__:
                String[] strings = (String[])value;
                if (strings.length == 0 || strings[0].isEmpty()) {
                    url = "about:blank";
                } else {
                    url = strings[0];
                    //check paths
                    if (url.contains("://") == false) {
                        Path p = Paths.get(url);
                        if (p.isAbsolute() == false) {
                            url = CommonFileUtils.getCWD() + "/" + url;
                        }
                        url = "file:///" + url;
                    } else {
                        if (url.startsWith("file://")) {
                            Pattern pattern = Pattern.compile("(file:/+)(.*)");
                            Matcher matcher = pattern.matcher(url);
                            if (matcher.find()) {
                                String head = matcher.group(1);
                                String path = matcher.group(2);
                                Path p  = Paths.get(path);
                                if (p.isAbsolute() == false) {
                                    url = head + CommonFileUtils.getCWD() + "/" + path;
                                }
                            }
                        }
                    }
                }

                browser_.loadURL(url);
                break;
            case __GO_UI_DATA__:
                String data = (String)value;
                if (data.equals("")) return; //ignore empty data (used to clean previous value)

                // Escape for JS double-quoted string literal:
                // backslashes first, then double quotes, then newlines/tabs
                data = data.replace("\\", "\\\\");
                data = data.replace("\"", "\\\"");
                data = data.replace("\n", "\\n");
                data = data.replace("\r", "\\r");
                data = data.replace("\t", "\\t");
                String code = String.format("fromScilabInternal(\"%s\")", data);
                browser_.executeJavaScript(code, "localhost", 1);
                break;
            case __GO_CALLBACK__:
                callback = (String)value;
                break;
            case __GO_CALLBACKTYPE__:
                callbackType = (Integer)value;
                break;
            case __GO_UI_DEBUG__:
                debug = (Boolean)value;
                cefPanel.setDebug(debug);
                break;
            default : {
                SwingViewWidget.update(this, property, value);
                break;
            }
        }
    }

    public void openDebug() {
        cefPanel.openDevTools();
    }

    private int ExecuteCallBack(String msg, long queryId) {
        if (callback != null && callback.equals("") == false) {
            String data = msg.replace("\"", "\"\"");
            data = data.replace("\'", "\'\'");
            // The browser fires events asynchronously. If the uicontrol was
            // destroyed before the queued command runs, the callback target
            // is no longer valid — skip invoking the user callback.
            String str = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
                    + "gcbo = getcallbackobject(" + uid + ");"
                    + "if ~isempty(gcbo) && is_handle_valid(gcbo) then "
                    + "clear %cb;%cb = #(data) -> (u = gcbo;u.data = struct(\"scilabcallbackID\", " + queryId + ", \"data\", data););"
                    + callback + "(fromJSON(\"" + data + "\"), %cb);"
                    + "end;"
                    + "if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;"
                    + "clear %cb;";

            switch (callbackType) {
                case CallBack.SCILAB_INSTRUCTION:
                case CallBack.SCILAB_FUNCTION:
                    return InterpreterManagement.putCommandInScilabQueue(str);
                case CallBack.SCILAB_NOT_INTERRUPTIBLE_INSTRUCTION:
                case CallBack.SCILAB_NOT_INTERRUPTIBLE_FUNCTION:
                    return InterpreterManagement.requestScilabExec(str);
            }
        }

        return 0;
    }

    private String cookies = "";
    public String getCookies() {
        return cookies;
    }

    public void updateCookies() {
        CefCookieManager.getGlobalManager().visitAllCookies(new CefCookieVisitor() {
            List<CefCookie> l = new ArrayList<CefCookie>();

            @Override
            public boolean visit(CefCookie cookie, int count, int total, BoolRef delete) {
                if (cookie.hasExpires == false) {
                    cookie = new CefCookie(cookie.name, cookie.value, cookie.domain, cookie.path, cookie.secure, cookie.httponly, cookie.creation, cookie.lastAccess, cookie.hasExpires, new Date(0));
                }

                l.add(cookie);
                if (count == total - 1) {
                    Gson gson = new Gson();
                    cookies = gson.toJson(l);
                }

                return true;
            }
        });
    }
}
