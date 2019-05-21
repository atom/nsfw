#ifndef NSFW_H
#define NSFW_H

#include "NativeInterface.h"
#include <nan.h>
#include <uv.h>
#include <vector>
#include <atomic>

using namespace Nan;

class NSFW : public ObjectWrap {
public:
  static NAN_MODULE_INIT(Init);

  static void cleanupEventCallback(void *arg);
  static void fireErrorCallback(uv_async_t *handle);
  static void fireEventCallback(uv_async_t *handle);
  static void pollForEvents(void *arg);

  Persistent<v8::Object> mPersistentHandle;
  uint32_t mDebounceMS;
  uv_async_t mErrorCallbackAsync;
  uv_async_t mEventCallbackAsync;
  AsyncResource *mResource;
  Callback *mErrorCallback;
  Callback *mEventCallback;
  NativeInterface *mInterface;
  uv_mutex_t mInterfaceLock;
  bool mInterfaceLockValid;
  std::string mPath;
  uv_thread_t mPollThread;
  std::atomic<bool> mRunning;
private:
  NSFW(uint32_t debounceMS, std::string path, AsyncResource *resource, Callback *eventCallback, Callback *errorCallback);
  ~NSFW();

  struct ErrorBaton {
    NSFW *nsfw;
    std::string error;
  };

  struct EventBaton {
    NSFW *nsfw;
    std::unique_ptr<std::vector<std::unique_ptr<Event>>> events;
  };

  static NAN_METHOD(JSNew);

  static NAN_METHOD(Start);
  class StartWorker : public AsyncWorker {
  public:
    StartWorker(NSFW *nsfw, AsyncResource *resource, Callback *callback);
    void Execute();
    void HandleOKCallback();
  private:
    NSFW *mNSFW;
    AsyncResource *mResource;
  };

  static NAN_METHOD(Stop);
  class StopWorker : public AsyncWorker {
  public:
    StopWorker(NSFW *nsfw, AsyncResource *resource, Callback *callback);
    void Execute();
    void HandleOKCallback();
  private:
    NSFW *mNSFW;
    AsyncResource *mResource;
  };

  static Persistent<v8::Function> constructor;
};

#endif
