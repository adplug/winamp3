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

#include <stdio.h>
#include <common/metatags.h>

#include "component.h"
#include "adplug_decoder.h"

#define BUFFER_SIZE		512			// Buffer size in samples

#define SAMPLERATE		44100

AdPlugDecoder::AdPlugDecoder()
	: p(0)
{
	puts("AdPlugDecoder::AdPlugDecoder()");
	opl = new CEmuopl(cfgSamplerate, cfgBit16, cfgStereo);

	ci.addInfo("srate", SAMPLERATE);
	ci.addInfo("bps", cfgBit16 ? 16 : 8);
	ci.addInfo("nch", cfgStereo ? 2 : 1);
}

AdPlugDecoder::~AdPlugDecoder()
{
	puts("AdPlugDecoder::~AdPlugDecoder()");
	if(p) delete p;
	delete opl;
}

int AdPlugDecoder::canConvertFrom(svc_fileReader *reader, const char *name,
								  const char *chunktype)
{
	printf("AdPlugDecoder::canConvertFrom(reader, \"%s\", chunktype):\n", name);
	if(!name) return 0;

	puts("trying to get a player from AdPlug...");
	p = CAdPlug::factory(name, opl);
	if(p) puts("returned 1."); else puts("returned 0.");
	return (p ? 1 : 0);
}

int AdPlugDecoder::getInfos(MediaInfo *infos)
{
	unsigned long songlength;

	puts("AdPlugDecoder::getInfos(infos)");
	if(!p) { puts("NO PLAYER!"); return 0; }

	songlength = p->songlength();
	printf("Songlength: %d\n", songlength);

	infos->setTitle(p->gettitle().c_str());
	infos->setLength(songlength);

	infos->setData(MT_ARTIST, p->getauthor().c_str());
	infos->setData(MT_TITLE, p->gettitle().c_str());
	infos->setData(MT_COMMENT, p->getdesc().c_str());
	infos->setData(MT_LENGTH, (const char *)&songlength);

//	infos->setData(MT_PLAYSTRING, "AdPlug: Test Playstring");
	infos->setData(MT_INFO, StringPrintf("%s Bit, %s, %d Hz",
		cfgBit16 ? "16" : "8", cfgStereo ? "Stereo" : "Mono", SAMPLERATE));

	return 0;
}

int AdPlugDecoder::processData(MediaInfo *infos, ChunkList *chunk_list,
							   bool *killswitch)
{
	bool playing;
	unsigned long buffersize;
	short *buf;

	puts("AdPlugDecoder::processData()");
	if(!p) { puts("NO PLAYER!"); return 0; }

	playing = p->update();

	buffersize = 44100 / p->getrefresh();
	buf = (short *)malloc(buffersize * 2);
	opl->update(buf, buffersize);
	chunk_list->setChunk("PCM", buf, buffersize * 2/*, &ci */);
	free(buf);

	if(playing) puts("returned 1."); else puts("returned 0.");
	return (playing ? 1 : 0);
}

int AdPlugDecoder::corecb_onSeeked(int newpos)
{
	printf("AdPlugDecoder::corecb_onSeeked(%d)\n", newpos);
	return 0;
}
