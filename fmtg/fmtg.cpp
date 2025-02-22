#include "fmtg.h"

uint16_t addr;

fmtg::fmtg(uint16_t src, uint16_t dst, uint16_t is, uint16_t ir, byte type, uint8_t ischannel, uint8_t irchannel)
{
	this->src = src;
	this->dst = dst;
	this->is = is;
	this->ir = ir;
	this->type = type;
	this->ischannel = ischannel;
	this->irchannel = irchannel;
	memset(this->payload, 0, sizeof(this->payload));
}

void fmtg::attachPayload(byte type, byte buff[], int n)
{
	if(n<17)
	{
		this->payload[0] = type;
		memcpy(&(this->payload[1]), buff, n);
	}
}
