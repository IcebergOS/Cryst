// Copyright 2016 Iceberg OS
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=============================================================================
// ■ Timer.h
//-----------------------------------------------------------------------------
//   i686基础驱动：Timer。
//=============================================================================

#ifndef INCLUDE_TIMER_H
#define INCLUDE_TIMER_H

#include "Basic/idt.h"
#include "Basic/io.h"
#include "config.h"

#ifdef DEBUG
#include "Basic/debug.h"
#endif

class Timer {
	public:
	//---------------------------------------------------------------------------
	// ● 时钟初始化
	//---------------------------------------------------------------------------
	Timer(uint32_t frequency);
};

#endif
