/* TIDE
 *
 * Windows DLL declaration.
 *
 * Copyright 2011 Geoffrey Biggs geoffrey.biggs@aist.go.jp
 *     RT-Synthesis Research Group
 *     Intelligent Systems Research Institute,
 *     National Institute of Advanced Industrial Science and Technology (AIST),
 *     Japan
 *     All rights reserved.
 *
 * This file is part of TIDE.
 *
 * TIDE is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * TIDE is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with TIDE. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined(TIDE_WIN_DLL_H_)
#define TIDE_WIN_DLL_H_

#if defined(WIN32)
    #if defined(TIDE_STATIC)
        #define TIDE_EXPORT
    #elif defined(tide_EXPORTS)
        #define TIDE_EXPORT __declspec(dllexport)
    #else
        #define TIDE_EXPORT __declspec(dllimport)
    #endif
#else
    #define TIDE_EXPORT
#endif

#endif // TIDE_WIN_DLL_H_

