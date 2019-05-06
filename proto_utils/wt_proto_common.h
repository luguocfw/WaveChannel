/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#ifndef __WT_PROTO_COMMON_H__
#define __WT_PROTO_COMMON_H__

/*global config*/

typedef unsigned short WTFreqCodeType;

#define ONE_FREQ_TIME_MS                 (24)
#define time_ms_to_length(time_ms,sample_rate)          (((time_ms)*(sample_rate))/1000)

#define COMPARE_FREQ_ST_NUM                 (1)
#define COMPARE_FREQ_DATA_NUM               (4)
#define COMPARE_DATA_NUM_LEN                (2)
#define COMPARE_FREQ_DATA_BIT               (8)
#define COMPARE_FREQ_CHECKSUM_NUM           (4)
#define COMPARE_FREQ_NUM                    (18)
#define COMPARE_FREQ_BIT                    (9)
#define COMPARE_FREQ_PARITY_BIT              (1)
#define COMPARE_FREQ_NONE                    (0x00)
#define COMPARE_ST_CODE                     {0x34}
typedef struct {
  int left_freq_;
  int right_freq_;
  int bool_;
}CompareFreqNode;

extern CompareFreqNode compare_freq_list_[];
#define MAX_FREQ                  (7250)


/*recv side config*/
#define RECV_SAMPLE_BIT                         (16)
#define RECV_SAMPLE_RATE                        (16000)
#define FREQ_ANALYZE_SAMPLE_TIME_MS             (8)
#if (RECV_SAMPLE_BIT==8)
typedef char    RecvAudioType;
#endif
#if (RECV_SAMPLE_BIT==16)
typedef short   RecvAudioType;
#endif



/*send side config*/
#define  AUDIO_AMPLITUDE_SCALE            (35)  //AUDIO_AMPLITUDE_SCALE/100 * max value
#define  AUDIO_NONE_TIME_MS               (200)
#define  AUDIO_NONE_LEN(sample_rate)      ((AUDIO_NONE_TIME_MS*(sample_rate))/1000)




#endif
