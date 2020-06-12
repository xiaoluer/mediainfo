#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "Analyzer.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("[error] argv is too short.\n");
		fflush(stdout);

#ifdef _WIN32
		Sleep(5000);
#else
		usleep(5.0 * 1000000);
#endif
		return -1;
	}

	if (strcmp("-i", argv[1]) != 0 &&
		strcmp("-I", argv[1]) != 0)
	{
		printf("[error] input argv error.\n");
		return -1;
	}

	char streamname[MIDLEN] = { "0" };
	strncpy(streamname, argv[2], strlen(argv[2]) > MIDLEN ? MIDLEN : strlen(argv[2]));

	CAnalyzer *analyzer = new CAnalyzer;
	analyzer->init();
	analyzer->setStreamUrl(streamname);
	analyzer->streamOpen();
	analyzer->mediainfoDump();
	analyzer->streamClose();

	delete analyzer;
	analyzer = NULL;

	return 0;
}