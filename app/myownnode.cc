

// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libplatform/libplatform.h"
#include "v8-context.h"
#include "v8-initialization.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-primitive.h"
#include "v8-script.h"

#include "./myuv.h"

using v8::Isolate;
using v8::Local;
using v8::MaybeLocal;
using v8::NewStringType;
using v8::String;
using v8::V8;

const char *ToCString(const v8::String::Utf8Value &value)
{
    return *value ? *value : "<string conversion failed>";
}

// Executes a string within the current v8 context.
// bool ExecuteString(v8::Isolate *isolate, v8::Local<v8::String> source,
//                    v8::Local<v8::Value> name, bool print_result,
//                    bool report_exceptions)
// {
//     v8::HandleScope handle_scope(isolate);
//     v8::TryCatch try_catch(isolate);
//     v8::ScriptOrigin origin(name);
//     v8::Local<v8::Context> context(isolate->GetCurrentContext());
//     v8::Local<v8::Script> script;
//     if (!v8::Script::Compile(context, source, &origin).ToLocal(&script))
//     {
//         // Print errors that happened during compilation.
//         // if (report_exceptions)
//         //     ReportException(isolate, &try_catch);
//         return false;
//     }
//     else
//     {
//         v8::Local<v8::Value> result;
//         if (!script->Run(context).ToLocal(&result))
//         {
//             assert(try_catch.HasCaught());
//             // Print errors that happened during execution.
//             // if (report_exceptions)
//             //     ReportException(isolate, &try_catch);
//             return false;
//         }
//         else
//         {
//             assert(!try_catch.HasCaught());
//             if (print_result && !result->IsUndefined())
//             {
//                 // If all went well and the result wasn't undefined then print
//                 // the returned value.
//                 v8::String::Utf8Value str(isolate, result);
//                 const char *cstr = ToCString(str);
//                 printf("%s\n", cstr);
//             }
//             return true;
//         }
//     }
// }

// Reads a file into a v8 string.
MaybeLocal<String> ReadFile(Isolate *isolate, const char *name)
{
    FILE *file = fopen(name, "rb");
    if (file == NULL)
        return MaybeLocal<String>();
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    char *chars = new char[size + 1];
    chars[size] = '\0';
    for (size_t i = 0; i < size;)
    {
        i += fread(&chars[i], 1, size - i, file);
        if (ferror(file))
        {
            fclose(file);
            return MaybeLocal<String>();
        }
    }
    fclose(file);
    MaybeLocal<String> result = String::NewFromUtf8(
        isolate, chars, NewStringType::kNormal, static_cast<int>(size));
    delete[] chars;
    return result;
}

int runV8(int argc, char *argv[])
{
    // Initialize V8.
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);
    {
        v8::Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);

        // Create a new context.
        Local<v8::Context> context = v8::Context::New(isolate);

        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);

        {
            Local<String> source;
            ReadFile(isolate, argv[1])
                .ToLocal(&source);

            v8::ScriptOrigin origin(isolate, v8::String::NewFromUtf8(context->GetIsolate(), argv[1]).ToLocalChecked());

            Local<v8::Script> script =
                v8::Script::Compile(context, source, &origin).ToLocalChecked();

            // Run the script to get the result.
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(isolate, result);
            printf("%s\n", *utf8);
        }
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete create_params.array_buffer_allocator;
    return 0;
}

int main(int argc, char *argv[])
{

    runV8(argc, argv);
    runUV();

    return 0;
}
