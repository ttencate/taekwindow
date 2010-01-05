<?xml version="1.0" encoding="utf-8"?>

<stylesheet version="1.0" xmlns="http://www.w3.org/1999/XSL/Transform">

	<import href="../meta.xslt"/>

	<output method="text" omit-xml-declaration="yes"/>

	<variable name="history" select="document('../doc/history.xml')/history"/>

	<!-- Gives a plain-text change log of the latest version, for copy/pasting into SourceForge -->
	<template match="/">
		<for-each select="$history/version">
			<if test="@nr = $shortversion">
				<for-each select="feature">
					<text xml:space="preserve">- </text>
					<value-of select="normalize-space(.)"/>
					<text xml:space="preserve">
</text>
				</for-each>
				<for-each select="bugfix">
					<text xml:space="preserve">- Bugfix: </text>
					<value-of select="normalize-space(.)"/>
					<text xml:space="preserve">
</text>
				</for-each>
			</if>
		</for-each>
	</template>

</stylesheet>
