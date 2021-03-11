/// INCLUDES ///
// External //
#include <stdio.h>	// Standard libraries		//
#include <MsXml6.h>	// For XML functionality	//

// Internal //
#include "MACROS.h"
#include "Logic.h"

/// MAIN ///
int main(int argc, char* args[])
{	/// Initialize ///
	HRESULT hr = CoInitialize(NULL);
	if (!SUCCEEDED(hr)) return 0;	// If we failed, end now //

	/// Main logic ///
	CHK_HR(RUN(L"data.xml", L"operations.xml", L"output_JZO.xml"));

	/// Clean up ///
CleanUp:
	CoUninitialize();

	// Output //
	return 0;	// main always returns 0! //
}