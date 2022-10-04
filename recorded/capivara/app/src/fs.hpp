#include "v8.h"

class Fs
{
public:

  // Copied from https://github.com/v8/v8/blob/master/samples/shell.cc
  static v8::MaybeLocal<v8::String> ReadFile(v8::Isolate *isolate, const char *name)
  {
    FILE *file = fopen(name, "rb");
    if (file == NULL)
      return v8::MaybeLocal<v8::String>();

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
        return v8::MaybeLocal<v8::String>();
      }
    }
    fclose(file);

    v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(
        isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
    delete[] chars;
    return result;
  }
};