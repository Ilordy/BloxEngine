#include "ObjLoader.h"
#include "core/blx_defines.h"
#include "core/blx_memory.h"
#include <cstddef>
BLXEXTERN_C_START
#include "core/blx_logger.h"
#include "core/blx_string.h"
#include "utils/blx_fileManagement.h"
BLXEXTERN_C_END
#include "maths/Vector3.h"
#include "utils/List.h"
#include "utils/Map.h"
#include "utils/MemHelper.h"
#include <cctype>
#include <cstdlib>
#include <functional>
#include <string.h>
#include <string>
#include <utility>

// TODO: Add ear clipping support for quads and polygons. we currently use fan triangulation which is not ideal for quads and polygons, but it works for now and we can add ear clipping support in the future when we need it.

using namespace BlxMath;

namespace BlxCore
{
    ObjLoader::ObjLoader(const char* objPath) :
        objPath((const char*)blxAllocate(blxStr_LenWithNull(objPath) * sizeof(char), BLXMEMORY_TAG_STRING)),
        vertexToIndexMap(),
        positions(4096),
        texCoords(4096),
        normals(4096)
    {
        // TODO: Objpath is probably only needed for debug mode.
        blxStr_Cpy(this->objPath, objPath);
    }

    ObjLoader::~ObjLoader()
    {
        blxFree((void*)objPath, BLXMEMORY_TAG_STRING);
    }

    bool ObjLoader::ParseVertexPosition(const char* line, Vector3& outPosition)
    {
        return sscanf(line, "v %f %f %f", &outPosition.x, &outPosition.y, &outPosition.z) == 3;
    }

    bool ObjLoader::ParseVertexNormal(const char* line, Vector3& outNormal)
    {
        return sscanf(line, "vn %f %f %f", &outNormal.x, &outNormal.y, &outNormal.z) == 3;
    }

    bool ObjLoader::ParseVertexTexCoord(const char* line, Vector2& outTexCoord)
    {
        return sscanf(line, "vt %f %f", &outTexCoord.x, &outTexCoord.y) == 2;
    }

    i32 ObjLoader::ResolveVertexIndex(i32 objIndex, u32 listSize)
    {
        // OBJ format uses 1-based indexing, so we need to subtract 1 from the index.
        // Negative indices are relative to the end of the list, so we need to add the list size to the index.
        if (objIndex > 0)
        {
            return objIndex - 1;
        }
        else if (objIndex < 0)
        {
            return objIndex + listSize;
        }

        // Invalid index
        return -1;
    }

    bool ObjLoader::ParseFace(char* line, BlxStl::List<VertexIndex>& outVertexIndices)
    {
        outVertexIndices.Clear();

        if (line[0] == 'f' && blxStr_Len(line) > 2)
        {
            line += 2; // Skip the "f " at the beginning of the line
        }

        char* context;
        while (const char* token = strtok_s(line, " ", &context))
        {
            u32 tokenLength = blxStr_Len(token);
            VertexIndex vertexIndex;
            char* endPtr;
            u32 slashIndex;
            // TODO: try catch error handling here when we make our own str class.
            // TODO: we can also use strtol to parse the integers and handle the error cases ourselves.
            i32 posIndex = ResolveVertexIndex(
                strtol(token, &endPtr, 10),
                positions.Count());

            if (posIndex == -1)
            {
                BLXERROR("Invalid vertex position index in face definition: %s\n In file: %s", token, objPath);
                return false;
            }

            vertexIndex.posIndex = posIndex;

            slashIndex = u32(endPtr - token);

            // Check if there are slashes (v/vt/vn or v//vn format)
            // If this if statement is not true, then the format is just "v" and we can skip parsing tex coord and normal.
            if (slashIndex < tokenLength && token[slashIndex] == '/')
            {
                /*
                Move past the first slash
                f v/vt/vn
                    ^ = slashIndex (vt)
                f v//vn
                    ^ = slashIndex (second '/')
                */
                slashIndex++;

                // Parse Tex coord if present
                if (slashIndex < tokenLength && std::isdigit(token[slashIndex]))
                {
                    vertexIndex.texIndex = ResolveVertexIndex(
                        strtol(token + slashIndex, &endPtr, 10),
                        texCoords.Count());

                    slashIndex = u32(endPtr - token);
                }

                /*
                Move past the second slash
                f v/vt/vn
                       ^ = slashIndex (vn)
                f v//vn
                     ^ = slashIndex (vn)
                */
                slashIndex++;

                // Parse normal if present
                if (slashIndex < tokenLength && std::isdigit(token[slashIndex]))
                {
                    vertexIndex.normIndex = ResolveVertexIndex(
                        strtol(token + slashIndex, &endPtr, 10),
                        normals.Count());
                }
            }

            outVertexIndices.Add(vertexIndex);
        }

        return true;
    }

