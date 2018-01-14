// Packet.h

#pragma once

using namespace System;
const int TQ_SEALSIZE = 8;
const int _MAX_MSGSIZE = 1024;
const int EncryptValue = 256;

namespace Packet {
	class Packet
	{
		typedef unsigned char BYTE;
		typedef float         FLOAT;
		typedef unsigned int  UINT;
		typedef unsigned long ULONG;
		typedef unsigned short USHORT;
		typedef unsigned int UINT;
		typedef unsigned long decimal;
		typedef unsigned char UCHAR;
		typedef unsigned char* String;
		typedef unsigned char* Bites;
		typedef unsigned long       DWORD;
		typedef int                 BOOL;
		typedef void*         STRUCT;
#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif
	public:
		int MAX_SIZE = 1024;
		int TQ_SEALSIZE = 8;
		char* ServerSeal = "TQSERVER";
		char* ClientSeal = "TQCLIENT";
		void Finalize(USHORT type, BOOL Server);
		Packet();
		virtual ~Packet();
		bool Create();
		bool Create(STRUCT Struct, USHORT type);
		void Process(void* pInfo);
		int Size;
		void Seek(int POS);
		void SeekForward(int POS);
		void SeekBackWord(int POS);
		int Position = (int)(Stream - Memory);
		void InitWriter();
		void Write(BYTE value);
		void Write(USHORT value);
		void Write(ULONG value);
		void Write(String value);
		void Write(char* value);
		void Write(char* value, int Length);
		void Write(SByte value);
		void Write(short value);
		void Write(long value);
		void Write(char value);
		void Write(UINT value);
		void Write(int value);
		void WriteStruct(STRUCT value);
		void Decrypted(char** Base);
		void Encrypted(char** Base);
		void ZeroFill(int amount);
		void PrepareSeal(BOOL Server);
		void ReadStruct(STRUCT value);
		short ReadShort();
		int ReadInt();
		long ReadLong();
		unsigned short ReadUshort();
		unsigned int ReadUInt();
		unsigned long ReadULong();
		Packet&  Packet::operator=(const char** Buff);


	protected:
		char* Memory[_MAX_MSGSIZE];
		char** Stream;
	};
}
