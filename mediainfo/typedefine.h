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
	int64_t frames;				// ��֡��
	int8_t codec_id;			// ������id
	char codec_name[LITLEN];	// ����������
	int16_t width;				// ��Ƶ��
	int16_t height;				// ��Ƶ��
	char pix_fmt[LITLEN];		// ɫ�ʿռ�����
	int64_t bitrate;			// ���ʣ�kb/s��
	double avg_frame_rate;		// ƽ��֡��
}videoinfo_st, *pvideoinfo_st;

typedef struct _audioinfo_st
{
	int8_t id;
	int8_t codec_id;			// ������id
	char codec_name[LITLEN];	// ����������
	int32_t	sample_rate;		// ������kHz
	int8_t channels;			// ������
	int8_t frame_size;			// ÿ֡�Ĳ�����
	int64_t nb_frames;			// ��֡��
	int64_t	bitrate;			// ����
}audioinfo_st, *paudioinfo_st;

typedef struct _media_info_st
{
	uint8_t mediatype;			// 1 ֻ����Ƶ 2 ֻ����Ƶ 3 ��Ƶ����Ƶ
	char completename[MAXLEN];	// ����
	char format[LITLEN];		// ��ʽ
	char formatinfo[MINLEN];	// ��ʽ�ſ�
	char codecid[LITLEN];		// ������ID
	int64_t filesize;			// �ļ���С��Byte��
	int64_t duration;			// ����ʱ�䣨�룩
	int32_t overallbitrate;		// ƽ��������ʣ�kb/s��

	videoinfo_st video;
	audioinfo_st audio;
}media_info_st, *pmedia_info_st;

#endif // TYPEDEFINE_H