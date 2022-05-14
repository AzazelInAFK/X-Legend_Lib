//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Code produced by Azazel.			                                                                //
//You can contact me on Ragezone here "https://forum.ragezone.com/members/2000318605.html".		        //
//I take no responsaiblity over this code or how it is used.						        //
//This code was tested on a few Eden Eternal files, it is meant to be used in newer games, if you're modifying  //
//Grand Fantasia or any other older game, use "FSM.h" instead of this one.                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FSM2.h"
#include "File.h"
#include <iostream>

#define CellStride 0x1AAC8 - 0x80

bool FSM2::Load(const char* pcFileName) {
	Seraph::File FSM;
	Assert(FSM.open(pcFileName, std::ios::in | std::ios::binary));
	Assert(FSM.read(m_acSignature, 0x54));
	Assert(FSM.read(&m_ulHeightMaps));

	Assert(FSM.read(&m_HMHeader));
	m_pCells = new Cell[m_HMHeader.m_ulNumCells];
	for (unsigned long ulCell{ 0ul }; ulCell < m_HMHeader.m_ulNumCells; ulCell++) {
		Assert(FSM.read(&m_pCells[ulCell].m_Header));
		memset(&m_pCells[ulCell].m_DarkMap, 0, sizeof(DarkMap));
		VertexData** ppVertices = new VertexData * [m_pCells[ulCell].m_Header.m_ulHeight];
		m_pCells[ulCell].m_ppVertices = ppVertices;
		for (unsigned long Y{ 0ul }; Y < m_pCells[ulCell].m_Header.m_ulHeight; Y++) {
			ppVertices[Y] = new VertexData[m_pCells[ulCell].m_Header.m_ulWidth];
			Assert(FSM.read(ppVertices[Y], m_pCells[ulCell].m_Header.m_ulWidth));
		}
		unsigned long ulSkip = CellStride - m_pCells[ulCell].m_Header.m_ulHeight *
			m_pCells[ulCell].m_Header.m_ulWidth *
			sizeof(VertexData);

		if (ulSkip != 0) // skip the remaining.
			FSM.ignore(ulSkip);

		Assert(FSM.read(&m_pCells[ulCell].m_DarkMap));
	}

	Assert(FSM.read(&m_ObjCoreHeader));
	m_pObjInstances = new InstancedObject[m_ObjCoreHeader.m_ulNumInstancedObjects];
	Assert(FSM.read(m_pObjInstances, m_ObjCoreHeader.m_ulNumInstancedObjects));
	Assert(FSM.read(&m_ObjInstanceHeader));
	m_pInstances = new InstanceData[m_ObjInstanceHeader.m_ulNumInstances];
	Assert(FSM.read(m_pInstances, m_ObjInstanceHeader.m_ulNumInstances));
	Assert(FSM.read(&m_ObjHeader));
	m_pObjects = new ObjectData[m_ObjHeader.m_ulNumObjects];
	Assert(FSM.read(m_pObjects, m_ObjHeader.m_ulNumObjects));

	return true;
}
