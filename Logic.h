#pragma once
/// INCLUDE ///
// External //
#include <stdio.h>	// Standard libraries		//
#include <string>
#include <regex>
#include <vector>

#include <MsXml6.h>	// For XML functionality	//

// Internal //
#include "MACROS.h"
#include "XMLfactory.h"
#include "Operation.h"
#include "City.h"

/// FUNCTION DEFINITIONS ///
// Conversions between BSTR and std::string for use in regular expressions //
std::string BSTR2str(BSTR bstr)
{	// Define a wide string //
	std::wstring wstr = bstr;
	if (wstr.empty()) return std::string();	// If there's nothing, then return nothing //

	// Allocate memory //
	int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string str(sizeNeeded, 0);

	// Write to string //
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str[0], sizeNeeded, NULL, NULL);
	return str;
}
BSTR Str2BSTR(std::string str)
{	
	if (str.empty()) return NULL;			// If there's nothing, then return nothing //

	// Allocate memory //
	int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	BSTR bstr = SysAllocStringLen(NULL, sizeNeeded);

	// Write to string //
	MultiByteToWideChar(CP_UTF8, 0, &str[0], str.length(), bstr, sizeNeeded);
	return bstr;
}

// For performing operations on city data //
double PerformOperation(std::vector<City> cities, Operation op)
{	// Analyzes the Operation `op` and applies it to the list of cities `cities`.//

	/// Initialize ///
	// Depending on the operation function, initialize the result to 0 or the maximum double //
	double res = (wcscmp(op.Function, L"min") == 0) ? DBL_MAX : 0.0;

	// For the average function, we need to keep track of the number of cities used //
	unsigned int count_cities = 0;

	// Obtain the regular expression needed to filter out the cities //
	std::regex reg = std::regex(BSTR2str(op.Filter));

	/// Iterate through cities ///
	for (City c : cities)
	{	// Ensure that this city's name fits the filter //
		if (!std::regex_match(BSTR2str(c.Name), reg)) continue;

		// Now that we know we're dealing with a city in the filter, check the data attribute to use //
		double data_value = 0.0;
		if (wcscmp(op.Attribute, L"population") == 0)	data_value = c.Population;
		else											data_value = c.Area;

		// Then check which function we are using //
		if (wcscmp(op.Function, L"average") == 0 || wcscmp(op.Function, L"sum") == 0)
			res += data_value;
		else if (wcscmp(op.Function, L"max") == 0)
			res = (data_value > res) ? data_value : res;
		else if (wcscmp(op.Function, L"min") == 0)
			res = (data_value < res) ? data_value : res;

		// This city has been added into the list of usable cities //
		count_cities++;
	}

	// Check if we used the `average` function and divide by the number of passed cities //
	if (wcscmp(op.Function, L"average") == 0 && count_cities > 0)
		res /= (double)count_cities;	// No integer division and no division by zero! //

	/// Output ///
	return res;
}

