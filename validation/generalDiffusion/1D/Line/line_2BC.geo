// This file is part of TLMBHT.
// Copyright (C) 2017 Hugo Fernando Maia Milan
// 
// Acknowledgment: Brazilian National Counsel of Technological and Scientific
// Development (Proc. 203312/2014-7) for scholarship to HFMM
//
// Gmsh project created on Thu Jan 19 12:20:49 2017

Point(1) = {0.00, 0, 0, 0.001};
Point(2) = {1.00, 0, 0, 0.1};
Line(1) = {1, 2};

Physical Line(10) = {1};
Physical Point(20) = {1};
Physical Point(21) = {2};
