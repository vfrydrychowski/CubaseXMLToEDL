#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

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
    /* int i = 0;
    int fin =0;
    while((i<result->nodesetval->nodeNr) && (!fin)){
        if (xmlStrcmp(xmlGetProp(result->nodesetval->nodeTab[i], (xmlChar*)"name"), (xmlChar*)"Events")){
            fin = 1;
            cur = result->nodesetval->nodeTab[i];
        }
        i++;
    } */
    cur=NULL;
    cur=result->nodesetval->nodeTab[0];//a refaire
    for (int i =1; (i<result->nodesetval->nodeNr) && !(xmlStrcmp((xmlChar*)"Events", xmlGetProp(result->nodesetval->nodeTab[i], (xmlChar*)"name"))); i++){
        cur=result->nodesetval->nodeTab[i];
    }
    
    xmlNodePtr curi;
    //printf("%s\n", cur->name);
    //printf("%s\n", xmlGetProp(cur, (xmlChar*)"name"));
    
    cur = xmlFirstElementChild(cur);
    
    
    
    xmlChar *c;
    int fin = 0;
	while (cur != NULL) {
        curi = xmlFirstElementChild(cur);
		//printf("%s\n", curi->name);
        //printf("cur = %s curi = %s\n", cur->name, curi->name);
        fin=0;
        while((curi !=NULL) && !fin){
            //printf("%s\n", curi->name);
            if(!xmlStrcmp(curi->name, (xmlChar*)"string")){
                fin =1;
				//printf("%s\n", curi->name);
                c = xmlGetProp(curi, (xmlChar*)"value");
                printf("%s\n", c);
            }
            curi = xmlNextElementSibling(curi);
        }
	    cur = xmlNextElementSibling(cur);
	} 
	//xmlXPathFreeObject(result);
    //xmlFreeNode(cur);
    //xmlFreeNode(curi);
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
