#pragma once

#ifdef CENTERSCREEN_EXPORTS
#define CENTERSCREEN_API __declspec(dllexport)
#else
#define CENTERSCREEN_API __declspec(dllimport)
#endif