    void ObjLoader::FanTriangulateFace(BlxStl::List<VertexIndex>& vertexIndices)
    {
        u32 originalCount = vertexIndices.Count();
        if (originalCount < 3)
        {
            return;
        }

        // A polygon with n vertices can be triangulated into n-2 triangles using fan triangulation,
        // where the first vertex is shared among all triangles.
        u32 numTriangles = originalCount - 2;

        // Resize the list to hold triangle indices.
        u32 newSize = numTriangles * 3;
        vertexIndices.Resize(newSize);

        // Fan triangulation: for a polygon with vertices v0, v1, v2, ...,
        // vn, we create triangles (v0, v1, v2), (v0, v2, v3), ..., (v0, vn-1, vn).
        VertexIndex firstVertex = vertexIndices[0];
        u32 writeIndex = 0;
        for (u32 i = 1; i < originalCount - 1; ++i)
        {
            vertexIndices[writeIndex++] = firstVertex;
            vertexIndices[writeIndex++] = vertexIndices[i];
            vertexIndices[writeIndex++] = vertexIndices[i + 1];
        }
    }

    bool ObjLoader::Load(const char* objPath, BlxRendering::Mesh** outMeshPtr)
    {
        blxFile* objFile;
        if (!blxOpenFile(objPath, BLX_FILE_MODE_READ, &objFile))
        {
            return false;
        }

        BlxStl::Map<VertexIndex, u32> vertexToIndexMap;

        BlxStl::List<Vector3> positions(4096);
        BlxStl::List<Vector3> normals(4096);
        BlxStl::List<Vector2> texCoords(4096);

        BlxStl::List<BlxRendering::Vertex> vertices;
        BlxStl::List<u32> indices;

        char lineBuffer[512];
        char* p = &lineBuffer[0];
        while (blxFileReadLine(objFile, 512, &p, nullptr))
        {
            char firstChar = lineBuffer[0];
            switch (firstChar)
            {
            case 'o':
            {
                // TODO: Object Name Line, we can ignore this for now since we are only loading one object per file, but in the future we will need to handle this when we add support for multiple objects in one file.
                break;
            }
            case 'v':
            {
                char secondChar = lineBuffer[1];
                switch (secondChar)
                {

                // Vertex Position
                case ' ':
                {
                    Vector3 position;
                    if (!ParseVertexPosition(lineBuffer, position))
                    {
                        BLXWARNING("Failed to parse vertex position from line: %s\n In file: %s", lineBuffer, objPath);
                    }
                    positions.Add(position);
                    break;
                }

                // Vertex Normal
                case 'n':
                {
                    Vector3 normal;
                    if (!ParseVertexNormal(lineBuffer, normal))
                    {
                        BLXWARNING("Failed to parse vertex normal from line: %s\n In file: %s", lineBuffer, objPath);
                    }
                    normals.Add(normal);
                    break;
                }

                // Vertex Tex Coord
                case 't':
                {
                    Vector2 texCoord;
                    if (!ParseVertexTexCoord(lineBuffer, texCoord))
                    {
                        BLXWARNING("Failed to parse vertex tex coord from line: %s\n In file: %s", lineBuffer, objPath);
                    }
                    texCoords.Add(texCoord);
                    break;
                }
                }
                break;
            }
            case 'f':
            {
                BlxStl::List<VertexIndex> vertexIndices;
                if (!ParseFace(lineBuffer, vertexIndices))
                {
                    return false;
                }

                if (vertexIndices.Count() < 3)
                {
                    BLXERROR("Face with less than 3 vertices found in line: %s\n In file: %s", lineBuffer, objPath);
                    return false;
                }

                FanTriangulateFace(vertexIndices);

                for (u64 i = 0; i < vertexIndices.Count(); ++i)
                {
                    VertexIndex& vertexIndex = vertexIndices[i];
                    u32 index;
                    if (vertexToIndexMap.TryGetValue(vertexIndex, index))
                    {
                        // Vertex already exists, add the existing index to the index list.
                        indices.Add(index);
                    }
                    else
                    {
                        // Vertex does not exist, add it to the vertex list and add the new index to the index list.
                        Vector3& position = positions[vertexIndex.posIndex];
                        Vector2& texCoord = texCoords[vertexIndex.texIndex == -1 ? 0 : vertexIndex.texIndex];
                        Vector3& normal = normals[vertexIndex.normIndex == -1 ? 0 : vertexIndex.normIndex];
                        BlxRendering::Vertex vertex(position, normal, texCoord);

                        vertices.Add(vertex);
                        index = vertices.Count() - 1;
                        vertexToIndexMap[vertexIndex] = index;
                        indices.Add(index);
                    }
                }

                break;
            }
            }

            return true;
        }
    }
} // namespace BlxCore
