#include "pch.h"

#include "screenShot.h"

#include "cmdline.h"

//#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#ifndef CATCH_CONFIG_MAIN
int main(int argc, char *argv[]) {
	screenShot();
	//showTrk();
}
#endif//CATCH_CONFIG_MAIN
