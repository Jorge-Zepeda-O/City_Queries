/// INCLUDES ///
// External //
#include <stdio.h>	// Standard libraries		//
#include <MsXml6.h>	// For XML functionality	//

// Internal //
#include "MACROS.h"
#include "City.h"

/// METHOD DEFINITIONS ///
// Constructor / Destructor //
City::City(IXMLDOMNode* pNode)
{	/// Initialize ///
	HRESULT hr = S_OK;

	IXMLDOMNamedNodeMap *pAttrMap;
	IXMLDOMNode *pElem = NULL;
	IXMLDOMNode *pAttr = NULL;
	
	BSTR bstr = NULL;

	VARIANT_BOOL varStatus;
	VARIANT varData;
	VariantInit(&varData);

	/// Attribute Query ///
	CHK_HR(pNode->get_attributes(&pAttrMap));

	// Get the name attribute //
	bstr = SysAllocString(L"name");
	CHK_ALLOC(bstr);
	CHK_HR(pAttrMap->getNamedItem(bstr, &pAttr));
	CHK_HR(pAttr->get_nodeValue(&varData));
	Name = SysAllocString(varData.bstrVal);

	// Get the population attribute //
	bstr = SysAllocString(L"population");
	CHK_ALLOC(bstr);
	CHK_HR(pAttrMap->getNamedItem(bstr, &pAttr));
	CHK_HR(pAttr->get_nodeValue(&varData));
	Population = (long)wcstod(SysAllocString(varData.bstrVal), NULL);

	// Get the area element //
	bstr = SysAllocString(L"area");
	CHK_ALLOC(bstr);
	CHK_HR(pNode->selectSingleNode(bstr, &pElem));
	CHK_HR(pElem->get_text(&bstr));
	Area = wcstod(SysAllocString(bstr), NULL);

	/// Clean up //
CleanUp:
	// Free pointers //
	SAFE_RELEASE(pAttrMap);
	SAFE_RELEASE(pElem);
	SAFE_RELEASE(pAttr);

	// Free strings //
	SysFreeString(bstr);

	// Free variants //
	VariantClear(&varData);
}