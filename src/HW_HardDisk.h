/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HW_HardDisk.h
 * Author: cancian
 *
 * Created on 29 de Agosto de 2016, 11:55
 */

#ifndef HW_HARDDISK_H
#define HW_HARDDISK_H

#include <list>

#define DISK_SECTOR_SIZE 64

class HW_HardDisk {
    friend class ProblemTester;
public:
    typedef unsigned char DiskSectorData[DISK_SECTOR_SIZE];
    typedef unsigned int blockNumber;
public:

    struct DiskSector {
        DiskSectorData data;
        unsigned int surface;
        unsigned int track;
        unsigned int sector;
    };

    enum HDCommand {
        GET_SURFACES          = 0x00000001,
        GET_TRACKSPERSURFACE  = 0x00000002,
        GET_SECTORSPERTRACK   = 0x00000004,
        GET_TOTALSECTORS      = 0x00000008,
        GET_HEADTRACKPOSITION = 0x00000010,
        GET_SECTORSIZE        = 0x00000020,
        READ_LOGICALSECTOR    = 0x00000040,
        WRITE_LOGICALSECTOR   = 0x00000080,
		JUMP_TO_LOGICALSECTOR = 0x00000100
    };

    struct AccountInformation {
    	unsigned int _totalHeadMov;//Quantidade total de movimentos do cabeçote
    	unsigned int _totalReadSectors;//Quantidade total de setores lidos
    	unsigned int _totalWrittenSectors;//Quantidade total de setores escritos
    };

public:
    HW_HardDisk();

    HW_HardDisk(const HW_HardDisk& orig);
    virtual ~HW_HardDisk();
    void setCommandRegister(unsigned int _commandRegister);
    unsigned int getStatusRegister() const;
    void setDataRegister(unsigned int _dataRegister);
    unsigned int getDataRegister() const;
    void setStreamRegister(DiskSectorData _streamRegister);
    DiskSectorData* getStreamRegister();

    //Estatisticas
    /**
     * Retorna o numero total de movimentos do cabeçote do disco
     * @return O numero total de movimentos do cabeçote do disco
     */
    unsigned int getTotalHeadMoviment(){
    	return _accountInfo._totalHeadMov;
    }

    /**
     * Retorna o numero total de bytes lidos pelo disco
     * @return O numero total de bytes lidos pelo disco
     */
    unsigned int getTotalBytesRead(){
    	return _accountInfo._totalReadSectors * DISK_SECTOR_SIZE;
    }

    /**
     * Retorna o numero total de bytes escritos no disco
     * @return O numero total de bytes escritos no disco
     */
    unsigned int getTotalBytesWritten(){
    	return _accountInfo._totalWrittenSectors * DISK_SECTOR_SIZE;
    }

    /**
     * Retorna o numero total de bytes manipulados pelo disco, lidos e escritos
     * @return O numero total de bytes manipulados pelo disco, lidos e escritos
     */
    unsigned int getTotalBytesHandled(){
    	return getTotalBytesRead() + getTotalBytesWritten();
    }

private:
    DiskSectorData _streamRegister;
    unsigned int _dataRegister;
    unsigned int _commandRegister;
    unsigned int _statusRegister;
    AccountInformation _accountInfo;
private:
    std::list<DiskSector*> *_hardDisk;
    unsigned int _headTrackPosition;
};

#endif /* HW_HARDDISK_H */

