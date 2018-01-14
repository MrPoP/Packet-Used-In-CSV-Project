// This is the main DLL file.

#include "stdafx.h"
#include "string.h"
#include "Packet.h"
#define Writing
#define Reading
#define Encrypt
bool Packet::Packet::Create()
{
		memset(Memory, 0L, sizeof(Memory));
		Stream = Memory;
		return true;
};
Packet::Packet::~Packet()
{
	this->Stream = NULL;
	this->Size = 0;
};
Packet::Packet& Packet::Packet::operator=(const char** Buff)
{
	if (this->Create() == true)
	{
		memcpy(Buff, this->Stream, sizeof(Buff));
#ifdef Encrypt
		Decrypted(this->Stream);
#endif
	}
	return *this;
}
void Packet::Packet::Seek(int POS)
{
	Stream = &Memory[POS];
}
void Packet::Packet::SeekForward(int POS)
{
	Stream = &Memory[Position + POS];
}
void Packet::Packet::SeekBackWord(int POS)
{
	Stream = &Memory[Position - POS];
}
void Packet::Packet::InitWriter()
{
	Stream = &Memory[4];
}
void Packet::Packet::ZeroFill(int value)
{
	if (Position + value >= MAX_SIZE)
		return;
	memset(Stream, 0, value);
	Stream += value;
}
void Packet::Packet::PrepareSeal(BOOL Server)
{
	if (Server == TRUE)
	{
		Write(ServerSeal, TQ_SEALSIZE);
	}
	else if (Server == FALSE)
	{
		Write(ClientSeal, TQ_SEALSIZE);
	}
}
void Packet::Packet::Finalize(USHORT type, BOOL Server)
{
	if (TQ_SEALSIZE > 0)
	{
		PrepareSeal(Server);
	}
	this->Size = Position;
	Seek(0);
	Write((USHORT)(this->Size - TQ_SEALSIZE));
	Write(type);
#ifdef Encrypt
	Encrypted(Stream);
#endif
}
bool Packet::Packet::Create(STRUCT Struct, USHORT type)
{
	memset(Memory, 0L, sizeof(Memory));
	Stream = Memory;
	WriteStruct(Struct);
	Finalize(type, TRUE);
	return true;
};
#ifdef Writing
void Packet::Packet::Write(Byte value)
{
	if (Position + 1 >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream++;
}
void Packet::Packet::Write(USHORT value)
{
	if (Position + 2 >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream += sizeof(USHORT);
}
void Packet::Packet::Write(ULONG value)
{
	if (Position + 8 >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream += sizeof(ULONG);
}
void Packet::Packet::Write(UINT value)
{
	if (Position + 4 >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream += sizeof(UINT);
}
void Packet::Packet::Write(SByte value)
{
	if (Position + 1 >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream ++;
}
void Packet::Packet::Write(short value)
{
	if (Position + 2 >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream += sizeof(short);
}
void Packet::Packet::Write(long value)
{
	if (Position + 8 >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream += sizeof(long);
}
void Packet::Packet::Write(int value)
{
	if (Position + 4 >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream += sizeof(int);
}
void Packet::Packet::Write(char value)
{
	if (Position + sizeof(char) >= MAX_SIZE)
		return;
	*Stream = (char*)value;
	Stream += sizeof(char);
}
void Packet::Packet::Write(String value)
{
	if (Position + sizeof(value) >= MAX_SIZE)
		return;
	for (int x = 0; x < sizeof(value); x++)
	{
		*Stream = (char*)value[x];
	}
	Stream += sizeof(value);
}
void Packet::Packet::Write(char* value)
{
	if (Position + sizeof(value) >= MAX_SIZE)
		return;
	for (int x = 0; x < sizeof(value); x++)
	{
		*Stream = (char*)value[x];
	}
	Stream += sizeof(value);
}
void Packet::Packet::Write(char* value, int Length)
{
	int amount = 0;
	if (Length > sizeof(value))
	{
		amount = (Length - sizeof(value));
	}
	else if (sizeof(value) > Length)
	{
		amount = (sizeof(value) - Length);
	}
	else
	{
		amount = 0;
	}
	Write(value);
	if (amount > 0)
	{
		ZeroFill(amount);
	}
}
void Packet::Packet::WriteStruct(void* value)
{
	if (Position + sizeof(value) >= MAX_SIZE)
		return;
	//memset(Stream, value, sizeof(value));
	memcpy(Stream, value, sizeof(value));
	Stream += sizeof(value);
}
#endif
#ifdef Reading
void Packet::Packet::ReadStruct(STRUCT value)
{
	if (Position + sizeof(value) >= MAX_SIZE)
		return;
	memcpy(value, Stream, sizeof(value));
	Stream += sizeof(value);
}
short Packet::Packet::ReadShort()
{
	if (Position + sizeof(short) >= MAX_SIZE)
		return 0;
	short value = (short)Stream;
	Stream += sizeof(short);
	return value;
}
int Packet::Packet::ReadInt()
{
	if (Position + sizeof(int) >= MAX_SIZE)
		return 0;
	int value = (int)Stream;
	Stream += sizeof(int);
	return value;
}
long Packet::Packet::ReadLong()
{
	if (Position + sizeof(long) >= MAX_SIZE)
		return 0;
	long value = (long)Stream;
	Stream += sizeof(long);
	return value;
}
unsigned short Packet::Packet::ReadUshort()
{
	if (Position + sizeof(unsigned short) >= MAX_SIZE)
		return 0;
	unsigned short value = (unsigned short)Stream;
	Stream += sizeof(unsigned short);
	return value;
}
unsigned int Packet::Packet::ReadUInt()
{
	if (Position + sizeof(unsigned int) >= MAX_SIZE)
		return 0;
	unsigned int value = (unsigned int)Stream;
	Stream += sizeof(unsigned int);
	return value;
}
unsigned long Packet::Packet::ReadULong()
{
	if (Position + sizeof(unsigned long) >= MAX_SIZE)
		return 0;
	unsigned long value = (unsigned long)Stream;
	Stream += sizeof(unsigned long);
	return value;
}
#endif
#ifdef Encrypt
void Packet::Packet::Encrypted(char** Base)
{
	for (int x = 0; x < sizeof(Base); x++)
	{
		Base[x] += EncryptValue;
	}
};
void Packet::Packet::Decrypted(char** Base)
{
	for (int x = 0; x < sizeof(Base); x++)
	{
		Base[x] -= EncryptValue;
	}
};
#endif
