#include <TSDPointClusterTestEvaluationIndexCalculationExport.h>

#include <iostream>

int main(int argc, char* argv[])
{
	const char* test_input_json = R"({
	"standard_station_data": [
		{
			"salinity": 0,
			"temperature": 0,
			"depth": 1
		},
		{
			"salinity": 4,
			"temperature": 0,
			"depth": 2
		},
		{
			"salinity": 4,
			"temperature": 4,
			"depth": 3
		},
		{
			"salinity": 2,
			"temperature": 2,
			"depth": 4
		},
		{
			"salinity": 0,
			"temperature": 4,
			"depth": 5
		}
	],
	"test_station_data": [
		{
			"salinity": 0,
			"temperature": 6,
			"depth": 10
		},
		{
			"salinity": 10,
			"temperature": 10, 
			"depth": 11
		}
	]
})";

	char output_json[1024];
	unsigned long size = sizeof(output_json);
	bool result = TSDPointClusterTestEvaluationIndexCalculation_v1(test_input_json, output_json, &size);

	std::cout << output_json << "\n";

	//output_jsonÊ¾Àý:
	//{
	//	"N": 13.4
	//}
}