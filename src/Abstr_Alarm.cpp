/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Alarm.cpp
 * Author: cancian
 *
 * Created on 29 de Agosto de 2016, 14:42
 */

#include "Abstr_Alarm.h"
#include <iostream>

Alarm::Alarm() {
}

Alarm::Alarm(const Alarm& orig) {
}

Alarm::~Alarm() {
}

void Alarm::delay(const Microsecond & _time) {
	OperatingSystem::Process_Scheduler()->choosen()->setWakeUpTime(_time);
	Thread::sleep(Alarm::getQueue());
	Alarm::getQueue()->order();
}

bool Alarm::wakeUpAlarm() {
	bool end = false;
	bool change = false;
	while (!end && !(Alarm::getQueue()->empty())) {
		if (Alarm::getQueue()->top()->getWakeUpTime() <= Simulator::getInstance()->getTnow()) {
			Alarm::getQueue()->top()->setPeriod();
			Thread::wakeup(Alarm::getQueue());
			change = true;
		} else {
			end = true;
		}
	}
	return change;
}

void Alarm::init() {
}

