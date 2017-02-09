/*
 * File:   System.cpp
 * Author: cancian
 *
 * Created on October 9, 2015, 9:04 PM
 */
#include <string>

#include "OperatingSystem.h"
#include "Application.h"
#include "HW_CPU.h"
#include "HW_Machine.h"
#include "Simulator.h"
#include "BestFitAllocation.h"


void OperatingSystem::LoadApplication(Application* app, MMU::PhysicalAddress address) {
    Debug::cout(Debug::Level::trace, "OperatingSystem::LoadApplication(" + std::to_string(reinterpret_cast<unsigned long> (app)) + "," + std::to_string(address) + ")");
    std::list<Application::Information> code = app->getCode();
    HW_MMU::Information info;
    for(std::list<HW_MMU::Information>::iterator it = code.begin(); it != code.end(); it++) {
        info = (*it);
        HW_Machine::RAM()->write(address, info);
        address++;
    }
}

void OperatingSystem::SetBootApplication(Application* app) {
    Debug::cout(Debug::Level::trace, "OperatingSystem::SetBootApplication(" + std::to_string(reinterpret_cast<unsigned long> (app)) + ")");
    HW_CPU::Register address = HW_Machine::CPU()->readRegister(HW_CPU::pc);
    LoadApplication(app, address);
}


void OperatingSystem::Init() {
    Debug::cout(Debug::Level::trace, "OperatingSystem::init()");
    HW_Machine::Init();

    OperatingSystem::CPU_Mediator();
    OperatingSystem::DMA_Mediator();
    OperatingSystem::HardDisk_Mediator();
    OperatingSystem::MMU_Mediator();
    OperatingSystem::Timer_Mediator();

    SetBootApplication(Application::DefaultBootApplication());  // load boot application into RAMs
}

