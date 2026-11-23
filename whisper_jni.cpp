#include <jni.h>
#include <atomic>
#include <mutex>
#include <string>
#include "whisper.h"

static whisper_context *g_ctx = nullptr;
static std::atomic<bool> g_abort(false);

static std::atomic<int> g_progress(0);
static std::mutex g_mu;
static std::string g_partial;

static bool abort_cb(void *) { return g_abort.load(); }

static void progress_cb(struct whisper_context *, struct whisper_state *, int progress, void *) {
    g_progress.store(progress);
}

static void new_segment_cb(struct whisper_context *ctx, struct whisper_state *, int n_new, void *) {
    int n = whisper_full_n_segments(ctx);
    std::lock_guard<std::mutex> lk(g_mu);
    for (int i = n - n_new; i < n; i++) {
        if (i < 0) continue;
        const char *t = whisper_full_get_segment_text(ctx, i);
        long long t0 = (long long) whisper_full_get_segment_t0(ctx, i);
        std::string text(t != nullptr ? t : "");
        for (char &c : text) if (c == '\n' || c == '\r') c = ' ';
        g_partial += std::to_string(t0);
        g_partial += "|";
        g_partial += text;
        g_partial += "\n";
    }
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_shaikh_seamless_Whisper_nInit(JNIEnv *env, jobject, jstring jpath) {
    if (g_ctx != nullptr) return JNI_TRUE;
    const char *path = env->GetStringUTFChars(jpath, nullptr);
    whisper_context_params cp = whisper_context_default_params();
    cp.use_gpu = false;
    g_ctx = whisper_init_from_file_with_params(path, cp);
    env->ReleaseStringUTFChars(jpath, path);
    return g_ctx != nullptr ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_shaikh_seamless_Whisper_nRelease(JNIEnv *, jobject) {
    if (g_ctx != nullptr) {
        whisper_free(g_ctx);
        g_ctx = nullptr;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_shaikh_seamless_Whisper_nCancel(JNIEnv *, jobject) {
    g_abort.store(true);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_shaikh_seamless_Whisper_nProgress(JNIEnv *, jobject) {
    return (jint) g_progress.load();
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_shaikh_seamless_Whisper_nPartial(JNIEnv *env, jobject) {
    std::string copy;
    {
        std::lock_guard<std::mutex> lk(g_mu);
        copy = g_partial;
    }
    jbyteArray arr = env->NewByteArray((jsize) copy.size());
    if (!copy.empty()) env->SetByteArrayRegion(arr, 0, (jsize) copy.size(), reinterpret_cast<const jbyte *>(copy.data()));
    return arr;
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_shaikh_seamless_Whisper_nTranscribe(JNIEnv *env, jobject, jfloatArray jsamples, jint threads, jstring jlang) {
    std::string out;
    if (g_ctx != nullptr) {
        g_abort.store(false);
        g_progress.store(0);
        { std::lock_guard<std::mutex> lk(g_mu); g_partial.clear(); }
        const char *l = env->GetStringUTFChars(jlang, nullptr);
        std::string lang(l != nullptr ? l : "auto");
        if (l != nullptr) env->ReleaseStringUTFChars(jlang, l);

        jsize n = env->GetArrayLength(jsamples);
        jfloat *data = env->GetFloatArrayElements(jsamples, nullptr);

        whisper_full_params p = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
        p.n_threads = threads;
        p.translate = false;
        p.no_context = true;
        p.single_segment = false;
        p.print_special = false;
        p.print_progress = false;
        p.print_realtime = false;
        p.print_timestamps = false;
        p.token_timestamps = true;
        p.max_len = 40;
        p.split_on_word = true;
        p.suppress_blank = true;
        p.temperature_inc = 0.0f;
        p.language = lang.c_str();
        p.abort_callback = abort_cb;
        p.abort_callback_user_data = nullptr;
        p.progress_callback = progress_cb;
        p.progress_callback_user_data = nullptr;
        p.new_segment_callback = new_segment_cb;
        p.new_segment_callback_user_data = nullptr;

        int rc = whisper_full(g_ctx, p, data, (int) n);
        env->ReleaseFloatArrayElements(jsamples, data, JNI_ABORT);

        if (rc == 0) {
            int ns = whisper_full_n_segments(g_ctx);
            for (int i = 0; i < ns; i++) {
                const char *t = whisper_full_get_segment_text(g_ctx, i);
                long long t0 = (long long) whisper_full_get_segment_t0(g_ctx, i);
                std::string text(t != nullptr ? t : "");
                for (char &c : text) if (c == '\n' || c == '\r') c = ' ';
                out += std::to_string(t0);
                out += "|";
                out += text;
                out += "\n";
            }
        }
    }
    jbyteArray arr = env->NewByteArray((jsize) out.size());
    if (!out.empty()) env->SetByteArrayRegion(arr, 0, (jsize) out.size(), reinterpret_cast<const jbyte *>(out.data()));
    return arr;
}
