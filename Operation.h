#pragma once
/// INCLUDES ///
// External //
#include <stdio.h>	// Standard libraries		//
#include <string>
#include <MsXml6.h>	// For XML functionality	//

/// CLASS DECLARATION ///
class Operation
{
public:
	/// Parameters ///
	BSTR Name;
	BSTR Type;
	BSTR Function;
	BSTR Attribute;
	BSTR Filter;

	/// Methods ///
	// Constructor / Destructor //
	Operation(IXMLDOMNode* node);
	~Operation() {}
};