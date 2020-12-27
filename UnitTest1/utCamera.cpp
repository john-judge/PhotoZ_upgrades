#include "stdafx.h"
#include "CppUnitTest.h"


#include "../PhotoZ/Source/Camera.h"
#include "../PhotoZ/Source/Camera.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestCamera
{		
	TEST_CLASS(UnitTestCamera)
	{
	public:
		
		TEST_METHOD(DeinterleaveTest1)
		{
			Camera cam;
			cam.setCamProgram(7); // 256 x 40 images

			short memory[256 * 40];

			// The method of populating fake interleaved data should follow an
			// algorithm different from the deinterleaving logic to make this
			// test more meaningful & robust.
			// So we will use a "4-pointers" striding algorithm to simulate interleaved acquisition.
			int n = 20;  // number of rows per quadrant
			int m = 128; // number of cols per quadrant

			int x0 = 0;			int y0 = 0;				// Channel 0
			int x1 = 2 * m;		int y1 = 0;				// Channel 1
			int x2 = 0;			int y2 = - 2 * n;		// Channel 2
			int x3 = -2 * m;	int y3 = -2 * n;		// Channel 3

			int count = 0;
			// Create interleaved image
			while (count < 256 * 40) {
				int whichChannel = count % 4;

				count++;
			}

			//Assert::AreEqual();
		}

		TEST_METHOD(DeinterleaveTest2)
		{
			Camera cam;
			cam.setCamProgram(7); // Sets to 256 x 40 images

			//Assert::AreEqual();
		}

	};
}