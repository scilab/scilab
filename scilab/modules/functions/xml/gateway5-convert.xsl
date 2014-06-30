<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" doctype-system="../../functions/xml/gateway.dtd"/>
    <xsl:template match="/">
        <xsl:comment>
            /*
            *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
            *  Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
            *
            *  This file must be used under the terms of the CeCILL.
            *  This source file is licensed as described in the file COPYING, which
            *  you should have received as part of this distribution.  The terms
            *  are also available at
            *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
            *
            */
            Scilab
            Interface description. In this file, we define the list of the function which
            will be available into Scilab and the link to the "native" function.
            
            Don't touch if you do not know what you are doing !
        </xsl:comment>
        
        <module>
            <xsl:attribute name="name">
                <xsl:value-of select="GATEWAY/@name" />
            </xsl:attribute>
            
            <xsl:for-each select="GATEWAY/PRIMITIVE">
                <gateway type="0">
                    <xsl:attribute name="name">
                        <xsl:value-of select="concat('sci_', @primitiveName)" />
                    </xsl:attribute>
                    <xsl:attribute name="function">
                        <xsl:value-of select="@primitiveName" />
                    </xsl:attribute>
                    <xsl:attribute name="type">0</xsl:attribute>
                </gateway>
            </xsl:for-each>
        </module>
    </xsl:template>
    
</xsl:stylesheet>

