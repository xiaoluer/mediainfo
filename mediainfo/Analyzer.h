#pragma once

#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
# include "stdint.h"

extern "C"
{
#endif
#include "libavformat/avformat.h"
#include "libavutil/pixdesc.h"
#ifdef __cplusplus
}
#endif
#include "typedefine.h"

class CAnalyzer
{
public:
	CAnalyzer();
	~CAnalyzer();

public:
	int32_t init();
	int32_t unInit();
	void setStreamUrl(const char *url);
	int32_t streamOpen();
	void mediainfoDump();
	int32_t streamClose();

public:
	static int AVInterruptCallBackFun(void *ctx);
	int AVInterruptCallBackFun1();

private:
	AVFormatContext *m_pFormatCtx;
	pmedia_info_st m_mediainfo;
	FILE *m_file;
	char m_filename[LITLEN];
	char m_url[MIDLEN];
};

