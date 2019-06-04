#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <d3d9.h>

DWORD WINAPI Thing(LPVOID);

bool HDReflections, HDReflectionBlur, HDMirror, GeometryFix, RestoreShaders, OptimizeRenderDistance, MirrorTint, ExpandSlotPool;
static int ResolutionX, ResolutionY, ImproveReflectionLOD, RestoreDetails, MirrorTintR, MirrorTintG, MirrorTintB;
static float RoadScale, VehicleScale, MirrorScale;
static float SkyboxRenderDistace = 0.1f;
DWORD GameState;
HWND windowHandle;

DWORD RestoreFEReflectionCodeCaveExit = 0x6BD502;
DWORD VehicleReflectionCodeCaveExit = 0x6BD533;
DWORD HDReflectionBlurCodeCaveExit = 0x6BCE74;
DWORD RoadReflectionResX = 0x0000000;
DWORD RoadReflectionResY = 0x0000000;
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
DWORD RenderDistanceCodeCaveExit = 0x6E73A6;
DWORD RenderDistanceCodeCaveExit2 = 0x6E73AB;

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

void __declspec(naked) HDReflectionBlurCodeCave()
{
	__asm {
		cmp byte ptr ds : [HDReflectionBlur], 0x01
		je HDReflectionBlurCodeCaveConditional
		push 0x000000F0 // 240
		push 0x00000140 // 320
		push eax
		jmp HDReflectionBlurCodeCavePart2

		HDReflectionBlurCodeCaveConditional :
		fild dword ptr ds : [ResolutionX]
		fmul dword ptr ds : [RoadScale]
		fistp dword ptr ds : [RoadReflectionResX]
		push dword ptr ds : [RoadReflectionResX]
		fild dword ptr ds : [ResolutionY]
		fmul dword ptr ds : [RoadScale]
		fistp dword ptr ds : [RoadReflectionResY]
		push dword ptr ds : [RoadReflectionResY]
		push eax
		jmp HDReflectionBlurCodeCavePart2
		
	HDReflectionBlurCodeCavePart2 :
		call dword ptr ds : [ecx + 0x5C]
		jmp HDReflectionBlurCodeCaveExit
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

void __declspec(naked) RenderDistanceCodeCave()
{
	__asm {
		push ebx
		push 0x00
		mov ecx, edi
		cmp dword ptr ds : [esi + 0x04], 0x12 // Vehicle Reflection
		jnl RenderDistanceCodeCavePart2
		cmp dword ptr ds : [esi + 0x04], 0x03 // RVM
		je RenderDistanceCodeCavePart3
		jmp RenderDistanceCodeCaveExit

		RenderDistanceCodeCavePart2 :
		cmp dword ptr ds : [esi + 0x04], 0x03
		push 0x42C80000 // 100.0f
		jmp RenderDistanceCodeCaveExit2

		RenderDistanceCodeCavePart3 :
		push 0x43400000 // 192.0f
		jmp RenderDistanceCodeCaveExit2
	}
}

void Init()
{
	// Read values from .ini
	CIniReader iniReader("NFSMWHDReflections.ini");

	// Resolution
	ResolutionX = iniReader.ReadInteger("RESOLUTION", "ResolutionX", 1920);
	ResolutionY = iniReader.ReadInteger("RESOLUTION", "ResolutionY", 1080);
	RoadScale = iniReader.ReadFloat("RESOLUTION", "RoadScale", 1.0);
	VehicleScale = iniReader.ReadFloat("RESOLUTION", "VehicleScale", 1.0);
	MirrorScale = iniReader.ReadFloat("RESOLUTION", "MirrorScale", 1.0);

	// General
	HDReflections = iniReader.ReadInteger("GENERAL", "HDRoadReflections", 1);
	HDReflectionBlur = iniReader.ReadInteger("GENERAL", "HDReflectionBlur", 1);
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 2);
	GeometryFix = iniReader.ReadInteger("GENERAL", "GeometryFix", 1);
	RestoreDetails = iniReader.ReadInteger("GENERAL", "RestoreDetails", 1);
	RestoreShaders = iniReader.ReadInteger("GENERAL", "RestoreShaders", 1);
	OptimizeRenderDistance = iniReader.ReadInteger("GENERAL", "OptimizeRenderDistance", 1);

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
		injector::WriteMemory<uint32_t>(0x6CFC26, ResolutionX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BCDF5, ResolutionX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BD17D, ResolutionX * RoadScale, true);
		// Road Reflection Y
		injector::WriteMemory<uint32_t>(0x6BD184, ResolutionY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6CFC2D, ResolutionY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BCDEF, ResolutionY * RoadScale, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x8F8FF4, ResolutionY * VehicleScale, true);
		// RVM Reflection
		// Aspect ratio is based on NFSU2 because true aspect ratio is unknown
		injector::WriteMemory<uint32_t>(0x8F9008, ResolutionY * MirrorScale, true);
		injector::WriteMemory<uint32_t>(0x8F900C, (ResolutionY / 3) * MirrorScale, true);
	}

	if (ImproveReflectionLOD >= 1)
	{
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x6BFEBD, 0x00000000, true);
		// RVM LOD
		injector::WriteMemory<uint32_t>(0x6BFE58, 0x00000000, true);
		// Road Reflection LOD
		injector::WriteMemory<uint8_t>(0x4FAE9A, 0xEB, true);
		
		if (ImproveReflectionLOD >= 2)
		// Full RVM LOD
		injector::WriteMemory<uint8_t>(0x4FAEB0, 0xEB, true);
		// Full Road Reflection LOD
		if (ImproveReflectionLOD >= 2)
		injector::WriteMemory<uint8_t>(0x6BFF21, 0x06, true);
		if (ImproveReflectionLOD >= 2)
		injector::WriteMemory<uint8_t>(0x7293DB, 0x06, true);
	}

	// HDReflectionBlur
	{
		injector::MakeJMP(0x6BCE6E, HDReflectionBlurCodeCave, true);
	}

	if (GeometryFix)
	{
		injector::WriteMemory<uint8_t>(0x8FAE44, 0x00, true);
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

	if (OptimizeRenderDistance)
	{
		injector::MakeJMP(0x6E73A1, RenderDistanceCodeCave, true);
		injector::WriteMemory(0x6DB5C0, &SkyboxRenderDistace, true);
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