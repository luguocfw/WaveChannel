#
#
# author: ChenFawang
# librs.gyp
# 2019-01-09
# mail: cfwang_9984@163.com
#
{   
    'includes':[
      './../common.gypi',
    ],
    'targets':[
      {
        'target_name': 'librs',
        'type': 'static_library',
        'sources': [
          'rs_code.cc',
          'rs_code.h',
        ],
        'cflags': [
          '-std=c++11',
        ],
        'direct_dependent_settings': {
          'include_dirs': [
            '<(PRO_ROOT)/rs_codec',
          ],
          'ldflags': [
            '-lstdc++',
          ],
        },
      },
      {
        'target_name': 'rs_test',
        'type': 'executable',
        'include_dirs': [
          '.',
        ],
        'sources': [
          'rs_test.c',
        ],
        'dependencies': [
          'librs',
        ],
      }, #target rs_test
    ],
}
