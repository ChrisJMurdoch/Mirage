
#include "utility/fbxLoader.hpp"

#include "utility/geometry.hpp"
#include "utility/resource.hpp"

#include <fbxsdk.h>

#include <iostream>
#include <string>
#include <format>

/// Human-readable attribute names
std::string getAttributeName(fbxsdk::FbxNodeAttribute::EType type)
{
    switch(type) {
        case fbxsdk::FbxNodeAttribute::eUnknown: return "unknown";
        case fbxsdk::FbxNodeAttribute::eNull: return "null";
        case fbxsdk::FbxNodeAttribute::eMarker: return "marker";
        case fbxsdk::FbxNodeAttribute::eSkeleton: return "skeleton";
        case fbxsdk::FbxNodeAttribute::eMesh: return "mesh";
        case fbxsdk::FbxNodeAttribute::eNurbs: return "nurbs";
        case fbxsdk::FbxNodeAttribute::ePatch: return "patch";
        case fbxsdk::FbxNodeAttribute::eCamera: return "camera";
        case fbxsdk::FbxNodeAttribute::eCameraStereo: return "stereo";
        case fbxsdk::FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
        case fbxsdk::FbxNodeAttribute::eLight: return "light";
        case fbxsdk::FbxNodeAttribute::eOpticalReference: return "optical reference";
        case fbxsdk::FbxNodeAttribute::eOpticalMarker: return "marker";
        case fbxsdk::FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
        case fbxsdk::FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
        case fbxsdk::FbxNodeAttribute::eBoundary: return "boundary";
        case fbxsdk::FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
        case fbxsdk::FbxNodeAttribute::eShape: return "shape";
        case fbxsdk::FbxNodeAttribute::eLODGroup: return "lodgroup";
        case fbxsdk::FbxNodeAttribute::eSubDiv: return "subdiv";
        default: return "error";
    }
}

/// Human-readable translation, rotation, scale details
std::string getDetails(fbxsdk::FbxNode *node)
{
    FbxDouble3 t = node->LclTranslation.Get();
    FbxDouble3 r = node->LclRotation.Get();
    FbxDouble3 s = node->LclScaling.Get();
    return std::format(
        "T({},{},{}) R({},{},{}) S({},{},{})",
        t[0], t[1], t[2],
        r[0], r[1], r[2],
        s[0], s[1], s[2]
    );
}

void print(fbxsdk::FbxNode *node, std::string indent="   ")
{
    // Node data
    std::cout << indent << "Node " << node->GetName() << " " << getDetails(node) << std::endl;

    // Attribute data
    for(int i=0; i<node->GetNodeAttributeCount(); i++)
        std::cout << indent << " - Attribute[" << getAttributeName(node->GetNodeAttributeByIndex(i)->GetAttributeType()) << "] " << node->GetNodeAttributeByIndex(i)->GetName() << std::endl;

    // Recurse to child nodes
    for(int i=0; i<node->GetChildCount(); i++)
        print(node->GetChild(i), indent+"   ");
}

Mesh fbxLoader::loadFbx(char const *filepath, float const scale, bool const verbose)
{
    std::cout << "Loading FBX: " << filepath << std::endl;

    // Initialise memory manager for FBX SDK
    FbxResource<fbxsdk::FbxManager> manager{};

    // Initialise settings
    FbxResource<fbxsdk::FbxIOSettings> settings{manager, IOSROOT};

    // Initialise importer
    FbxResource<fbxsdk::FbxImporter> importer{manager, "Mirage FBX Importer"};
    falseThrow(importer->Initialize(filepath, -1, settings), std::string("Failed to load ") + filepath);

    // Create blank scene
    FbxResource<fbxsdk::FbxScene> scene(manager,"Imported Scene");

    // Import scene data
    importer->Import(scene);

    fbxsdk::FbxNode *root(scene->GetRootNode());
    if(root) {
        print(root);
        //for(int i=0; i<root->GetChildCount(); i++)
        //    print(root->GetChild(i));
    }

    std::cout << std::endl;

    return Mesh{std::vector<Vertex>{0}, std::vector<unsigned int>{0}};
}
