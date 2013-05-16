#ifndef _UTILS_H_
#define _UTILS_H_

#pragma once

#include <string>

std::string make_point(int id, int seq, double x, double y);

/*
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */

/**
 * Returns the peak (maximum so far) resident set size (physical
 * memory use) measured in bytes, or zero if the value cannot be
 * determined on this OS.
 */
std::size_t getPeakRSS();


/**
 * Returns the current resident set size (physical memory use) measured
 * in bytes, or zero if the value cannot be determined on this OS.
 */
std::size_t getCurrentRSS();

#endif /* _UTILS_H_ */
