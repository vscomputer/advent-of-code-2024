#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstdio> 

class DetectsSafeReports
{
public:
	DetectsSafeReports();
	~DetectsSafeReports();
	void LoadReports(std::string filename);
	int GetNumberOfReports();
	std::vector<std::vector<int>> Reports;
	bool ReportIsSafe(int reportNumber);
private:
	
};

DetectsSafeReports::DetectsSafeReports()
{
}

DetectsSafeReports::~DetectsSafeReports()
{
}

void DetectsSafeReports::LoadReports(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	bool fileIsOpen;

	fileIsOpen = file.is_open();

	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string numberAsString;
		std::regex delimiter(" ");

		std::sregex_token_iterator it(line.begin(), line.end(), delimiter, -1);
		std::sregex_token_iterator end;
		std::vector<int> report;

		while (it != end) {
			report.push_back(std::stoi(*it));
			++it;
		}
		Reports.push_back(report);
	}
}

int DetectsSafeReports::GetNumberOfReports()
{
	return Reports.size();
}

bool DetectsSafeReports::ReportIsSafe(int reportNumber)
{
	std::vector<int> report = Reports[reportNumber];

	/*for (int i = 0; i < report.size(); i++)
	{
		std::cout << report[i] << " ";
	}*/

	int current = -1;
	int next = -1;
	bool descending = false;

	for (int i = 0; i < report.size() - 1; i++)
	{		
		current = report[i];
		next = report[i + 1];
		if (i == 0 )
		{
			descending = current > next;
		}
		else if ((descending && next > current) || (!descending && next < current))
		{
			return false;
		}
		
		if (abs(current - next) > 3 || abs(current - next) == 0)
			return false;
	}
	
	return true;
}

TEST(DetectsSafeReports, LoadReports_TestFile_CorrectNumberOfReports)
{
	DetectsSafeReports subject;

	subject.LoadReports("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-2-test.txt");

	int result = subject.GetNumberOfReports();
	EXPECT_EQ(result, 6);
}

TEST(DetectsSafeReports, LoadReports_TestFile_CorrectSampleData)
{
	DetectsSafeReports subject;

	subject.LoadReports("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-2-test.txt");

	EXPECT_EQ(subject.Reports[0][0], 7);	
	EXPECT_EQ(subject.Reports[5][4], 9);
}

TEST(DetectsSafeReports, ReportIsSafe_SampleData_CorrectReports)
{
	DetectsSafeReports subject;

	subject.LoadReports("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-2-test.txt");

	EXPECT_TRUE(subject.ReportIsSafe(0)); // because the levels are all decreasing by 1 or 2.
	EXPECT_FALSE(subject.ReportIsSafe(1)); // because 2 7 is an increase of 5.
	EXPECT_FALSE(subject.ReportIsSafe(2)); // Unsafe because 6 2 is a decrease of 4.
	EXPECT_FALSE(subject.ReportIsSafe(3)); // because 1 3 is increasing but 3 2 is decreasing.
	EXPECT_FALSE(subject.ReportIsSafe(4)); // because 4 4 is neither an increase or a decrease.
	EXPECT_TRUE(subject.ReportIsSafe(5)); // because the levels are all decreasing by 1,2, or 3.
}

TEST(DetectsSafeReports, DISABLED_ReportIsSafe_ActualData_CorrectReports)
{
	DetectsSafeReports subject;

	subject.LoadReports("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-2-actual-real.txt");

	int result = 0;

	for (int i = 0; i < subject.GetNumberOfReports(); i++)
	{
		bool safe = subject.ReportIsSafe(i);

		if (safe)
			result++;
		//std::cout << " " << safe << std::endl;
	}

	EXPECT_EQ(result, 0);
}