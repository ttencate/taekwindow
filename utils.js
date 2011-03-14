function replaceEmail() {
	spans = document.getElementsByTagName("span");
	for (var i = 0; i < spans.length; i++) {
		var span = spans[i];
		if (span.className == "email") {
			realMail = span.innerHTML.replace("(AT)", "@").replace("(DOT)", ".").replace("(PLUS)", "+");
			span.innerHTML = "<a href=\"mailto:" + realMail + "\">" + realMail + "</a>";
		}
	}
}
