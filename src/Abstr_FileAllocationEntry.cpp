/* 
 * File:   FileAttributes.cpp
 * Author: leonardo Horstmann
 * 
 * Created on 20 de Outubro de 2016, 10:14
 */

#include "Abstr_FileAllocationTable.h"

FileAllocationEntry::FileAllocationEntry(fileIdentifier inode, HW_HardDisk::blockNumber block, FileAttributes attributes) {
	this->inode = inode;
	this->block = block;
	this->attributes = attributes;
}

FileAllocationEntry::fileIdentifier FileAllocationEntry::getNode() {
	return this->inode;
}

HW_HardDisk::blockNumber FileAllocationEntry::getBlock() {
	return this->block;
}

FileAttributes FileAllocationEntry::getAttributes() {
	return this->attributes;
}

void FileAllocationEntry::setNode(fileIdentifier inode) {
	this->inode = inode;
}

void FileAllocationEntry::setBlock(HW_HardDisk::blockNumber block) {
	this->block = block;
}

void FileAllocationEntry::setAttributes(FileAttributes attributes) {
	this->attributes = attributes;
}

