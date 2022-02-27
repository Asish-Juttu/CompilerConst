#ifndef _LOG_
#define _LOG_
#ifdef ENABLE_LOG
#define LOG(...) printf(__VA_ARGS__)
#else 
#define LOG(...)
#endif
#endif
