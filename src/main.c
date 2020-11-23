#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

typedef struct marker_s{
	unsigned char* name;
	float start;
	float duration;
}Marker;

typedef struct marker_tab_s{
	int size;
	Marker* tab;
}Marker_tab;

void markerToString(Marker m){
	printf("name : %s start : %f duration : %f\n", m.name, m.start, m.duration);
}

int count_MarkerListNode(xmlNodePtr MarkerList){
	
	xmlNodePtr ptrNode = (xmlNodePtr)malloc(sizeof(xmlNode));
	
	ptrNode = MarkerList;
	int i=0;
	while(ptrNode != NULL){
		ptrNode = xmlNextElementSibling(ptrNode);
		i++;
	}
	return i;
}

void getMarkerValues (xmlDocPtr doc, Marker_tab* markerTab) {

    xmlNodePtr markerListNode = NULL;
	xmlNodePtr markerNode;
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

	//retrieve the right node
	markerListNode=result->nodesetval->nodeTab[0];
    for (int i =1; (i<result->nodesetval->nodeNr) && !(xmlStrcmp((xmlChar*)"Events", xmlGetProp(result->nodesetval->nodeTab[i], (xmlChar*)"name"))); i++){
        markerListNode=result->nodesetval->nodeTab[i];
    }
    printf("retrieve the node ok\n");
    markerListNode = xmlFirstElementChild(markerListNode);
	markerTab->size = count_MarkerListNode(markerListNode);
	markerTab->tab = (Marker*)malloc(sizeof(Marker)*markerTab->size);
	printf("markerList size : %d\n", markerTab->size);
    int fin = 0;
	for (int i = 0; i<markerTab->size; i++) {
        markerNode = xmlFirstElementChild(markerListNode);
        fin=0;
        while((markerNode !=NULL) && !fin){
            if(!xmlStrcmp(markerNode->name, (xmlChar*)"string")){
                fin =1;
                markerTab->tab[i].name = xmlGetProp(markerNode, (xmlChar*)"value");
            }
			if(!xmlStrcmp(markerNode->name, (xmlChar*)"float")){
				if(!xmlStrcmp(xmlGetProp(markerNode, (xmlChar*)"name"), (xmlChar*)"Start")){
					markerTab->tab[i].start = atof((char *)xmlGetProp(markerNode, (xmlChar*)"value"));
				}
				if(!xmlStrcmp(xmlGetProp(markerNode, (xmlChar*)"name"), (xmlChar*)"Length")){
					markerTab->tab[i].duration = atof((char *)xmlGetProp(markerNode, (xmlChar*)"value"));
				}
            }
            markerNode = xmlNextElementSibling(markerNode);
        }
	    markerListNode = xmlNextElementSibling(markerListNode);
	} 
}


xmlDocPtr parseDoc(char *docname) {

	xmlDocPtr doc;
	xmlNodePtr markerListNode;

	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}
	markerListNode = xmlDocGetRootElement(doc);
	if (markerListNode == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return NULL;
	}
	return doc;
}

int main(int argc, char **argv) {
	xmlDocPtr doc;
	char *docname;
	Marker_tab Mt;
	if (argc <= 1 && argc > 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	doc = parseDoc (docname);
	getMarkerValues(doc, &Mt);

	for(int i = 0; i<Mt.size; i++){
		markerToString(Mt.tab[i]);
	}
	
	return (1);
}
