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
// ■ Timer.c
//-----------------------------------------------------------------------------
//   i686基础驱动：Timer。
//=============================================================================

#include "Basic/timer.h"

uint64_t tick = 0;
uint64_t millis_from_boot = 0;
uint32_t timer_frequency = 0;

static task_t* kernel_regs;

//---------------------------------------------------------------------------
// ● 时钟信号处理函数
//---------------------------------------------------------------------------
//#define TIMER_VERBOSE_TICKS
bool a,b ;
intptr_t ptra, ptrb;
volatile task_t* timer_callback(task_t *regs) {
	tick++;
	millis_from_boot += 1000 / timer_frequency;

	#ifdef DEBUG
		#ifdef TIMER_VERBOSE_TICKS
			kprintf("[%s : %d]\r\n", "Tick", tick);
		#endif
	#endif

	io_out8(0x20, 0x20); //重设时钟中断

	volatile task_t* oregs = regs;
	if (a) {
		create_task(regs, stb + 4096, taskb);
		create_task(regs, sta + 4096, taska);
		a = false;

		ptra = sta + 4096 - sizeof(task_t);
		ptrb = stb + 4096 - sizeof(task_t);

		oregs = ptra;

	} else if (!b) {
		ptra = regs;
		oregs = ptrb;
	} else {
		ptrb = regs;
		oregs = ptra;
	}
	b=!b;
//

	return oregs;
}

void Timer_set_frequency(uint32_t frequency) {
	io_cli();

	// Intel 8253/8254 PIT芯片 I/O端口地址范围是40h~43h
	// 输入频率为 1193180 (12MHz)，frequency 即每秒中断次数
	uint32_t divisor = 1193180 / frequency;
	timer_frequency = frequency;

	// 分别写入低字节和高字节
	io_out8(0x40, (uint8_t)(divisor & 0xFF));
	io_out8(0x40, (uint8_t)((divisor >> 8) & 0xFF));

	io_sti();
}

//---------------------------------------------------------------------------
// ● 时钟初始化
//---------------------------------------------------------------------------
void Init_Timer(uint32_t frequency) {
	a=true;
	b=true;
	// 注册时间相关的处理函数
	//idt_register_interrupt_handler(IRQ0, timer_callback); // Deprecated
	// Now timer has dedicated function implementation

	// D7 D6 D5 D4 D3 D2 D1 D0
	// 0  0  1  1  0  1  1  0
	// 即就是 36 H
	// 设置 8253/8254 芯片工作在模式 3 下
	io_out8(0x43, 0x36);

	Timer_set_frequency(frequency);
}
