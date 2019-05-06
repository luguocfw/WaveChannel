/*
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
*/
#include "wt_proto_physical_layer.h"
#include "kiss_fft/kiss_fft.h"
#include "kiss_fft/kiss_fftr.h"
#include "log_utils/wt_log.h"
#include "checksum_utils/parity_codec.h"
#include <stdio.h>
#define PI                      3.1415926535897932384626433832795028841971 
#define MAX_FREQ_MISTAKE                       ((int)(1000.0/FREQ_ANALYZE_SAMPLE_TIME_MS))

typedef struct {
  int               freq_;
  double            phase_;
}FreqAndPhaseInfo;

typedef struct {
  int                       freq_num_;
  FreqAndPhaseInfo           freq_and_phase_[COMPARE_FREQ_NUM];
}FreqEncodeInfo;


static double theta = 0;

static int GetFFTResult(const RecvAudioType *pcm_buf, int len, kiss_fft_cpx *out_data)
{
  kiss_fft_cpx *in_data = NULL;
  kiss_fft_cfg fft_cfg = NULL;
  in_data = (kiss_fft_cpx *)malloc(sizeof(kiss_fft_cpx)*len);
  if (in_data == NULL) {
    goto error_exit;
  }
  int i;
  for (i = 0; i < len; i++) {
    in_data[i].r = (float)pcm_buf[i];
    in_data[i].i = 0;
  }
  //printf("################################################\n");
  fft_cfg = kiss_fft_alloc(len, 0, NULL, NULL);
  if (fft_cfg == NULL) {
    goto error_exit;
  }
  kiss_fft(fft_cfg, in_data, out_data);
  free(in_data);
  KISS_FFT_FREE(fft_cfg);
  kiss_fft_cleanup();
  return 0;

error_exit:
  if (in_data != NULL) {
    free(in_data);
  }
  if (fft_cfg != NULL) {
    KISS_FFT_FREE(fft_cfg);
  }
  kiss_fft_cleanup();
  return -1;
}

static void GetCpxForFreqAndPhase(int amplitude, double phase, kiss_fft_cpx *cpx)
{
  WtLog("input phase:%llf,amplitude:%d\n", phase, amplitude);
  //phase = PI/2;
  double slope = tan(phase);
  WtLog("slope:%llf\n", slope);
  if (phase > (PI*0.5) && phase < PI*1.5) {
    cpx->i = (float)sqrt(amplitude / (1 + pow(slope, 2)));
  }
  else {
    cpx->i = -(float)sqrt(amplitude / (1 + pow(slope, 2)));
  }
  cpx->r = (float)(cpx->i * slope);
}

static void CreatePluralFreqsForFreqCode(int nfft, const FreqEncodeInfo *freqs_info, kiss_fft_cpx *plural_data, int sample_rate, int amplitude)
{
  //memset(plural_data, 0, sizeof(kiss_fft_cpx)*((nfft / 2) + 1));
  memset(plural_data, 0, sizeof(kiss_fft_cpx)*nfft);
  float temp;
  int n;
  int i;
  for (i = 0; i < freqs_info->freq_num_; i++) {
    temp = ((freqs_info->freq_and_phase_[i].freq_ * (nfft - 1) * 2.0) / sample_rate);
    n = (int)temp;
    if (temp - n > 0.5) {
      n++;
    }
    //plural_data[n].r = amplitude / 18;
    GetCpxForFreqAndPhase(amplitude*nfft, freqs_info->freq_and_phase_[i].phase_, &plural_data[n]);
    //GetCpxForFreqAndPhase(amplitude*nfft, freqs_info->freq_and_phase_[i].phase_, &plural_data[nfft-n]);
    // plural_data[nfft - n - 1].r = amplitude / 8;
  }
}

