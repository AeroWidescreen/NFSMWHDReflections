#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <d3d9.h>

DWORD WINAPI Thing(LPVOID);

bool HDReflections, GeometryFix, RestoreShaders, MirrorTint, ExpandSlotPool;
static int ResolutionX, ResolutionY, ImproveReflectionLOD, RestoreDetails, MirrorTintR, MirrorTintG, MirrorTintB;
DWORD GameState;
HWND windowHandle;

DWORD RestoreFEReflectionCodeCaveExit = 0x6BD502;
DWORD VehicleReflectionCodeCaveExit = 0x6BD533;
DWORD TrafficLightRestorationCodeCaveExit = 0x6DE9F8;
DWORD TrafficLightFunctionJump = 0x507781;
DWORD sub_505E80 = 0x505E80;
DWORD FlareAndShadowLayeringCodeCaveExit = 0x6DE990;
DWORD sub_723FA0 = 0x723FA0;
DWORD sub_750B10 = 0x750B10;
DWORD sub_6E2F50 = 0x6E2F50;
DWORD sub_503D00 = 0x503D00;
DWORD EnableCarShadowCodeCaveExit = 0x74E843;
DWORD EnableCarShadowCodeCaveJump = 0x74E818;
DWORD EnableShadersCodeCaveExit = 0x6DABDB;
DWORD EnableShadersCodeCaveJump = 0x6DABFC;
DWORD EnableParticlesCodeCaveExit = 0x6DE9E8;
DWORD MirrorRGBCodeCaveExit = 0x6E7126;
DWORD RemoveRoadReflectionCodeCaveExit = 0x6D71F6;
DWORD RemoveRoadReflectionCodeCavePart2Exit = 0x6D7264;

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

void __declspec(naked) TrafficLightRestorationCodeCave()
{
	__asm {
		call TrafficLightFunction
		push 0x00919730
		call sub_505E80
		add esp, 0x0C
		jmp TrafficLightRestorationCodeCaveExit

	TrafficLightFunction:
		sub esp, 0x08
		push ebp
		push edi
		mov edi, dword ptr ds : [esp + 0x14]
		mov ecx, dword ptr ds : [edi + 0x04]
		mov eax, 0x02 // Removes buggy sun flare in RVM
		jmp TrafficLightFunctionJump
	}
}

void __declspec(naked) FlareAndShadowLayeringCodeCave()
{
	__asm {
		push 01
		push 0x00919730
		lea ecx, dword ptr ds : [esp + 0xE8]
		call sub_723FA0
		call sub_6E2F50
		push ebx
		push 0x00919730
		call sub_750B10
		call sub_6E2F50
		jmp FlareAndShadowLayeringCodeCaveExit
	}
}

void __declspec(naked) EnableCarShadowCodeCave()
{
	__asm {
		cmp dword ptr ds : [edi + 0x04], 0x01
		je EnableCarShadowCodeCaveConditional
		cmp dword ptr ds : [edi + 0x04], 0x03
		je EnableCarShadowCodeCaveConditional
		jmp EnableCarShadowCodeCaveExit
		
	EnableCarShadowCodeCaveConditional:
		jmp EnableCarShadowCodeCaveJump
	}
}

void __declspec(naked) EnableShadersCodeCave()
{
	__asm {
		cmp eax, 0x03
		jle EnableShadersCodeCaveConditional
		cmp dword ptr ds : [edi + 0x04], 0x00000000
		je EnableShadersCodeCaveConditional
		mov eax, dword ptr ds : [edi + 0x04]
		jmp EnableShadersCodeCaveExit

		EnableShadersCodeCaveConditional :
		jmp EnableShadersCodeCaveJump
	}
}

void __declspec(naked) EnableParticlesCodeCave()
{
	__asm {
		add esp, 0x0C
		cmp eax, ebx
		push 0x919730
		mov ecx, 0x9166C0
		call sub_503D00
		jmp EnableParticlesCodeCaveExit
	}
}

