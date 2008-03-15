<?xml version="1.0" encoding="US-ASCII"?>
<!--This file was created automatically by xsl2profile-->
<!--from the DocBook XSL stylesheets.-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:d="http://docbook.org/ns/docbook"
xmlns:ng="http://docbook.org/docbook-ng" xmlns:db="http://docbook.org/ns/docbook" xmlns:exsl="http://exslt.org/common" xmlns:exslt="http://exslt.org/common" exslt:dummy="dummy" ng:dummy="dummy" db:dummy="dummy" extension-element-prefixes="exslt" version="1.0" exclude-result-prefixes="exsl db ng exslt d">
  
<xsl:import href="../html/chunk.xsl"/>

<!-- ********************************************************************
     $Id: eclipse.xsl 7242 2007-08-15 10:06:24Z mzjn $
     ********************************************************************

     This file is part of the XSL DocBook Stylesheet distribution.
     See ../README or http://docbook.sf.net/release/xsl/current/ for
     copyright and other information.

     ******************************************************************** -->

<xslo:include xmlns:xslo="http://www.w3.org/1999/XSL/Transform" href="../profiling/profile-mode.xsl"/><xslo:variable xmlns:xslo="http://www.w3.org/1999/XSL/Transform" name="profiled-content"> <xslo:choose> <xslo:when test="namespace-uri(*[1]) != 'http://docbook.org/ns/docbook'"> <xsl:message>Adding DocBook namespace to version 4 DocBook document</xsl:message> <xsl:variable name="addns"> <xsl:apply-templates mode="addNS" select="/"/> </xsl:variable> <xsl:apply-templates select="exsl:node-set($addns)" mode="profile"/> </xslo:when> <xslo:otherwise> <xslo:apply-templates select="/" mode="profile"/> </xslo:otherwise> </xslo:choose> </xslo:variable><xslo:variable xmlns:xslo="http://www.w3.org/1999/XSL/Transform" name="profiled-nodes" select="exslt:node-set($profiled-content)"/><xsl:template match="/">
  <!-- * Get a title for current doc so that we let the user -->
  <!-- * know what document we are processing at this point. -->
  <xsl:variable name="doc.title">
    <xsl:call-template name="get.doc.title"/>
  </xsl:variable>
  <xsl:choose>
    
    <!-- include extra test for Xalan quirk -->
    <xsl:when test="false()"/>
    <xsl:otherwise>
  <xsl:choose>
    <xsl:when test="$rootid != ''">
      <xsl:choose>
        <xsl:when test="count($profiled-nodes//*[@id=$rootid]) = 0">
          <xsl:message terminate="yes">
            <xsl:text>ID '</xsl:text>
            <xsl:value-of select="$rootid"/>
            <xsl:text>' not found in document.</xsl:text>
          </xsl:message>
        </xsl:when>
        <xsl:otherwise>
          <xsl:if test="$collect.xref.targets = 'yes' or                         $collect.xref.targets = 'only'">
            <xsl:apply-templates select="key('id', $rootid)" mode="collect.targets"/>
          </xsl:if>
          <xsl:if test="$collect.xref.targets != 'only'">
            <xsl:message>Formatting from <xsl:value-of select="$rootid"/></xsl:message>
            <xsl:apply-templates select="$profiled-nodes//*[@id=$rootid]" mode="process.root"/>
            <xsl:call-template name="etoc"/>
            <xsl:call-template name="plugin.xml"/>
          </xsl:if>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:otherwise>
      <xsl:if test="$collect.xref.targets = 'yes' or                     $collect.xref.targets = 'only'">
        <xsl:apply-templates select="$profiled-nodes" mode="collect.targets"/>
      </xsl:if>
      <xsl:if test="$collect.xref.targets != 'only'">
        <xsl:apply-templates select="$profiled-nodes" mode="process.root"/>
        <xsl:call-template name="etoc"/>
        <xsl:call-template name="plugin.xml"/>
      </xsl:if>
    </xsl:otherwise>
  </xsl:choose>
</xsl:otherwise>
</xsl:choose>
</xsl:template>

