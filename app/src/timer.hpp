struct timer
{
  uv_timer_t req;
  v8::Isolate *isolate;
  v8::Global<v8::Function> cb;
};

uv_loop_t *loop;

class Timer
{

public:
  static void Initialize(uv_loop_t *evloop)
  {
    loop = evloop;
  }

  static void Timeout(const v8::FunctionCallbackInfo<v8::Value> &args)
  {
    auto isolate = args.GetIsolate();

    auto context = isolate->GetCurrentContext();
    int64_t delay = args[0]->IntegerValue(context).ToChecked();
    int64_t repeat = args[1]->IntegerValue(context).ToChecked();
    v8::Local<v8::Value> callback = args[2];

    if (!callback->IsFunction())
    {
      printf("callback not declared!");
      return;
    }

    timer *timerWrap = new timer();

    timerWrap->isolate = isolate;
    timerWrap->cb.Reset(isolate, callback.As<v8::Function>());
    timerWrap->req.data = (void *)timerWrap;

    uv_timer_init(loop, &timerWrap->req);
    uv_timer_start(&timerWrap->req, OnTimerCb, delay, repeat);
  }

  static void OnTimerCb(uv_timer_t *handle)
  {
    timer *timerWrap = (timer *)handle->data;

    v8::Isolate *isolate = timerWrap->isolate;
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    if (isolate->IsDead())
    {
      printf("isolate is dead\n");
      return;
    }

    v8::Local<v8::Value> result;
    v8::Handle<v8::Value> resultr[] = {v8::Undefined(isolate), v8_str("hello world")};
    v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(isolate, timerWrap->cb);

    if (callback->Call(
                    context,
                    v8::Undefined(isolate),
                    2,
                    resultr)
            .ToLocal(&result))
    {
      // Ok, the callback succeded
    }
    else
    {
      // some exception happened
    }
  }
};
