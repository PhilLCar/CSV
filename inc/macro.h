#ifdef WIN
#ifdef CSV_EXPORT
#define CSV_API  __declspec(dllexport)
#else
#define CSV_API __declspec(dllimport)
#endif
#else
#define CSV_API
#endif
