<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="description">
		<h2>Description</h2>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="installation">
		<h2>Installation</h2>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="usage">
		<h2>Usage</h2>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="configuration">
		<h2>Configuration</h2>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="bugs">
		<h2>Bugs</h2>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="feature-requests">
		<h2>Feature requests</h2>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="versions">
		<h2>Version history</h2>
		<xsl:for-each select="version">
			<p>
				<xsl:value-of select="@nr"/>
			</p>
			<ul>
				<xsl:for-each select="feature">
					<li>
						<xsl:value-of select="."/>
						<xsl:if test="@see!=''">
							<xsl:text xml:space="preserve"> </xsl:text>
							(see <xsl:element name="a" xml:space="preserve"><xsl:attribute name="href"><xsl:value-of select="@see"/></xsl:attribute>tracker item</xsl:element>)
						</xsl:if>
					</li>
				</xsl:for-each>
				<xsl:for-each select="bugfix">
					<li>
						Bugfix:
						<xsl:value-of select="."/>
						<xsl:if test="@see!=''">
							<xsl:text xml:space="preserve"> </xsl:text>
							(see <xsl:element name="a" xml:space="preserve"><xsl:attribute name="href"><xsl:value-of select="@see"/></xsl:attribute>tracker item</xsl:element>)
						</xsl:if>
					</li>
				</xsl:for-each>
			</ul>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="contact">
		<h2>Contact</h2>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="thanks">
		<h2>Thanks</h2>
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="thanklist">
		<ul>
			<xsl:for-each select="thank">
				<li>
					<strong>
						<xsl:value-of select="@who"/>
					</strong>
					<xsl:value-of select="."/>
				</li>
			</xsl:for-each>
		</ul>
	</xsl:template>

	<xsl:template match="license">
		<h2>License</h2>
		<xsl:apply-templates/>
	</xsl:template>
	
	<!-- HTML-like tags -->

	<xsl:template match="p">
		<p>
			<xsl:apply-templates/>
		</p>
	</xsl:template>

	<xsl:template match="code" xml:space="preserve"><code><xsl:apply-templates/></code></xsl:template>

	<xsl:template match="ul">
		<ul>
			<xsl:apply-templates/>
		</ul>
	</xsl:template>

	<xsl:template match="ol">
		<ol>
			<xsl:apply-templates/>
		</ol>
	</xsl:template>

	<xsl:template match="li">
		<li>
			<xsl:apply-templates/>
		</li>
	</xsl:template>

	<xsl:template match="a">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="@href"/>
			</xsl:attribute>
			<xsl:value-of select="."/>
		</xsl:element>
	</xsl:template>

</xsl:stylesheet>
