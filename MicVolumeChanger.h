#include "stdafx.h"

#include <windows.h>
#include <assert.h>
#include <functiondiscoverykeys.h>
#include <EndpointVolume.h>

using namespace std;

extern IMMDevice * device;

LPWSTR GetDeviceName(IMMDeviceCollection *DeviceCollection, UINT DeviceIndex);

void initMic();

void setMuteMicrophone(bool notMute);

