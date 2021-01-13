#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_map>
#include <vector>

#include "../PhotoZ/Source/Camera.h"
#include "../PhotoZ/Source/Camera.cpp"

// Locating 
string GetDirectoryName(string path) {
	const size_t last_slash_idx = path.rfind('\\');
	if (std::string::npos != last_slash_idx)
	{
		return path.substr(0, last_slash_idx + 1) + "\\TestData\\";
	}
	return "";
}

//Returns solution's directory
#define TEST_CASE_DATA_DIRECTORY GetDirectoryName(__FILE__)

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTestCamera)
	{
	public:
		/*
		TEST_METHOD(DeinterleaveTest)
		{
			// The method of populating fake interleaved data should follow an
			// algorithm different from the deinterleaving logic to make this
			// test more meaningful & robust.
			// So we will use a "4-pointers" striding algorithm to simulate interleaved acquisition.
			
			int n = 87;  // number of rows per quadrant
			int m = 85;  // number of cols per quadrant

			Assert::IsTrue(4 * n * m < 32767); // Note that 4nm should not exceed range of short int

			Camera cam;
			unsigned short* memory = new unsigned short[4 * n * m];
			for (int i = 0; i < 4 * n*m; i++) memory[i] = 0;

			// These iterators and positions define the interleaving pattern, quadrant-by-quadrant
			unordered_map<int, vector<int>> pointers;
			// pointers[channel] = {row,	col, rowItrDir, colItrDir, colLimit, colStart}
			pointers[0] =		   { 0,		0,		 1,		 1,		 m,		0	};
			pointers[1] =		   { 0,		m-1,	 1,		-1,		-1,		m-1 };
			pointers[2] =		   { n-1,	0,		-1,		 1,		 m,		0	};
			pointers[3] =		   { n-1,	m-1,	-1,		-1,		-1,		m-1 };

			short count = 0;
			// Create interleaved image
			while (count < 4 * n * m) {
				int iChn = count % 4;
				memory[iChn * n * m + pointers[iChn][0] * m + pointers[iChn][1]] = count;
				pointers[iChn][1] += pointers[iChn][3]; // increment col position
				if (pointers[iChn][1] == pointers[iChn][4]) { // check col position bounds
					pointers[iChn][1] = pointers[iChn][5];	// reset col position
					pointers[iChn][0] += pointers[iChn][2]; // increment row position
				}
				count++;
			}

			// Test data should be correct.
			Assert::AreEqual((short)0, (short)memory[0]);
			Assert::AreEqual((short)4, (short)memory[1]);
			Assert::AreEqual((short)(4 * (n * m - 1)), (short)memory[n*m-1]);
			Assert::AreEqual((short)(4 * (m-1) + 1), (short)memory[n*m]);
			Assert::AreEqual((short)(4 * (m - 1) + 2), (short)memory[3*n*m-1]);
			Assert::AreEqual((short)3, (short)memory[4*n*m-1]);
	
			cam.deinterleave(memory,n,m);

			for (short i = 0; i < 4 * n * m; i++) {
				Assert::AreEqual(i, (short)memory[i]);
			}
			delete[] memory;
		}*/

		
		TEST_METHOD(DeinterleaveTest)
		{
			int qWidth = 256;  // number of rows per quadrant
			int qHeight = 20;  // number of cols per quadrant
			

			unsigned short* memory = new unsigned short[qWidth * qHeight * 2];
			
			// Row 1
			memory[0] = 0;
			memory[1] = 64;
			memory[2] = 128;
			memory[3] = 192;
			
			memory[4] = 1;
			memory[5] = 65;
			memory[6] = 129;
			memory[7] = 193;

			memory[255] = 255;
			memory[256] = 1000; // reset position 0
			memory[257] = 1064; // reset position 1

			// Row 2
			memory[512] = 0;
			memory[512 + 8] = 2;
			
			Camera cam;
			cam.deinterleave2(memory, qHeight, qWidth);
			int camChannelWidth = qWidth / 4;
			
			// In first row (first 512)
			Assert::AreEqual((short)(3 * camChannelWidth), (short)memory[0]);
			Assert::AreEqual((short)(2 * camChannelWidth), (short)memory[64]);
			Assert::AreEqual((short)(0 * camChannelWidth), (short)memory[128]);
			Assert::AreEqual((short)(1 * camChannelWidth), (short)memory[192]);

			Assert::AreEqual((short)0, (short)memory[camChannelWidth * 2]);
			Assert::AreEqual((short)64, (short)memory[camChannelWidth * 3]);
			Assert::AreEqual((short)128, (short)memory[camChannelWidth * 1]);
			Assert::AreEqual((short)192, (short)memory[camChannelWidth * 0]);

			Assert::AreEqual((short)255, (short)memory[(int)(255 / 4)]);
			Assert::AreEqual((short)1000, (short)memory[256 + camChannelWidth * 2]);
			Assert::AreEqual((short)1064, (short)memory[256 + camChannelWidth * 3]);

			// In second row
			Assert::AreEqual((short)0, (short)memory[512 + camChannelWidth * 2]);
			Assert::AreEqual((short)2, (short)memory[512 + 2 +camChannelWidth * 2]);

			delete[] memory;
		}
		

		TEST_METHOD(reassembleImageTest1) {
			// Chun's functions, modified in DapController.cpp
			// open a test raw image and output a processed image for visual inspection

			// This is the doubled-width quadrant size before CDS subtraction
			//int quadrantSize = 256 * 20; // the other 3 quadrants are 0s
			int quadrantSize = 2 * 256 * 20; // 10240	
			// Read in test image data (
			std::ifstream file;
			file.open(TEST_CASE_DATA_DIRECTORY + "RLI-5.txt");
			const wchar_t * message = L"File should be open";
			Assert::IsTrue(file.is_open(), message);

			unsigned short *rawMemory = new unsigned short[quadrantSize];
			unsigned short *new_image = new unsigned short[quadrantSize];

			string index, data;
			int i = 0;
			while (file >> index >> data && i < quadrantSize) {
				rawMemory[i] = (unsigned short)(stod(data) * 65535);
				i++;
			}
			file.close();

			message = L"Expected to fill a quadrant.\n";
			Assert::AreEqual(quadrantSize, i, message);


			int quad_height = 5;
			int quad_width = 1024;

			message = L"Dimensions for a quadrant are incorrect.\n";
			Assert::AreEqual(quadrantSize, quad_height * quad_width * 2, message);
			
			Camera cam;
			// There are 4 camera channels within each PDV channel
			cam.deinterleave2(rawMemory, quad_height, quad_width);

			// write test data to 
			std::ofstream outFile;

			string dstFileName = "Output-5-5.txt";

			remove((TEST_CASE_DATA_DIRECTORY + dstFileName).c_str());
			outFile.open(TEST_CASE_DATA_DIRECTORY + dstFileName, std::ofstream::out | std::ofstream::app);

			for (int i = 0; i < quadrantSize; i++)
				outFile << i << "\t" << (double)rawMemory[i] / (double)65535 << "\n";

			outFile.close();

			delete[] rawMemory;
			delete[] new_image;

		}

	};
}