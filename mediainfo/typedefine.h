#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

#include <stdint.h>

#ifndef SOURCELISTFILENAME_
#define SOURCELISTFILENAME_
//const char *SOURCELISTFILENAME = "sourcelist.xml";
#endif
const uint32_t MAXLEN = 512;
const uint32_t MIDLEN = 256;
const uint32_t MINLEN = 128;
const uint32_t LITLEN = 64;


typedef struct _videoinfo_st
{
	int8_t id;
	int64_t frames;				// 总帧数
	int8_t codec_id;			// 编码器id
	char codec_name[LITLEN];	// 编码器名称
	int16_t width;				// 视频高
	int16_t height;				// 视频宽
	char pix_fmt[LITLEN];		// 色彩空间名称
	int64_t bitrate;			// 码率（kb/s）
	double avg_frame_rate;		// 平均帧率
}videoinfo_st, *pvideoinfo_st;

typedef struct _audioinfo_st
{
	int8_t id;
	int8_t codec_id;			// 编码器id
	char codec_name[LITLEN];	// 编码器名称
	int32_t	sample_rate;		// 采样率kHz
	int8_t channels;			// 声道数
	int8_t frame_size;			// 每帧的采样数
	int64_t nb_frames;			// 总帧数
	int64_t	bitrate;			// 码率
}audioinfo_st, *paudioinfo_st;

typedef struct _media_info_st
{
	uint8_t mediatype;			// 1 只有视频 2 只有音频 3 视频和音频
	char completename[MAXLEN];	// 名称
	char format[LITLEN];		// 格式
	char formatinfo[MINLEN];	// 格式概况
	char codecid[LITLEN];		// 编码器ID
	int64_t filesize;			// 文件大小（Byte）
	int64_t duration;			// 持续时间（秒）
	int32_t overallbitrate;		// 平均混合码率（kb/s）

	videoinfo_st video;
	audioinfo_st audio;
}media_info_st, *pmedia_info_st;

#endif // TYPEDEFINE_H