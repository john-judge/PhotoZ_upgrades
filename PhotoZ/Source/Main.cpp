//=============================================================================
// Main.cpp
//=============================================================================
#include "MainController.h"
#include <cstdlib>
//=============================================================================
MainController *mc;
void quit();
//=============================================================================
int main()
{
	mc = new MainController();
	mc->start();
	std::atexit(quit);
	return 0;
}

void quit() {
	mc->~MainController();
	//delete mc;
}
//=============================================================================
