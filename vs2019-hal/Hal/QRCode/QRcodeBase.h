#include "stdafx.h"
#pragma once

class CQrcodeBitBuffer
{
public:
	void Put(BOOL bit);
	void Put(int num, int length);
	BOOL Get(int index);
	CString ToString();
	int GetLengthInBits();
	BYTE* GetBuffer();
	CQrcodeBitBuffer();
	virtual ~CQrcodeBitBuffer();
private:
	BYTE *buffer;
	int buffer_len;
	int length;
	int inclements;
};

class CQrcodePolynomial
{
public:
	CQrcodePolynomial Mod(CQrcodePolynomial &e);
	CQrcodePolynomial Multiply(CQrcodePolynomial &e);
	CString ToLogString();
	CString ToString();
	int GetLength();
	int Get(int idx);
	
	CQrcodePolynomial(CUIntArray& nums, int shift);
	CQrcodePolynomial(const CQrcodePolynomial &p);
	CQrcodePolynomial& operator=(const CQrcodePolynomial &p);

	virtual ~CQrcodePolynomial();

	CUIntArray m_nums;
};

class CQrcodeUtil
{
public:
	static int UnsignedRightShift(int v, int n);
	static int GetBCHTypeNumber(int data);
	static int GetBCHTypeInfo(int data);
	static int GetBCHDigit(int data);
	static BOOL IsHanZi(CString s);
	static BOOL IsAlphaNum(CString s);
	static BOOL IsNumber(CString s);
	static int GetMode(CString s);
	static BOOL GetMask(int maskPattern, int i, int j);
	static CQrcodePolynomial GetErrorCorrectPolynomial(int errorCorrectLength);
	static void GetPatternPosition(int typeNumber, CUIntArray &p);
	static int GetMaxLength(int typeNumber, int mode, int errorCorrectLevel);

	CQrcodeUtil();
	virtual ~CQrcodeUtil();

};

class CQrcodeMath
{
public:
	int Gexp(int n);
	int Glog(int n);

	static CQrcodeMath * GetInstance();

	CQrcodeMath();
	virtual ~CQrcodeMath();


private:
	CUIntArray EXP_TABLE;
	CUIntArray LOG_TABLE;
};


class CQrcodeData : public CObject
{
public:
	int GetLengthInBits(int type);
	virtual void Write(CQrcodeBitBuffer &buffer);
	virtual int GetLength();

	int GetMode();
	CQrcodeData(int mode, CString data);
	virtual ~CQrcodeData();

	CString data;

private:
	int mode;
};

class CQrcodeNumber : public CQrcodeData
{
public:
	virtual void Write(CQrcodeBitBuffer &buffer);
	int ParseInt(CString s);
	CQrcodeNumber(CString data);
	virtual ~CQrcodeNumber();

};

class CQrcodeAlphaNum : public CQrcodeData
{
public:
	int GetCode(char c);
	virtual void Write(CQrcodeBitBuffer &buffer);
	CQrcodeAlphaNum(CString data);
	virtual ~CQrcodeAlphaNum();

};

class CQrcodeHanzi : public CQrcodeData
{
public:
	virtual void Write(CQrcodeBitBuffer &buffer);
	virtual int GetLength();
	CQrcodeHanzi(CString data);
	virtual ~CQrcodeHanzi();

};



class CQrcode8BitByte : public CQrcodeData
{
public:
	virtual void Write(CQrcodeBitBuffer &buffer);
	CQrcode8BitByte(CString data);
	virtual ~CQrcode8BitByte();

};



class CQrcode2DIntArray
{
public:
	CUIntArray * GetIntArray(int r);
	int GetAt(int r, int c);
	void SetAt(int r, int c, int v);
	CQrcode2DIntArray();
	virtual ~CQrcode2DIntArray();
private:
	CObArray a;
};



class CQrcodeRSBlock : public CObject
{
public:
	CQrcodeRSBlock(int totalCount, int dataCount);
	virtual ~CQrcodeRSBlock();

	int GetDataCount();
	int GetTotalCount();
	static void GetRsBlockTable(int typeNumber, int errorCorrectLevel, CUIntArray &a);
	static void GetRSBlocks(int typeNumber, int errorCorrectLevel, CObArray &RBSlocks);

private:
	int totalCount;
	int dataCount;

};


class CQRcode
{
public:
	void SaveToBmp(CString filename, int cellSize, int margin);
	void Draw(CDC *pdc, int cellSize, int margin);
	void CQRcode::Draw(CDC *pdc, int cellSize, int margin, int offsetX, int offsetY);
	static CQRcode* GetMinimumQRCode(CString data, int errorCorrectLevel);
	void MapData(BYTE* bytes, int bytes_size, int maskPattern);
	static BYTE* CreateData(int typeNumber, int errorCorrectLevel, CObArray &dataArray, int* bytesSize);
	static BYTE* CreateBytes(CQrcodeBitBuffer &buffer, CObArray &rsBlocks, int* bytesSize);
	void SetupTypeNumber(BOOL test);
	void SetupTypeInfo(BOOL test, int maskPattern);
	void SetupTimingPattern();
	void SetupPositionAdjustPattern();
	void SetupPositionProbePattern(int row, int col);
	static int GetLostPoint(CQRcode *qrcode);
	int GetBestMaskPattern();
	void Make();
	void Make(BOOL test, int maskPattern);
	int GetModuleCount();
	BOOL IsDark(int row, int col);
	CQrcodeData * GetData(int index);
	int GetDataCount();
	void ClearData();
	void AddData(CString data, int mode);
	void AddData(CString data);
	void SetErrorCorrectLevel(int errorCorrectLevel);
	void SetTypeNumber(int typeNumber);
	CQRcode();
	virtual ~CQRcode();

private:
	BYTE *modules;
	int moduleCount;
	int typeNumber;
	int errorCorrectLevel;
	CObArray qrDataList;
	CObArray forMemoryLeak;
};

#define ErrorCorrectLevel_L  1
#define ErrorCorrectLevel_M  0
#define ErrorCorrectLevel_Q  3
#define ErrorCorrectLevel_H  2
