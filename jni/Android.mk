# Copyright (c) 2011 The LevelDB Authors. All rights reserved.                 # Use of this source code is governed by a BSD-style license that can be       # found in the LICENSE file. See the AUTHORS file for names of contributors.   

# To build for Android, add the Android NDK to your path and type 'ndk-build'.

#LOCAL_PATH := $(call my-dir)
# 
#include $(CLEAR_VARS)
# 
#include common.mk
# 
#LOCAL_MODULE := leveldb
#LOCAL_C_INCLUDES := $(C_INCLUDES)
#LOCAL_CPP_EXTENSION := .cc
#LOCAL_CFLAGS := -DLEVELDB_PLATFORM_ANDROID  -std=gnu++0x
#LOCAL_SRC_FILES := $(SOURCES:%.cc=../%.cc) main.cc
#LOCAL_LDLIBS +=  -llog -ldl
# 
# 
#include $(BUILD_SHARED_LIBRARY)



LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := leveldb
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include
LOCAL_CPP_EXTENSION := .cc
LOCAL_CFLAGS := -DLEVELDB_PLATFORM_ANDROID -std=gnu++0x
LOCAL_SRC_FILES := main.cc
LOCAL_STATIC_LIBRARIES +=  leveldbnative
LOCAL_LDLIBS +=  -llog -ldl

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := leveldbnative
LOCAL_CFLAGS := -D_REENTRANT -DOS_ANDROID -DLEVELDB_PLATFORM_POSIX -DNDEBUG 
LOCAL_CPP_EXTENSION := .cc
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/../  $(LOCAL_PATH)/../include 
LOCAL_SRC_FILES := \
../db/builder.cc \
../db/c.cc \
../db/db_impl.cc \
../db/db_iter.cc \
../db/dbformat.cc \
../db/filename.cc \
../db/leveldbutil.cc \
../db/log_reader.cc \
../db/log_writer.cc \
../db/memtable.cc \
../db/repair.cc \
../db/table_cache.cc \
../db/version_edit.cc \
../db/version_set.cc \
../db/write_batch.cc \
../table/block.cc \
../table/block_builder.cc \
../table/filter_block.cc \
../table/format.cc \
../table/iterator.cc \
../table/merger.cc \
../table/table.cc \
../table/table_builder.cc \
../table/two_level_iterator.cc \
../util/arena.cc \
../util/bloom.cc \
../util/cache.cc \
../util/coding.cc \
../util/comparator.cc \
../util/crc32c.cc \
../util/env.cc \
../util/env_posix.cc \
../util/filter_policy.cc \
../util/hash.cc \
../util/histogram.cc \
../util/logging.cc \
../util/options.cc \
../util/status.cc \
../port/port_posix.cc \
../port/port_posix_sse.cc

include $(BUILD_STATIC_LIBRARY)

