/*
 * gtest_main.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: prof-ahmed-saad-isa
 */


#include <stdio.h>
#include <iostream>
#include <Server.h>
#include "gtest/gtest.h"
using namespace std;

GTEST_API_ int main(int argc, char **argv)
{
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
