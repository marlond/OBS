/********************************************************************************
 Copyright (C) 2014 Timo Rothenpieler <timo@rothenpieler.org>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
********************************************************************************/

#include "nvmain.h"
#include "NVENCEncoder.h"

bool doLog = true;
ConfigFile **NvAppConfig = 0;

unsigned int encoderRefs = 0;

extern "C" __declspec(dllexport) bool __cdecl InitNVENCEncoder(ConfigFile **appConfig)
{
    NvAppConfig = appConfig;

    return true;
}

extern "C" __declspec(dllexport) bool __cdecl CheckNVENCHardwareSupport(bool log)
{
    doLog = log;

    if (!checkNvEnc())
        return false;
    
    return true;
}

extern "C" __declspec(dllexport) VideoEncoder* __cdecl CreateNVENCEncoder(int fps, int width, int height, int quality, CTSTR preset, bool bUse444, ColorDescription &colorDesc, int maxBitRate, int bufferSize, bool bUseCFR)
{
    if (!initNvEnc())
        return 0;

    NVENCEncoder *res = new NVENCEncoder(fps, width, height, quality, preset, bUse444, colorDesc, maxBitRate, bufferSize, bUseCFR);

    if (!res->isAlive())
    {
        delete res;
        return 0;
    }

    encoderRefs += 1;

    return res;
}

void encoderRefDec()
{
    assert(encoderRefs);

    encoderRefs -= 1;

    if (encoderRefs == 0)
        deinitNvEnc();
}
