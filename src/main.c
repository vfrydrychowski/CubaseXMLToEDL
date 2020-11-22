#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

typedef struct marker_s{
	char* name;
	float start;
	float duration;
}Marker;

typedef struct marker_tab{
	int size;
	Marker* markerTab;
}Marker_tab;

void getMarkerValue (xmlDocPtr doc) {

    xmlNodePtr cur;
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
		printf("Error in xmlXPathNewContext\n");
        exit(1);
	}

	xmlXPathObjectPtr result= xmlXPathEvalExpression((xmlChar*)"//list", context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
        printf("No result\n");
	}

    cur=NULL;
    //cur=result->nodesetval->nodeTab[0];//a refaire
    for (int i =0; (i<result->nodesetval->nodeNr) && !(xmlStrcmp((xmlChar*)"Events", xmlGetProp(result->nodesetval->nodeTab[i], (xmlChar*)"name"))); i++){
        cur=result->nodesetval->nodeTab[i];
    }
    
    xmlNodePtr curi;
    
    cur = xmlFirstElementChild(cur);
    
    
    
    xmlChar *c;
    int fin = 0;
	while (cur != NULL) {
        curi = xmlFirstElementChild(cur);
        fin=0;
        while((curi !=NULL) && !fin){
            if(!xmlStrcmp(curi->name, (xmlChar*)"string")){
                fin =1;
                c = xmlGetProp(curi, (xmlChar*)"value");
                printf("%s\n", c);
            }
            curi = xmlNextElementSibling(curi);
        }
	    cur = xmlNextElementSibling(cur);
	} 
}


void parseDoc(char *docname) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	
	/* if (xmlStrcmp(cur->name, (const xmlChar *) "story")) {
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return;
	} */
	
	getMarkerValue (doc);
	xmlFreeDoc(doc);
	return;
}

int main(int argc, char **argv) {

	char *docname;
	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	parseDoc (docname);
	
	return (1);
}
