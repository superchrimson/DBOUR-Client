//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/AnchorsAndShapesFromIMesh.h"
#include "libs/PathEngine_Interface/interface/cMesh.h"
#include "libs/Content_Processing/interface/AnchorsAndPinnedShapes.h"
#include "libs/Geometry/interface/tPoint.h"

static void
AddAnonymousAnchor(const cMesh& mesh, const cPosition& position, float anchorHeight, cAnchorsAndPinnedShapes& result)
{
    result._anchors.push_back(cAnchor());
    cAnchor& added = result._anchors.back();
    float groundHeight = mesh.heightAtPositionF(position);
    added._x = position.x;
    added._y = position.y;
    added._zStart = groundHeight - anchorHeight / 2.f;
    added._zEnd = groundHeight + anchorHeight / 2.f;
    added._resolvesToTerrain = (mesh.getSectionID(position) == 0);
}

void
AnchorsAndShapesFromIMesh(const iMesh& meshI, float anchorHeight, cAnchorsAndPinnedShapes& result)
{
    const cMesh& mesh = static_cast<const cMesh&>(meshI);
    const std::vector<cObstacleInfo>& baseObstacles = mesh.refBaseObstacles();
    const std::vector<cCollisionShape>& baseObstacleShapes = mesh.refBaseObstacleShapes();
    long i;
    for(i = 0; i != mesh.getNumberOfAnchors(); ++i)
    {
        result._anchors.push_back(cAnchor());
        cAnchor& added = result._anchors.back();
        cPosition position;
        const char* id;
        mesh.retrieveAnchorByIndex(i, position, added._orientation, id);
        added._name = std::string(id);
        float groundHeight = mesh.heightAtPositionF(position);
        added._x = position.x;
        added._y = position.y;
        added._zStart = groundHeight - anchorHeight / 2.f;
        added._zEnd = groundHeight + anchorHeight / 2.f;
        added._resolvesToTerrain = (mesh.getSectionID(position) == 0);
    }
    for(i = 0; i != SizeL(baseObstacles); ++i)
    {
        result._pinnedShapes.push_back(cPinnedShape());
        cPinnedShape& added = result._pinnedShapes.back();
        added._type = PE_PinnedShapeType_BurntIn;
        cInternalPosition p = baseObstacles[i]._position;
        cPosition cp;
        mesh.convertPosition(p, cp);
        added._anchorIndex = SizeL(result._anchors);
        AddAnonymousAnchor(mesh, cp, anchorHeight, result);
        const cCollisionShape& cs = baseObstacleShapes[i];
        long j;
        for(j = 0; j != cs.size(); ++j)
        {
            added._coords.push_back(cs.getVertex(j).getX());
            added._coords.push_back(cs.getVertex(j).getY());
        }
    }
    for(i = 0; i != mesh.getNumberOfNamedObstacles(); ++i)
    {
        result._pinnedShapes.push_back(cPinnedShape());
        cPinnedShape& added = result._pinnedShapes.back();
        added._type = PE_PinnedShapeType_NamedObstacle;
        iAgent* agent;
        const char* id;
        mesh.retrieveNamedObstacleByIndex(i, agent, id);
        added._name = id;
        cPosition p = agent->getPosition();
        added._anchorIndex = SizeL(result._anchors);
        AddAnonymousAnchor(mesh, p, anchorHeight, result);
        const iShape* shape = agent->getShape();
        const cCollisionShape& cs = static_cast<const cShape*>(shape)->refCS();
        long j;
        for(j = 0; j != cs.size(); ++j)
        {
            added._coords.push_back(cs.getVertex(j).getX());
            added._coords.push_back(cs.getVertex(j).getY());
        }
    }
}
