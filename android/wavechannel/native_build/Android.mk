LOCAL_PATH:=$(call my-dir)
CONFIG_BUILD_FOR_NMOB := true
include $(CLEAR_VARS)
SRC_PATH := $(LOCAL_PATH)/../../..

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../cpp
LOCAL_C_INCLUDES += $(SRC_PATH)
LOCAL_C_INCLUDES += $(SRC_PATH)/rs_codec

LOCAL_SRC_FILES:= \
        $(LOCAL_PATH)/../cpp/native.cpp \
        $(LOCAL_PATH)/../cpp/env_storage.cpp \
        $(LOCAL_PATH)/../cpp/java_string_buffer.cpp \
        $(LOCAL_PATH)/../cpp/jni_utils.cpp \
        $(LOCAL_PATH)/../cpp/pcm_info.cpp \
        $(LOCAL_PATH)/../cpp/wav_info.cpp \
	    $(SRC_PATH)/buff_utils/queue.c \
        $(SRC_PATH)/buff_utils/ring_buff.c \
        $(SRC_PATH)/checksum_utils/crc_codec.c \
        $(SRC_PATH)/checksum_utils/parity_codec.c \
        $(SRC_PATH)/kiss_fft/kiss_fastfir.c \
        $(SRC_PATH)/kiss_fft/kiss_fft.c \
        $(SRC_PATH)/kiss_fft/kiss_fftr.c \
        $(SRC_PATH)/log_utils/wt_log.c \
        $(SRC_PATH)/proto_utils/wt_proto_common.c \
        $(SRC_PATH)/proto_utils/wt_proto_link_layer.c \
        $(SRC_PATH)/proto_utils/wt_proto_physical_layer.c \
        $(SRC_PATH)/transceiver/recv/wt_recv_link_layer.c \
        $(SRC_PATH)/transceiver/recv/wt_recv_physical_layer.c \
        $(SRC_PATH)/transceiver/send/wt_send_link_layer.c \
        $(SRC_PATH)/transceiver/send/wt_send_physical_layer.c \
        $(SRC_PATH)/interface/wave_trans_recv.c \
        $(SRC_PATH)/interface/wave_trans_send.c \
        $(SRC_PATH)/audio_codec/pcm_to_wav.c \
        $(SRC_PATH)/rs_codec/rs_code.cc \


LOCAL_CXXFLAGS := -Wno-write-strings


LOCAL_MODULE:= wavetrans
LOCAL_LDLIBS := -llog -lz -ljnigraphics
LOCAL_CPPFLAGS := $(GLOBAL_CFLAGS) -fvisibility=hidden

include $(BUILD_SHARED_LIBRARY)
