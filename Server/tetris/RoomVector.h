#ifndef __CLT_GAME_ROOM_H_
#define __CLT_GAME_ROOM_H_

#include "tetris_global.h"
#include "syncobj.h"

#include <vector>

class CRoomVector:public CSyncVector<stRoomInfo>
{
public:
	struct stVisitor
	{
		virtual void Update(CRoomVector* ,int nId) = 0;
		virtual void Visit(CRoomVector* ) = 0;
	};

	void Update(stVisitor* pVisitor,int _nId)
	{
		pVisitor->Update(this,_nId);
	}

	void Accept(stVisitor* pVisitor) {
		pVisitor->Visit(this);
	}
};

#endif //__CLT_GAME_ROOM_H_