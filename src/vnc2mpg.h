/*******************************************************************************
**
** Copyright (C) 2023 Yura Beznos at You-ra.info
**
** This file is part of the Screen recorder application project.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write an email to the license@you-ra.info
*******************************************************************************/

#ifndef VNC2MPG_H_INCLUDED
#define VNC2MPG_H_INCLUDED

#ifdef __cplusplus
   extern "C" {
#endif

extern "C" void cmain(const char *filename_const, int *stop, int *scale, 
                      int *fps, int *bps, int *codec);

#ifdef __cplusplus
   }
#endif

#endif /* VNC2MPG_H_INCLUDED */
