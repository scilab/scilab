<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:d="http://docbook.org/ns/docbook"
xmlns:office="http://openoffice.org/2000/office"
  xmlns:style="http://openoffice.org/2000/style"
  xmlns:text="http://openoffice.org/2000/text"
  xmlns:table="http://openoffice.org/2000/table"
  xmlns:draw="http://openoffice.org/2000/drawing"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:number="http://openoffice.org/2000/datastyle"
  xmlns:svg="http://www.w3.org/2000/svg"
  xmlns:chart="http://openoffice.org/2000/chart"
  xmlns:dr3d="http://openoffice.org/2000/dr3d"
  xmlns:math="http://www.w3.org/1998/Math/MathML"
  xmlns:form="http://openoffice.org/2000/form"
  xmlns:script="http://openoffice.org/2000/script"
  xmlns:doc='http://docbook.org/ns/docbook'
  exclude-result-prefixes='doc'>

  <xsl:import href='dbk2wp.xsl'/>

  <xsl:output method="xml" indent='yes'/>

  <!-- ********************************************************************
       $Id: dbk2ooo.xsl 6910 2007-06-28 23:23:30Z xmldoc $
       ********************************************************************

       This file is part of the XSL DocBook Stylesheet distribution.
       See ../README or http://docbook.sf.net/release/xsl/current/ for
       copyright and other information.

       ******************************************************************** -->

  <xsl:include href='../VERSION'/>

  <xsl:template match="/" name='ooo.top'>
    <xsl:param name='doc' select='/'/>

    <office:document
      office:class='text'
      office:version='1.0'>

      <office:script/>
      <office:font-decls>
        <style:font-decl style:name="Lucida Grande1" fo:font-family="'Lucida Grande'"/>
        <style:font-decl style:name="Arial1" fo:font-family="Arial" style:font-pitch="variable"/>
        <style:font-decl style:name="Hiragino Mincho Pro W3" fo:font-family="'Hiragino Mincho Pro W3'" style:font-pitch="variable"/>
        <style:font-decl style:name="Lucida Grande" fo:font-family="'Lucida Grande'" style:font-pitch="variable"/>
        <style:font-decl style:name="Times New Roman" fo:font-family="'Times New Roman'" style:font-family-generic="roman" style:font-pitch="variable"/>
        <style:font-decl style:name="Arial" fo:font-family="Arial" style:font-family-generic="swiss" style:font-pitch="variable"/>
      </office:font-decls>
      <office:automatic-styles/>

      <xsl:apply-templates select='$doc/*'
        mode='doc:toplevel'/>
    </office:document>
  </xsl:template>

  <xsl:template name='doc:make-body'>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <office:body>
      <text:sequence-decls>
        <text:sequence-decl text:display-outline-level="0" text:name="Illustration"/>
        <text:sequence-decl text:display-outline-level="0" text:name="Table"/>
        <text:sequence-decl text:display-outline-level="0" text:name="Text"/>
        <text:sequence-decl text:display-outline-level="0" text:name="Drawing"/>
      </text:sequence-decls>

      <xsl:copy-of select='$content'/>
    </office:body>
  </xsl:template>

  <xsl:template name='doc:make-subsection'>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <xsl:copy-of select='$content'/>
  </xsl:template>

  <xsl:template name='doc:make-paragraph'>
    <xsl:param name='style' select='"d:unknown"'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>
    <xsl:param name='outline.level' select='0'/>
    <xsl:param name='attributes.node' select='.'/>

    <text:p text:style-name='{$style}'>

      <xsl:call-template name='attributes'>
        <xsl:with-param name='node' select='$attributes.node'/>
      </xsl:call-template>

      <xsl:copy-of select='$content'/>
    </text:p>
  </xsl:template>

  <xsl:template name='doc:make-phrase'>
    <xsl:param name='style' select='"d:unknown"'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <text:span text:style-name='{$style}'>
      <xsl:copy-of select='$content'/>
    </text:span>
  </xsl:template>

  <xsl:template name='doc:make-hyperlink'/>
  <xsl:template name='doc:make-hyperlink-not-implemented'>
    <xsl:param name='target'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

    <text:link href='{$target}'>
    </text:link>
  </xsl:template>

  <xsl:template name='doc:make-table'/>
  <xsl:template name='doc:make-table-not-yet-implemented'>
    <xsl:param name='columns'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

  </xsl:template>

  <xsl:template name='doc:make-table-row'>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>
    <xsl:param name='is-header' select='false()'/>

  </xsl:template>

  <xsl:template name='doc:make-table-cell'>
    <xsl:param name='width' select='0'/>
    <xsl:param name='hidden' select='false()'/>
    <xsl:param name='rowspan' select='1'/>
    <xsl:param name='colspan' select='1'/>
    <xsl:param name='content'>
      <xsl:apply-templates mode='doc:body'/>
    </xsl:param>

  </xsl:template>

  <xsl:template name='doc:make-soft-break'>
    <text:br/>
  </xsl:template>

  <xsl:template name='attributes'>
    <xsl:param name='node' select='.'/>

  </xsl:template>
</xsl:stylesheet>
