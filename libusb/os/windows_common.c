/*
 * windows backend for libusb 1.0
 * Copyright © 2009-2012 Pete Batard <pete@akeo.ie>
 * With contributions from Michael Plante, Orin Eman et al.
 * Parts of this code adapted from libusb-win32-v1 by Stephan Meyer
 * HID Reports IOCTLs inspired from HIDAPI by Alan Ott, Signal 11 Software
 * Hash table functions adapted from glibc, by Ulrich Drepper et al.
 * Major code testing contribution by Xiaofan Chen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <config.h>

#include <stdio.h>

#include "libusbi.h"
#include "windows_common.h"

/*
 * Dynamically loads a DLL from the Windows system directory.  Unlike the
 * LoadLibraryA() function, this function will not search through any
 * directories to try and find the library.
 */
HMODULE load_system_library(struct libusb_context *ctx, const char *name)
{
	char library_path[MAX_PATH];
	char *filename_start;
	UINT length;

	length = GetSystemDirectoryA(library_path, sizeof(library_path));
	if ((length == 0) || (length >= (UINT)sizeof(library_path))) {
		usbi_err(ctx, "program assertion failed - could not get system directory");
		return NULL;
	}

	filename_start = library_path + length;
	// Append '\' + name + ".dll" + NUL
	length += 1 + strlen(name) + 4 + 1;
	if (length >= (UINT)sizeof(library_path)) {
		usbi_err(ctx, "program assertion failed - library path buffer overflow");
		return NULL;
	}

	sprintf(filename_start, "\\%s.dll", name);
	return LoadLibraryA(library_path);
}
