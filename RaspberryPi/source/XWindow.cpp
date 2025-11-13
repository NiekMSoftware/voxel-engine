// Global includes
#include "XWindow.h"
#include <EGL/egl.h>
#include <GLES3/gl31.h>
#include <EGL/eglplatform.h>

// X11
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <cstring>

// framework includes
#include "framework/Game.h"

void XWindow::CreateWindow()
{
	display = XOpenDisplay(nullptr);
	root = RootWindowOfScreen(DefaultScreenOfDisplay(display));

	XSetWindowAttributes swa;
	XSetWindowAttributes  xattr;
	XWMHints hints;

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;

	swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask;
	swa.background_pixmap = None;
	swa.background_pixel = 0;
	swa.border_pixel = 0;
	swa.override_redirect = true;

	window = XCreateWindow(display, root, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa);

	XSelectInput(display, window, KeyPressMask | KeyReleaseMask);
	xattr.override_redirect = false;
	XChangeWindowAttributes(display, window, CWOverrideRedirect, &xattr);

	hints.input = true;
	hints.flags = InputHint;
	XSetWMHints(display, window, &hints);

	// Make the window visible on the screen
	XMapWindow(display, window);
	XStoreName(display, window, "Pi Project 3D");
	XClearWindow(display, window);
	XMapRaised(display, window);

	// Get identifiers for the provided atom name strings
	const Atom wm_state = XInternAtom(display, "_NET_WM_STATE", false);

	// - Uncomment to make the window Fullscreen -
	//Atom wm_fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", false);
	//XChangeProperty(display, window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wm_fullscreen, 1);

	state.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(state.display, &majorVersion, &minorVersion);
	eglGetConfigs(state.display, nullptr, 0, &numConfigs);
	eglChooseConfig(state.display, attribute_list, &state.config, 1, &numConfigs);

	// OpenGL3 setup.
	state.context = eglCreateContext(state.display, state.config, nullptr, context_attributes);
	state.surface = eglCreateWindowSurface(state.display, state.config, window, nullptr);
	eglMakeCurrent(state.display, state.surface, state.surface, state.context);
	eglSurfaceAttrib(display, state.surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
	eglSwapInterval(state.display, 1);

	// - Uncomment to hide the mouse -
	// HideMouse();
}

void XWindow::HideMouse() const
{
	XColor black;
	static char noData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	black.red = black.green = black.blue = 0;
	const Pixmap bitmapNoData = XCreateBitmapFromData(display, window, noData, 8, 8);
	const Cursor invisibleCursor = XCreatePixmapCursor(display, bitmapNoData, bitmapNoData, &black, &black, 0, 0);

	XDefineCursor(display, window, invisibleCursor);
	XFreeCursor(display, invisibleCursor);
	XFreePixmap(display, bitmapNoData);
}

const EGLState& XWindow::GetState() const
{
	return state;
}

Display& XWindow::GetDisplay() const
{
	return *display;
}

Window& XWindow::GetWindow()
{
	return window;
}
