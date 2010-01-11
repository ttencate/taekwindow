#ifndef CONFIGSHEET_HPP
#define CONFIGSHEET_HPP

#include <windows.h>

#include "propsheet.hpp"
#include "config.hpp"
#include "comctl.hpp"
#include "gdipluswrap.hpp"
#include "imagelist.hpp"

class ConfigSheet : private PropSheet {

	static size_t const s_numPages;
	static int const s_templateIds[];
	static DLGPROC const s_pageProcs[];

	static size_t const s_numImages;
	static int const s_resourceIds[];
	static int const s_controlIds[];

	/* The configuration that is currently entered in the dialog.
	 */
	Configuration d_newConfig;

	CommonControls d_comCtls;
	GdiPlusWrap d_gdiPlus;

	ImageList d_imageList;

	/* Handle of the configuration dialog's window.
	 */
	HWND d_handle;

	/* Original window procedure of the property sheet dialog.
	 * We override this with our own, but call the original.
	 */
	WNDPROC d_origWindowProc;

	HWND d_exitButton;

	public:

		ConfigSheet(Configuration const &config);
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
		void initDynamicLabels(HWND dialogHandle);
		void createExitButton();
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
		
		/* Returns true if the configuration dialog is currently visible.
		 */
		bool isShowing();

		/* Brings the configuration dialog to the foreground and gives it focus,
		 * if it is currently showing.
		 */
		void bringToFront();

};

#endif
