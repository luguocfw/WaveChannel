#
#
# author: ChenFawang
# main.cc
# 2019-01-09
# mail: cfwang_9984@163.com
#
{
  'target_defaults': {
    'conditions' :[
        ['OS=="win"', {
          'defines': [
            'WIN32',
          ],
          'configurations': {
            'Debug': {
              'msvs_settings':{
                'VCCLCompilerTool': {
                  'RuntimeLibrary': '3',
                  'Optimization':'0',
                  'DebugInformationFormat':'4',
                },
                'VCLinkerTool': {
                  'GenerateDebugInformation': 'true',
                  'GenerateMapFile': 'false',
                  'SubSystem': '1',
                },
              },
            }, #Debug
            'Release': {
              'msvs_settings': {
                'VCCLCompilerTool': {
                  'RuntimeLibrary': '2',
                  'Optimization' : '2',
                  'EnableIntrinsicFunctions':'true',
                  'DebugInformationFormat': '3',
                },
                'VCLinkerTool': {
                  'GenerateDebugInformation': 'true',
                  'GenerateMapFile': 'false,'
                },
              }, #msvs_settings
            }, #Release
          }, #configurations
        }],  #OS=="win"
      ], #conditions
  },
}