// Main function to run //
HRESULT RUN(PCWSTR file_dat, PCWSTR file_ops, PCWSTR file_out)
{	/// Initialize Everything ///
	HRESULT hr = S_OK;

	// Parameters //
	long len_ops, len_dat;
	std::vector<City> cities;

	// XML Items //
	IXMLDOMDocument *pDoc_dat = NULL;	// Document for data		//
	IXMLDOMDocument *pDoc_ops = NULL;	// Document for operations	//
	IXMLDOMDocument *pDoc_out = NULL;	// Document for output		//

	IXMLDOMNodeList *pNodes = NULL;	// A recycled node list		//
	IXMLDOMNode *pNode = NULL;		// A recycled singular node	//	

	IXMLDOMElement *pRoot = NULL;	// The root element of the output	//
	IXMLDOMElement *pElem = NULL;	// A recycled element				//
	IXMLDOMNode *pElemOut = NULL;	// An unused recycled element		//

									// Bytestrings //
	BSTR bstrQuery_Data = SysAllocString(L"//data[1]/*");		// Data node query		//
	BSTR bstrQuery_Ops = SysAllocString(L"//operations[1]/*");	// Operation node query //

	BSTR bstr = NULL;	// Temporary storage string //

						/// Checking and XML Initialization ///
						// Check allocations //
	CHK_ALLOC(bstrQuery_Data);
	CHK_ALLOC(bstrQuery_Ops);

	// Initialize the DOM for each document //
	CHK_HR(InitDOM(&pDoc_dat));
	CHK_HR(InitDOM(&pDoc_ops));
	CHK_HR(InitDOM(&pDoc_out));

	// Load in data and operations files //
	CHK_HR(LoadDocument(file_dat, pDoc_dat));	// Load the data document //
	CHK_HR(LoadDocument(file_ops, pDoc_ops));	// Load in the operations document //

												// Initialize output file //
	CHK_HR(CreateElement(pDoc_out, L"results", &pRoot));

	/// Perform operations on data //	
	// Load in each city's information //
	CHK_HR(pDoc_dat->selectNodes(bstrQuery_Data, &pNodes));
	CHK_HR(pNodes->get_length(&len_dat));
	printf("City Data:\n-----------\n");
	for (long i = 0; i < len_dat; i++)
	{	// Get this data node from the list //
		CHK_HR(pNodes->get_item(i, &pNode));

		// And make it into an object and store for simplicity //
		City c = City(pNode);
		cities.emplace_back(c);

		// Debugging //
		printf("%S:  \tpop %i,\tarea %.2f\n", c.Name, c.Population, c.Area);
	}
	printf("\n");
	SAFE_RELEASE(pNodes);	// Clean it out //

							// Perform each operation //
	CHK_HR(pDoc_ops->selectNodes(bstrQuery_Ops, &pNodes));
	CHK_HR(pNodes->get_length(&len_ops));
	printf("\nOperations:\n-----------\n");
	for (long i = 0; i < len_ops; i++)
	{	// Get this operation from the list //
		CHK_HR(pNodes->get_item(i, &pNode));

		// And make it into an object for simplicity //
		Operation op = Operation(pNode);
		double op_res = PerformOperation(cities, op);

		// Okay, now we need to start creating the output nodes //
		CHK_HR(CreateElement(pDoc_out, L"result", &pElem));
		CHK_HR(AddNode_Attribute(pDoc_out, pElem, L"name", op.Name));

		// Convert result from double --> string (%.2f) --> BSTR //
		std::string str(16, '\0');
		str.resize(std::snprintf(&str[0], str.size(), "%.2f", op_res));

		// Append result to node value //
		CHK_HR(pElem->put_text(Str2BSTR(str)));
		CHK_HR(AddNode_Text(pDoc_out, pRoot, L"\n\t"));	// Add in a newline + tab //
		CHK_HR(pRoot->appendChild(pElem, &pElemOut));	// Only append when done //

														// Debugging //
		printf("%S: func %S, attr %S, filter %S | result: %.2f\n", op.Name, op.Function, op.Attribute, op.Filter, op_res);
	}

	// Append root to document //
	CHK_HR(AddNode_Text(pDoc_out, pRoot, L"\n"));	// Add in a newline //
	CHK_HR(pDoc_out->appendChild(pRoot, &pElemOut));

	// Debugging //
	CHK_HR(pDoc_out->get_xml(&bstr));
	printf("\nOutput XML:\n\n%S\n\n", bstr);
	SysFreeString(bstr);

	/// Save to output file ///
	SaveDocument(file_out, pDoc_out);

	/// Clean up ///
CleanUp:
	// Free pointers //
	SAFE_RELEASE(pDoc_dat);
	SAFE_RELEASE(pDoc_ops);
	SAFE_RELEASE(pDoc_out);

	SAFE_RELEASE(pNodes);
	SAFE_RELEASE(pNode);

	SAFE_RELEASE(pRoot);
	SAFE_RELEASE(pElem);
	SAFE_RELEASE(pElemOut);

	// Free strings //
	SysFreeString(bstrQuery_Data);
	SysFreeString(bstrQuery_Ops);
	SysFreeString(bstr);

	// Output //
	return hr;
}