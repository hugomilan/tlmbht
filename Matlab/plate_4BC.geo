// This file is part of TLMBHT.
// Copyright (C) 2016 Hugo Fernando Maia Milan
// 
// Acknowledgment: Brazilian National Counsel of Technological and Scientific
// Development (Proc. 203312/2014-7) for scholarship to HFMM
//
// Gmsh project created on Wed Nov  4 22:23:49 2015

Point(1) = {0, 0, 0, 1.0};
Point(2) = {1, 0, 0, 1.0};
Point(3) = {0, 1, 0, 1.0};
Point(4) = {1, 1, 0, 1.0};
Line(1) = {1, 2};
Line(2) = {4, 2};
Line(3) = {4, 3};
Line(4) = {3, 1};
Line Loop(5) = {3, 4, 1, -2};
Plane Surface(6) = {5};
Physical Surface(7) = {6};
Physical Line(8) = {2};
Physical Line(9) = {4};
Physical Line(10) = {1};
Physical Line(11) = {3};
