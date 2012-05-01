#pragma once
#include <vector>
#include <map>
#include "stdafx.h"

using namespace std;

typedef map<CStringA, CStringA> attribList;


typedef struct Element{
	char name[64];
	attribList attributes;
	int length;
	Element() : length(0) {}
	void Clear() {attributes.clear(); length=0; strcpy(name, "");}

} Element;

class XMLreader
{
public:
	XMLreader(void);
	~XMLreader(void);
	bool OpenXML(char * filename);
	void CloseXML();
	bool ReadElement(Element &element);
	bool WriteElement(const Element &element, FILE* fileOut);

private:
	FILE * xmlfile;
};
