#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_map>
#include <vector>


#include "../PhotoZ/Source/Camera.h"
#include "../PhotoZ/Source/Camera.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTestCamera)
	{
	public:

		TEST_METHOD(DeinterleaveTest0)
		{
			int n = 1;  // number of rows per quadrant
			int m = 2;  // number of cols per quadrant
			Camera cam;

			unsigned short* memory = new unsigned short[4 * n * m];
			// Channel 0
			memory[0] = 0;
			memory[1] = 1;

			// Channel 1
			memory[2] = 3;
			memory[3] = 2;

			// Channel 2
			memory[4] = 4;
			memory[5] = 5;

			// Channel 3
			memory[6] = 7;
			memory[7] = 6;

			cam.deinterleave(memory,n,m);
			for (short i = 0; i < 4 * n * m; i++) 
				Assert::AreEqual(i, (short)memory[i]);
			delete[] memory;

		}

		TEST_METHOD(DeinterleaveTest1)
		{
			int n = 2;  // number of rows per quadrant
			int m = 3;  // number of cols per quadrant
			Camera cam;

			unsigned short* memory = new unsigned short[4 * n * m];
			// Channel 0
			memory[0] = 0;
			memory[1] = 1;
			memory[2] = 2;
			memory[3] = 3;
			memory[4] = 4;
			memory[5] = 5;

			// Channel 1
			memory[6] = 8;
			memory[7] = 7;
			memory[8] = 6;
			memory[9] = 11;
			memory[10] = 10;
			memory[11] = 9;

			// Channel 2
			memory[12] = 15;
			memory[13] = 16;
			memory[14] = 17;
			memory[15] = 12;
			memory[16] = 13;
			memory[17] = 14;

			// Channel 3
			memory[18] = 23;
			memory[19] = 22;
			memory[20] = 21;
			memory[21] = 20;
			memory[22] = 19;
			memory[23] = 18;

			cam.deinterleave(memory, n, m);
			for ( short i = 0; i < 4 * n * m; i++)
				Assert::AreEqual(i, (short)memory[i]);
			delete[] memory;
		}


		TEST_METHOD(mapToRawLocationTest)
		{
			Camera cam;

			Assert::AreEqual((short)0, (short)cam.mapToRawLocation(0, 0, 3, 7, 0));
			Assert::AreEqual((short)(21 + 6), (short)cam.mapToRawLocation(0, 6, 3, 7, 1));
			Assert::AreEqual((short)(21 * 2), (short)cam.mapToRawLocation(0, 0, 3, 7, 2));
			Assert::AreEqual((short)(21 * 4 - 1), (short)cam.mapToRawLocation(2, 6, 3, 7, 3));
		}

		TEST_METHOD(mapQuadrantsTest1)
		{
			Camera cam;

			int n = 4;
			int m = 7;
			unsigned short* memory = new unsigned short[4 * n * m];

			for (int quad = 0; quad < 4; quad++) {
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						memory[cam.mapToRawLocation(i, j, n, m, quad)] = quad;
					}
				}
			}

			cam.mapQuadrants(memory,n,m);

			for (int i = 0; i < n; i++) {
				// Cbannel 0 final location
				for (int j = 0; j < m; j++) {
					Assert::AreEqual((short)0, (short)memory[i * 2 * m + j]);
				}
				// Cbannel 1 final location
				for (int j = m; j < 2 * m; j++) {
					Assert::AreEqual((short)1, (short)memory[i * 2 * m + j]);
				}
			}

			for (int i = n; i < 2 * n; i++) {
				// Cbannel 2 final location
				for (int j = 0; j < m; j++) {
					Assert::AreEqual((short)2, (short)memory[i * 2 * m + j]);
				}
				// Cbannel 3 final location
				for (int j = m; j < 2 * m; j++) {
					Assert::AreEqual((short)3, (short)memory[i * 2 * m + j]);
				}
			}
			delete[] memory;
		}
	};
}