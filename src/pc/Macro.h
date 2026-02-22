#pragma once

#ifdef PC_EXPORT
#define PCAPI __declspec(dllexport)
#else
#define PCAPI __declspec(dllimport)
#endif

#define PCNDAPI [[nodiscard]] PCAPI

#if defined(LL_PLAT_S)
#define PCAPI_C  [[deprecated("Client API not available on SERVER")]] PCAPI
#define PCAPI_S  PCAPI
#define PCNDAPI_C [[deprecated("Client API not available on SERVER")]] PCNDAPI
#define PCNDAPI_S PCNDAPI
#elif defined(LL_PLAT_C)
#define PCAPI_C  PCAPI
#define PCAPI_S  [[deprecated("Server API not available on CLIENT")]] PCAPI
#define PCNDAPI_C PCNDAPI
#define PCNDAPI_S [[deprecated("Server API not available on CLIENT")]] PCNDAPI
#endif