void __declspec(naked) RemoveRoadReflectionCodeCave()
{
	__asm {
		mov eax, dword ptr ds : [0x982A20]
		cmp dword ptr ds : [eax + 0x04], 0x03
		je RemoveRoadReflectionCodeCavePart2
		push ebp
		mov ebp, esp
		and esp, 0xFFFFFFF0
		jmp RemoveRoadReflectionCodeCaveExit

	RemoveRoadReflectionCodeCavePart2:
		push ebp
		mov ebp, esp
		and esp, 0xFFFFFFF0
		sub esp, 0x58
		mov eax, dword ptr ds : [0x982A20]
		test eax, eax
		push esi
		push edi
		mov esi, ecx
		mov eax, dword ptr ds : [eax + 0x04]
		fld dword ptr ds : [0x890968]
		mov ecx, dword ptr ds : [0x9B392C]
		imul eax, eax, 0x70
		add eax, 0x9195E0
		mov eax, dword ptr ds : [eax + 0x68]
		test eax, eax
		je RemoveRoadReflectionCodeCavePart2Jump1
		fstp st(0)
		fld dword ptr ds : [eax + 0x290]

		RemoveRoadReflectionCodeCavePart2Jump1:
		test eax, eax
		fsubr dword ptr ds : [0x89096C]
		fld st(0)
		fmul dword ptr ds : [ecx + 0x40]
		fstp dword ptr ds : [esp + 0x10]
		fld st(0)
		fmul dword ptr ds : [ecx + 0x44]
		fstp dword ptr ds : [esp + 0x14]
		fmul dword ptr ds : [ecx + 0x48]
		fstp dword ptr ds : [esp + 0x18]
		fld dword ptr ds : [0x890968]
		je RemoveRoadReflectionCodeCavePart2Jump2
		fstp st(0)
		fld dword ptr ds : [eax + 0x36A0]

		RemoveRoadReflectionCodeCavePart2Jump2:
		mov eax, dword ptr ds : [esi + 0x44]
		// fstp dword ptr [esp+1C] removing instruction removes road reflection
		jmp RemoveRoadReflectionCodeCavePart2Exit
	}
}

void __declspec(naked) MirrorRGBCodeCave()
{
	__asm {
		mov byte ptr ds : [eax], 0xFF // Alpha
		push ebx
		mov ebx, dword ptr ds : [MirrorTintR]
		call MirrorRGBValueCheckCodeCave
		mov byte ptr ds : [eax - 0x03], bl // Red
		mov ebx, dword ptr ds : [MirrorTintG]
		call MirrorRGBValueCheckCodeCave
		mov byte ptr ds : [eax - 0x02], bl // Green
		mov ebx, dword ptr ds : [MirrorTintB]
		call MirrorRGBValueCheckCodeCave
		mov byte ptr ds : [eax - 0x01], bl // Blue
		pop ebx
		add eax, 0x04
		jmp MirrorRGBCodeCaveExit

	MirrorRGBValueCheckCodeCave:
		cmp ebx, 0x000000FF
		jg MirrorRGBValueCheckCodeCaveMax
		cmp ebx, 0x00000000
		jl MirrorRGBValueCheckCodeCaveMin
		ret
		MirrorRGBValueCheckCodeCaveMax:
		mov ebx, 0x000000FF
		ret
		MirrorRGBValueCheckCodeCaveMin:
		mov ebx, 0x00000000
		ret
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
	RestoreDetails = iniReader.ReadInteger("GENERAL", "RestoreDetails", 1);
	RestoreShaders = iniReader.ReadInteger("GENERAL", "RestoreShaders", 1);

	// Extra
	ExpandSlotPool = iniReader.ReadInteger("EXTRA", "ExpandSlotPool", 1);
	MirrorTint = iniReader.ReadInteger("EXTRA", "MirrorTint", 1);
	MirrorTintR = iniReader.ReadInteger("EXTRA", "MirrorTintR", 255);
	MirrorTintG = iniReader.ReadInteger("EXTRA", "MirrorTintG", 255);
	MirrorTintB = iniReader.ReadInteger("EXTRA", "MirrorTintB", 255);


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
		if (ImproveReflectionLOD >= 2)
		injector::WriteMemory<uint8_t>(0x4FAE9A, 0xEB, true);
	}

	if (GeometryFix)
	{
		injector::WriteMemory<uint8_t>(0x6C69AE, 0xEB, true);
	}

	if (RestoreDetails >= 1)
	{
		// Adds missing traffic lights to the rearview mirror
		injector::MakeJMP(0x6DE9F0, TrafficLightRestorationCodeCave, true);
		// Solves flare and car shadow layering issue
		injector::MakeJMP(0x6DE96D, FlareAndShadowLayeringCodeCave, true);
		// Solves flare culling issue 
		injector::WriteMemory<uint8_t>(0x729479, 0xEB, true);
		// Adds missing car shadow to the rearview mirror
		injector::MakeJMP(0x74E812, EnableCarShadowCodeCave, true);

		if (RestoreDetails >= 2)
		// Adds missing particle effects to the rearview mirror
		injector::MakeJMP(0x6DE9E3, EnableParticlesCodeCave, true);
	}

	if (RestoreShaders)
	{
		// Enables all shader effects for reflections
		// Credit to osdever for discovery
		injector::MakeJMP(0x6DABD4, EnableShadersCodeCave, true);
		// Removes road reflections from rearview mirror
		injector::MakeJMP(0x6D71F0, RemoveRoadReflectionCodeCave, true);
	}

	if (MirrorTint)
	{
		// Controls the RGB of the mirror
		injector::MakeJMP(0x6E7120, MirrorRGBCodeCave, true);
	}

	if (ExpandSlotPool)
	{
		// Fixes disappearing objects
		injector::WriteMemory<uint32_t>(0x5009D2, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x5009DC, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x500A01, 0xFA000, true);
		injector::WriteMemory<uint32_t>(0x500A12, 0xFA000, true);
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