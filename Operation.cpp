/// INCLUDES ///
// External //
#include <stdio.h>	// Standard libraries		//
#include <string>
#include <MsXml6.h>	// For XML functionality	//

// Internal //
#include "MACROS.h"
#include "Operation.h"

/// METHOD DEFINITIONS ///
// Constructor / Destructor //
Operation::Operation(IXMLDOMNode* pNode)
{	/// Initialize ///
	HRESULT hr = S_OK;

	IXMLDOMNamedNodeMap *pAttrMap = NULL;
	IXMLDOMNode *pAttr = NULL;

	BSTR bstr = NULL;

	VARIANT_BOOL varStatus;
	VARIANT varData;
	VariantInit(&varData);

	/// Attribute Query ///
	// Get the attributes //
	CHK_HR(pNode->get_attributes(&pAttrMap));

	// Get the name attribute //
	bstr = SysAllocString(L"name");
	CHK_ALLOC(bstr);
	CHK_HR(pAttrMap->getNamedItem(bstr, &pAttr));
	CHK_HR(pAttr->get_nodeValue(&varData));
	Name = SysAllocString(varData.bstrVal);

	// Get the name attribute //
	bstr = SysAllocString(L"type");
	CHK_ALLOC(bstr);
	CHK_HR(pAttrMap->getNamedItem(bstr, &pAttr));
	CHK_HR(pAttr->get_nodeValue(&varData));
	Type = SysAllocString(varData.bstrVal);

	// Get the name attribute //
	bstr = SysAllocString(L"func");
	CHK_ALLOC(bstr);
	CHK_HR(pAttrMap->getNamedItem(bstr, &pAttr));
	CHK_HR(pAttr->get_nodeValue(&varData));
	Function = SysAllocString(varData.bstrVal);

	// Get the name attribute //
	bstr = SysAllocString(L"attrib");
	CHK_ALLOC(bstr);
	CHK_HR(pAttrMap->getNamedItem(bstr, &pAttr));
	CHK_HR(pAttr->get_nodeValue(&varData));
	Attribute = SysAllocString(varData.bstrVal);

	// Get the name attribute //
	bstr = SysAllocString(L"filter");
	CHK_ALLOC(bstr);
	CHK_HR(pAttrMap->getNamedItem(bstr, &pAttr));
	CHK_HR(pAttr->get_nodeValue(&varData));
	Filter = SysAllocString(varData.bstrVal);

	/// Clean up ///
CleanUp:
	// Free pointers //
	SAFE_RELEASE(pAttrMap);
	SAFE_RELEASE(pAttr);

	// Free strings //
	SysFreeString(bstr);

	// Free variants //
	VariantClear(&varData);
}