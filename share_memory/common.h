#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "."
#define ID 1

int Common(int size, int flag);

int CreateShm(int size);

int GetShm(int size);

void DestroyShm(int shmid);