static int EncodeSoundMixingForFreqCode(const FreqEncodeInfo *freqs_info, void *buffer, int buffer_length, int sample_bit, int sample_rate)
{
  kiss_fftr_cfg fftr_cfg = NULL;
  kiss_fft_cpx *in_data = NULL;
  float *out_data = NULL;
  int pcm_num = buffer_length / (sample_bit / 8);
  int in_data_num = (pcm_num / 2) + 1;
  fftr_cfg = kiss_fftr_alloc(pcm_num, 1, NULL, NULL);
  if (fftr_cfg == NULL) {
    return -1;
  }
  in_data = (kiss_fft_cpx *)malloc(sizeof(kiss_fft_cpx)*in_data_num);
  if (in_data == NULL) {
    KISS_FFT_FREE(fftr_cfg);
    return -1;
  }
  int amplitude;
  switch (sample_bit) {
    case 8:amplitude = (127 * AUDIO_AMPLITUDE_SCALE) / 100; break;
    case 16:amplitude = (32767 * AUDIO_AMPLITUDE_SCALE) / 100; break;
    default:
      free(in_data);
      KISS_FFT_FREE(fftr_cfg);
      return -1;
  }
  CreatePluralFreqsForFreqCode(in_data_num, freqs_info, in_data, sample_rate, amplitude);
  out_data = (float*)malloc(sizeof(float)*pcm_num);
  if (out_data == NULL) {
    free(in_data);
    KISS_FFT_FREE(fftr_cfg);
    return -1;
  }
  kiss_fftri(fftr_cfg, in_data, out_data);
  int i;
  for (i = 0; i < pcm_num; i++) {
    switch (sample_bit) {
      case 8: ((signed char *)buffer)[i] = (signed char)out_data[i]; break;
      case 16: ((short *)buffer)[i] = (short)out_data[i]; break;
      default:
        free(out_data);
        free(in_data);
        KISS_FFT_FREE(fftr_cfg);
        return -1;
    }
  }
  free(in_data);
  free(out_data);
  KISS_FFT_FREE(fftr_cfg);
  return 0;
}

WTFreqCodeType GetFreqCodeFromFFTData(const kiss_fft_cpx *fft_data, int fft_len, int sample_time_ms)
{
  int i;
  int step = 1000 / sample_time_ms;
  //FILE *fp = fopen("result.txt", "w");
  WTFreqCodeType temp = 0x0000;
  long min_item = 999999;
  int min_bit;
  for (i = 0; compare_freq_list_[i].left_freq_ != -1; i++) {
    int fft_left, fft_right;
    fft_left = compare_freq_list_[i].left_freq_ / step;
    fft_right = compare_freq_list_[i].right_freq_ / step;
    long left_item, right_item;
    left_item = (long)sqrt(pow(fft_data[fft_left].r, 2)+pow(fft_data[fft_left].i, 2));
    right_item = (long)sqrt(pow(fft_data[fft_right].r, 2)+pow(fft_data[fft_right].i, 2));
    if (left_item > right_item) {
      if (left_item < min_item) {
        min_item = left_item;
        min_bit = i;
      }
      if (compare_freq_list_[i].bool_) {
        temp |= (0x0001) << i;
      }
    }
    else {
      if (right_item < min_item) {
        min_item = right_item;
        min_bit = i;
      }
      if (!compare_freq_list_[i].bool_) {
        temp |= (0x0001) << i;
      }
    }
  }
  if (ParityDecode(temp, COMPARE_FREQ_BIT, ((temp&(0x0001 << 9))>>9)) != 1) {
    if (temp&(0x0001 << min_bit)) {
      temp &= ~(0x0001 << min_bit);
    }
    else {
      temp |= 0x0001 << min_bit;
    }
  }
  temp &= ~(0x0001 << 9);
  WtLog(" %d ", temp);
  //fclose(fp);
  //printf("temp:%d\n", temp);
  return temp;
}

int WTPhyAnalysisNumToRealNum(int ana_num)
{
  int remainder = ana_num % (ONE_FREQ_TIME_MS / FREQ_ANALYZE_SAMPLE_TIME_MS);
  int divsor = ana_num / (ONE_FREQ_TIME_MS / FREQ_ANALYZE_SAMPLE_TIME_MS);

  if (remainder >= ((ONE_FREQ_TIME_MS / FREQ_ANALYZE_SAMPLE_TIME_MS) - 1)) {
    remainder = 1;
  }
  else {
    remainder = 0;
  }
  return remainder + divsor;
}

int WTPhysicalPcmDecode(const RecvAudioType * pcm_buf, int pcm_len, WTFreqCodeType * code)
{
  kiss_fft_cpx *fft_out_data = NULL;
  fft_out_data = (kiss_fft_cpx *)malloc(sizeof(kiss_fft_cpx)*pcm_len);
  if (fft_out_data == NULL) {
    return -1;
  }
  if (GetFFTResult(pcm_buf, pcm_len, fft_out_data) != 0) {
    free(fft_out_data);
    return -1;
  }
  int sample_time_ms = (1000 * pcm_len) / RECV_SAMPLE_RATE;
  *code = GetFreqCodeFromFFTData(fft_out_data, pcm_len, sample_time_ms);
  free(fft_out_data);
  return 0;
}

