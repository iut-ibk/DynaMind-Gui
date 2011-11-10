/**
 * @file
 * @author  Chrisitan Urich <christian.urich@gmail.com>
 * @version 1.0
 * @section LICENSE
 *
 * This file is part of VIBe2
 *
 * Copyright (C) 2011  Christian Urich

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
#include "testmodule.h"
#include "rasterdata.h"
#include <iostream>
//#include <omp.h>
#include <cmath>
#include <iostream>
VIBe_DECLARE_NODE_NAME( TestModule,Modules )
TestModule::TestModule() {

    Logger(Debug) << "Create Testmodule";
    //this->addParameter("Import", VIBe2::VECTORDATA_OUT, & this->vec_in);
    this->addParameter("SomeImport", VIBe2::VECTORDATA_OUT, & this->vec_out);

}



void TestModule::run() {
    Logger(Debug) << "Run Testmodule";

    std::vector<Point> points;
    Point p(0,1,2);
    points.push_back(p);

    this->vec_out->setPoints("Points", points);

    std::vector<std::string> names = this->vec_out->getPointsNames();

    int i = 0;
    i=1;

}

TestModule::~TestModule() {
    Logger(Debug) << "Destructor Testmodule";

}