void OperatingSystem::ExecuteTestCode() {
    Debug::cout(Debug::Level::trace, "OperatingSystem::ExecuteTestCode()");
    Simulator* simulator = Simulator::getInstance();
    Entity* entity = simulator->getEntity();
    Module* module = simulator->getModule();
    int executionStep = std::stoi(entity->getAttribute("ExecutionStep")->getValue());
    double timeNow = simulator->getTnow();

    // INSERT HERE YOUR CODE
    // You can write a test code that will be executed and will invoke system calls or whenever you want
    // Follow the examples...
    // ...
    HW_RAM *ram = new HW_RAM();
    Process::memManager = OperatingSystem::Memory_Manager();
    auto strategy = new BestFitAllocation();
    Process::getMemoryManager()->setAllocationStrategy(strategy);
    Queue<Thread> q;

    switch (executionStep) {
        case 0:
            {  // ExecutionStep is initialized with 0
                entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
                simulator->insertEvent(timeNow + 10.0, module, entity); // future event when execution will advance
                std::cout << "Testing function 'exec' from the class 'Process':" << std::endl;

                auto process_1 = Process::exec();

                std::cout << "Hello, world! I'm the process of ID:" << process_1->getId() << std::endl;
                std::cout << "Testing function 'exit' from the class 'Process':" << std::endl;

                auto id = process_1->getId();
                process_1->exit();

                std::cout << "Goodbye, world! Process " << id << " has been killed!" << std::endl;
            }

        case 1:
            {
                entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
                std::cout << "Testing resource allocation function from the class 'Process':"  << std::endl;

                auto process_1 = Process::exec();
                process_1->alocarRecurso(1, true);

                std::cout << "Process " << process_1->getId() << " has " << process_1->getQtdRecursos() << " resources "<< std::endl;
                std::cout << "Testing resource deallocation function from the class 'Process':"  << std::endl;

                process_1->desalocarRecurso();

                std::cout << "Process " << process_1->getId() << " has " << process_1->getQtdRecursos() << " resources " << std::endl;
                process_1->exit();
            }

        case 2:
            {
                entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
                std::cout << "Testing thread create function from the class 'Thread':" << std::endl;

                auto process_1 = Process::exec();
                auto thread_1 = Thread::thread_create(process_1);

                std::cout << "Hello, world! I'm the the process of ID:" << thread_1->getId() << std::endl;
                std::cout << "Testing thread exit function from the class 'Thread':" << std::endl;

                auto id = thread_1->getId();
                thread_1->exit();

                std::cout << "Goodbye, world! Thread " << id << " has been killed!" << std::endl;

                process_1->exit();
            }

        case 3:
            {
                entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
                std::cout << "Testing thread sleep function from the class 'Thread':" << std::endl;

                auto process_1 = Process::exec();
                auto thread_1 = Thread::thread_create(process_1);

                thread_1->sleep(&q);

                std::cout << "Thread " << thread_1->getId() << " is sleeping." << std::endl;
                std::cout << "Testing thread wakeup function from the class 'Thread':" << std::endl;

                thread_1->wakeup(&q);

                std::cout << "Thread " << thread_1->getId() << " woke up successfully." << std::endl;

                process_1->exit();
            }

        case 4:
            {
                entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
                std::cout << "Testing thread yield function from the class 'Thread':" << std::endl;

                auto process_1 = Process::exec();
                auto thread_1 = Thread::thread_create(process_1);

                thread_1->yield();
                std::cout << "Thread " << thread_1->getId() << " executed yield." << std::endl;
            }

        case 5:
            {
                entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
                std::cout << "Testing thread join function from the class 'Thread':" << std::endl;

                auto process_1 = Process::exec();
                auto thread_1 = Thread::thread_create(process_1);
                auto thread_2 = Thread::thread_create(process_1);

                std::cout << "Thread " << thread_1->getId() << " and " << thread_2->getId() << " were created and belong to the process " << process_1->getId() <<  std::endl;

                thread_1->join();

                std::cout << "Thread " << thread_1->getId() << " executed join." << std::endl;
            }

        case 6:
            {
                entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
                std::cout << "Testing defragment from the class 'Memory_Manager':" << std::endl;


                std::cout << "Creating 6 processes." << std::endl;
                Process * p1, * p2, *p3, *p4, *p5, *p6;

                p1 = Process::exec();
                p2 = Process::exec();
                p3 = Process::exec();
                p4 = Process::exec();
                p5 = Process::exec();
                p6 = Process::exec();

                std::cout << "Deleting 2 processes." << std::endl;
                p3->exit();
                p4->exit();
                Process::getMemoryManager()->showStatistics();

                std::cout << "Defragmenting memory." << std::endl;
                Process::getMemoryManager()->defragment();
                Process::getMemoryManager()->showStatistics();
                Process::getMemoryManager()->showMemory();
            }

        case 7:
            {
            entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
            std::cout << "Testing File System:" << std::endl;

            FileSystem *fileSys = OperatingSystem::File_System();
            std::cout << "Creating Directory: '/o'" << std::endl;
            fileSys->criarDiretorio((const unsigned char*)"/o ");

            const unsigned char* file1 = (const unsigned char*)"/o/t ";
            const unsigned char* file2 = (const unsigned char*)"/o/p ";
            std::cout << "Creating Files: '" << file1 << "' and '" << file2 << "'" << std::endl;

            fileSys->criarArquivo(file1);
            fileSys->criarArquivo(file2);
            std::cout << "Trying to open non existant file: '/o/s'" << std::endl
                      << "Response from 'abrirArquivo': " << fileSys->abrirArquivo((const unsigned char*)"/o/s ") << std::endl
                      << "Trying to open file: '" << file2 << "'" << std::endl;

            unsigned int node1 = fileSys->abrirArquivo(file2);
            std::cout << "Local that '" << file2 << "' is: " << node1 << std::endl;

            int size = 64;
            auto buff = new unsigned char[size];
            for (int i = 0; i < size; i++) {
                buff[i] = 'T';
            }

            buff[63] = 'F';
            std::cout << "Writing '" << buff << "' in '" << file2 << "'" << std::endl
                      << fileSys->escreverEmArquivo(node1, 0, buff, size) << std::endl;

            auto buff2 = new unsigned char[size];
            std::cout << "Reading from file: '" << file2 << "'" << std::endl
                      << fileSys->leDoArquivo(node1, 0, buff2, size) << std::endl
                      << "Checking Errors" << std::endl;

            int errors = 0;
            for (int i = 0; i < size; i++) {
                if (buff2[i] != buff[i]) {
                    errors++;
                }
            }

            std::cout << "Result: " << errors << " errors." << std::endl;

            auto node2 = fileSys->abrirArquivo(file1);
            std::cout << "Local that '" << file2 << "' is: " << node2 << std::endl;
            std::cout << "Writing in '" << file2 << "': " << fileSys->escreverEmArquivo(node2, 5, buff, size) << std::endl;

            unsigned char* buff3;
            unsigned char vector3[size];
            buff3 = vector3;
            std::cout << "Reading from '" << file2 << "': " << fileSys->leDoArquivo(node2, 5, buff3, size) << std::endl;
            errors = 0;
            for (int i = 0; i < size; i++) {
                if (buff3[i] == buff[i]) {
                    errors++;
                }
            }
            std::cout << "Number of errors found: " <<  errors <<  std::endl;

            std::cout << "Deleting " << file1 << std::endl;

            fileSys->removerArquivo(file1);
            std::cout << "Veryfying if file was deleted, number of deleted files: " << fileSys->getNumArquivosDeletados() << std::endl;
            std::cout << "Trying to open " << file1 << ": " << fileSys->abrirArquivo(file1) << std::endl;

            fileSys->removerArquivo(file2);
            }

        case 8:
            {
            entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
            std::cout << "Testing request's and disk acess:" << std::endl;
            // Adiciona os 2 jumps para fazer o disco sempre ir até as bordas
            auto scheduler = OperatingSystem::Disk_Scheduler();
            auto hd_mediator = OperatingSystem::HardDisk_Mediator();

            // Request para jump no track 0
            std::cout << "Adding requests!" << std::endl;
            auto diskSector = new HW_HardDisk::DiskSector{{},0,0,0};
            auto request = new DiskAccessRequest(DiskAccessRequest::JUMP, 0, diskSector);
            scheduler->insert(request);

            // Request para jump no track maxTracks-1
            diskSector = new HW_HardDisk::DiskSector{{},0,hd_mediator->getTracksPerSurface()-1,0};
            request = new DiskAccessRequest(DiskAccessRequest::JUMP,0,diskSector);
            scheduler->insert(request);

            // Testes
            // Adicionar requisicoes de escrita ao escalonador de disco
            std::cout << "Adding request's of write" << std::endl;
            diskSector = new HW_HardDisk::DiskSector{{0,0,1,1},0,150,0};
            request = new DiskAccessRequest(DiskAccessRequest::WRITE,0,diskSector);
            scheduler->insert(request);

            diskSector = new HW_HardDisk::DiskSector{{1,1,1,1},0,50,0};
            request = new DiskAccessRequest(DiskAccessRequest::WRITE,0,diskSector);
            scheduler->insert(request);

            diskSector = new HW_HardDisk::DiskSector{{1,1},0,250,0};
            request = new DiskAccessRequest(DiskAccessRequest::WRITE,0,diskSector);
            scheduler->insert(request);

            // Pegar a 1* requisição do escalonador
            request = scheduler->choose();

            // Chamar o Mediator_HardDisk passando esta primeira requisição
            hd_mediator->accessBlock(request);
            std::cout << "The request with most priority had been atended." << std::endl;
            }

            case 9:
            {
            entity->getAttribute("ExecutionStep")->setValue(std::to_string(++executionStep)); // advance execution step
            std::cout << "Testing read request" << std::endl;

            auto scheduler = OperatingSystem::Disk_Scheduler();
            auto hd_mediator = OperatingSystem::HardDisk_Mediator();

            // Adiciona requisicoes de leitura ao escalonador de disco
            auto diskSector = new HW_HardDisk::DiskSector{{},0,250,0};
            auto request = new DiskAccessRequest(DiskAccessRequest::READ,0,diskSector);
            scheduler->insert(request);

            diskSector = new HW_HardDisk::DiskSector{{},0,50,0};
            request = new DiskAccessRequest(DiskAccessRequest::READ,0,diskSector);
            scheduler->insert(request);

            request = scheduler->choose();
            hd_mediator->accessBlock(request);
            std::cout << "The read request with most priority had been atended." << std::endl;
            }
        default:
            //entity->getAttribute("ExecutionStep")->setValue(std::to_string(executionStep++)); // advance execution step
            break;
    }
    std::cout << "fim" << std::endl;
}

 /*
  Not used
  */
 HW_MMU::Information OperatingSystem::asmm(std::string mnemonic) {
     HW_MMU::Information bincode;
     /* @TODO
      */
     if (mnemonic == "Process:exec()") {
         bincode = 0xFFFFFFFF; // - (HW_CPU::addi<<24) +
     }
     //@TODO
     bincode = (HW_CPU::addi<<26) + (HW_CPU::s0<<21) + (HW_CPU::s1<<16) + 1; // for tests purpose only
     return bincode;
 }

 /*

    thread exit
    escalonamento
    strategys

  */