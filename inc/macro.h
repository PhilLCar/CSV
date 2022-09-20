#ifdef WIN
#ifdef CSV_EXPORT
#define CSV_API  __declspec(dllexport)
#else
#define CSV_API __declspec(dllimport)
#endif
#else
#define CSV_API
#endif

#ifdef CSV_EXTERN
#define CSV_LINK extern "C"
#define M(CLASS, NAME) CLASS ## _ ## NAME
#define CSV_BEGIN
#define CSV_END
#else
#define CSV_LINK
#define M(CLASS, NAME) NAME
#define CSV_BEGIN namespace csv {
#define CSV_END }
#endif

#define MOVE(Type, Pointer) Type* _type = Pointer; Type _return = *_type; delete _type; return _return