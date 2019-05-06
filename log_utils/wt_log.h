/*
#
# Copyright 2018, sohu ipc
# author: ChenFawang
# main.cc
# 2018-01-26
# mail: fawangchen@sohu-inc.com
*/
#ifndef __WT_LOG_H__
#define __WT_LOG_H__
#include <stdio.h>


//#define WT_LOG_ON
//#define WT_LOG_TO_FILE
#define LOG_FILE_PATH                   "wt.log"
extern FILE *log_fp_;
#ifdef WT_LOG_TO_FILE
#define WtLogOpen()             do{\
                                  log_fp_ = fopen(LOG_FILE_PATH,"w");\
                                }while (0);
#define WtLogClose()            do{\
                                  if(log_fp_!=NULL){\
                                    fclose(log_fp_); \
                                    log_fp_ = NULL;\
                                  }\
                                }while (0);
#else
#define WtLogOpen()
#define WtLogClose()
#endif

#ifdef WT_LOG_ON
#ifndef WIN32
#ifndef WT_LOG_TO_FILE
#define WtLog(fmt,args...)              printf(fmt,##args)         
#else
#define WtLog(fmt,args...)              do{\
                                          if (log_fp_ != NULL) {\
                                            fprintf(log_fp_,fmt, ##args);\
                                          }\
                                        }while(0);
#endif
#else
#ifndef WT_LOG_TO_FILE
#define WtLog(format,...)               printf(format,##__VA_ARGS__)
#else
#define WtLog(format,...)               do{\
                                          if (log_fp_ != NULL) {\
                                            fprintf(log_fp_,format, ##__VA_ARGS__);\
                                          }\
                                        }while(0);
#endif
#endif
#else
#ifndef WIN32
#define WtLog(fmt,args...)                       
#else
#define WtLog(format,...)               
#endif
#endif





#endif