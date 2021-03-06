//
// This console app enumerates and mutes all audio capture endpoints using the
// IAudioEndpointVolume::SetMute() method of the Vista/Win7 Core Audio API
// 
// Copyright (c) Fotios Basagiannis. All rights reserved
//
// EndpointVolumeChanger.cpp : Endpoint Volume Changing sample application.
//

#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <assert.h>
#include <functiondiscoverykeys.h>
#include <EndpointVolume.h>

using namespace std;

#define MAX_BUF_LEN 32

IMMDevice * device = NULL;

//
//  Retrieves the device friendly name for a particular device in a device collection.  
//
//  The returned string was allocated using malloc() so it should be freed using free();
//
LPWSTR GetDeviceName(IMMDeviceCollection *DeviceCollection, UINT DeviceIndex)
{
    IMMDevice *device;
    LPWSTR deviceId;
    HRESULT hr;

    hr = DeviceCollection->Item(DeviceIndex, &device);
    if (FAILED(hr))
    {
        printf("Unable to get device %d: %x\n", DeviceIndex, hr);
        return NULL;
    }
    hr = device->GetId(&deviceId);
    if (FAILED(hr))
    {
        printf("Unable to get device %d id: %x\n", DeviceIndex, hr);
        return NULL;
    }

    IPropertyStore *propertyStore;
    hr = device->OpenPropertyStore(STGM_READ, &propertyStore);
    SafeRelease(&device);
    if (FAILED(hr))
    {
        printf("Unable to open device %d property store: %x\n", DeviceIndex, hr);
        return NULL;
    }

    PROPVARIANT friendlyName;
    PropVariantInit(&friendlyName);
    hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
    SafeRelease(&propertyStore);

    if (FAILED(hr))
    {
        printf("Unable to retrieve friendly name for device %d : %x\n", DeviceIndex, hr);
        return NULL;
    }

    wchar_t deviceName[128];
    hr = StringCbPrintf(deviceName, sizeof(deviceName), L"%s (%s)", friendlyName.vt != VT_LPWSTR ? L"Unknown" : friendlyName.pwszVal, deviceId);
    if (FAILED(hr))
    {
        printf("Unable to format friendly name for device %d : %x\n", DeviceIndex, hr);
        return NULL;
    }

    PropVariantClear(&friendlyName);
    CoTaskMemFree(deviceId);

    wchar_t *returnValue = _wcsdup(deviceName);
    if (returnValue == NULL)
    {
        printf("Unable to allocate buffer for return\n");
        return NULL;
    }
    return returnValue;
}

void initMic() {
    IMMDeviceEnumerator *deviceEnumerator = NULL;

    //We initialize the device enumerator here
    hr = CoCreateInstance( __uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, 
                           IID_PPV_ARGS(&deviceEnumerator) );
    if (FAILED(hr))
    {
        printf("Unable to instantiate device enumerator: %x\n", hr);
        goto Exit;
    }


    IMMDeviceCollection *deviceCollection = NULL;
    
    //Here we enumerate the audio endpoints of interest (in this case audio capture endpoints)
    //into our device collection. We use "eCapture" for audio capture endpoints, "eRender" for 
    //audio output endpoints and "eAll" for all audio endpoints 
    hr = deviceEnumerator->EnumAudioEndpoints( eCapture, DEVICE_STATE_ACTIVE, 
                                               &deviceCollection );
    if (FAILED(hr))
    {
        printf("Unable to retrieve device collection: %x\n", hr);
        goto Exit;
    }


    UINT deviceCount;

    hr = deviceCollection->GetCount(&deviceCount);
    if (FAILED(hr))
    {
        printf("Unable to get device collection length: %x\n", hr);
        goto Exit;
    }
    
    //
    //This loop goes over each audio endpoint in our device collection,
    //gets and diplays its friendly name and then tries to mute it
    //
    for (UINT i = 0 ; i < deviceCount ; i += 1)
    {
        LPWSTR deviceName;

        //Here we use the GetDeviceName() function provided with the sample 
        //(see source code zip)
        deviceName = GetDeviceName(deviceCollection, i); //Get device friendly name

        if (deviceName == NULL) goto Exit;
        
        printf("Device to be muted has index: %d and name: %S\n", i, deviceName);

        free(deviceName); //this needs to be done because name is stored in a heap allocated buffer


        device = NULL;

        //Put device ref into device var
        hr = deviceCollection->Item(i, &device);
        if (FAILED(hr))
        {
            printf("Unable to retrieve device %d: %x\n", i, hr);
            goto Exit;
        }

    }

Exit: //Core Audio and COM clean up here
    SafeRelease(&deviceCollection);
    SafeRelease(&deviceEnumerator);
    SafeRelease(&device);
    CoUninitialize();
    return;
}

void setMuteMicrophone(bool notMute) {
    if (!device) {
        cout << "microphone does not exist!" << endl;
        return;
    }

    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    
    //This is the Core Audio interface of interest
    IAudioEndpointVolume *endpointVolume = NULL;

    //We activate it here
    hr = device->Activate( __uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, 
                           reinterpret_cast<void **>(&endpointVolume) );
    if (FAILED(hr))
    {
        printf("Unable to activate endpoint volume on output device: %x\n", hr);
        return;
    }


    hr = endpointVolume->SetMute(notMute, NULL); //Try to mute endpoint here
    if (FAILED(hr))
    {
        printf("Unable to set mute state on endpoint: %x\n", hr);
        return;
    } else
        printf("Endpoint muted successfully: %d\n", notMute);
}

int wmain(int argc, wchar_t* argv[])
{
    //
    //  A GUI application should use COINIT_APARTMENTTHREADED instead of COINIT_MULTITHREADED.
    //
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        printf("Unable to initialize COM: %x\n", hr);
        goto Exit;
    }

Exit: //Core Audio and COM clean up here
    SafeRelease(&deviceCollection);
    SafeRelease(&deviceEnumerator);
    SafeRelease(&device);
    CoUninitialize();
    return 0;
}

