/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileAllocationTable.cpp
 * Author: cancian
 * 
 * Created on 9 de Setembro de 2016, 10:14
 */

#include "Abstr_FileAllocationTable.h"
#include <vector>

std::vector<FileAllocationEntry> entradas;

FileAllocationTable::FileAllocationTable(HardDisk* disk) {
	this->disk = disk;		
}

FileAllocationTable::FileAllocationTable(const FileAllocationTable& orig) {

}

int FileAllocationTable::getTotalSize() {
	int totalSize = 0;
	for (int i = 0; i < entradas.size(); i++) {
		totalSize += entradas[i].getAttributes().getSize();
	}	
	return totalSize;
}	

void FileAllocationTable::addFileEntry(FileAllocationEntry fatEntry) {
	entradas.insert(entradas.end(),fatEntry);
}	

void FileAllocationTable::removeFileEntryByNode(FileAllocationEntry::fileIdentifier inode) {
	for (unsigned i = 0; i < entradas.size(); i ++) { 
		FileAllocationEntry atual = entradas[i];		
		if (atual.getNode() == inode) {
			entradas.erase(entradas.begin()+i);
		}
	}
}

FileAllocationEntry FileAllocationTable::getFileEntryByNode(FileAllocationEntry::fileIdentifier inode) {
	for (unsigned i = 0; i < entradas.size(); i ++) { 
		FileAllocationEntry atual = entradas[i];		
		if (atual.getNode() == inode) {
			return entradas[i];
		}
	}
}

void FileAllocationTable::removeFileEntryByRank(unsigned int rank) {
	entradas.erase(entradas.begin()+rank);
}

FileAllocationEntry FileAllocationTable::getFileEntryByRank(unsigned int rank) {
	return entradas[rank];
}

void setFileEntry(unsigned int rank, FileAllocationEntry fatEntry) {
	entradas[rank] = fatEntry;
}
