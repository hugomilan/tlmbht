#
# TLMBHT - Transmission-line Modeling Method applied to BioHeat Transfer Problems.
# 
# Copyright (C) 2015 to 2017 by Cornell University. All Rights Reserved.
# 
# Written by Hugo Fernando Maia Milan.
# 
# Free for educational, research and non-profit purposes.
# Refer to the license file for details.
#
########################################## ATTENTION ##########################################
# cmake was not fully configured. You should use the make scripts.
#
cmake_minimum_required(VERSION 2.8)

# project name
project(tlmbht)
SET(TLMBHT_VERSION_MAJOR "0")
SET(TLMBHT_VERSION_MINOR "2")
SET(TLMBHT_VERSION_PATCH "0")
SET(TLMBHT_VERSION_PRERELEASE "-alpha")
SET(VERSION "${TLMBHT_VERSION_MAJOR}.${TLMBHT_VERSION_MINOR}.${TLMBHT_VERSION_PATCH}${TLMBHT_VERSION_PRERELEASE}.")


include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)