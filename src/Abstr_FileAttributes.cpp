/* 
 * File:   FileAttributes.cpp
 * Author: leonardo Horstmann
 * 
 * Created on 20 de Outubro de 2016, 10:14
 */

#include "Abstr_FileAllocationTable.h"

FileAttributes::FileAttributes() {
	filename = new char;
}

char* FileAttributes::getFilename() {
	return this->filename;
}

void FileAttributes::setFilename(char* filename) {
	this->filename = new char (*filename);
}

unsigned long FileAttributes::getSize() {
	return this->size;
}

void FileAttributes::setSize(unsigned long size) {
	this->size = size;
}

FileAttributes::fileType FileAttributes::getType() {
	return this->type;	
}

void FileAttributes::setType(FileAttributes::fileType type) {
	this->type = type;
}
