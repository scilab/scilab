<?xml version="1.0" encoding="UTF-8"?>
<!--
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 * XSLT stylesheet for converting DocBook 5 refentry pages to HTML5
 * for the JCEF-based Scilab help browser.
 -->
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:db="http://docbook.org/ns/docbook"
    xmlns:xlink="http://www.w3.org/1999/xlink"
    xmlns:mml="http://www.w3.org/1998/Math/MathML"
    xmlns:svg="http://www.w3.org/2000/svg"
    xmlns:scilab="http://www.scilab.org"
    exclude-result-prefixes="db xlink svg">

    <xsl:output method="html" encoding="UTF-8" indent="no"
        doctype-system="about:legacy-compat"/>

    <!-- Web shell directory (modules/helptools/src/web), holding style.css,
         the Scilab grammar/theme for highlight.js (scilab.min.js/css),
         scilab-init.js, doczoom.js and docsearch.js. Set by the Java browser
         to an absolute file:// URL. -->
    <xsl:param name="base-path" select="'.'"/>
    <!-- Source XML file directory, used to resolve relative image paths. -->
    <xsl:param name="xml-dir" select="'.'"/>
    <!-- Third-party JS directory (thirdparty/web), holding highlight.js core
         (highlightjs/highlight.min.js) and MathJax. -->
    <xsl:param name="js-dir" select="'.'"/>

    <!-- ================================================================== -->
    <!-- ROOT: refentry                                                      -->
    <!-- ================================================================== -->
    <xsl:template match="db:refentry">
        <html lang="{@xml:lang}">
        <head>
            <meta charset="UTF-8"/>
            <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
            <title><xsl:value-of select="db:refnamediv/db:refname"/> — Scilab Help</title>
            <link rel="stylesheet" href="{$base-path}/style.css"/>
            <link rel="stylesheet" href="{$base-path}/scilab.min.css"/>
            <script src="{$js-dir}/highlightjs/highlight.min.js">/* highlight.js */</script>
            <script src="{$base-path}/scilab.min.js">/* Scilab grammar */</script>
            <script>
                window.MathJax = {
                    tex: { inlineMath: [['\\(','\\)']] },
                    options: { skipHtmlTags: ['pre', 'code', 'script'] }
                };
            </script>
            <script src="{$js-dir}/mathjax/tex-mml-svg.js" defer="defer">/* MathJax */</script>
            <style>
                /* Floating buttons (section menu, back to top): one look, the
                   one of the page, following its light and dark colors */
                .scroll-top, .toc-menu-btn { position: fixed; right: 20px; width: 32px; height: 32px; padding: 0; border-radius: var(--radius, 6px); background: var(--color-code-bg, #f8fafc); border: 1px solid var(--color-code-border, #e2e8f0); color: #64748b; cursor: pointer; font-size: 16px; display: flex; align-items: center; justify-content: center; z-index: 10; transition: background-color 0.15s, color 0.15s, border-color 0.15s; }
                .scroll-top:hover, .toc-menu-btn:hover { background: #eef2ff; color: #2563eb; border-color: #2563eb; }
                .scroll-top:focus-visible, .toc-menu-btn:focus-visible { outline: 2px solid var(--color-link, #2563eb); outline-offset: 2px; }
                .toc-menu-btn { top: 12px; }
                /* Back to top: fades in once the page is scrolled (script below),
                   out of the tab order while hidden */
                .scroll-top { bottom: 20px; opacity: 0; visibility: hidden; transform: translateY(6px); transition: background-color 0.15s, color 0.15s, border-color 0.15s, opacity 0.2s, transform 0.2s, visibility 0s linear 0.2s; }
                .scroll-top.visible { opacity: 1; visibility: visible; transform: none; transition: background-color 0.15s, color 0.15s, border-color 0.15s, opacity 0.2s, transform 0.2s, visibility 0s; }
                /* Chevron drawn in CSS: crisp at any zoom, colored like the icon */
                .scroll-top::before { content: ""; width: 8px; height: 8px; border: solid currentColor; border-width: 2px 0 0 2px; border-radius: 1px; transform: translateY(2px) rotate(45deg); }
                .toc-menu { position: fixed; top: 48px; right: 20px; background: #fff; border: 1px solid #e2e8f0; border-radius: 8px; box-shadow: 0 4px 16px rgba(0,0,0,0.1); padding: 6px 0; z-index: 10; display: none; min-width: 180px; max-width: min(360px, calc(100vw - 40px)); max-height: calc(100vh - 68px); overflow-y: auto; }
                .toc-menu.visible { display: block; }
                .toc-menu a { display: block; padding: 6px 16px; color: #334155; text-decoration: none; font-size: 13px; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
                /* Anchors: indented under their section, long labels (C prototypes) cut */
                .toc-menu a.toc-anchor { padding: 4px 16px 4px 30px; font-size: 12px; }
                .toc-menu a:hover { background: #eef2ff; color: #2563eb; }
                @media (prefers-color-scheme: dark) {
                    .scroll-top, .toc-menu-btn { background: #1e293b; border-color: #334155; color: #94a3b8; }
                    .scroll-top:hover, .toc-menu-btn:hover { background: #1e3a5f; color: #60a5fa; border-color: #60a5fa; }
                    .toc-menu { background: #1e293b; border-color: #334155; }
                    .toc-menu a { color: #cbd5e1; }
                    .toc-menu a:hover { background: #1e3a5f; color: #60a5fa; }
                }
            </style>
        </head>
        <body>
            <button class="scroll-top" id="scroll-top" title="Back to top" aria-label="Back to top"></button>
            <button class="toc-menu-btn" id="toc-menu-btn" title="Sections">&#9776;</button>
            <div class="toc-menu" id="toc-menu"></div>
            <article class="refentry" data-id="{@xml:id}">
                <xsl:apply-templates/>
            </article>
            <script src="{$base-path}/scilab-init.js">/* enrich grammar + highlight */</script>
            <script src="{$base-path}/doczoom.js">/* Ctrl+wheel zoom of the document only */</script>
            <script src="{$base-path}/docsearch.js">/* find in page, driven by the shell */</script>
            <script>
                /* Cross-reference navigation: notify parent shell via postMessage */
                document.addEventListener("click", function(e) {
                    var link = e.target.closest("a[data-linkend]");
                    if (link &amp;&amp; window.parent !== window) {
                        e.preventDefault();
                        window.parent.postMessage({type: "navigateById", id: link.dataset.linkend, linkType: link.dataset.linkType || ""}, "*");
                        return;
                    }
                    /* External links: open in system browser */
                    var extLink = e.target.closest("a[href^='http']");
                    if (extLink &amp;&amp; window.parent !== window) {
                        e.preventDefault();
                        window.parent.postMessage({type: "openExternal", url: extLink.href}, "*");
                    }
                });

                /* Example buttons: run in console / open in SciNotes */
                document.addEventListener("click", function(e) {
                    var btn = e.target.closest(".btn-run, .btn-edit");
                    if (!btn || window.parent === window) return;
                    var wrapper = btn.closest(".example-wrapper");
                    if (!wrapper) return;
                    var pre = wrapper.querySelector("pre.programlisting");
                    if (!pre) return;
                    var code = pre.textContent;
                    if (btn.classList.contains("btn-run")) {
                        window.parent.postMessage({type: "execCode", code: code}, "*");
                    } else {
                        window.parent.postMessage({type: "editCode", code: code}, "*");
                    }
                });

                /* Click to open Xcos diagrams */
                document.addEventListener("click", function(e) {
                    var container = e.target.closest(".xcos-image-container[data-xcos-file]");
                    if (container &amp;&amp; window.parent !== window) {
                        window.parent.postMessage({type: "openXcos", file: container.getAttribute("data-xcos-file")}, "*");
                    }
                });

                /* Scroll to top button */
                (function() {
                    var btn = document.getElementById("scroll-top");
                    window.addEventListener("scroll", function() {
                        btn.classList.toggle("visible", window.scrollY > 200);
                    });
                    btn.addEventListener("click", function() {
                        window.scrollTo({top: 0, behavior: "smooth"});
                    });
                })();

                /* Section menu (hamburger) */
                (function() {
                    var menuBtn = document.getElementById("toc-menu-btn");
                    var menu = document.getElementById("toc-menu");
                    /* Section titles and anchors, in document order */
                    var marks = document.querySelectorAll("article.refentry > section > h2, article.refentry span.anchor");
                    if (marks.length === 0) { menuBtn.style.display = "none"; return; }

                    marks.forEach(function(mark) {
                        var target, label;
                        if (mark.tagName === "H2") {
                            target = mark.parentNode;
                            label = mark.textContent;
                        } else {
                            /* An anchor has no text: it labels the paragraph holding it,
                               or the block that follows it when it stands alone */
                            target = mark.closest("p, li, dt, dd, td");
                            if (!target) {
                                target = mark.nextElementSibling;
                                while (target &amp;&amp; target.classList.contains("anchor")) target = target.nextElementSibling;
                            }
                            label = target ? target.textContent : "";
                        }
                        label = label.replace(/\s+/g, " ").trim() || mark.id;
                        var a = document.createElement("a");
                        a.href = "#";
                        a.textContent = label;
                        a.title = label;
                        if (mark.tagName !== "H2") a.className = "toc-anchor";
                        a.addEventListener("click", function(e) {
                            e.preventDefault();
                            (target || mark).scrollIntoView({behavior: "smooth"});
                            menu.classList.remove("visible");
                        });
                        menu.appendChild(a);
                    });

                    // Also add "Top" as first entry
                    var topLink = document.createElement("a");
                    topLink.href = "#";
                    topLink.textContent = "Top";
                    topLink.style.fontWeight = "600";
                    topLink.addEventListener("click", function(e) {
                        e.preventDefault();
                        window.scrollTo({top: 0, behavior: "smooth"});
                        menu.classList.remove("visible");
                    });
                    menu.insertBefore(topLink, menu.firstChild);

                    menuBtn.addEventListener("click", function() {
                        menu.classList.toggle("visible");
                    });

                    // Close menu on click outside
                    document.addEventListener("click", function(e) {
                        if (!menuBtn.contains(e.target) &amp;&amp; !menu.contains(e.target)) {
                            menu.classList.remove("visible");
                        }
                    });
                })();
            </script>
        </body>
        </html>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- HEADER: refnamediv                                                  -->
    <!-- ================================================================== -->
    <xsl:template match="db:refnamediv">
        <header class="refnamediv">
            <h1>
                <span class="refname"><xsl:value-of select="db:refname"/></span>
                <xsl:if test="db:refpurpose">
                    <span class="refpurpose-sep"> — </span>
                    <span class="refpurpose"><xsl:value-of select="db:refpurpose"/></span>
                </xsl:if>
            </h1>
        </header>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- SYNTAX: refsynopsisdiv                                              -->
    <!-- ================================================================== -->
    <xsl:template match="db:refsynopsisdiv">
        <section class="refsynopsisdiv">
            <xsl:apply-templates select="db:title"/>
            <xsl:apply-templates select="*[not(self::db:title)]"/>
        </section>
    </xsl:template>

    <!-- Synopsis content is plain text: strip the XML source indentation
         (leading/trailing whitespace on each line) and surrounding blank
         lines, so the JS-side dedent is not required for correct layout. -->
    <xsl:template match="db:synopsis"><pre class="synopsis"><xsl:call-template name="dedent-text"><xsl:with-param name="text" select="string(.)"/></xsl:call-template></pre>
    </xsl:template>

    <!-- Trim leading/trailing whitespace (spaces, tabs, newlines) -->
    <xsl:template name="trim">
        <xsl:param name="s"/>
        <xsl:choose>
            <xsl:when test="string-length($s) &gt; 0 and contains(' &#9;&#10;&#13;', substring($s, 1, 1))">
                <xsl:call-template name="trim">
                    <xsl:with-param name="s" select="substring($s, 2)"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="string-length($s) &gt; 0 and contains(' &#9;&#10;&#13;', substring($s, string-length($s), 1))">
                <xsl:call-template name="trim">
                    <xsl:with-param name="s" select="substring($s, 1, string-length($s) - 1)"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise><xsl:value-of select="$s"/></xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Trim each line of a multi-line string, keeping internal blank lines -->
    <xsl:template name="dedent-lines">
        <xsl:param name="text"/>
        <xsl:choose>
            <xsl:when test="contains($text, '&#10;')">
                <xsl:call-template name="trim">
                    <xsl:with-param name="s" select="substring-before($text, '&#10;')"/>
                </xsl:call-template>
                <xsl:text>&#10;</xsl:text>
                <xsl:call-template name="dedent-lines">
                    <xsl:with-param name="text" select="substring-after($text, '&#10;')"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:call-template name="trim">
                    <xsl:with-param name="s" select="$text"/>
                </xsl:call-template>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Drop surrounding blank lines, then left/right-trim every line -->
    <xsl:template name="dedent-text">
        <xsl:param name="text"/>
        <xsl:variable name="trimmed">
            <xsl:call-template name="trim">
                <xsl:with-param name="s" select="$text"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:call-template name="dedent-lines">
            <xsl:with-param name="text" select="string($trimmed)"/>
        </xsl:call-template>
    </xsl:template>

    <!-- Trim trailing whitespace (spaces, tabs, newlines) -->
    <xsl:template name="trim-right">
        <xsl:param name="s"/>
        <xsl:choose>
            <xsl:when test="string-length($s) &gt; 0 and contains(' &#9;&#10;&#13;', substring($s, string-length($s), 1))">
                <xsl:call-template name="trim-right">
                    <xsl:with-param name="s" select="substring($s, 1, string-length($s) - 1)"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise><xsl:value-of select="$s"/></xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Drop leading blank lines, keeping the first non-blank line intact -->
    <xsl:template name="drop-leading-blank-lines">
        <xsl:param name="text"/>
        <xsl:choose>
            <xsl:when test="contains($text, '&#10;') and normalize-space(substring-before($text, '&#10;')) = ''">
                <xsl:call-template name="drop-leading-blank-lines">
                    <xsl:with-param name="text" select="substring-after($text, '&#10;')"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise><xsl:value-of select="$text"/></xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Count of leading spaces/tabs of a single line -->
    <xsl:template name="indent-of">
        <xsl:param name="s"/>
        <xsl:param name="n" select="0"/>
        <xsl:choose>
            <xsl:when test="string-length($s) &gt; 0 and contains(' &#9;', substring($s, 1, 1))">
                <xsl:call-template name="indent-of">
                    <xsl:with-param name="s" select="substring($s, 2)"/>
                    <xsl:with-param name="n" select="$n + 1"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise><xsl:value-of select="$n"/></xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Minimum indentation over the non-blank lines of a multi-line string -->
    <xsl:template name="min-indent">
        <xsl:param name="text"/>
        <xsl:param name="min" select="99999"/>
        <xsl:variable name="line">
            <xsl:choose>
                <xsl:when test="contains($text, '&#10;')"><xsl:value-of select="substring-before($text, '&#10;')"/></xsl:when>
                <xsl:otherwise><xsl:value-of select="$text"/></xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:variable name="newmin">
            <xsl:choose>
                <xsl:when test="normalize-space($line) = ''"><xsl:value-of select="$min"/></xsl:when>
                <xsl:otherwise>
                    <xsl:variable name="ind">
                        <xsl:call-template name="indent-of">
                            <xsl:with-param name="s" select="string($line)"/>
                        </xsl:call-template>
                    </xsl:variable>
                    <xsl:choose>
                        <xsl:when test="number($ind) &lt; $min"><xsl:value-of select="$ind"/></xsl:when>
                        <xsl:otherwise><xsl:value-of select="$min"/></xsl:otherwise>
                    </xsl:choose>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:choose>
            <xsl:when test="contains($text, '&#10;')">
                <xsl:call-template name="min-indent">
                    <xsl:with-param name="text" select="substring-after($text, '&#10;')"/>
                    <xsl:with-param name="min" select="number($newmin)"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise><xsl:value-of select="$newmin"/></xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Remove the first $indent characters of every line, right-trim each -->
    <xsl:template name="strip-indent-lines">
        <xsl:param name="text"/>
        <xsl:param name="indent"/>
        <xsl:variable name="line">
            <xsl:choose>
                <xsl:when test="contains($text, '&#10;')"><xsl:value-of select="substring-before($text, '&#10;')"/></xsl:when>
                <xsl:otherwise><xsl:value-of select="$text"/></xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:call-template name="trim-right">
            <xsl:with-param name="s" select="substring($line, $indent + 1)"/>
        </xsl:call-template>
        <xsl:if test="contains($text, '&#10;')">
            <xsl:text>&#10;</xsl:text>
            <xsl:call-template name="strip-indent-lines">
                <xsl:with-param name="text" select="substring-after($text, '&#10;')"/>
                <xsl:with-param name="indent" select="$indent"/>
            </xsl:call-template>
        </xsl:if>
    </xsl:template>

    <!-- Drop surrounding blank lines, then remove the common indentation
         (relative indentation is preserved) -->
    <xsl:template name="dedent-block">
        <xsl:param name="text"/>
        <xsl:variable name="lead">
            <xsl:call-template name="drop-leading-blank-lines">
                <xsl:with-param name="text" select="$text"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="body">
            <xsl:call-template name="trim-right">
                <xsl:with-param name="s" select="string($lead)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="indent">
            <xsl:call-template name="min-indent">
                <xsl:with-param name="text" select="string($body)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:call-template name="strip-indent-lines">
            <xsl:with-param name="text" select="string($body)"/>
            <xsl:with-param name="indent" select="number($indent)"/>
        </xsl:call-template>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- SECTIONS: refsection, refsect2, refsect3                            -->
    <!-- ================================================================== -->
    <xsl:template match="db:refsection">
        <section class="refsection">
            <xsl:if test="@role">
                <xsl:attribute name="data-role"><xsl:value-of select="@role"/></xsl:attribute>
            </xsl:if>
            <xsl:apply-templates/>
        </section>
    </xsl:template>

    <xsl:template match="db:refsect2">
        <section class="refsect2">
            <xsl:apply-templates/>
        </section>
    </xsl:template>

    <xsl:template match="db:refsect3">
        <section class="refsect3">
            <xsl:apply-templates/>
        </section>
    </xsl:template>

    <!-- Section titles: h2 for refsection, h3 for refsect2, h4 for refsect3 -->
    <xsl:template match="db:refsection/db:title | db:refsynopsisdiv/db:title">
        <h2><xsl:apply-templates/></h2>
    </xsl:template>

    <xsl:template match="db:refsect2/db:title">
        <h3><xsl:apply-templates/></h3>
    </xsl:template>

    <xsl:template match="db:refsect3/db:title">
        <h4><xsl:apply-templates/></h4>
    </xsl:template>

    <!-- Generic title fallback -->
    <xsl:template match="db:title">
        <h3><xsl:apply-templates/></h3>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- PARAGRAPHS                                                          -->
    <!-- ================================================================== -->

    <!-- para containing block-level children (warning, note, tip, programlisting, etc.)
         must NOT be a <p> (invalid HTML). Use <div> instead. -->
    <xsl:template match="db:para[db:warning or db:note or db:tip or db:important or db:caution or db:informaltable or db:table or db:orderedlist or db:itemizedlist or db:simplelist[not(@type='inline')]]">
        <div class="para"><xsl:apply-templates/></div>
    </xsl:template>

    <xsl:template match="db:para">
        <p><xsl:apply-templates/></p>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- CODE BLOCKS: programlisting, screen                                 -->
    <!-- ================================================================== -->
    <!-- Code content is plain text: strip the XML source indentation (the
         common minimum, keeping relative indentation) and surrounding blank
         lines, so the JS-side dedent is not required for correct layout. -->
    <xsl:template match="db:programlisting[@role='example' or not(@role)]">
        <div class="example-wrapper"><div class="example-buttons"><button class="btn-run" title="Execute in console">&#9654;</button><button class="btn-edit" title="Open in SciNotes">&#9998;</button></div><pre class="programlisting language-scilab" data-role="example"><xsl:call-template name="dedent-block"><xsl:with-param name="text" select="string(.)"/></xsl:call-template></pre></div>
    </xsl:template>

    <xsl:template match="db:programlisting[@role='no-scilab-exec']"><pre class="programlisting language-scilab"><xsl:if test="@role"><xsl:attribute name="data-role"><xsl:value-of select="@role"/></xsl:attribute></xsl:if><xsl:call-template name="dedent-block"><xsl:with-param name="text" select="string(.)"/></xsl:call-template></pre>
    </xsl:template>

    <!-- screen = console output of an example: plain text, no syntax
         highlighting (no language-scilab class, so highlight.js skips it) -->
    <xsl:template match="db:screen"><pre class="screen"><xsl:call-template name="dedent-block"><xsl:with-param name="text" select="string(.)"/></xsl:call-template></pre>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- INLINE ELEMENTS                                                     -->
    <!-- ================================================================== -->
    <xsl:template match="db:literal">
        <code class="literal"><xsl:apply-templates/></code>
    </xsl:template>

    <xsl:template match="db:varname">
        <code class="varname"><xsl:apply-templates/></code>
    </xsl:template>

    <xsl:template match="db:command">
        <code class="command"><xsl:apply-templates/></code>
    </xsl:template>

    <xsl:template match="db:function">
        <code class="function"><xsl:apply-templates/></code>
    </xsl:template>

    <xsl:template match="db:code">
        <code><xsl:apply-templates/></code>
    </xsl:template>

    <xsl:template match="db:constant">
        <code class="constant"><xsl:apply-templates/></code>
    </xsl:template>

    <!-- emphasis -->
    <xsl:template match="db:emphasis">
        <xsl:choose>
            <xsl:when test="@role='bold'">
                <strong><xsl:apply-templates/></strong>
            </xsl:when>
            <xsl:otherwise>
                <em><xsl:apply-templates/></em>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="db:subscript">
        <sub><xsl:apply-templates/></sub>
    </xsl:template>

    <xsl:template match="db:superscript">
        <sup><xsl:apply-templates/></sup>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- LINKS                                                               -->
    <!-- ================================================================== -->
    <!-- Anchor: an empty marker, listed in the section menu of the page -->
    <xsl:template match="db:anchor[@xml:id]">
        <span class="anchor" id="{@xml:id}"></span>
    </xsl:template>

    <xsl:template match="db:link[@linkend]">
        <a href="#{@linkend}" class="link" data-linkend="{@linkend}">
            <xsl:if test="@type">
                <xsl:attribute name="data-link-type"><xsl:value-of select="@type"/></xsl:attribute>
            </xsl:if>
            <xsl:apply-templates/>
        </a>
    </xsl:template>

    <xsl:template match="db:link[@xlink:href]">
        <a href="{@xlink:href}" class="link">
            <xsl:apply-templates/>
        </a>
    </xsl:template>

    <xsl:template match="db:ulink">
        <a href="{@url}" class="ulink" target="_blank" rel="noopener">
            <xsl:apply-templates/>
        </a>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- VARIABLE LISTS (Arguments, Parameters)                              -->
    <!-- ================================================================== -->
    <xsl:template match="db:variablelist">
        <dl class="variablelist">
            <xsl:apply-templates/>
        </dl>
    </xsl:template>

    <xsl:template match="db:varlistentry">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="db:varlistentry/db:term">
        <dt class="term"><xsl:apply-templates/></dt>
    </xsl:template>

    <xsl:template match="db:varlistentry/db:listitem">
        <dd><xsl:apply-templates/></dd>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- ITEMIZED / ORDERED LISTS                                            -->
    <!-- ================================================================== -->
    <xsl:template match="db:itemizedlist">
        <ul class="itemizedlist">
            <xsl:apply-templates/>
        </ul>
    </xsl:template>

    <xsl:template match="db:orderedlist">
        <ol class="orderedlist">
            <xsl:apply-templates/>
        </ol>
    </xsl:template>

    <xsl:template match="db:itemizedlist/db:listitem | db:orderedlist/db:listitem">
        <li><xsl:apply-templates/></li>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- SIMPLE LISTS                                                        -->
    <!-- ================================================================== -->
    <!-- type="inline": comma-separated; any other simplelist is a bulleted list -->
    <xsl:template match="db:simplelist[@type='inline']">
        <span class="simplelist inline-list">
            <xsl:for-each select="db:member">
                <xsl:if test="position() > 1">, </xsl:if>
                <!-- skip the indentation around the member content: it would render as a space before the comma -->
                <xsl:apply-templates select="node()[not(self::text() and normalize-space() = '' and (not(preceding-sibling::node()) or not(following-sibling::node())))]"/>
            </xsl:for-each>
        </span>
    </xsl:template>

    <xsl:template match="db:simplelist">
        <ul class="simplelist">
            <xsl:for-each select="db:member">
                <li><xsl:apply-templates/></li>
            </xsl:for-each>
        </ul>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- TABLES                                                              -->
    <!-- ================================================================== -->
    <xsl:template match="db:table">
        <div class="table-wrapper">
            <xsl:if test="db:title">
                <div class="table-title"><xsl:value-of select="db:title"/></div>
            </xsl:if>
            <table class="table">
                <xsl:apply-templates select="*[not(self::db:title)]"/>
            </table>
        </div>
    </xsl:template>

    <xsl:template match="db:informaltable">
        <table class="informaltable">
            <xsl:apply-templates/>
        </table>
    </xsl:template>

    <xsl:template match="db:tr">
        <tr><xsl:apply-templates/></tr>
    </xsl:template>

    <xsl:template match="db:th">
        <th>
            <xsl:if test="@colspan"><xsl:attribute name="colspan"><xsl:value-of select="@colspan"/></xsl:attribute></xsl:if>
            <xsl:if test="@rowspan"><xsl:attribute name="rowspan"><xsl:value-of select="@rowspan"/></xsl:attribute></xsl:if>
            <xsl:if test="@bgcolor"><xsl:attribute name="bgcolor"><xsl:value-of select="@bgcolor"/></xsl:attribute></xsl:if>
            <xsl:if test="@style"><xsl:attribute name="style"><xsl:value-of select="@style"/></xsl:attribute></xsl:if>
            <xsl:apply-templates/>
        </th>
    </xsl:template>

    <xsl:template match="db:td">
        <td>
            <xsl:if test="@colspan"><xsl:attribute name="colspan"><xsl:value-of select="@colspan"/></xsl:attribute></xsl:if>
            <xsl:if test="@rowspan"><xsl:attribute name="rowspan"><xsl:value-of select="@rowspan"/></xsl:attribute></xsl:if>
            <xsl:if test="@bgcolor"><xsl:attribute name="bgcolor"><xsl:value-of select="@bgcolor"/></xsl:attribute></xsl:if>
            <xsl:if test="@style"><xsl:attribute name="style"><xsl:value-of select="@style"/></xsl:attribute></xsl:if>
            <xsl:apply-templates/>
        </td>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- ADMONITIONS: note, tip, warning, important, caution               -->
    <!-- ================================================================== -->
    <xsl:template match="db:note">
        <div class="admonition note">
            <xsl:if test="db:title">
                <div class="admonition-title"><xsl:value-of select="db:title"/></div>
            </xsl:if>
            <xsl:apply-templates select="node()[not(self::db:title)]"/>
        </div>
    </xsl:template>

    <xsl:template match="db:tip">
        <div class="admonition tip">
            <xsl:if test="db:title">
                <div class="admonition-title"><xsl:value-of select="db:title"/></div>
            </xsl:if>
            <xsl:apply-templates select="node()[not(self::db:title)]"/>
        </div>
    </xsl:template>

    <xsl:template match="db:warning">
        <div class="admonition warning">
            <xsl:if test="db:title">
                <div class="admonition-title"><xsl:value-of select="db:title"/></div>
            </xsl:if>
            <xsl:apply-templates select="node()[not(self::db:title)]"/>
        </div>
    </xsl:template>

    <!-- important and caution: rendered as a warning, with their own class
         for a specific style -->
    <xsl:template match="db:important | db:caution">
        <div class="admonition warning {local-name()}">
            <xsl:if test="db:title">
                <div class="admonition-title"><xsl:value-of select="db:title"/></div>
            </xsl:if>
            <xsl:apply-templates select="node()[not(self::db:title)]"/>
        </div>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- SCILAB:IMAGE - Lazy generation via JS bridge                        -->
    <!-- ================================================================== -->
    <xsl:template match="scilab:image">
        <div class="scilab-image-container">
            <xsl:attribute name="data-scilab-code"><xsl:value-of select="."/></xsl:attribute>
            <div class="scilab-image-placeholder">
                <span class="scilab-image-loading">Loading plot...</span>
            </div>
        </div>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- LATEX - Inline LaTeX rendered by MathJax                            -->
    <!-- ================================================================== -->
    <xsl:template match="db:latex">
        <span class="math-tex">\(<xsl:value-of select="."/>\)</span>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- LATEX - Displayed LaTeX rendered by MathJax                            -->
    <!-- ================================================================== -->
    <xsl:template match="db:latex[@style='display']">
        <span class="math-tex math-tex-display">\[<xsl:value-of select="."/>\]</span>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- MathML - Serialize as text for MathJax (Xalan can't output MathML   -->
    <!-- elements in HTML mode)                                              -->
    <!-- ================================================================== -->
    <xsl:template match="mml:math">
        <span class="math-mml" style="display:none"><xsl:apply-templates select="." mode="mathml-serialize"/></span>
    </xsl:template>

    <xsl:template match="mml:*" mode="mathml-serialize">
        <xsl:value-of select="concat('&lt;', local-name())"/>
        <xsl:for-each select="@*[not(namespace-uri()='http://www.w3.org/1998/Math/MathML')]">
            <xsl:value-of select="concat(' ', name(), '=&quot;', ., '&quot;')"/>
        </xsl:for-each>
        <xsl:for-each select="@mml:*">
            <xsl:value-of select="concat(' ', local-name(), '=&quot;', ., '&quot;')"/>
        </xsl:for-each>
        <xsl:text>&gt;</xsl:text>
        <xsl:apply-templates select="node()" mode="mathml-serialize"/>
        <xsl:value-of select="concat('&lt;/', local-name(), '&gt;')"/>
    </xsl:template>

    <xsl:template match="text()" mode="mathml-serialize">
        <xsl:value-of select="."/>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- MEDIA OBJECTS (images, inline images)                               -->
    <!-- ================================================================== -->
    <xsl:template match="db:mediaobject">
        <div class="mediaobject">
            <xsl:apply-templates/>
        </div>
    </xsl:template>

    <xsl:template match="db:inlinemediaobject">
        <span class="inlinemediaobject">
            <xsl:apply-templates/>
        </span>
    </xsl:template>

    <xsl:template match="db:imageobject">
        <xsl:apply-templates/>
    </xsl:template>

    <!-- imagedata with MathML inside (common pattern in Scilab docs) -->
    <xsl:template match="db:imagedata[mml:math]">
        <xsl:apply-templates select="mml:math"/>
    </xsl:template>

    <!-- imagedata with xcos/zcos/ssp fileref: lazy conversion via Scilab -->
    <xsl:template match="db:imagedata[contains(@fileref,'.zcos') or contains(@fileref,'.xcos') or contains(@fileref,'.ssp')]" priority="1">
        <div class="xcos-image-container" data-xcos-file="{$xml-dir}/{@fileref}">
            <div class="scilab-image-placeholder">
                <span class="scilab-image-loading">Loading diagram...</span>
            </div>
        </div>
    </xsl:template>

    <!-- imagedata with fileref (static images: png, gif, etc.) -->
    <xsl:template match="db:imagedata[@fileref]">
        <img src="{$xml-dir}/{@fileref}">
            <xsl:if test="@width"><xsl:attribute name="width"><xsl:value-of select="@width"/></xsl:attribute></xsl:if>
            <xsl:if test="@depth"><xsl:attribute name="height"><xsl:value-of select="@depth"/></xsl:attribute></xsl:if>
            <xsl:attribute name="alt">image</xsl:attribute>
        </img>
    </xsl:template>

    <!-- imagedata fallback (no fileref, no mathml) -->
    <xsl:template match="db:imagedata">
        <xsl:apply-templates/>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- REVISION HISTORY                                                    -->
    <!-- ================================================================== -->
    <xsl:template match="db:revhistory">
        <table class="revhistory">
            <tr class="title">
                <td colspan="2">History</td>
            </tr>
            <xsl:apply-templates select="db:revision"/>
        </table>
    </xsl:template>

    <xsl:template match="db:revision">
        <tr>
            <td class="revnumber"><xsl:value-of select="db:revnumber"/></td>
            <td class="revdescription">
                <xsl:apply-templates select="db:revremark/node() | db:revdescription/node()"/>
            </td>
        </tr>
    </xsl:template>

    <!-- ================================================================== -->
    <!-- CATCH-ALL: strip unknown wrapper elements, keep text                -->
    <!-- ================================================================== -->
    <xsl:template match="text()">
        <xsl:value-of select="."/>
    </xsl:template>

    <!-- Suppress comments -->
    <xsl:template match="comment()"/>

</xsl:stylesheet>
