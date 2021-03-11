#pragma once
/// INCLUDES ///
// External //
#include <stdio.h>	// Standard libraries		//
#include <string>
#include <MsXml6.h>	// For XML functionality	//

/// CLASS DECLARATION ///
class City
{
public:
	/// Parameters ///
	BSTR Name;
	long Population;
	double Area;

	/// Methods ///
	// Constructor / Destructor //
	City(IXMLDOMNode* node);
	~City() {}
};