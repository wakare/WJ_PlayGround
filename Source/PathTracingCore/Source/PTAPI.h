#pragma once

#ifdef PT_API_EXPORT
#define PT_API __declspec(dllexport)
#else
#define PT_API __declspec(dllimport)
#endif
