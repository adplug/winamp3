/*

  Nullsoft WASABI Source File License

  Copyright 1999-2001 Nullsoft, Inc.

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.


  Brennan Underwood
  brennan@nullsoft.com

*/

/*
 * Copyright (C) 2003 Simon Peter <dn.tlp@gmx.net>
 */

#include "component.h"
#include "adplug_decoder.h"
#include "debug.h"

static WACAdPlug	wac;
WAComponentClient	*the = &wac;

// Configuration attributes
_bool cfgStereo("Stereo", false), cfgBit16("16 bits", true);
_int cfgSamplerate("Sample rate", 44100);

// {9038D6C0-69F0-477f-ADE3-EC0F3960CB4C}
static const GUID guid = 
{ 0x9038d6c0, 0x69f0, 0x477f, { 0xad, 0xe3, 0xec, 0xf, 0x39, 0x60, 0xcb, 0x4c } };


WACAdPlug::WACAdPlug()
	: WAComponentClient("AdPlug Winamp3 Component")
{
#ifdef _DEBUG
	debug_init();
#endif

	puts("WACAdPlug::WACAdplug(): registering media converter service...");
	registerService(new waServiceT<svc_mediaConverter, AdPlugDecoder>);
}

WACAdPlug::~WACAdPlug()
{
	puts("WACAdPlug::~WACAdPlug()");
}

GUID WACAdPlug::getGUID()
{
	puts("WACAdPlug::getGUID()");
	return guid;
}

void WACAdPlug::onRegisterServices()
{
	puts("WACAdPlug::onRegisterServices(): registering file extensions.");
	api->core_registerExtension("*.sng","Faust Music Creator Files");

	puts("registering configuration attributes.");
	registerAttribute(&cfgStereo);
	registerAttribute(&cfgBit16);
	registerAttribute(&cfgSamplerate);
}
