#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <cstdio> 

class UnsafeReport
{
public:
	UnsafeReport();
	~UnsafeReport();
	std::vector<int> Report;
	int CurrentWhenRejected;
private:

};

UnsafeReport::UnsafeReport()
{
}

UnsafeReport::~UnsafeReport()
{
}

class DetectsSafeReports
{
public:
	DetectsSafeReports();
	~DetectsSafeReports();
	void LoadReports(std::string filename);
	int GetNumberOfReports();
	std::vector<std::vector<int>> Reports;
	bool ReportIsSafe(int reportNumber);
	bool ReportIsSafe(std::vector<int> report, bool collectUnsafeReports);
	std::vector<UnsafeReport> UnsafeReports;
	bool UnsafeReportIsTolerable(int unsafeReportNumber);
private:
	void CollectUnsafeReport(std::vector<int> report, int current);
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
	return ReportIsSafe(Reports[reportNumber], true);	
}

bool DetectsSafeReports::ReportIsSafe(std::vector<int> report, bool collectUnsafeReports = true)
{
	int current = -1;
	int next = -1;
	bool descending = false;

	for (int i = 0; i < report.size() - 1; i++)
	{
		current = report[i];
		next = report[i + 1];
		if (i == 0)
		{
			descending = current > next;
		}
		else if ((descending && next > current) || (!descending && next < current))
		{
			if(collectUnsafeReports)
				CollectUnsafeReport(report, i);
			return false;
		}

		if (abs(current - next) > 3 || abs(current - next) == 0)
		{
			if (collectUnsafeReports)
				CollectUnsafeReport(report, i);
			return false;
		}
	}

	return true;
}

bool DetectsSafeReports::UnsafeReportIsTolerable(int unsafeReportNumber)
{	
	if (UnsafeReports[unsafeReportNumber].CurrentWhenRejected >= 1)
	{
		std::vector<int> leftCandidate = UnsafeReports[unsafeReportNumber].Report;
		leftCandidate.erase(leftCandidate.begin() + UnsafeReports[unsafeReportNumber].CurrentWhenRejected - 1);
		if (ReportIsSafe(leftCandidate, false))
			return true;
	}

	std::vector<int> centerCandidate = UnsafeReports[unsafeReportNumber].Report;
	centerCandidate.erase(centerCandidate.begin() + UnsafeReports[unsafeReportNumber].CurrentWhenRejected);
	if (ReportIsSafe(centerCandidate, false))
		return true;

	std::vector<int> rightCandidate = UnsafeReports[unsafeReportNumber].Report;
	rightCandidate.erase(rightCandidate.begin() + UnsafeReports[unsafeReportNumber].CurrentWhenRejected + 1);
	if (ReportIsSafe(rightCandidate, false))
		return true;
	
	return false;
}

void DetectsSafeReports::CollectUnsafeReport(std::vector<int> report, int current)
{
	UnsafeReport unsafeReport;
	unsafeReport.Report = report;
	unsafeReport.CurrentWhenRejected = current;
	UnsafeReports.push_back(unsafeReport);
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
	}

	EXPECT_EQ(result, -1);
}

TEST(DetectsSafeReports, DISABLED_ReportIsSafe_ActualData_CorrectUnsafeReports)
{
	DetectsSafeReports subject;

	subject.LoadReports("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-2-actual-real.txt");
	
	for (int i = 0; i < subject.GetNumberOfReports(); i++)
	{
		subject.ReportIsSafe(i);				
	}

	EXPECT_EQ(subject.UnsafeReports.size(), -1);
}

TEST(DetectsSafeReports, UnsafeReportIsTolerable_SampleData_CorrectReports)
{
	DetectsSafeReports subject;

	subject.LoadReports("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-2-test.txt");
	for (int i = 0; i < subject.GetNumberOfReports(); i++)
	{
		subject.ReportIsSafe(i);
	}

	EXPECT_FALSE(subject.UnsafeReportIsTolerable(0)); //Unsafe regardless of which level is removed.
	EXPECT_FALSE(subject.UnsafeReportIsTolerable(1)); //Unsafe regardless of which level is removed.
	EXPECT_TRUE(subject.UnsafeReportIsTolerable(2)); //Safe by removing the second level, 3.
	EXPECT_TRUE(subject.UnsafeReportIsTolerable(3)); //Safe by removing the third level, 4.
}

TEST(DetectsSafeReports, DISABLED_ReportIsSafe_ActualData_CorrectReportsIncludingTolerable)
{
	DetectsSafeReports subject;

	subject.LoadReports("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-2-actual-real.txt");

	int safeResult = 0;

	for (int i = 0; i < subject.GetNumberOfReports(); i++)
	{
		bool safe = subject.ReportIsSafe(i);

		if (safe)
			safeResult++;
	}

	int tolerableResult = 0;
	for (int i = 0; i < subject.UnsafeReports.size(); i++)
	{
		bool tolerable = subject.UnsafeReportIsTolerable(i);

			if (tolerable)
				tolerableResult++;
	}
	
	int result = safeResult + tolerableResult;
	
	EXPECT_EQ(result, -1);
}