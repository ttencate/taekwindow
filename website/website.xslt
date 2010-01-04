<?xml version="1.0" encoding="utf-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method="xml" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd" doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN" encoding="utf-8" indent="yes"/>

	<xsl:template match="/">
		<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
			<head>
				<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
				<link rel="alternate" type="application/rss+xml" title="Taekwindow News RSS" href="http://sourceforge.net/export/rss2_projnews.php?group_id=185457&amp;rss_fulltext=1"/>
				<link rel="stylesheet" type="text/css" href="screen.css"/>
				<script type="text/javascript" src="utils.js"></script>
				<title>
					Taekwindow - <xsl:value-of select="$title"/>
				</title>
			</head>
			<body onload="replaceEmail()">
				<div id="everything">
					<div id="topbar">
						<h1>
							<img src="logo.png" alt="Taekwindow logo" class="inline"/>
							Taekwindow
						</h1>
					</div>
					<div id="navbar">
						<ul class="navbar">
							<li>
								<a href="index.html">Home</a>
							</li>
							<li>
								<a href="faq.html">FAQ</a>
							</li>
							<li>
								<a href="download.html">Download</a>
							</li>
							<li>
								<a href="manual.html">Manual</a>
							</li>
							<li>
								<a href="contact.html">Contact</a>
							</li>
							<li>
								<a href="http://sourceforge.net/projects/taekwindow/" class="sf">SF.net project page</a>
							</li>
						</ul>
					</div>
					<div id="content">

						<xsl:copy-of select="$content"/>

					</div>
					<div id="footer">
						<p>
							The <img src="sficon.gif" alt="SourceForge icon"/> icon indicates links to the SourceForge project page.
						</p>
						<p class="rightmargin">
							<a href="http://sourceforge.net/export/rss2_projnews.php?group_id=185457&amp;rss_fulltext=1" title="Taekwindow News RSS" style="margin-right:10px">
								<img src="feed-icon-28x28.png" alt="Taekwindow News RSS" height="28" width="28"/>
							</a>
							<a href="http://validator.w3.org/check?uri=referer" title="Validate this page" style="margin-right:5px">
								<img src="valid-xhtml10.png" alt="Valid XHTML 1.0 Strict" height="31" width="88"/>
							</a>
							<a href="http://sourceforge.net" title="Hosted by SourceForge.net">
								<img src="http://sflogo.sourceforge.net/sflogo.php?group_id=185457&amp;type=3" width="125" height="37" alt="SourceForge.net logo"/>
							</a>
						</p>
					</div>
				</div>
				<script type="text/javascript">
					var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
					document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
				</script>
				<script type="text/javascript">
					try {
					var pageTracker = _gat._getTracker("UA-1179382-1");
					pageTracker._trackPageview();
					} catch(err) {}
				</script>
			</body>

		</html>

	</xsl:template>

</xsl:stylesheet>