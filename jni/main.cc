/*
 * This file contains sample wrappers showing Android Java Developers
 * who are not familiar with the NDK or JNI how to get data in and out of the
 * CPP and how to incorporate the sample code in
 * http://leveldb.googlecode.com/svn/trunk/doc/index.html into their JNI
 */
#include <string.h>
#include <jni.h>

#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>

#include "leveldb/db.h"

#define  LOG_TAG    "AndroidLevelDB"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define JOWW(rettype, name)                                             \
  extern "C" rettype JNIEXPORT JNICALL Java_com_google_code_p_leveldb_##name

static leveldb::DB* getDB(JNIEnv *env, jobject object) {
    jclass cls = (*env).GetObjectClass(object);
    jfieldID field = (*env).GetFieldID(cls, "db", "J");
    jlong db = (*env).GetLongField(object, field);
    return (leveldb::DB*)db;
}

static void setDB(JNIEnv *env, jobject object, leveldb::DB *db) {
    jclass cls = (*env).GetObjectClass(object);
    jfieldID field = (*env).GetFieldID(cls, "db", "J");
    (*env).SetLongField(object, field, (jlong)db);
}

JOWW(jstring, LevelDB_get)(JNIEnv * env, jobject thiz, jstring key1) {

    leveldb::DB *db = getDB(env, thiz);
	if(!db) {
        return NULL;
	}
	const char* key = env->GetStringUTFChars(key1,0);

	std::string value;
	leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &value);

	if (status.ok()) {
		const char* re =  value.c_str();
		return env->NewStringUTF(re);
	}else{
        return NULL;
	}
}

JOWW(jboolean, LevelDB_set)(JNIEnv * env, jobject thiz, jstring key1, jstring value1)
{
    leveldb::DB *db = getDB(env, thiz);
	if(!db){
        return JNI_FALSE;
	}

	const char* key = env->GetStringUTFChars(key1,0);
	const char* value = env->GetStringUTFChars(value1,0);

	leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);
	if (status.ok()) {
        return JNI_TRUE;
	}else{
        return JNI_FALSE;
	}
}


JOWW(jboolean, LevelDB_delete)(JNIEnv * env, jobject thiz, jstring key1)
{
    leveldb::DB *db = getDB(env, thiz);
    if (!db) {
        return JNI_FALSE;
    }

	const char* key = env->GetStringUTFChars(key1,0);
	leveldb::Status status = db->Delete(leveldb::WriteOptions(), key);
	if (status.ok()) {
        return JNI_TRUE;
	}else{
        return JNI_FALSE;
	}
}


JOWW(jboolean, LevelDB_open)(JNIEnv* env, jobject thiz, jstring dbpath)
{
    leveldb::DB *db = getDB(env, thiz);
	if (db) {
        return JNI_TRUE;
	}
	const char* path = env->GetStringUTFChars(dbpath,0);

	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, path, &db);
	if (status.ok()) {
        setDB(env, thiz, db);
        return JNI_TRUE;
	} else {
        return JNI_FALSE;
	}
}

JOWW(void, LevelDB_close)(JNIEnv* env, jobject thiz) {
    leveldb::DB *db = getDB(env, thiz);
    delete db;
    setDB(env, thiz, NULL);
}

JOWW(jobject, LevelDB_newIterator)(JNIEnv *env, jobject thiz) {
    leveldb::DB *db = getDB(env, thiz);
    if (!db) {
        return NULL;
    }
    leveldb::Iterator *iter = db->NewIterator(leveldb::ReadOptions());
    jclass cls = env->FindClass("com/google/code/p/leveldb/LevelDBIterator");
    jmethodID init_method = env->GetMethodID(cls, "<init>", "(J)V"); 
    
    jobject o =  env->NewObject(cls, init_method, (jlong)iter);
    return o;
}

static leveldb::Iterator* getIter(JNIEnv *env, jobject object) {
    jclass cls = (*env).GetObjectClass(object);
    jfieldID field = (*env).GetFieldID(cls, "iter", "J");
    jlong db = (*env).GetLongField(object, field);
    return (leveldb::Iterator*)db;
}

static void setIter(JNIEnv *env, jobject object, leveldb::Iterator *iter) {
    jclass cls = (*env).GetObjectClass(object);
    jfieldID field = (*env).GetFieldID(cls, "iter", "J");
    (*env).SetLongField(object, field, (jlong)iter);
}

JOWW(void , LevelDBIterator_seekToLast)(JNIEnv *env, jobject object) {
    leveldb::Iterator *iter = getIter(env, object);
    iter->SeekToLast();
}

JOWW(void , LevelDBIterator_seekToFirst)(JNIEnv *env, jobject object) {
    leveldb::Iterator *iter = getIter(env, object);
    iter->SeekToFirst();
}

JOWW(void , LevelDBIterator_seek)(JNIEnv *env, jobject object, jstring target) {
    leveldb::Iterator *iter = getIter(env, object);
    const char* path = env->GetStringUTFChars(target,0);
    leveldb::Slice t(path);
    iter->Seek(t);
}

JOWW(jboolean, LevelDBIterator_isValid)(JNIEnv *env, jobject object) {
    leveldb::Iterator *iter = getIter(env, object);
    return iter->Valid();
}

JOWW(void, LevelDBIterator_next)(JNIEnv *env, jobject object) {
    leveldb::Iterator *iter = getIter(env, object);
    iter->Next();
}

JOWW(void, LevelDBIterator_prev)(JNIEnv *env, jobject object) {
    leveldb::Iterator *iter = getIter(env, object);
    iter->Prev();
}

JOWW(jstring, LevelDBIterator_getKey)(JNIEnv *env, jobject object) {
    leveldb::Iterator *iter = getIter(env, object);
    leveldb::Slice k = iter->key();
    std::string s = k.ToString();
    return env->NewStringUTF(s.c_str());
}

JOWW(jstring, LevelDBIterator_getValue)(JNIEnv *env, jobject object) {
    leveldb::Iterator *iter = getIter(env, object);
    leveldb::Slice k = iter->value();
    std::string s = k.ToString();
    return env->NewStringUTF(s.c_str());
}

JOWW(void , LevelDBIterator_close)(JNIEnv *env, jobject object) {
    leveldb::Iterator *iter = getIter(env, object);
    delete iter;
    setIter(env, object, NULL);
}
