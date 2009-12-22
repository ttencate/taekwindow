#ifndef SHELLLINK_HPP
#define SHELLLINK_HPP

#include <windows.h>

/* These objects are supposed to be short-lived.
 * They do not copy any pointer argument that is passed in.
 */
class ShellLink {

	TCHAR const *d_filename;

	public:

		ShellLink(TCHAR const *filename);

		bool exists() const;
		void create(TCHAR *target, TCHAR *workingDir, TCHAR *description);
		void destroy();

};

#endif
