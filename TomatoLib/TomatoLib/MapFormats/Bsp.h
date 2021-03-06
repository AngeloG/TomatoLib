#pragma once
#ifndef __TL__BSP_H_
#define __TL__BSP_H_

#include "../Graphics/Render.h"
#include "../Math/Vector3.h"
#include "../Utilities/List.h"
#include "../Game/Camera.h"
#include <string>

#define IDBSPHEADER	(('P'<<24)+('S'<<16)+('B'<<8)+'V')
#define HEADER_LUMPS 64

#define BSP_LUMPS_ENTITIES 1

namespace TomatoLib {
	namespace MapFormats {
		namespace Lumps {
			enum Lumps {
				Entities, Planes, TexData, Vertices, Visibility, Nodes, TexInfo, Faces, Lightning, Occlusion, Leafs, FaceIDs, Edges, Surfedges, Models, WorldLights, LeafFaces, LeafBrushes, Brushes, BrushSides, Areas, AreaPortals, PropCollision, PropHulls, PropHullVertices, PropTris, DisplayInfo, OriginalFaces, PhysDisp, PhysCollide, VertNormals, VertNormalIndices, DisplayLightmapAlphas, DisplayVertices, DisplayLightmapSamplePositions, Game, LeafWaterData, Primitives, PrimitiveVertices, PrimitiveIndices, PakFile, ClipPortalVertices, Cubemaps, TexDataStringData, TexDataStringTable, Overlays, LeafMindIsToWater, FaceMacroTextureInfo, DisplayTris, PropBlob, WaterOverlays, LeafAmbientIndexHDR, LeafAmbientIndex, LightningHDR, WorldLightsHDR, LeafAmbientLightningHDR, LeafAmbientLightning, XZipPakfile_depricated, FacesHDR, MapFlags, OverlayFades, OverlaySystemLevels, PhysLevel, DisplayMutliBlend
			};
		}
	}

	struct dmodel_t {
		Vector3	mins, maxs;		// bounding box
		Vector3	origin;			// for sounds or lights
		int	headnode;		// index into node array
		int	firstface, numfaces;	// index into face array
	};

	struct dtexdata_t {
		Vector3	reflectivity;		// RGB reflectivity
		int	nameStringTableID;	// index into TexdataStringTable
		int	width, height;		// source image
		int	view_width, view_height;
	};

	struct dgamelumpheader_t {
		int lumpCount;	// number of game lumps
	};
	
	struct dgamelump_t {
		int		id;		// gamelump ID
		unsigned short	flags;		// flags
		unsigned short	version;	// gamelump version
		int		fileofs;	// offset to this gamelump
		int		filelen;	// length
	};

	struct texinfo_t {
		float	textureVecs[2][4];	// [s/t][xyz offset]
		float	lightmapVecs[2][4];	// [s/t][xyz offset] - length is in units of texels/area
		int	flags;			// miptex flags	overrides
		int	texdata;		// Pointer to texture name, size, etc.
	};

	struct dleaf_t {
		int			contents;		// OR of all brushes (not needed?)
		short			cluster;		// cluster this leaf is in
		short			area : 9;			// area this leaf is in
		short			flags : 7;		// flags
		short			mins[3];		// for frustum culling
		short			maxs[3];
		unsigned short		firstleafface;		// index into leaffaces
		unsigned short		numleaffaces;
		unsigned short		firstleafbrush;		// index into leafbrushes
		unsigned short		numleafbrushes;
		short			leafWaterDataID;	// -1 for not in water

		//!!! NOTE: for maps of version 19 or lower uncomment this block
		/*
		CompressedLightCube	ambientLighting;	// Precaculated light info for entities.
		short			padding;		// padding to 4-byte boundary
		*/
	};

	struct dnode_t {
		int		planenum;	// index into plane array
		int		children[2];	// negative numbers are -(leafs + 1), not nodes
		short		mins[3];	// for frustum culling
		short		maxs[3];
		unsigned short	firstface;	// index into face array
		unsigned short	numfaces;	// counting both sides
		short		area;		// If all leaves below this node are in the same area, then
		// this is the area index. If not, this is -1.
		short		paddding;	// pad to 32 bytes length
	};

	struct dbrushside_t {
		unsigned short	planenum;	// facing out of the leaf
		short		texinfo;	// texture info
		short		dispinfo;	// displacement info
		short		bevel;		// is the side a bevel plane?
	};

	struct dbrush_t {
		int	firstside;	// first brushside
		int	numsides;	// number of brushsides
		int	contents;	// contents flags
	};

	struct dface_t {
		unsigned short	planenum;		// the plane number
		unsigned char		side;			// faces opposite to the node's plane direction
		unsigned char		onNode;			// 1 of on node, 0 if in leaf
		int		firstedge;		// index into surfedges
		short		numedges;		// number of surfedges
		short		texinfo;		// texture info
		short		dispinfo;		// displacement info
		short		surfaceFogVolumeID;	// ?
		unsigned char		styles[4];		// switchable lighting info
		int		lightofs;		// offset into lightmap lump
		float		area;			// face area in units^2
		int		LightmapTextureMinsInLuxels[2];	// texture lighting info
		int		LightmapTextureSizeInLuxels[2];	// texture lighting info
		int		origFace;		// original face this was split from
		unsigned short	numPrims;		// primitives
		unsigned short	firstPrimID;
		unsigned int	smoothingGroups;	// lightmap smoothing group
	};

	struct dedge_t {
		unsigned short	start;	// vertex indices
		unsigned short	end;	// vertex indices
	};
	
	struct doverlay_t {
		int		Id;
		short		TexInfo;
		unsigned short	FaceCountAndRenderOrder;
		int		Ofaces[64];
		float		U[2];
		float		V[2];
		Vector3		UVPoints[4];
		Vector3		Origin;
		Vector3		BasisNormal;
	};

	struct dplane_t {
		Vector3	normal;	// normal vector
		float	dist;	// distance from origin
		int	type;	// plane axis identifier
	};

	struct bsp_lzma_header_t {
		unsigned int	id;
		unsigned int	actualSize;		// always little endian
		unsigned int	lzmaSize;		// always little endian
		unsigned char	properties[5];
	};

	struct bsp_lump_t {
		int	fileofs;	// offset into file (bytes)
		int	filelen;	// length of lump (bytes)
		int	version;	// lump format version
		char	fourCC[4];	// lump ident code
	};

	struct bsp_header_t {
		int	ident;                // BSP file identifier
		int	version;              // BSP file version
		bsp_lump_t	lumps[HEADER_LUMPS];  // lump directory array
		int	mapRevision;          // the map's revision (iteration, version) number
	};

	struct bsp_mesh_vertex_t {
		Vector3 pos;
		float u;
		float v;
	};

	struct bsp_pak_file_t {
		char fileName[128];
		unsigned int length;
		unsigned char* data;
	};

	struct bsp_mesh_t {
		int TextureID;
		Texture Texture;

		List<bsp_mesh_vertex_t> Vertices;
		List<GLushort> Indices;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
	};

	class Bsp {
		FILE* FilePtr;

	public:
		Shader shader;

		bsp_header_t Header;
		List<bsp_mesh_t*> Meshes;

		Bsp(std::string file);
		~Bsp();

		void Draw(Render& r, Camera& cam);
		void DoGLStuff();
	};
}

#endif