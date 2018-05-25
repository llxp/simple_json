#pragma once
#ifdef _USRDLL
	#ifndef DLLEXPORT
	#define DLLEXPORT __declspec(dllexport)
	#endif
#else
	#ifndef DLLEXPORT
	#define DLLEXPORT
	#endif
#endif