<xsl:template name="etoc">
  <xsl:call-template name="write.chunk">
    <xsl:with-param name="filename">
      <xsl:if test="$manifest.in.base.dir != 0">
        <xsl:value-of select="$base.dir"/>
      </xsl:if>
      <xsl:value-of select="'toc.xml'"/>
    </xsl:with-param>
    <xsl:with-param name="method" select="'xml'"/>
    <xsl:with-param name="encoding" select="'utf-8'"/>
    <xsl:with-param name="indent" select="'yes'"/>
    <xsl:with-param name="content">
      <xsl:choose>

        <xsl:when test="$rootid != ''">
          <xsl:variable name="title">
            <xsl:if test="$eclipse.autolabel=1">
              <xsl:variable name="label.markup">
                <xsl:apply-templates select="$profiled-nodes//*[@id=$rootid]" mode="label.markup"/>
              </xsl:variable>
              <xsl:if test="normalize-space($label.markup)">
                <xsl:value-of select="concat($label.markup,$autotoc.label.separator)"/>
              </xsl:if>
            </xsl:if>
            <xsl:apply-templates select="$profiled-nodes//*[@id=$rootid]" mode="title.markup"/>
          </xsl:variable>
          <xsl:variable name="href">
            <xsl:call-template name="href.target.with.base.dir">
              <xsl:with-param name="object" select="key('id',$rootid)"/>
            </xsl:call-template>
          </xsl:variable>
          
          <toc label="{normalize-space($title)}" topic="{$href}">
            <xsl:apply-templates select="$profiled-nodes//*[@id=$rootid]/*" mode="etoc"/>
          </toc>
        </xsl:when>

        <xsl:otherwise>
          <xsl:variable name="title">
            <xsl:if test="$eclipse.autolabel=1">
              <xsl:variable name="label.markup">
                <xsl:apply-templates select="$profiled-nodes/*" mode="label.markup"/>
              </xsl:variable>
              <xsl:if test="normalize-space($label.markup)">
                <xsl:value-of select="concat($label.markup,$autotoc.label.separator)"/>
              </xsl:if>
            </xsl:if>
            <xsl:apply-templates select="$profiled-nodes/*" mode="title.markup"/>
          </xsl:variable>
          <xsl:variable name="href">
            <xsl:call-template name="href.target.with.base.dir">
              <xsl:with-param name="object" select="$profiled-nodes"/>
            </xsl:call-template>
          </xsl:variable>
          
          <toc label="{normalize-space($title)}" topic="{$href}">
            <xsl:apply-templates select="$profiled-nodes/*/*" mode="etoc"/>
          </toc>
        </xsl:otherwise>

      </xsl:choose>
    </xsl:with-param>
  </xsl:call-template>
</xsl:template>

<xsl:template match="d:book|d:part|d:reference|d:preface|d:chapter|d:bibliography|d:appendix|d:article|d:glossary|d:section|d:sect1|d:sect2|d:sect3|d:sect4|d:sect5|d:refentry|d:colophon|d:bibliodiv|d:index" mode="etoc">
  <xsl:variable name="title">
    <xsl:if test="$eclipse.autolabel=1">
      <xsl:variable name="label.markup">
        <xsl:apply-templates select="." mode="label.markup"/>
      </xsl:variable>
      <xsl:if test="normalize-space($label.markup)">
        <xsl:value-of select="concat($label.markup,$autotoc.label.separator)"/>
      </xsl:if>
    </xsl:if>
    <xsl:apply-templates select="." mode="title.markup"/>
  </xsl:variable>

  <xsl:variable name="href">
    <xsl:call-template name="href.target.with.base.dir">
      <xsl:with-param name="context" select="/"/>        <!-- Generate links relative to the location of root file/toc.xml file -->
    </xsl:call-template>
  </xsl:variable>

  <topic label="{normalize-space($title)}" href="{$href}">
    <xsl:apply-templates select="d:part|d:reference|d:preface|d:chapter|d:bibliography|d:appendix|d:article|d:glossary|d:section|d:sect1|d:sect2|d:sect3|d:sect4|d:sect5|d:refentry|d:colophon|d:bibliodiv|d:index" mode="etoc"/>
  </topic>

</xsl:template>

<xsl:template match="text()" mode="etoc"/>

<xsl:template name="plugin.xml">
  <xsl:call-template name="write.chunk">
    <xsl:with-param name="filename">
      <xsl:if test="$manifest.in.base.dir != 0">
        <xsl:value-of select="$base.dir"/>
      </xsl:if>
      <xsl:value-of select="'plugin.xml'"/>
    </xsl:with-param>
    <xsl:with-param name="method" select="'xml'"/>
    <xsl:with-param name="encoding" select="'utf-8'"/>
    <xsl:with-param name="indent" select="'yes'"/>
    <xsl:with-param name="content">
      <plugin name="{$eclipse.plugin.name}" id="{$eclipse.plugin.id}" version="1.0" provider-name="{$eclipse.plugin.provider}">

        <extension point="org.eclipse.help.toc">
          <toc file="toc.xml" primary="true"/>
        </extension>
          
      </plugin>
    </xsl:with-param>
  </xsl:call-template>
</xsl:template>

</xsl:stylesheet>
