#include <TSDPointClusterTestEvaluationIndexCalculation.h>
#include <TSDPointClusterTestEvaluationIndexCalculationExport.h>

#include <cstring>

namespace TSDPointClusterTestEvaluationIndexCalculation
{
	//
}

extern "C" DLLEXPORT bool __stdcall TSDPointClusterTestEvaluationIndexCalculation_v1(_In_ const char* input_json, _Out_writes_to_opt_(*output_json_size, *output_json_size + 1) char* output_json, _Inout_ unsigned long* output_json_size)
{
	std::string output_json_;
	bool result = TSDPointClusterTestEvaluationIndexCalculation::N(input_json, output_json_);

	std::strncpy(output_json, output_json_.c_str(), *output_json_size);

	return result;
}
