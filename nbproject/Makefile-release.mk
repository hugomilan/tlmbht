#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/src/configs/libboundconfig.o \
	${OBJECTDIR}/src/configs/libconfig.o \
	${OBJECTDIR}/src/configs/libequationconfig.o \
	${OBJECTDIR}/src/configs/libfunctionconfig.o \
	${OBJECTDIR}/src/configs/libmatconfig.o \
	${OBJECTDIR}/src/configs/libmeshconfig.o \
	${OBJECTDIR}/src/configs/libsimuconfig.o \
	${OBJECTDIR}/src/configs/libsourceconfig.o \
	${OBJECTDIR}/src/configs/libvariablename.o \
	${OBJECTDIR}/src/meshreader/libgmshreader.o \
	${OBJECTDIR}/src/meshreader/libmeshreader.o \
	${OBJECTDIR}/src/meshreader/libmeshtlmbht.o \
	${OBJECTDIR}/src/meshreader/libtbnreader.o \
	${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmark.o \
	${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmarkeigen.o \
	${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmatmult.o \
	${OBJECTDIR}/src/miscellaneous/liberrorcode.o \
	${OBJECTDIR}/src/miscellaneous/liblinalg.o \
	${OBJECTDIR}/src/miscellaneous/libmiscellaneous.o \
	${OBJECTDIR}/src/miscellaneous/libstringtlmbht.o \
	${OBJECTDIR}/src/miscellaneous/libwritetofiletlmbht.o \
	${OBJECTDIR}/src/solver/libinterfaceceigen.o \
	${OBJECTDIR}/src/solver/libsolver.o \
	${OBJECTDIR}/src/solver/tlmsolver/libtlmsolver.o \
	${OBJECTDIR}/src/solver/tlmsolver/libtlmsolvereigen.o \
	${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes.o \
	${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes1dsolver.o \
	${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes2dsolver.o \
	${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes3dsolver.o \
	${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpenneseigen.o


# C Compiler Flags
CFLAGS=-fopenmp -DNDEBUG -march=native

# CC Compiler Flags
CCFLAGS=-fopenmp -DNDEBUG -march=native
CXXFLAGS=-fopenmp -DNDEBUG -march=native

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tlmbht

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tlmbht: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tlmbht ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/src/configs/libboundconfig.o: src/configs/libboundconfig.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libboundconfig.o src/configs/libboundconfig.c

${OBJECTDIR}/src/configs/libconfig.o: src/configs/libconfig.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libconfig.o src/configs/libconfig.c

${OBJECTDIR}/src/configs/libequationconfig.o: src/configs/libequationconfig.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libequationconfig.o src/configs/libequationconfig.c

${OBJECTDIR}/src/configs/libfunctionconfig.o: src/configs/libfunctionconfig.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libfunctionconfig.o src/configs/libfunctionconfig.c

${OBJECTDIR}/src/configs/libmatconfig.o: src/configs/libmatconfig.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libmatconfig.o src/configs/libmatconfig.c

${OBJECTDIR}/src/configs/libmeshconfig.o: src/configs/libmeshconfig.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libmeshconfig.o src/configs/libmeshconfig.c

${OBJECTDIR}/src/configs/libsimuconfig.o: src/configs/libsimuconfig.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libsimuconfig.o src/configs/libsimuconfig.c

${OBJECTDIR}/src/configs/libsourceconfig.o: src/configs/libsourceconfig.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libsourceconfig.o src/configs/libsourceconfig.c

${OBJECTDIR}/src/configs/libvariablename.o: src/configs/libvariablename.c
	${MKDIR} -p ${OBJECTDIR}/src/configs
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/configs/libvariablename.o src/configs/libvariablename.c

${OBJECTDIR}/src/meshreader/libgmshreader.o: src/meshreader/libgmshreader.c
	${MKDIR} -p ${OBJECTDIR}/src/meshreader
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/meshreader/libgmshreader.o src/meshreader/libgmshreader.c

${OBJECTDIR}/src/meshreader/libmeshreader.o: src/meshreader/libmeshreader.c
	${MKDIR} -p ${OBJECTDIR}/src/meshreader
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/meshreader/libmeshreader.o src/meshreader/libmeshreader.c

${OBJECTDIR}/src/meshreader/libmeshtlmbht.o: src/meshreader/libmeshtlmbht.c
	${MKDIR} -p ${OBJECTDIR}/src/meshreader
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/meshreader/libmeshtlmbht.o src/meshreader/libmeshtlmbht.c

${OBJECTDIR}/src/meshreader/libtbnreader.o: src/meshreader/libtbnreader.c
	${MKDIR} -p ${OBJECTDIR}/src/meshreader
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/meshreader/libtbnreader.o src/meshreader/libtbnreader.c

${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmark.o: src/miscellaneous/benchmark/libbenchmark.c
	${MKDIR} -p ${OBJECTDIR}/src/miscellaneous/benchmark
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmark.o src/miscellaneous/benchmark/libbenchmark.c

${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmarkeigen.o: src/miscellaneous/benchmark/libbenchmarkeigen.cpp
	${MKDIR} -p ${OBJECTDIR}/src/miscellaneous/benchmark
	$(COMPILE.cc) -O3 -Iinclude -o ${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmarkeigen.o src/miscellaneous/benchmark/libbenchmarkeigen.cpp

${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmatmult.o: src/miscellaneous/benchmark/libbenchmatmult.c
	${MKDIR} -p ${OBJECTDIR}/src/miscellaneous/benchmark
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/miscellaneous/benchmark/libbenchmatmult.o src/miscellaneous/benchmark/libbenchmatmult.c

${OBJECTDIR}/src/miscellaneous/liberrorcode.o: src/miscellaneous/liberrorcode.c
	${MKDIR} -p ${OBJECTDIR}/src/miscellaneous
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/miscellaneous/liberrorcode.o src/miscellaneous/liberrorcode.c

${OBJECTDIR}/src/miscellaneous/liblinalg.o: src/miscellaneous/liblinalg.c
	${MKDIR} -p ${OBJECTDIR}/src/miscellaneous
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/miscellaneous/liblinalg.o src/miscellaneous/liblinalg.c

${OBJECTDIR}/src/miscellaneous/libmiscellaneous.o: src/miscellaneous/libmiscellaneous.c
	${MKDIR} -p ${OBJECTDIR}/src/miscellaneous
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/miscellaneous/libmiscellaneous.o src/miscellaneous/libmiscellaneous.c

${OBJECTDIR}/src/miscellaneous/libstringtlmbht.o: src/miscellaneous/libstringtlmbht.c
	${MKDIR} -p ${OBJECTDIR}/src/miscellaneous
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/miscellaneous/libstringtlmbht.o src/miscellaneous/libstringtlmbht.c

${OBJECTDIR}/src/miscellaneous/libwritetofiletlmbht.o: src/miscellaneous/libwritetofiletlmbht.cpp
	${MKDIR} -p ${OBJECTDIR}/src/miscellaneous
	$(COMPILE.cc) -O3 -Iinclude -o ${OBJECTDIR}/src/miscellaneous/libwritetofiletlmbht.o src/miscellaneous/libwritetofiletlmbht.cpp

${OBJECTDIR}/src/solver/libinterfaceceigen.o: src/solver/libinterfaceceigen.cpp
	${MKDIR} -p ${OBJECTDIR}/src/solver
	$(COMPILE.cc) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/libinterfaceceigen.o src/solver/libinterfaceceigen.cpp

${OBJECTDIR}/src/solver/libsolver.o: src/solver/libsolver.c
	${MKDIR} -p ${OBJECTDIR}/src/solver
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/libsolver.o src/solver/libsolver.c

${OBJECTDIR}/src/solver/tlmsolver/libtlmsolver.o: src/solver/tlmsolver/libtlmsolver.c
	${MKDIR} -p ${OBJECTDIR}/src/solver/tlmsolver
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/tlmsolver/libtlmsolver.o src/solver/tlmsolver/libtlmsolver.c

${OBJECTDIR}/src/solver/tlmsolver/libtlmsolvereigen.o: src/solver/tlmsolver/libtlmsolvereigen.cpp
	${MKDIR} -p ${OBJECTDIR}/src/solver/tlmsolver
	$(COMPILE.cc) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/tlmsolver/libtlmsolvereigen.o src/solver/tlmsolver/libtlmsolvereigen.cpp

${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes.o: src/solver/tlmsolver/pennes/libtlmpennes.c
	${MKDIR} -p ${OBJECTDIR}/src/solver/tlmsolver/pennes
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes.o src/solver/tlmsolver/pennes/libtlmpennes.c

${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes1dsolver.o: src/solver/tlmsolver/pennes/libtlmpennes1dsolver.c
	${MKDIR} -p ${OBJECTDIR}/src/solver/tlmsolver/pennes
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes1dsolver.o src/solver/tlmsolver/pennes/libtlmpennes1dsolver.c

${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes2dsolver.o: src/solver/tlmsolver/pennes/libtlmpennes2dsolver.c
	${MKDIR} -p ${OBJECTDIR}/src/solver/tlmsolver/pennes
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes2dsolver.o src/solver/tlmsolver/pennes/libtlmpennes2dsolver.c

${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes3dsolver.o: src/solver/tlmsolver/pennes/libtlmpennes3dsolver.c
	${MKDIR} -p ${OBJECTDIR}/src/solver/tlmsolver/pennes
	$(COMPILE.c) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpennes3dsolver.o src/solver/tlmsolver/pennes/libtlmpennes3dsolver.c

${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpenneseigen.o: src/solver/tlmsolver/pennes/libtlmpenneseigen.cpp
	${MKDIR} -p ${OBJECTDIR}/src/solver/tlmsolver/pennes
	$(COMPILE.cc) -O3 -Iinclude -o ${OBJECTDIR}/src/solver/tlmsolver/pennes/libtlmpenneseigen.o src/solver/tlmsolver/pennes/libtlmpenneseigen.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
