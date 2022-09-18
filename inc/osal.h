#ifdef WIN
#ifdef CSV_EXPORT
#define CSV_API  __declspec(dllexport)
#define CSV_LINK extern "C"
#else
#define CSV_API __declspec(dllimport)
#define CSV_LINK
#endif
#else
#define CSV_API
#define CSV_LINK
#endif