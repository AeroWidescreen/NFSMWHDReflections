#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <d3d9.h>

DWORD WINAPI Thing(LPVOID);

bool HDReflections, GeometryFix, RestoreLights;
static int ResolutionX, ResolutionY, ImproveReflectionLOD;
DWORD GameState;
HWND windowHandle;

DWORD RestoreFEReflectionCodeCaveExit = 0x6BD502;
DWORD VehicleReflectionCodeCaveExit = 0x6BD533;
DWORD FlareAnimationRestorationCodeCaveExit = 0x6DE9F8;
DWORD FlareAnimationFunctionJump = 0x507781;
DWORD sub_505E80 = 0x505E80;

void __declspec(naked) RestoreFEReflectionCodeCave()
{
	__asm {
		mov ecx, 0x100
		jmp RestoreFEReflectionCodeCaveExit
	}
}

void __declspec(naked) VehicleReflectionCodeCave()
{
	__asm {
		mov edi, dword ptr ds : [0x8F8FF4]
		mov edx, dword ptr ds : [0x8F8FF8]
		jmp VehicleReflectionCodeCaveExit
	}
}

void __declspec(naked) FlareAnimationRestorationCodeCave()
{
	__asm {
		call FlareAnimationFunction // Flare Animation call
		push 0x00919730
		call sub_505E80
		add esp, 0x0C
		jmp FlareAnimationRestorationCodeCaveExit

	FlareAnimationFunction:
		sub esp, 0x08
		push ebp
		push edi
		mov edi, dword ptr ds : [esp + 0x14]
		mov ecx, dword ptr ds : [edi + 0x04]
		mov eax, 0x02 // Removes buggy sun flare in RVM
		jmp FlareAnimationFunctionJump
	}
}

void Init()
{
	// Read values from .ini
	CIniReader iniReader("NFSMWHDReflections.ini");

	// Resolution
	ResolutionX = iniReader.ReadInteger("RESOLUTION", "ResolutionX", 1920);
	ResolutionY = iniReader.ReadInteger("RESOLUTION", "ResolutionY", 1080);

	// General
	HDReflections = iniReader.ReadInteger("GENERAL", "HDReflections", 1);
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 1);
	GeometryFix = iniReader.ReadInteger("GENERAL", "GeometryFix", 1);
	RestoreLights = iniReader.ReadInteger("GENERAL", "RestoreLights", 1);


	if (HDReflections)
	{
		// Jumps
		injector::MakeJMP(0x6BD4FC, RestoreFEReflectionCodeCave, true);
		injector::MakeJMP(0x6BD52D, VehicleReflectionCodeCave, true);
		// Road Reflection X
		injector::WriteMemory<uint32_t>(0x6CFC26, ResolutionX, true);
		injector::WriteMemory<uint32_t>(0x6BCDF5, ResolutionX, true);
		injector::WriteMemory<uint32_t>(0x6BD17D, ResolutionX, true);
		// Road Reflection Y
		injector::WriteMemory<uint32_t>(0x6BD184, ResolutionY, true);
		injector::WriteMemory<uint32_t>(0x6CFC2D, ResolutionY, true);
		injector::WriteMemory<uint32_t>(0x6BCDEF, ResolutionY, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x8F8FF4, ResolutionY, true);
		// RVM Reflection
		// Aspect ratio is based on NFSU2 because true aspect ratio is unknown
		injector::WriteMemory<uint32_t>(0x8F9008, ResolutionY, true);
		injector::WriteMemory<uint32_t>(0x8F900C, ResolutionY / 3, true);
	}

	if (ImproveReflectionLOD >= 1)
	{
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x6BFEBD, 0x00000000, true);
		// RVM LOD
		injector::WriteMemory<uint32_t>(0x6BFE58, 0x00000000, true);
		
		if (ImproveReflectionLOD >= 2)
		// Full LOD Improvement
		injector::WriteMemory<uint8_t>(0x4FAEB0, 0xEB, true);
	}

	if (GeometryFix)
	{
		injector::WriteMemory<uint8_t>(0x6C69AE, 0xEB, true);
	}

	if (RestoreLights)
	{
		// Adds missing flare animation for the rearview mirror
		injector::MakeJMP(0x6DE9F0, FlareAnimationRestorationCodeCave, true);
		// Solves flare culling issue 
		injector::WriteMemory<uint8_t>(0x729479, 0xEB, true);
	}
}
	

BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x7C4040) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			Init();

		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.3 English speed.exe (5,75 MB (6.029.312 bytes)).", "NFSMW HD Reflections", MB_ICONERROR);
			return FALSE;
		}
	}
	return TRUE;
	
}
