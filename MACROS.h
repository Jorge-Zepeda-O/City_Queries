#pragma once
/// INCLUDES ///
// External //
#include <stdio.h>	// Standard libraries	//
#include <MsXml6.h>	// For XML - included	//

/// MACRO DEFINITIONS ///
// Checks if the HR is fine? //
#define CHK_HR(stmt)	do { hr=(stmt); if(FAILED(hr)) goto CleanUp; } while(0)

// Check if a pointer `p` is allocated properly //
#define CHK_ALLOC(p)	do { if(!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

// Safely releases and nulls the pointer `p` //
#define SAFE_RELEASE(p)	do { if((p)) { (p)->Release(); (p) = NULL; } } while(0)

///