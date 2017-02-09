/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   HD.cpp
 * Author: cancian
 *
 * Created on 29 de Agosto de 2016, 14:46
 */

#include "Mediator_HardDisk.h"
#include "HW_Machine.h"
#include "OperatingSystem.h"
#include "Simulator.h"
#include <math.h>

// DiskAccessRequest
DiskAccessRequest::DiskAccessRequest(Operation operation,
		HW_HardDisk::blockNumber blockNumber, HW_HardDisk::DiskSector* diskSector) {
	 _operation = operation;
	 _blockNumber = blockNumber;
	 _diskSector = diskSector;
	 _priority = 0;
	 _arrivalTime = Simulator::getInstance()->getTnow();

	 this->updatePriority();
 }

void DiskAccessRequest::updatePriority(){
	HardDisk *hd = OperatingSystem::HardDisk_Mediator();
	unsigned int headPos = hd->getHeadPosition();
	unsigned int maxTracks = hd->getTracksPerSurface();
	unsigned int track = this->_diskSector->track;

	if(track >= headPos)//req.track esta a frente da head?
		this->_priority = ceil(maxTracks / 2.0) + track;//então ela tem prioridade maior
	else
		this->_priority = 2.0 * maxTracks + track;

	// Proteção para que os JUMPs não fiquem sempre com a maior
	// prioridade
	if(this->_operation == JUMP && track == headPos)
		this->_priority = 2.0 * maxTracks + track;
}

std::ostream& operator<<(std::ostream& os, const DiskAccessRequest* c){
	std::string op = "";
	switch(c->GetOperation()){
	case DiskAccessRequest::READ:
		op = "READ";
		break;
	case DiskAccessRequest::WRITE:
		op = "WRITE";
		break;
	case DiskAccessRequest::JUMP:
		op = "JUMP";
		break;
	}
	os << "Request{ op: " << op <<
				", priority: " << c->getPriority() <<
				", Track: " << c->GetDiskSector()->track <<
				", Sector: " << c->GetDiskSector()->sector <<
				"}";
	return os;
}

// HardDisk
HardDisk::HardDisk(unsigned int instance) {
    _instance = instance;
    HW_HardDisk* hd = HW_Machine::HardDisk();
    hd->setCommandRegister(HW_HardDisk::GET_SECTORSIZE);
    _blocksize = hd->getDataRegister();
    hd->setCommandRegister(HW_HardDisk::GET_TOTALSECTORS);
    _maxBlocks = hd->getDataRegister();
    hd->setCommandRegister(HW_HardDisk::GET_TRACKSPERSURFACE);
	_tracksPerSurface = hd->getDataRegister();
}

HardDisk::HardDisk(const HardDisk& orig) {
}

HardDisk::~HardDisk() {
}

void HardDisk::interrupt_handler() {     // Hard Disk Interrupt Handler
    // INSERT YOUR CODE HERE
    // ...

	HardDisk *hd = OperatingSystem::HardDisk_Mediator();
	Scheduler<DiskAccessRequest>* scheduler = OperatingSystem::Disk_Scheduler();
	DiskAccessRequest *request;

	// Remove a requisição que acabou de ser atendida
	request = scheduler->choosen();
	scheduler->remove(request);
	if (request == nullptr) {
		// Checa para ver se é um jump
		if(request->GetOperation() == DiskAccessRequest::JUMP){
			// Caso seja um Jump para track 0, deve-se atualizar
			// as prioridades das requisições na fila
			if(request->GetDiskSector()->track == 0){
				scheduler->reschedule();
			}

			// Jumps devem ter sua prioridade ajustada e devem ser adicionados denovo
			// na lista [fazendo a lista se reordenar]
			request->updatePriority();
			scheduler->insert(request);
		}else{
			// Se não for um Jump apenas deleta a requisição
			delete request;
		}
	}

	// Pega a proxima requisição do escalonador e a atende
	request = scheduler->choose();

	if(request != nullptr)
		hd->accessBlock(request);
}

void HardDisk::flush() {

}

void HardDisk::writeBlock(DiskAccessRequest* request) {
	HW_HardDisk* hd = HW_Machine::HardDisk();
	HW_HardDisk::DiskSector* sector = request->GetDiskSector();
	//TODO rever esta conta
	hd->setDataRegister(sector->surface + sector->track + sector->sector);
	hd->setStreamRegister(sector->data);
	hd->setCommandRegister(HW_HardDisk::WRITE_LOGICALSECTOR);
}


void HardDisk::readBlock(DiskAccessRequest* request) {
	HW_HardDisk* hd = HW_Machine::HardDisk();
	HW_HardDisk::DiskSector* sector = request->GetDiskSector();
	//TODO rever esta conta
	hd->setDataRegister(sector->surface + sector->track + sector->sector);
	hd->setCommandRegister(HW_HardDisk::READ_LOGICALSECTOR);

	unsigned char *myData = *(hd->getStreamRegister());
    for (int i = 0; i < this->_blocksize; i++) {
        request->GetDiskSector()->data[i] = *(myData+i);
    }
}

void HardDisk::setBlockSize(const unsigned int blocksize) {
	this->_blocksize = blocksize;
}

void HardDisk::accessBlock(DiskAccessRequest* request) {
	// Acredito que os jumps não contem para isso?
	if(request->GetOperation() != DiskAccessRequest::JUMP){
		this->_accountInfo._totalReqsAttended++;
		this->_accountInfo._totalTimeToAttend += (Simulator::getInstance()->getTnow() -
				request->getArrivalTime());
	}

	switch(request->GetOperation()){
	case DiskAccessRequest::READ:
		readBlock(request);
		break;
	case DiskAccessRequest::WRITE:
		writeBlock(request);
		break;
	case DiskAccessRequest::JUMP:
		jumpToBlock(request);
		break;
	default:
		break;
	}
}

unsigned int HardDisk::getBlockSize() {
	return _blocksize;
}

void HardDisk::setMaxBlocks(const HW_HardDisk::blockNumber maxBlocks) {
	this->_blocksize = maxBlocks;
}

HW_HardDisk::blockNumber HardDisk::getMaxBlocks() {
	return _maxBlocks;
}

void HardDisk::jumpToBlock(DiskAccessRequest* request){
	HW_HardDisk* hd = HW_Machine::HardDisk();
	HW_HardDisk::DiskSector* sector = request->GetDiskSector();
	//TODO rever esta conta
	hd->setDataRegister(sector->surface + sector->track + sector->sector);
	hd->setCommandRegister(HW_HardDisk::JUMP_TO_LOGICALSECTOR);
}

unsigned int HardDisk::getHeadPosition(){
    HW_HardDisk* hd = HW_Machine::HardDisk();
    hd->setCommandRegister(HW_HardDisk::GET_HEADTRACKPOSITION);
    return hd->getDataRegister();
}

unsigned int HardDisk::getTracksPerSurface(){
	return _tracksPerSurface;
}

void HardDisk::showStatistics(){
	HW_HardDisk* hd = HW_Machine::HardDisk();
	std::cout << "\t\tHead moviments: " << hd->getTotalHeadMoviment() <<
			"\n\t\tTotal Bytes Read: " << hd->getTotalBytesRead() <<
			"\n\t\tTotal Bytes Written: " << hd->getTotalBytesWritten() <<
			"\n\t\tRequests Attended: " << this->getTotalReqsAttended() <<
			"\n\t\tMean Time to Attend a request: " << this->getMeanTimeToAttendReq() <<
            "\n";
}
