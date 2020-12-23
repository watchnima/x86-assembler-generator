#include "compiler.h"

#include <libxml/parser.h>
#include "nasm.h"
#include "nasmlib.h"
#include "parseXML.h"
#include "error.h"
#include "model.h"

static const char *xmlfiles[2] =
{
    /* must put Consts before TK, so the TK can find relevant constGroup */
    "fixedpointConsts.xml",
    "fixedpointTK.xml"
};
char *TKpath = "/home/lawrence/Workspace/git/x86-assembler-generator/xmlmodel/tks";


static int getElemsSize(xmlNodePtr node)
{
    int num = 0;
    for (; node != NULL; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            num++;
        }
    }
    return num;
}

static void parseCGs(xmlNodePtr cgsNode)
{
    for (xmlNodePtr cgNode = cgsNode->children; cgNode != NULL; cgNode = cgNode->next) {
        if (cgNode->type != XML_ELEMENT_NODE)
            continue;

        int i = 0;
        WDTree *cgTree;
        constVal *cgVal;
        const char *key;
        struct hash_insert hi;

        cgTree = wdtree_create();
        cgTree->isleaf = true;
        cgTree->size = getElemsSize(cgNode->children);
        cgTree->consts = (constVal *)nasm_malloc(cgTree->size * sizeof(constVal));
        cgVal = cgTree->consts;

        for (xmlNodePtr cNode = cgNode->children; cNode != NULL; cNode = cNode->next) {
            if (cNode->type != XML_ELEMENT_NODE)
                continue;

            int imm = hex2dec((const char *)cNode->children->content);
            //const char *cNodeName = (const char *)cNode->name;
            //if (strcmp(cNodeName, "Imm8") == 0) {
            //    cgVal[i].imm8 = (uint8_t)imm;
            //} else if (strcmp(cNodeName, "Unity") == 0) {
            //    cgVal[i].unity = (uint8_t)imm;
            //} else if (strcmp(cNodeName, "Imm16") == 0) {
            //    cgVal[i].imm16 = (uint16_t)imm;
            //} else if (strcmp(cNodeName, "Imm32") == 0) {
                cgVal[i].imm32 = (uint32_t)imm;
            //} else {
            //    printf("0x%x\n", imm);
            //}
            i++;
        }

        key = (const char *)xmlGetProp(cgNode, (const unsigned char*)"name");
        hash_find(&hash_wdtrees, key, &hi);
        hash_add(&hi, key, (void *)cgTree);
    }
}

static WDTree *parseTK(xmlNodePtr tkNode)
{
    int i = 0;
    int *weights;
    const char *key;
    struct hash_insert hi;
    WDTree *tkTree;
    WDTree **subtrees;

    tkTree = wdtree_create();
    tkTree->size = getElemsSize(tkNode->children);
    tkTree->weights = (int *)nasm_malloc(tkTree->size * sizeof(int));
    tkTree->children = (WDTree **)nasm_malloc(tkTree->size * sizeof(WDTree *));
    weights = tkTree->weights;
    subtrees = tkTree->children;

    for (xmlNodePtr nNode = tkNode->children; nNode != NULL; nNode = nNode->next) {
        if (nNode->type != XML_ELEMENT_NODE)
            continue;

        weights[i] = atoi((const char *)xmlGetProp(nNode, (const unsigned char*)"weight"));
        key = (const char *)xmlGetProp(nNode, (const unsigned char*)"name");
        subtrees[i] = *(WDTree **)hash_find(&hash_wdtrees, key, &hi);
        i++;
    }

    return tkTree;
}

static void parseTKs(xmlNodePtr tksNode)
{
    for (xmlNodePtr tkNode = tksNode->children; tkNode != NULL; tkNode = tkNode->next) {
        if (tkNode->type != XML_ELEMENT_NODE)
            continue;

        const char *key;
        struct hash_insert hi;
        TKmodel *tkm;

        tkm = tkmodel_create();
        tkm->initP = atof((const char *)xmlGetProp(tkNode, (const unsigned char*)"initP"));

        if (xmlGetProp(tkNode, (const unsigned char *)"diffSrcDest")) {
            WDTree *tkSrcTree;
            WDTree *tkDestTree;
            for (xmlNodePtr nNode = tkNode->children; nNode != NULL; nNode = nNode->next) {
                if (nNode->type != XML_ELEMENT_NODE)
                    continue;

                if (strcmp((const char *)nNode->name, "Src") == 0) {
                    tkSrcTree = parseTK(nNode);
                } else if (strcmp((const char *)nNode->name, "Dest") == 0) {
                    tkDestTree = parseTK(nNode);
                }
            }
            tkm->wdsrctree = tkSrcTree;
            tkm->wddesttree = tkDestTree;
            tkm->diffSrcDest = true;
        } else {
            WDTree *tkTree;
            tkTree = parseTK(tkNode);
            tkm->wdtree = tkTree;
            tkm->diffSrcDest = false;
        }

        key = (const char *)xmlGetProp(tkNode, (const unsigned char*)"inst");
        hash_find(&hash_tks, key, &hi);
        hash_add(&hi, key, (void *)tkm);
    }
}

static void parseXML_file(const char *fname)
{
    LIBXML_TEST_VERSION
    xmlDocPtr doc = xmlParseFile(fname);
    if (doc != NULL) {
        xmlNodePtr node = doc->children;
        if (node->type == XML_ELEMENT_NODE) {
            const char *nodeName = (const char *)node->name;
            if (strcmp(nodeName, "CGs") == 0) {
                parseCGs(node);
            } else if (strcmp(nodeName, "TKs") == 0) {
                parseTKs(node);
            } else {
                nasm_nonfatal("failed to parse element: %s", nodeName);
            }
        }
    } else {
        nasm_nonfatal("Unable to open %s\n", fname);
    }
    xmlFreeDoc(doc);
}

void parse_TKs(void)
{
#ifdef LIBXML_READER_ENABLED
    char fname[1024];
    for (size_t i = 0; i < ARRAY_SIZE(xmlfiles); i++) {
        sprintf(fname, "%s/%s", TKpath, xmlfiles[i]);
        parseXML_file(fname);
        memset(fname, 0, 1024);
    }
#else
    nasm_fatal("XInclude support not compiled in\n");
#endif
}
