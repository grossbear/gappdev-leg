#include <stdio.h>
#include "ModelData/MeshData.h"

int main()
{
    uint32t vtxcnt = 12;
    CMeshData meshdata(vtxcnt);
    CVertIndicesDataOrganazier dataorg(vtxcnt);

    uint32t indices1[] = { 3,0,4,1,5,2 };
    dataorg.AppendPrimitiveDataIndices(MESH_PRIMITIVE_TYPE_TRIANGLE_STRIP, indices1, sizeof(indices1) / sizeof(uint32t));
    uint32t indices2[] = { 6,3,7,4,8,5 };
    dataorg.AppendPrimitiveDataIndices(MESH_PRIMITIVE_TYPE_TRIANGLE_STRIP, indices2, sizeof(indices2) / sizeof(uint32t));
    uint32t indices3[] = { 9,6,7,8 };
    dataorg.AppendPrimitiveDataIndices(MESH_PRIMITIVE_TYPE_TRIANGLE_FAN, indices3, sizeof(indices3) / sizeof(uint32t));
    uint32t indices4[] = { 11,9,10 };
    dataorg.AppendPrimitiveDataIndices(MESH_PRIMITIVE_TYPE_TRIANGLES, indices4, sizeof(indices4) / sizeof(uint32t));

    meshdata.CreateIndicesData(dataorg);

    int xn = 0;

    return 0;
}