#
#
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: cfwang_9984@163.com
#
{   
   'includes':[
      'common.gypi',
    ],
    'targets': [
        {
        'target_name': 'libwavetrans',
        'type': 'static_library',
        'include_dirs': [
          '.',
        ],
        'direct_dependent_settings': {
          'include_dirs': [
            '.',       
           ],
        },
        'sources': [
          'buff_utils/queue.c',
          'buff_utils/queue.h',
          'buff_utils/ring_buff.c',
          'buff_utils/ring_buff.h',
          'checksum_utils/crc_codec.c',
          'checksum_utils/crc_codec.h',
          'checksum_utils/parity_codec.c',
          'checksum_utils/parity_codec.h',
          'kiss_fft/_kiss_fft_guts.h',
          'kiss_fft/kiss_fastfir.c',
          'kiss_fft/kiss_fastfir.h',
          'kiss_fft/kiss_fft.c',
          'kiss_fft/kiss_fft.h',
          'kiss_fft/kiss_fftr.c',
          'kiss_fft/kiss_fftr.h',
          'log_utils/wt_log.h',
          'log_utils/wt_log.c',
          'proto_utils/wt_proto_common.h',
          'proto_utils/wt_proto_common.c',
          'proto_utils/wt_proto_link_layer.c',
          'proto_utils/wt_proto_link_layer.h',
          'proto_utils/wt_proto_physical_layer.c',
          'proto_utils/wt_proto_physical_layer.h',
          'transceiver/recv/wt_recv_link_layer.c',
          'transceiver/recv/wt_recv_link_layer.h',
          'transceiver/recv/wt_recv_physical_layer.c',
          'transceiver/recv/wt_recv_physical_layer.h',
          'transceiver/send/wt_send_link_layer.c',
          'transceiver/send/wt_send_link_layer.h',
          'transceiver/send/wt_send_physical_layer.c',
          'transceiver/send/wt_send_physical_layer.h',
          'interface/wave_trans_recv.c',
          'interface/wave_trans_recv.h',
          'interface/wave_trans_send.c',
          'interface/wave_trans_send.h',
          'audio_codec/pcm_to_wav.c',
          'audio_codec/pcm_to_wav.h',
        ],
        'dependencies':[
          '<(PRO_ROOT)/rs_codec/librs.gyp:librs',
        ],
      }, # target libwavetrans
      {
        'target_name': 'wave_tran_recv',
        'type': 'executable',
        'include_dirs': [
          './demo',
          '.',
        ],
        'sources': [
          'demo/recv_test.c',
        ],
        'dependencies': [
          'libwavetrans',
          '<(PRO_ROOT)/rs_codec/librs.gyp:librs',
        ],
      }, #target wave_tran_recv
      {
        'target_name': 'wave_tran_send',
        'type': 'executable',
        'include_dirs': [
          './demo',
          '.',
        ],
        'sources': [
          'demo/send_test.c',
        ],
        'dependencies': [
          'libwavetrans',
          '<(PRO_ROOT)/rs_codec/librs.gyp:librs',
        ],
      }, #target wave_tran_send
      {
        'target_name': 'phy_test',
        'type': 'executable',
        'include_dirs': [
          './demo',
          '.',
        ],
        'sources': [
          'demo/phy_test.c',
        ],
        'dependencies': [
          'libwavetrans',
          '<(PRO_ROOT)/rs_codec/librs.gyp:librs',
        ],
      }, #target phy_test
      {
        'target_name': 'test_tools',
        'type': 'executable',
        'include_dirs': [
          './demo',
          '.',
        ],
        'sources': [
          'demo/test_tools.c',
        ],
        'dependencies': [
          'libwavetrans',
          '<(PRO_ROOT)/rs_codec/librs.gyp:librs',
        ],
      }, #target test_tools
    ],
}
