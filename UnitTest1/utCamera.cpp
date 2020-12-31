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

			short* memory = new short[4 * n * m];
			// Channel 0
			memory[0] = 0;
			memory[1] = 4;

			// Channel 1
			memory[2] = 5;
			memory[3] = 1;

			// Channel 2
			memory[4] = 2;
			memory[5] = 6;

			// Channel 3
			memory[6] = 7;
			memory[7] = 3;

			cam.deinterleave(memory,n,m);
			for (short i = 0; i < 4 * n * m; i++) 
				Assert::AreEqual(i, memory[i]);
			delete[] memory;

		}

		TEST_METHOD(DeinterleaveTest1)
		{
			int n = 2;  // number of rows per quadrant
			int m = 3;  // number of cols per quadrant
			Camera cam;

			short* memory = new short[4 * n * m];
			// Channel 0
			memory[0] = 0;
			memory[1] = 4;
			memory[2] = 8;
			memory[3] = 12;
			memory[4] = 16;
			memory[5] = 20;

			// Channel 1
			memory[6] = 9;
			memory[7] = 5;
			memory[8] = 1;
			memory[9] = 21;
			memory[10] = 17;
			memory[11] = 13;

			// Channel 2
			memory[12] = 14;
			memory[13] = 18;
			memory[14] = 22;
			memory[15] = 2;
			memory[16] = 6;
			memory[17] = 10;

			// Channel 3
			memory[18] = 23;
			memory[19] = 19;
			memory[20] = 15;
			memory[21] = 11;
			memory[22] = 7;
			memory[23] = 3;

			cam.deinterleave(memory, n, m);
			for (short i = 0; i < 4 * n * m; i++) 
				Assert::AreEqual(i, memory[i]);
			delete[] memory;
		}

		TEST_METHOD(DeinterleaveTest2)
		{
			// The method of populating fake interleaved data should follow an
			// algorithm different from the deinterleaving logic to make this
			// test more meaningful & robust.
			// So we will use a "4-pointers" striding algorithm to simulate interleaved acquisition.
			
			int n = 87;  // number of rows per quadrant
			int m = 85;  // number of cols per quadrant

			Assert::IsTrue(4 * n * m < 32767); // Note that 4nm should not exceed range of short int

			Camera cam;
			short* memory = new short[4 * n * m];
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
			Assert::AreEqual((short)0, memory[0]);
			Assert::AreEqual((short)4, memory[1]);
			Assert::AreEqual((short)(4 * (n * m - 1)), memory[n*m-1]);
			Assert::AreEqual((short)(4 * (m-1) + 1), memory[n*m]);
			Assert::AreEqual((short)(4 * (m - 1) + 2), memory[3*n*m-1]);
			Assert::AreEqual((short)3, memory[4*n*m-1]);
	
			cam.deinterleave(memory,n,m);

			for (short i = 0; i < 4 * n * m; i++) {
				Assert::AreEqual(i, memory[i]);
			}
			delete[] memory;
		}

		TEST_METHOD(mapFromInterleavedTest)
		{
			Camera cam;

			Assert::AreEqual(0, cam.mapFromInterleaved(0, 0, 3, 7, 0));
			Assert::AreEqual(21 + 6, cam.mapFromInterleaved(0, 6, 3, 7, 1));
			Assert::AreEqual(21 * 2, cam.mapFromInterleaved(0, 0, 3, 7, 2));
			Assert::AreEqual(21 * 4 - 1, cam.mapFromInterleaved(2, 6, 3, 7, 3));
		}

		TEST_METHOD(mapToDeinterleavedTest)
		{
			Camera cam;

			// corners
			Assert::AreEqual(0, cam.mapToDeinterleaved(0, 0, 3, 7, 0));
			Assert::AreEqual(1, cam.mapToDeinterleaved(0, 6, 3, 7, 1));
			Assert::AreEqual(2, cam.mapToDeinterleaved(2, 0, 3, 7, 2));
			Assert::AreEqual(3, cam.mapToDeinterleaved(2, 6, 3, 7, 3));

			Assert::AreEqual(6 * 4 + 1, cam.mapToDeinterleaved(0, 0, 3, 7, 1));
			Assert::AreEqual(14 * 4 + 2, cam.mapToDeinterleaved(0, 0, 3, 7, 2));
			Assert::AreEqual(12 * 4 + 3, cam.mapToDeinterleaved(1, 1, 3, 7, 3));
		}

	};
}