///////////////////////
//PhysxÇégópÇ∑ÇÈÇΩÇﬂÇÃèÄîı

#pragma once

#define			ERMS( message ){ 		MessageBox(NULL, TEXT( message ), TEXT("error"), MB_ICONWARNING); }

#include		"PxPhysicsAPI.h"

using namespace physx;

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib,"PhysX3DEBUG_x86.lib")
#pragma comment(lib,"PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib,"PhysX3CharacterKinematicDEBUG_x86.lib")
#pragma comment(lib,"PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib,"PhysXProfileSDKDEBUG.lib")
#pragma comment(lib,"PhysX3CookingDEBUG_x86.lib")
#else
#pragma comment(lib,"PhysX3_x86.lib")
#pragma comment(lib,"PhysX3Extensions.lib")
#pragma comment(lib,"PhysX3CharacterKinematic_x86.lib")
#pragma comment(lib,"PhysX3Common_x86.lib")
#pragma comment(lib,"PhysXProfileSDK.lib")
#pragma comment(lib,"PhysX3Cooking_x86.lib")
#endif

class PmAllocatorCallback : public PxAllocatorCallback
{
public:
	PmAllocatorCallback(){};
	~PmAllocatorCallback(){};

	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line)
	{
		return malloc(size);
	}

	virtual void deallocate(void* ptr)
	{
		free(ptr);
	}
};

class PmErrorCallback : public PxErrorCallback
{
public:
	PmErrorCallback(){};
	~PmErrorCallback(){};

	virtual void reporError(PxErrorCode::Enum code, const char* msg, const char* file, int line)
	{
		//ERMS(msg);
	}
};