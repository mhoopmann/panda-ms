#include "StdAfx.h"
#include "XMLreader.h"

using namespace std;

XMLreader::XMLreader(void)
{
}

XMLreader::~XMLreader(void)
{
}


bool XMLreader::OpenXML(char * filename)
{
	xmlfile = fopen(filename, "r");
	return true;
}

bool XMLreader::ReadElement(Element &element)
{
	char line[512];
	char str[512];
	char * str2;
	char attribValue[256];
	char attribName[64];
	char scanString[256] = "%s</";

	if (feof(xmlfile)){
		return false;
	}
	if (!fgets(line, 512, xmlfile)){
		return false;
	}
	if (!sscanf(line, "<%s", str)){
		return false;
	}
	str2 = strtok(str, ">");
	strcpy(element.name, str2);

//	fgets(line, 512, xmlfile);
	fgets(line, 512, xmlfile);
  //MH: the line below will capture all characters until whitespace, so I made a modification
  //to keep the whitepace and continue copying.
	//sscanf(line, "<%s", str); 
  strcpy(str,&line[1]);
	str2 = strtok(str, "/><");

	while(strcmp(str2, element.name)!=0 && !feof(xmlfile)){
		strcpy(attribName, str2);
		str2 = strtok(NULL, "/><");
		strcpy(attribValue, str2);
		element.attributes.insert(make_pair(CStringA(attribName), CStringA(attribValue)));
		element.length++;
		fgets(line, 512, xmlfile);
    //MH: Same change as above
		//sscanf(line, "<%s", str);
    strcpy(str,&line[1]);
		str2 = strtok(str, "/><");
	}
	return true;
}

bool XMLreader::WriteElement(const Element &element, FILE* fileOut)
{
	fprintf(fileOut, "<%s>\n", element.name);
	for(attribList::const_iterator it = element.attributes.begin(); it != element.attributes.end(); ++it){
		fprintf(fileOut, "<%s>%s</%s>\n", it->first, it->second, it->first);
	}
	fprintf(fileOut, "</%s>\n", element.name);
	
	return true;
}

void XMLreader::CloseXML()
{
	fclose(xmlfile);
}