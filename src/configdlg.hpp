#ifndef CONFIGDLG_HPP
#define CONFIGDLG_HPP

#include <windows.h>
#include <commctrl.h>

#include "gdipluswrap.hpp"
#include "image.hpp"
#include "propsheet.hpp"
#include "config.hpp"

class ConfigSheet : private PropSheet {

	static int const s_numImages = 5;

	/* Handle of the configuration dialog's window.
	 */
	HWND d_handle;

	/* Original window procedure of the property sheet dialog.
	 * We override this with our own, but call the original.
	 */
	WNDPROC d_origWindowProc;

	/* The configuration that is currently entered in the dialog.
	 * Only valid when the dialog is visible.
	 */
	Configuration d_newConfig;

	GdiPlusWrap d_gdiPlus;

	Image *d_images[s_numImages];

	public:

		ConfigSheet();
		~ConfigSheet();

		/* Shows the configuration dialog, or, if it's already showing, brings it to the front.
		 */
		void show();

	private:

		static ConfigSheet *s_instance;

		static BOOL CALLBACK generalPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		static BOOL CALLBACK buttonsPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		static BOOL CALLBACK resizingPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		static BOOL CALLBACK scrollingPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		static BOOL CALLBACK aboutPageFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK configFwd(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);

		void loadImages();
		void unloadImages();
		void initDynamicLabels(HWND dialogHandle);
		void drawImageControl(int controlID, DRAWITEMSTRUCT const &item);
		void hyperlinkClicked(int controlID, NMLINK const &nmLink);
		void mutexButtonRadios(HWND dialogHandle, WPARAM wParam, int move, int resize);

		BOOL commonDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL generalPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL buttonsPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL resizingPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL scrollingPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		BOOL aboutPageDialogProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);

		LRESULT configWindowProc(HWND dialogHandle, UINT message, WPARAM wParam, LPARAM lParam);
		virtual void callback(HWND hwnd, UINT msg, LPARAM lParam);

		/* Unconditionally shows the dialog.
		 * Must not be called if it is already showing.
		 */
		void doShow();
		
		/* Returns true if the configuration dialog is currently visible.
		 */
		bool isShowing();

		/* Brings the configuration dialog to the foreground and gives it focus,
		 * if it is currently showing.
		 */
		void bringToFront();

};

class ConfigDlg {

	ConfigSheet *d_sheet;

	public:

		ConfigDlg();
		~ConfigDlg();

		void show();

};

#endif
