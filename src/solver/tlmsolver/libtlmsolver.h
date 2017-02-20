/*
 * TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
 * 
 * Copyright (C) 2015 to 2017 by Cornell University. All Rights Reserved.
 * 
 * Written by Hugo Fernando Maia Milan.
 * 
 * Free for educational, research and non-profit purposes.
 * Refer to the license file for details.
 */

/* 
 * File:   libtlmsolver.h
 * Author: Hugo Fernando Maia Milan
 * Email:  hugofernando@gmail.com
 *
 * Created on July 2, 2016.
 *
 *
 *
 * Revision history:
 * 
 * Date: 
 * Who: 
 * Description: 
 *
 */

/*
 * Description of this file:
 * contains declaration of functions, structures, and enumerations for using the
 * transmission-line modeling method as the solver. See libtlmsolver.c for more details.
 *
 */

#ifndef LIBTLMSOLVER_H
#define LIBTLMSOLVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../configs/libconfig.h"
#include "../../meshreader/libmeshtlmbht.h"
#include "../libinterfaceceigen.h"

    /*
     * the structure connectionLeveln is intended to find out where TLM nodes
     * (which are interpreted as element, in the finite element terminology) connect. 
     * This is, I will have 'n' ports number that will tell me that they are connected.
     * The levels are defined this way:
     * 0) level 0: where the number of the ports are allocated.
     * 1) level 1: where the biggest numbers of the points of the connection is allocated.
     * 2) level 2: where the numbers of the points of the connection is allocated.
     * Note that the number of the point in level 2 is < the number of the point in level 1.
     * 3) level 3: same as level 2...
     * 
     * 
     * Variables in connectionLeveln:
     * a) quantitySaved: the quantity of porst or points saved
     * b) level: the number of the level
     * c) *portsOrPoints: pointer that has saved the numbers of the ports (at level 0)
     * or points (at level > 0)
     * d) quantityAllocated: the size of the pointer
     * e) *innerLevel: allocated for level > 0.
     */

    // level n: the connection per si. I have the quantity of ports (quantityOfPorts)
    // that are connected, their numbers (ports) and the size of this buffer (quantityOfAllocated)

    struct connectionLeveln {
        unsigned long long quantitySaved;
        unsigned int level;
        unsigned long long *portsOrPoints;
        unsigned long long quantityAllocated;

        struct connectionLeveln *innerLevel;

        unsigned long long *accumulatedIntersections; // shows how many intersections
        // each position has accumulated in total. Position 0 contains the the total number for that level
        // Useful to parallelize when I want
        // to read the intersection points.
    };


    // the structure for the boundary is the following:
    // boundaryData: pointer (vector), because each element represents a different
    // boundary call in the call file. The appropriate position of the boundaryData
    // is achieved by the input variable, verifying the tag. This variable does not
    // have tag.
    // Within each boundaryData, we have the quantity of boundaries that its
    // element has. For instance, it can have 2 (convection and radiation).
    // The pointer of the boundaryTypeAndData is what allocate the type of boundary
    // and the number of inputs that it has

    struct boundaryTypeAndData {
        unsigned int boundaryType; // integer code for the type of boundary.
        double *boundaryData; // contains the data necessary for applying that boundary
    };

    struct boundaryData {
        unsigned int quantityOfBoundaries;
        struct boundaryTypeAndData *boundaries;
    };


    // I need an interface to change the abstract number of the ports to the real
    // number of the ports.

    struct aPortToRealPort {
        // contains the biggest port number - 1 before this element. That is, the
        // port numbers of this element type will start at previousMaximumRealPort.
        // I have port number - 1 because C starts at zero.
        unsigned long long previousMaximumRealPort;
        unsigned long long previousMaximumAbstractPort;

        unsigned long long previousMaximumRealNode;
        // this is the number that the node should start in the next elementCode
        // It starts as 1
        unsigned long long previousMaximumAbstractNode;
        // this is the number that the port should start in the next elementCode
        // It starts as 1

        unsigned int portsPerNode;

        unsigned int pointerType;
        // is changed during the processing.
        // 0 - this element type is all boundary. We have nothing allocated
        // 1 - this element type is all material. We have nothing allocated
        // 2 - contains the number of the nodes that are boundary (and not defined)
        // 3 - contains the number of the nodes that are material

        unsigned long long quantityAllocated;
        unsigned long long quantitySaved;

        unsigned long long *nodesNumbers;
        // this is used when pointerType == 2 or pointerType == 3
        // When pointerType == 2, nodesNumber has allocated the (sorted) number of the 
        // nodes that are boundary.
        // When pointerType == 3, nodesNumber has allocated the (sorted) number of the 
        // nodes that are material.


        // Code to consider stubs
        unsigned long long previousMaximumRealStubPort;
        // I don't have abstract stub port number. I calculate the number of stubs
        // for each element code, which is this variable. Then, I use it to do
        // the offset to calculate the RealPortNumber.


        unsigned int pointerTypeStub;
        // this will be changed during processing
        // 0 - all nodes of this element type DO NOT have stub
        // 1 - all nodes of this element type DO have stub
        // 2 - contains the number of the nodes that DO NOT have stub
        // 3 - contains the number of the nodes that DO have stub

        unsigned long long quantityAllocatedStub;
        unsigned long long quantitySavedStub;
        // the size and the quantity of elements saved on the nodesNumbersStub.
        // It is not necessarily the number of stubs.

        unsigned long long *nodesNumbersStub;
        // this is used when pointerTypeStub == 2 or pointerTypeStub == 3
        // When pointerTypeStub == 2, nodesNumber has allocated the (sorted) number of the 
        // nodes that DO NOT have stub.
        // When pointerTypeStub == 3, nodesNumber has allocated the (sorted) number of the 
        // nodes that DO have stub.
    };

    struct TLMnumbers {
        unsigned long long Ports;
        // number of ports
        unsigned long long StubPorts;
        // number of stub ports
        unsigned long long Nodes;
        // number of nodes
        unsigned long long Output;
        // number of outputs
        unsigned long long Points_Output;
        // positions in space of the outputs
        unsigned long long Intersections;
        // number of intersections in the mesh
        unsigned long long *BoundaryElements;
        unsigned long long *MaterialElements;
        unsigned long long *NotDefinedElements;

        struct aPortToRealPort *abstractPortsToReal;
    };

    struct connectionAndBoundaryCoefficients {
        double *reflection;
        double *transmission;
        double *B;
        double *transmission_out;
        double *B_out;
        double *transmission_out_flux;
        double *B_out_flux;
        // reflection
        // 0: total reflection for port 1
        // 1: total reflection for port 2
        // ...

        // transmission
        // 0: port 1 to port 2
        // 1: port 1 to port 3
        // ...
        // startEnd[0]: port 2 to port 3
        // startEnd[0] + 1: port 2 to port 4
        // ...

        // B:
        // 0: value for port 1
        // 1: value for port 2
        // ...

        // transmission_out(_flux)
        // 0: effect of port 1 on TB (or q'')
        // 1: effect of port 2 on TB (or q'')
        // ...
        // n: effect of port n on TB (or q'')

        // B_out(_flux)
        // 0: constant effect of port 1 on TB (or q'')
        // 1: constant effect of port 2 on TB (or q'')
        // ...
        // n: constant effect of port n on TB (or q'')
        unsigned long long *startEnd;
        // 0 - number of ports saved
        // 1 - number of allocations
        // 2 - number of the port 0
        // 3 - number of the first port of the Node that contains port 0
        // 4 - number of the last port of the Node that contains  port 0
        // 5 - number of the port 0 with offset from stub
        // 6 - same as 3 + offset from stub
        // 7 - same as 4 + offset from stub and is the stub port number if this node has stub
        // 8 - number of the port 1
        // ...
        //
        // These are offsets related to startEnd.
        unsigned int distanceBetweenPorts, offsetRealPort, offsetStubPort,
        offsetStubPortFirst, offsetStubPortLast, offsetRealPort0,
        offsetStubPort0, offsetRealPort1, offsetStubPort1;

        unsigned long long *portsNumbers;
        // this pointer will contain the number of the ports
        // 0: size of the pointer
        // 1: quantity of port numbers
        // 2: number of the ports 0
        // ...
        // n: number of port n - 2
    };


    unsigned int solverTLM(struct dataForSimulation*, int, void**);

    unsigned int solveTimeStepTLM(struct dataForSimulation*, int, void**);

    unsigned int terminateBoundaryTypeAndData(struct boundaryData **, const struct dataForSimulation *, int);

    unsigned int initiate_connectionLeveln(struct connectionLeveln *, unsigned int,
            unsigned long long *);
    unsigned int reallocate_connectionLeveln(struct connectionLeveln *, unsigned int);
    unsigned int add_to_connectionLeveln(struct connectionLeveln *, unsigned long long*,
            unsigned int, unsigned long long*);
    unsigned int wrap_size_connectionLeveln(struct connectionLeveln *);
    unsigned int terminate_connectionLeveln(struct connectionLeveln *);
    unsigned int getquantityAllocated_connectionLeveln(struct connectionLeveln *, unsigned long long*);
    unsigned int getPortsOrPoints(struct connectionLeveln *, unsigned long long,
            unsigned long long**);

    unsigned int allocatePointsPort(unsigned long long*,
            struct connectionLeveln *,
            unsigned int, unsigned long long *);

    unsigned int getGeometricalVariablesTLMline(const struct node*,
            const struct node*, double*);
    unsigned int getGeometricalVariablesTLMtriangle(const struct node *,
            const struct node *, const struct node *, double*);
    unsigned int getGeometricalVariablesTLMquadrangle(const struct node *,
            const struct node *, const struct node *, const struct node *, double*);
    unsigned int getGeometricalVariablesTLMtetrahedron(const struct node *,
            const struct node *, const struct node *, const struct node *, double*);

    unsigned int initiateTLMnumbers(struct TLMnumbers*);
    unsigned int getTLMnumbers(const struct dataForSimulation *,
            struct TLMnumbers*, struct connectionLeveln *, int);
    unsigned int wrapTLMnumbers(const struct dataForSimulation *,
            struct TLMnumbers*, int);
    unsigned int changeNodesNumbersType(unsigned long long **,
            unsigned long long *, unsigned long long);
    unsigned int terminateTLMnumbers(struct TLMnumbers*);

    unsigned int getNumberOfPortsGivenElement(unsigned int, enum dimSim);



    unsigned int initiate_aPortToRealPort(const struct dataForSimulation *,
            struct aPortToRealPort**, int);

    unsigned int reallocate_aPortToRealPort(struct aPortToRealPort*);
    unsigned int reallocate_aPortToRealPortStub(struct aPortToRealPort*);
    unsigned int add_to_aPortToRealPort(unsigned int, unsigned int, unsigned long long,
            struct aPortToRealPort*);

    unsigned int terminate_aPortToRealPort(struct aPortToRealPort**);

    void getRealPortNumber_fromAbstractPortNumber(
            unsigned long long, struct aPortToRealPort*,
            unsigned long long*);

    void getRealNodeAndPort_fromAbstractNode(unsigned int,
            unsigned long long, struct aPortToRealPort*,
            unsigned long long *);

    unsigned int getBetweenPointFromRealPortNumber(struct aPortToRealPort*,
            unsigned long long, double*, double*, double*,
            const struct dataForSimulation *);

    unsigned int getBetweenForLine(const struct dataForSimulation *,
            const unsigned long long, const unsigned long long,
            double *, double *, double *);
    unsigned int getBetweenForTriangle(const struct dataForSimulation *,
            const unsigned long long, const unsigned long long,
            double *, double *, double *);
    unsigned int getBetweenForQuadrangle(const struct dataForSimulation *,
            const unsigned long long, const unsigned long long,
            double *, double *, double *);
    unsigned int getBetweenForTetrahedron(const struct dataForSimulation *,
            const unsigned long long, const unsigned long long,
            double *, double *, double *);

    unsigned int getProjectionFromRealPortNumber(struct aPortToRealPort*,
            unsigned long long, double*, double*, double*,
            const struct dataForSimulation *);

    unsigned int getOutsideProjectionLine(const struct dataForSimulation *,
            const unsigned long long, const unsigned long long,
            double *, double *, double *);
    unsigned int getOutsideProjectionTriangle(const struct dataForSimulation *,
            const unsigned long long, const unsigned long long,
            double *, double *, double *);
    unsigned int getOutsideProjectionQuadrangle(const struct dataForSimulation *,
            const unsigned long long, const unsigned long long,
            double *, double *, double *);
    unsigned int getOutsideProjectionTetrahedron(const struct dataForSimulation *,
            const unsigned long long, const unsigned long long,
            double *, double *, double *);

    unsigned int initiateConnectionAndBoundaryCoefficients(struct connectionAndBoundaryCoefficients *);
    unsigned int terminateConnectionAndBoundaryCoefficients(struct connectionAndBoundaryCoefficients *);

#ifdef __cplusplus
}
#endif

#endif /* LIBTLMSOLVER_H */

