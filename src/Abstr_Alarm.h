/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Alarm.h
 * Author: cancian
 *
 * Created on 29 de Agosto de 2016, 14:42
 */

#ifndef ALARM_H
#define ALARM_H

#include "Abstr_Thread.h"
#include "Util_Queue.h"
#include "OperatingSystem.h"
#include "Simulator.h"
//Classe Alarm
/*
	Implementa o conceito de alarme, onde pode se configurar um timer para que
	dispare em um periodo de tempo e possa disparar varias vezes repetidamente
	em intervalos de tempo definidos.
*/
class Alarm {
    friend class ProblemTester;
public:
    typedef double Microsecond;
public:
	//Construtor
	/*
	*/
    Alarm();
    //Construtor com parametro
    /*
     * \param Alarm& orig um alarme ja existente.
    */
    Alarm(const Alarm& orig);
    //Destrutor
    /*
     *
     */
    virtual ~Alarm();
public:
    //Delay em certa thread
    /*
     *  causar um tempo de pausa na Thread corrente onde ela não deve executar
     *  durante o tempo do delay, e deve voltar a executar logo que este
     *  tempo se acabe. (Até o fim do restante para resetar seu período).
     */
    static void delay(const Microsecond & time);

    static Queue<Thread>* getQueue() {
        static Queue<Thread>* _queue;
        if (_queue == nullptr)
            _queue = new Queue<Thread>;
        return _queue;
    }
    static bool wakeUpAlarm();
private:
    // Init
    /*
     * Inicializa o alarme.
     */
    static void init();
};

#endif /* ALARM_H */

