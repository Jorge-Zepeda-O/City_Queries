#pragma once
/// INCLUDES ///
// External //
#include <stdio.h>	// Standard libraries	//
#include <MsXml6.h>	// For XML - included	//

// Internal //
#include "MACROS.h"

/// HELPDERS ///
HRESULT VariantFromString(PCWSTR wszValue, VARIANT &Variant)	// ??? //
{	/// Initialize ///
	HRESULT hr = S_OK;

	BSTR bstr = SysAllocString(wszValue);
	CHK_ALLOC(bstr);

	/// ??? ///
	V_VT(&Variant) = VT_BSTR;
	V_BSTR(&Variant) = bstr;

	/// Clean up ///
CleanUp:
	// Free strings //
	SysFreeString(bstr);

	// Output //
	return hr;
}

/// XML DOM FACTORY DEFINITIONS ///
// Initialization //
HRESULT InitDOM(IXMLDOMDocument **ppDoc)
{	// Creates an XML DOM instance //
	/// Initialize ///
	HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
	if (SUCCEEDED(hr))
	{
		(*ppDoc)->put_async(VARIANT_FALSE);
		(*ppDoc)->put_validateOnParse(VARIANT_FALSE);
		(*ppDoc)->put_resolveExternals(VARIANT_FALSE);
		(*ppDoc)->put_preserveWhiteSpace(VARIANT_TRUE);
	}

	/// Output ///
	return hr;
}

// Appending Methods //
HRESULT AppendNode(IXMLDOMNode *pParent, IXMLDOMNode *pChild)
{	/// Initialize ///
	HRESULT hr = S_OK;
	IXMLDOMNode *pChildOut = NULL;

	/// Append ///
	CHK_HR(pParent->appendChild(pChild, &pChildOut));

	/// Clean up ///
CleanUp:
	// Free pointers //
	SAFE_RELEASE(pChildOut);

	// Output //
	return hr;
}

// Creation Methods //
HRESULT CreateElement(IXMLDOMDocument *pDoc, PCWSTR wszName, IXMLDOMElement **ppElement)
{	/// Initialize ///
	HRESULT hr = S_OK;
	*ppElement = NULL;

	BSTR bstrName = SysAllocString(wszName);
	CHK_ALLOC(bstrName);

	/// Create Element ///
	CHK_HR(pDoc->createElement(bstrName, ppElement));

	/// Clean up ///
CleanUp:
	// Free strings //
	SysFreeString(bstrName);

	// Output //
	return hr;
}
HRESULT AddNode_Attribute(IXMLDOMDocument *pDoc, IXMLDOMElement *pParent, PCWSTR wszName, PCWSTR wszValue)
{	/// Initialize ///
	HRESULT hr = S_OK;
	IXMLDOMAttribute *pAttribute = NULL;
	IXMLDOMAttribute *pAttributeOut = NULL;	// Intentionally unused //

	BSTR bstrName = NULL;
	VARIANT varValue;
	VariantInit(&varValue);

	// Pass in arguments // 
	bstrName = SysAllocString(wszName);
	CHK_ALLOC(bstrName);
	CHK_HR(VariantFromString(wszValue, varValue));

	/// Create & Add Attribute ///
	CHK_HR(pDoc->createAttribute(bstrName, &pAttribute));
	CHK_HR(pAttribute->put_value(varValue));
	CHK_HR(pParent->setAttributeNode(pAttribute, &pAttributeOut));

	/// Clean up ///
CleanUp:
	// Free pointers //
	SAFE_RELEASE(pAttribute);
	SAFE_RELEASE(pAttributeOut);

	// Free strings //
	SysFreeString(bstrName);

	// Free variants //
	VariantClear(&varValue);

	// Output //
	return hr;
}
HRESULT AddNode_Text(IXMLDOMDocument *pDoc, IXMLDOMNode *pParent, PCWSTR wszText)
{	/// Initialize ///
	HRESULT hr = S_OK;
	IXMLDOMText *pText = NULL;

	BSTR bstrText = SysAllocString(wszText);
	CHK_ALLOC(bstrText);

	/// Create & Add Text ///
	CHK_HR(pDoc->createTextNode(bstrText, &pText));
	CHK_HR(AppendNode(pParent, pText));

	/// Clean up ///
CleanUp:
	// Free pointers //
	SAFE_RELEASE(pText);

	// Free strings //
	SysFreeString(bstrText);

	// Output //
	return hr;
}

// I/O Methods //
HRESULT LoadDocument(PCWSTR filename, IXMLDOMDocument *pDoc)
{	/// Initialize ///
	HRESULT hr = S_OK;

	IXMLDOMParseError *pXMLErr = NULL;	// Parse errors //

	BSTR bstrXML = NULL;
	BSTR bstrErr = NULL;

	VARIANT_BOOL varStatus;
	VARIANT varFilename;
	VariantInit(&varFilename);

	/// Load the document ///
	CHK_HR(VariantFromString(filename, varFilename));
	CHK_HR(pDoc->load(varFilename, &varStatus));

	// Status Checking //
	if (varStatus == VARIANT_TRUE)
	{	// Pass the xml into memory //
		CHK_HR(pDoc->get_xml(&bstrXML));
		printf("XML loaded from %S:\n%S\n", filename, bstrXML);
	}
	else
	{	// Failed to load the data file! //
		CHK_HR(pDoc->get_parseError(&pXMLErr));
		CHK_HR(pXMLErr->get_reason(&bstrErr));
		printf("Failed to load XML from %S:\n%S\n", filename, bstrErr);
	}
	printf("\n");

	/// Clean up ///
CleanUp:
	// Free pointers //
	SAFE_RELEASE(pXMLErr);

	// Free strings //
	SysFreeString(bstrXML);
	SysFreeString(bstrErr);

	// Free variants //
	VariantClear(&varFilename);

	// Output //
	return hr;
}
HRESULT SaveDocument(PCWSTR filename, IXMLDOMDocument *pDoc)
{	/// Initialize ///
	HRESULT hr = S_OK;

	VARIANT varFilename;
	VariantInit(&varFilename);
	VariantFromString(filename, varFilename);
	
	/// Save Document ///
	CHK_HR(pDoc->save(varFilename));
	printf("Output saved to `%S`\n", filename);

	/// Clean up ///
CleanUp:
	// Free variants //
	VariantClear(&varFilename);

	// Output //
	return hr;
}