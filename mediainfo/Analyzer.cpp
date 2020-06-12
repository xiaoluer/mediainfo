#include "Analyzer.h"
#include <json/json.h>
#include <time.h>

//#define WRITEFILE 1

CAnalyzer::CAnalyzer()
{
}


CAnalyzer::~CAnalyzer()
{
}

int32_t CAnalyzer::init()
{
	m_mediainfo = new media_info_st;
	memset(m_mediainfo, 0, sizeof(media_info_st));
	av_register_all();
	avformat_network_init();
	m_pFormatCtx = avformat_alloc_context();
	m_pFormatCtx->interrupt_callback.callback = AVInterruptCallBackFun;
	m_pFormatCtx->interrupt_callback.opaque = this;

	return 0;
}

int32_t CAnalyzer::unInit()
{
	delete m_mediainfo;
	m_mediainfo = NULL;
	return 0;
}

void CAnalyzer::setStreamUrl(const char *url)
{
	snprintf(m_url, MIDLEN, url);
}

int32_t CAnalyzer::streamOpen()
{
	if (avformat_open_input(&m_pFormatCtx, m_url, NULL, NULL) < 0) {
		printf("[error] could not open source url %s\n", m_url);
		return -1;
	}

	if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0) {
		printf("[error] could not find stream information\n");
		return -1;
	}

	bool audio, video;
	audio = video = false;

	int32_t len = strlen(m_pFormatCtx->filename);
	strncpy(m_mediainfo->completename, m_pFormatCtx->filename, len>MAXLEN ? MAXLEN : len);	// 名称

	len = strlen(m_pFormatCtx->iformat->name);
	strncpy(m_mediainfo->format, m_pFormatCtx->iformat->name, len>LITLEN ? LITLEN : len);	// 编码器名称

	len = strlen(m_pFormatCtx->iformat->long_name);
	strncpy(m_mediainfo->formatinfo, m_pFormatCtx->iformat->long_name, len>MINLEN ? MINLEN : len);	// 编码器名称

	m_mediainfo->duration = m_pFormatCtx->duration / AV_TIME_BASE;	// 持续时间

	//int64_t start_time = m_pFormatCtx->start_time / AV_TIME_BASE;	// 起始时间
	m_mediainfo->overallbitrate = m_pFormatCtx->bit_rate / 1000;	// 平均混合码率

	int32_t video_stream_idx = av_find_best_stream(m_pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
	if (video_stream_idx >= 0) {;
		video = true;
		AVStream *video_stream = m_pFormatCtx->streams[video_stream_idx];
		m_mediainfo->video.id = m_pFormatCtx->streams[video_stream_idx]->stream_identifier;

		m_mediainfo->video.frames = video_stream->nb_frames;	// 总帧数

		m_mediainfo->video.codec_id = video_stream->codec->codec_id;	// 编解码器ID

		strcpy(m_mediainfo->video.codec_name, avcodec_get_name(video_stream->codec->codec_id)); // 编解码器名称
		
		m_mediainfo->video.width = video_stream->codec->width; // 宽度

		m_mediainfo->video.height = video_stream->codec->height; // 高度

		const AVPixFmtDescriptor *des = av_pix_fmt_desc_get(video_stream->codec->pix_fmt);// 色彩空间
		strcpy(m_mediainfo->video.pix_fmt, des->name);

		m_mediainfo->video.bitrate = (int64_t)video_stream->codec->bit_rate / 1000;	// 码率
		if (m_mediainfo->video.bitrate > 0)
		{
			//memset(strtowrite, 0, MIDLEN);
			//sprintf(strtowrite, "码率：%I64dkb/s\r\n", m_mediainfo->video.bitrate);
			//fwrite(strtowrite, 1, strlen(strtowrite), m_file);
		}

		//video_stream->codec->ticks_per_frame
		// 平均帧率
		m_mediainfo->video.avg_frame_rate = ((double)video_stream->avg_frame_rate.num) / ((double)video_stream->avg_frame_rate.den);
		//memset(strtowrite, 0, MIDLEN);
		//sprintf(strtowrite, "平均帧率：%.4fFPS ticks_per_frame:%d\r\n\r\n", m_mediainfo->video.avg_frame_rate, 
		//	video_stream->codec->ticks_per_frame);
		//fwrite(strtowrite, 1, strlen(strtowrite), m_file);
	}

	int audio_stream_idx = av_find_best_stream(m_pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
	if (audio_stream_idx >= 0) {
		audio = true;
		AVStream *audio_stream = m_pFormatCtx->streams[audio_stream_idx];
		m_mediainfo->audio.id = m_pFormatCtx->streams[audio_stream_idx]->stream_identifier;

		m_mediainfo->audio.codec_id = audio_stream->codec->codec_id;// 编码器ID

		strcpy(m_mediainfo->audio.codec_name, avcodec_get_name(audio_stream->codec->codec_id));//编码器名称

		m_mediainfo->audio.sample_rate = audio_stream->codec->sample_rate;// 采样率
		//memset(strtowrite, 0, MIDLEN);
		//sprintf(strtowrite, "采样率：%dkHz\r\n", m_mediainfo->audio.sample_rate);
		//fwrite(strtowrite, 1, strlen(strtowrite), m_file);

		m_mediainfo->audio.channels = audio_stream->codec->channels;//v声道数

		//audio_stream->codec->sample_fmt;
		m_mediainfo->audio.frame_size = audio_stream->codec->frame_size;// 采样长度

		m_mediainfo->audio.bitrate = (int64_t)audio_stream->codec->bit_rate / 1000;// 码率

		m_mediainfo->audio.nb_frames = audio_stream->nb_frames;// 总帧数
	}
	if (video && audio)
		m_mediainfo->mediatype = 3;
	if (video && !audio)
		m_mediainfo->mediatype = 1;
	if (!video && audio)
		m_mediainfo->mediatype = 2;

	return 0;
}

/*
{
	"Codecid": "",
	"Duration": 3959,
	"Filesize": 0,
	"Format": "flv",
	"Formatinfo": "FLV (Flash Video)",
	"Mediatype": 3,
	"Name": "C:\\video\\wuyue.flv",
	"Overallbitrate": 914,
	"Video": {
		"Avg_frame_rate": 25.0,
		"Bitrate": 0,
		"Codec_id": 28,
		"Codec_name": "h264",
		"Frames": 0,
		"Height": 720,
		"Id": 0,
		"Pix_fmt": "yuv420p",
		"Width": 1280
	},
	"Audio": {
		"Bitrate": 0,
		"Channels": 2,
		"Codec_id": 2,
		"Codec_name": "aac",
		"Frame_size": 0,
		"Id": 0,
		"Nb_frames": 0,
		"Sample_rate": 48000
	}
}
*/

void CAnalyzer::mediainfoDump()
{
	Json::Value root;
	Json::FastWriter writer;
	Json::Value Video;
	Json::Value Audio;

	root["Mediatype"] = m_mediainfo->mediatype;
	root["Name"] = m_mediainfo->completename;
	root["Format"] = m_mediainfo->format;
	root["Formatinfo"] = m_mediainfo->formatinfo;
	root["Codecid"] = m_mediainfo->codecid;
	root["Filesize"] = m_mediainfo->filesize;
	root["Duration"] = m_mediainfo->duration;
	root["Overallbitrate"] = m_mediainfo->overallbitrate;

	Video["Id"] = m_mediainfo->video.id;
	Video["Frames"] = m_mediainfo->video.frames;
	Video["Codec_id"] = m_mediainfo->video.codec_id;
	Video["Codec_name"] = m_mediainfo->video.codec_name;
	Video["Width"] = m_mediainfo->video.width;
	Video["Height"] = m_mediainfo->video.height;
	Video["Pix_fmt"] = m_mediainfo->video.pix_fmt;
	Video["Bitrate"] = m_mediainfo->video.bitrate;
	Video["Avg_frame_rate"] = m_mediainfo->video.avg_frame_rate;

	Audio["Id"] = m_mediainfo->audio.id;
	Audio["Codec_id"] = m_mediainfo->audio.codec_id;
	Audio["Codec_name"] = m_mediainfo->audio.codec_name;
	Audio["Sample_rate"] = m_mediainfo->audio.sample_rate;
	Audio["Channels"] = m_mediainfo->audio.channels;
	Audio["Frame_size"] = m_mediainfo->audio.frame_size;
	Audio["Nb_frames"] = m_mediainfo->audio.nb_frames;
	Audio["Bitrate"] = m_mediainfo->audio.bitrate;

	root["Video"] = Video;
	root["Audio"] = Audio;

	std::string jsonsetting = writer.write(root);

	printf("[result] %s\n", jsonsetting.c_str());

	return;
}

int CAnalyzer::AVInterruptCallBackFun(void *ctx)
{
	CAnalyzer *ana = (CAnalyzer*)ctx;
	if (ana == NULL)
		return 0;

	return ana->AVInterruptCallBackFun1();

	return 0;
}

#ifndef _WIN32
#define localtime_s(a, b) localtime_r(b, a)
#endif
int CAnalyzer::AVInterruptCallBackFun1()
{
/*	sprintf(m_filename, "%d-1.txt", this);
	m_file = fopen(m_filename, "a");
	char str[MIDLEN];
	memset(str, 0, MIDLEN);
	struct tm _tm;
	time_t t;

	t = time(NULL);

#ifndef _WIN32
	localtime_s(&_tm, &t);
	sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d： 出现中断\r\n", _tm.tm_year + 1900, _tm.tm_mon + 1,
		_tm.tm_mday, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
#endif
	fwrite(str, 1, strlen(str), m_file);

	fclose(m_file);
*/	return 0;
}

int32_t CAnalyzer::streamClose()
{
	avformat_close_input(&m_pFormatCtx);
	return 0;
}