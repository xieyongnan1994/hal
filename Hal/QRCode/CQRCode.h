#pragma once
#ifndef _CQRCODE_H
#define _CQRCODE_H

#include "qrencode.h"
#include <string>
#include <stdlib.h>
#include <windows.h>

using namespace std;

class CQRCode
{
public:
	CQRCode();
	~CQRCode();
public:
	string GenerateQrCode(string& generateStr);

};
#endif // !_CQRCODE_H

