#include "pch.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class ComparesLocations
{
public:
	ComparesLocations();
	~ComparesLocations();
	void LoadUnsortedLists(std::string filename);
	int GetLeftListLength();
	int GetRightListLength();
	int GetLinesLoaded();
	int GetLeftAtPosition(int pos);
	int GetRightAtPosition(int pos);
	void SortLists();
	int GetTotalDistance();
	int GetTotalSimilarity();
private:
	std::vector<int> leftList;
	std::vector<int> rightList;
	int linesLoaded;	
};

ComparesLocations::ComparesLocations()
{
	linesLoaded = 0;
}

ComparesLocations::~ComparesLocations()
{
}

void ComparesLocations::LoadUnsortedLists(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	bool fileIsOpen;

	fileIsOpen = file.is_open();

	if (fileIsOpen)
	{
		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			std::string numberAsString;
			ss >> numberAsString;
			leftList.push_back(std::stoi(numberAsString));
			ss >> numberAsString;
			rightList.push_back(std::stoi(numberAsString));
			linesLoaded++;
		}
	}
}

int ComparesLocations::GetLeftListLength()
{
	return leftList.size();
}

int ComparesLocations::GetRightListLength()
{
	return rightList.size();
}

int ComparesLocations::GetLinesLoaded()
{
	return linesLoaded;
}

int ComparesLocations::GetLeftAtPosition(int pos)
{
	return leftList[pos];
}

int ComparesLocations::GetRightAtPosition(int pos)
{
	return rightList[pos];
}

void ComparesLocations::SortLists()
{
	std::sort(leftList.begin(), leftList.end());
	std::sort(rightList.begin(), rightList.end());
}

int ComparesLocations::GetTotalDistance()
{
	int result = 0;
	for (int i = 0; i < linesLoaded; i++)
	{
		int left = leftList[i];
		int right = rightList[i];				
		result += abs(right - left);
	}
	return result;
}

int ComparesLocations::GetTotalSimilarity()
{
	int result = 0;
	for (int i = 0; i < GetLeftListLength(); i++)
	{
		int candidate = leftList[i];
		int candidateResult = 0;
		for (int j = 0; j < GetRightListLength(); j++)
		{
			if (rightList[j] == candidate)
			{
				candidateResult += rightList[j];
			}
		}
		result += candidateResult;
	}
	return result;
}

TEST(ComparesLocations, LoadUnsortedLists_TestFile_VectorsAreCorrectLength)
{
	ComparesLocations subject;
	subject.LoadUnsortedLists("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-1-test.txt");

	int resultLeft = subject.GetLeftListLength();
	int resultRight = subject.GetRightListLength();

	EXPECT_EQ(resultLeft, 6);
	EXPECT_EQ(resultRight, 6);
}

TEST(ComparesLocations, LoadUnsortedLists_TestFile_ReportsCorrectLengthOfLinesLoaded)
{
	ComparesLocations subject;
	subject.LoadUnsortedLists("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-1-test.txt");

	int result = subject.GetLinesLoaded();

	EXPECT_EQ(result, 6);
}

TEST(ComparesLocations, LoadUnsortedLists_TestFile_SampleLocationsAreCorrect)
{
	ComparesLocations subject;
	subject.LoadUnsortedLists("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-1-test.txt");

	EXPECT_EQ(subject.GetLeftAtPosition(0), 3);
	EXPECT_EQ(subject.GetRightAtPosition(0), 4);
	EXPECT_EQ(subject.GetLeftAtPosition(5), 3);
	EXPECT_EQ(subject.GetRightAtPosition(5), 3);
}

TEST(ComparesLocations, SortLists_TestFile_SampleLocationsAreCorrect)
{
	ComparesLocations subject;
	subject.LoadUnsortedLists("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-1-test.txt");

	subject.SortLists();
	EXPECT_EQ(subject.GetLeftAtPosition(0), 1);
	EXPECT_EQ(subject.GetRightAtPosition(0), 3);
	EXPECT_EQ(subject.GetLeftAtPosition(5), 4);
	EXPECT_EQ(subject.GetRightAtPosition(5), 9);
}

TEST(ComparesLocations, GetTotalDistance_TestFile_SampleDistanceIsCorrect)
{
	ComparesLocations subject;
	subject.LoadUnsortedLists("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-1-test.txt");
	subject.SortLists();

	int result = subject.GetTotalDistance();

	EXPECT_EQ(result, 11);
}

TEST(ComparesLocations, DISABLED_GetTotalDistance_ActualFile_SampleDistanceIsCorrect)
{
	ComparesLocations subject;
	subject.LoadUnsortedLists("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-1-actual.txt");
	subject.SortLists();

	int result = subject.GetTotalDistance();

	EXPECT_EQ(result, -1);
}

TEST(ComparesLocations, GetSimilarity_TestFile_SampleSimilarityIsCorrect)
{
	ComparesLocations subject;
	subject.LoadUnsortedLists("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-1-test.txt");
	subject.SortLists();

	int result = subject.GetTotalSimilarity();

	EXPECT_EQ(result, 31);
}

TEST(ComparesLocations, DISABLED_GetSimilarity_ActualFile_SampleSimilarityIsCorrect)
{
	ComparesLocations subject;
	subject.LoadUnsortedLists("C:\\Projects\\Git\\advent-of-code-2024-inputs\\day-1-actual.txt");
	subject.SortLists();

	int result = subject.GetTotalSimilarity();

	EXPECT_EQ(result, -1);
}