#pragma once

#include <cstddef>
#include <sal.h>

#if defined(TSDPointClusterTestEvaluationIndexCalculationExport)
#	define DLLEXPORT _declspec(dllexport)
#else
#	define DLLEXPORT _declspec(dllimport)
#endif

extern "C" DLLEXPORT bool __stdcall TSDPointClusterTestEvaluationIndexCalculation_v1(_In_ const char* input_json, _Out_writes_to_opt_(*output_json_size, *output_json_size + 1) char* output_json, _Inout_ unsigned long* output_json_size);