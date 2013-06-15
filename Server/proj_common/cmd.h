#include "network/packet.h"

#pragma pack(push,1)

DEFINEUSERCMD1(stGateWayBaseCmd,200);

#define GATEWAYMSG 0
struct stGateWayMsg:stGateWayBaseCmd<GATEWAYMSG> {
	unsigned int id;
	stZeroArray<char> msgBuffer; 
};

#pragma pack(pop)