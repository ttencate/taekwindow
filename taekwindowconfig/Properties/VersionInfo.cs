/* This file should be run through the C preprocessor to produce a usable C# file,
 * which can then be built into the assembly.
 * 
 * The syntax checker hates it.
 * 
 * It's all a bit flaky, so do not try fancy tricks in here.
 */

#include "../../version.inc"

namespace TaekwindowConfig {

	public static class VersionInfo
	{
		public const string Title = APPLICATION_TITLE;
		public const string Author = APPLICATION_AUTHOR;
		public const string AuthorFull = APPLICATION_AUTHOR_FULL;
		public const string Copyright = APPLICATION_COPYRIGHT;
		public const string VersionString = APPLICATION_VERSION_STRING;
		public const string Website = APPLICATION_WEBSITE;
		public const string Email = APPLICATION_EMAIL;
	}

}
