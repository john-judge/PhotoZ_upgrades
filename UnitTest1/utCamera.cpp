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


		// open a test raw image RLI-02062021.txt and output a processed image for visual inspection, with CDS
		TEST_METHOD(reassembleImageRLI) {
			const char* fileIn, *fileOut;
			int quad_height, quad_width;

			fileIn = "RLI-raw-full-out450.txt";
			fileOut = "OutputCDS-raw-full-out450.txt";
			quad_height = 40;
			quad_width = 1024; // Needs to change, incomplete image
			reassembleImage(fileIn, fileOut, quad_height, quad_width, 1, true);
			fileOut = "Output-raw-full-out450.txt";
			reassembleImage(fileIn, fileOut, quad_height, quad_width, 1, false);
		}


		// given height and width of a quadrant / image after CDS subtraction (the final dimensions)
		// SCALE is used to scale floating point numbers to the full range of unsigned char (255) or unsigned short (65535)
		void reassembleImage(const char* fileIn, const char* fileOut, int quad_height, int quad_width, double scale, bool CDS) {
			// Read in test image data (
			std::ifstream file;
			file.open(TEST_CASE_DATA_DIRECTORY + fileIn);
			const wchar_t * message = L"File should be open";
			Assert::IsTrue(file.is_open(), message);
			int quadrantSize = 2 * quad_height * quad_width;
			unsigned short *rawMemory = new unsigned short[quadrantSize];
			unsigned short *new_image = new unsigned short[quadrantSize];

			string index, data;
			int i = 0;
			while (file >> index >> data && i < quadrantSize) {
				if (scale == 1) rawMemory[i] = (unsigned short)(atoi(data.c_str()) * scale);
				else rawMemory[i] = (unsigned short)(stod(data) * scale);
				i++;
			}
			file.close();

			message = L"Expected to fill a quadrant.\n";
			Assert::AreEqual(quadrantSize, i, message);


			message = L"Dimensions for a quadrant are incorrect.\n";
			Assert::AreEqual(quadrantSize, quad_height * quad_width * 2, message);

			// Each PDV Quadrant has a different cam channel interleaving
			int channelOrder1[] = { 2, 3, 1, 0 };
			int channelOrder2[] = { 2, 3, 1, 0 };
			int channelOrder3[] = { 0, 1, 3, 2 };
			int channelOrder4[] = { 0, 1, 3, 2 };

			Camera cam;
			// There are 4 camera channels within each PDV channel
			cam.deinterleave(rawMemory, quad_height, quad_width, channelOrder1);
			if (CDS) cam.subtractCDS(rawMemory, quad_height, quad_width);

			// write test data to 
			std::ofstream outFile;

			remove((TEST_CASE_DATA_DIRECTORY + fileOut).c_str());
			outFile.open(TEST_CASE_DATA_DIRECTORY + fileOut, std::ofstream::out | std::ofstream::app);

			if (CDS) quadrantSize /= 2; // if CDS was applied, only write back the first half.

			for (int i = 0; i < quadrantSize; i++)
				if (scale == 1) outFile << i << "\t" << (unsigned short)rawMemory[i] << "\n";
				else outFile << i << "\t" << (double)rawMemory[i] / (double)scale << "\n";

			outFile.close();

			delete[] rawMemory;
			delete[] new_image;
		}


	};
}

