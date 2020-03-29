/*
 * dpl.cpp
 *
 * DustProdLight (STL version) initialization
 *
 *  Created on: Feb 25, 2020
 *      Author: Lorand Kedves
 */

#include "dpl/impl/dpl_module.h"


void DPLMain::init() {
	DPLModuleDPLMain->init();
}

void DPLMain::shutdown() {
	DPLModuleDPLMain->release();
}