static double GetNextPhaseAsCurent(double cur_phase, int freq, int sample_rate, int sample_num)
{
  double phase_step = (2 * PI*freq) / sample_rate;
  double end_phase = cur_phase + phase_step*sample_num;
  long temp = (long)(end_phase / (2 * PI));
  end_phase = end_phase - (2 * PI)*temp;
  return end_phase;
}

int WTPhysicalPcmEncode(WTFreqCodeType code, void * pcm_buf, int pcm_len, RefPhaseInfo *ref_phase, int sample_bit, int sample_rate)
{
  FreqEncodeInfo freq_code_info;
  freq_code_info.freq_num_ = 0;
  unsigned short temp_data = code;
  unsigned char parity = ParityEncode(temp_data, COMPARE_FREQ_BIT);
  if (parity) {
    temp_data |= (0x0001) << 9;
  }
  else {
    temp_data &= ~((0x0001) << 9);
  }
  int i;
  for (i = 0; i < COMPARE_FREQ_BIT+COMPARE_FREQ_PARITY_BIT; i++) {
    if ((0x0001 << i)&temp_data) {
      if (compare_freq_list_[i].bool_) {
        freq_code_info.freq_and_phase_[freq_code_info.freq_num_].freq_ = compare_freq_list_[i].left_freq_;
        freq_code_info.freq_and_phase_[freq_code_info.freq_num_].phase_ = ref_phase->left_phase_[i];
        ref_phase->left_phase_[i] = GetNextPhaseAsCurent(freq_code_info.freq_and_phase_[freq_code_info.freq_num_].phase_,
          freq_code_info.freq_and_phase_[freq_code_info.freq_num_].freq_, sample_rate, pcm_len / (sample_bit / 8));
        ref_phase->right_phase_[i] = 0.0;
      }
      else {
        freq_code_info.freq_and_phase_[freq_code_info.freq_num_].freq_ = compare_freq_list_[i].right_freq_;
        freq_code_info.freq_and_phase_[freq_code_info.freq_num_].phase_ = ref_phase->right_phase_[i];
        ref_phase->right_phase_[i] = GetNextPhaseAsCurent(freq_code_info.freq_and_phase_[freq_code_info.freq_num_].phase_,
          freq_code_info.freq_and_phase_[freq_code_info.freq_num_].freq_, sample_rate, pcm_len / (sample_bit / 8));
        ref_phase->left_phase_[i] = 0.0;
      }
    }
    else {
      if (!compare_freq_list_[i].bool_) {
        freq_code_info.freq_and_phase_[freq_code_info.freq_num_].freq_ = compare_freq_list_[i].left_freq_;
        freq_code_info.freq_and_phase_[freq_code_info.freq_num_].phase_ = ref_phase->left_phase_[i];
        ref_phase->left_phase_[i] = GetNextPhaseAsCurent(freq_code_info.freq_and_phase_[freq_code_info.freq_num_].phase_,
          freq_code_info.freq_and_phase_[freq_code_info.freq_num_].freq_, sample_rate, pcm_len / (sample_bit / 8));
        ref_phase->right_phase_[i] = 0.0;
      }
      else {
        freq_code_info.freq_and_phase_[freq_code_info.freq_num_].freq_ = compare_freq_list_[i].right_freq_;
        freq_code_info.freq_and_phase_[freq_code_info.freq_num_].phase_ = ref_phase->right_phase_[i];
        ref_phase->right_phase_[i] = GetNextPhaseAsCurent(freq_code_info.freq_and_phase_[freq_code_info.freq_num_].phase_,
          freq_code_info.freq_and_phase_[freq_code_info.freq_num_].freq_, sample_rate, pcm_len / (sample_bit / 8));
        ref_phase->left_phase_[i] = 0.0;
      }
    }
    freq_code_info.freq_num_++;
  }
  if (EncodeSoundMixingForFreqCode(&freq_code_info, pcm_buf, pcm_len, sample_bit, sample_rate) != 0) {
    return -1;
  }
  return 0;
}





