#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "..\includes\injector\injector.hpp"
#include <cstdint>
#include "..\includes\IniReader.h"
#include <d3d9.h>


bool HDReflections, OldGPUCompatibility, GeometryFix, RestoreShaders, OptimizeRenderDistance, ExpandMemoryPool;
int  ImproveReflectionLOD, RestoreVisualTreatment, RestoreDetails;
float RoadScale, VehicleScale, MirrorScale, VehicleReflectionBrightness;
float SkyboxRenderDistance = 0.5f;
float RGBAmbient = 0.5f;
float RGBDiffuse = 0.75f;
float RGBSpecular = 0.0f;

int RoadResX = 320;
int RoadResY = 240;
int VehicleRes = 256;
int MirrorResX = 256;
int MirrorResY = 256;

DWORD RestoreFEReflectionCodeCaveExit = 0x6BD502;
DWORD VehicleReflectionCodeCaveExit = 0x6BD533;
DWORD HDReflectionBlurCodeCaveExit = 0x6BCE74;
DWORD RoadReflectionResX;
DWORD RoadReflectionResY;
DWORD TrafficLightRestorationCodeCaveExit = 0x6DE9F8;
DWORD TrafficLightFunctionJump = 0x507781;
DWORD sub_505E80 = 0x505E80;
DWORD RestoreDetailsCodeCaveExit = 0x6DE990;
DWORD sub_723FA0 = 0x723FA0;
DWORD sub_750B10 = 0x750B10;
DWORD sub_6E2F50 = 0x6E2F50;
DWORD sub_503D00 = 0x503D00;
DWORD EnableCarShadowCodeCaveExit = 0x74E843;
DWORD EnableCarShadowCodeCaveJump = 0x74E818;
DWORD EnableParticlesCodeCaveExit = 0x6DE9E8;
DWORD MirrorRGBCodeCaveExit = 0x6E7126;
DWORD RemoveRoadReflectionCodeCaveExit = 0x6D728F;
DWORD RenderDistanceCodeCaveExit = 0x6E73A6;
DWORD RenderDistanceCodeCaveExit2 = 0x6E73AB;
DWORD SkyboxRenderDistanceCodeCaveExit = 0x6DB5C4;
DWORD ReflShadowOpacityCodeCaveExit1 = 0x6E043C;
DWORD ReflShadowOpacityCodeCaveExit2 = 0x6E0445;
DWORD VehicleReflBrightnessBugFixCodeCaveExit = 0x6DA52F;
DWORD sub_7538D0 = 0x7538D0;
DWORD sub_6C00D0 = 0x6C00D0;
DWORD sub_6D04E0 = 0x6D04E0;
DWORD SunflareArray[50];
DWORD SunflareCodeCave1Exit = 0x6DB265;
DWORD SunflareCodeCave2Exit = 0x6E6DB2;
DWORD SunflareCodeCave3Exit = 0x6DB120;
DWORD LODBypassCodeCaveExit1 = 0x6BFE44;
DWORD LODBypassCodeCaveExit2 = 0x6BFE57;

// Value1, Value4, Value7, Value10 = Brightness
// Value2, Value3, Value5, Value6, Value8, Value9 = Overlay Size
float VisualTreatmentArray[] = {0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f, 0.25f, 0.0f, 0.0f};
DWORD VisualTreatmentEnablerCodeCave1_Exit = 0x6DE9FE;
DWORD sub_0x6DBEC0 = 0x6DBEC0;
DWORD sub_0x6DBFE0 = 0x6DBFE0;
DWORD sub_0x6D53D0 = 0x6D53D0;
DWORD VisualTreatmentEnablerCodeCave2_Exit = 0x6DBF3F;
DWORD VisualTreatmentEnablerCodeCave3_Exit = 0x6DBFB5;
DWORD VisualTreatmentEnablerCodeCave3_Exit2 = 0x6DBFC8;
DWORD sub_0x6D2CD0 = 0x6D2CD0;
DWORD VisualTreatmentEnablerCodeCave4_Exit = 0x6D5433;
DWORD VisualTreatmentEnablerCodeCave5_Exit = 0x6D54BD;
DWORD VisualTreatmentEnablerCodeCave6_Exit = 0x6D54A0;

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
		mov eax, 0x02 // Removes vehicle sunflare from RVM
		jmp TrafficLightFunctionJump
	}
}

void __declspec(naked) RestoreDetailsCodeCave()
{
	__asm {
		// world
		push 0x01
		push 0x919730
		lea ecx, dword ptr ds : [esp + 0xE8]
		call sub_723FA0
		call sub_6E2F50
		cmp byte ptr ds : [ImproveReflectionLOD],0x02
		jl RestoreDetailsCodeCave_NoDestroyableObjects
		// destroyable objects
		push 0x00
		push 0x919730
		call sub_7538D0
		add esp, 0x08

	RestoreDetailsCodeCave_NoDestroyableObjects:
		// car shadow
		push ebx
		push 0x00919730
		call sub_750B10
		call sub_6E2F50
		// sunflare
		push 0x919730
		lea ecx, dword ptr ds : [SunflareArray]
		call sub_6D04E0
		jmp RestoreDetailsCodeCaveExit
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
	__asm 
	{
		mov edi, dword ptr ds : [0x982A20]
		mov edi, dword ptr ds : [edi + 0x04]
		imul edi, edi, 0x70
		mov edi, dword ptr ds : [edi + 0x9195E4]
		cmp edi, 0x01
		je RemoveRoadReflectionCodeCave_IsMAIN
		
		mov edi, 0x00000000
		jmp RemoveRoadReflectionCodeCaveExit

	RemoveRoadReflectionCodeCave_IsMAIN:
		mov edi, dword ptr ds : [0x982A6C]
		jmp RemoveRoadReflectionCodeCaveExit
	}
}

void __declspec(naked) VehicleReflBrightnessCodeCave()
{
	__asm {
		cmp dword ptr ds : [0x925E90], 0x06
		jne VehicleReflBrightnessCodeCavePart3
		push edx
		mov edx, dword ptr ds : [0x982A20]
		mov edx, dword ptr ds : [edx + 0x04]
		imul edx, edx, 0x70
		mov edx, dword ptr ds : [edx + 0x9195E4]
		cmp edx, 0x11
		jng VehicleReflBrightnessCodeCavePart2
		cmp edx, 0x16
		jg VehicleReflBrightnessCodeCavePart2
		fld dword ptr ds : [esi + 0x20]
		fmul dword ptr ds : [RGBAmbient]
		fstp dword ptr ds : [esi + 0x20]
		fld dword ptr ds : [esi + 0x24]
		fmul dword ptr ds : [RGBAmbient]
		fstp dword ptr ds : [esi + 0x24]
		fld dword ptr ds : [esi + 0x28]
		fmul dword ptr ds : [RGBAmbient]
		fstp dword ptr ds : [esi + 0x28]
		fld dword ptr ds : [esi + 0x30]
		fmul dword ptr ds : [RGBDiffuse]
		fstp dword ptr ds : [esi + 0x30]
		fld dword ptr ds : [esi + 0x34]
		fmul dword ptr ds : [RGBDiffuse]
		fstp dword ptr ds : [esi + 0x34]
		fld dword ptr ds : [esi + 0x38]
		fmul dword ptr ds : [RGBDiffuse]
		fstp dword ptr ds : [esi + 0x38]
		fld dword ptr ds : [esi + 0x40]
		fmul dword ptr ds : [RGBSpecular]
		fstp dword ptr ds : [esi + 0x40]
		fld dword ptr ds : [esi + 0x44]
		fmul dword ptr ds : [RGBSpecular]
		fstp dword ptr ds : [esi + 0x44]
		fld dword ptr ds : [esi + 0x48]
		fmul dword ptr ds : [RGBSpecular]
		fstp dword ptr ds : [esi + 0x48]
	
	VehicleReflBrightnessCodeCavePart2:
		pop edx
	VehicleReflBrightnessCodeCavePart3:
		pop esi
		mov esp, ebp
		pop ebp
		ret 0004
	}
}

void __declspec(naked) VehicleReflBrightnessBugFixCodeCave()
{
	__asm {
		div dword ptr ds : [ecx * 0x04 + 0x8F9018]
		mov ecx, 0x00000003 // overwrites reflection setting
		jmp VehicleReflBrightnessBugFixCodeCaveExit
	}
}

void __declspec(naked) MirrorRGBCodeCave()
{
	__asm 
	{
		mov byte ptr ds : [eax], 0xFF // Alpha: 255
		mov byte ptr ds : [eax - 0x03], 0x80 // Red: 128
		mov byte ptr ds : [eax - 0x02], 0x80 // Green: 128
		mov byte ptr ds : [eax - 0x01], 0x80 // Blue: 128
		add eax, 0x04
		jmp MirrorRGBCodeCaveExit
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

	RenderDistanceCodeCavePart2:
		cmp dword ptr ds : [esi + 0x04], 0x03
		push 0x42C80000 // 100.0f
		jmp RenderDistanceCodeCaveExit2

	RenderDistanceCodeCavePart3:
		push 0x43400000 // 192.0f
		jmp RenderDistanceCodeCaveExit2
	}
}

void __declspec(naked) SkyboxRenderDistanceCodeCave()
{
	__asm {
		mov eax, dword ptr ds : [0x982A20]
		mov eax, dword ptr ds : [eax + 0x04]
		imul eax, eax, 0x70
		mov eax, dword ptr ds : [eax + 0x9195E4]

		cmp eax, 0x12 // checks for vehicle reflections
		jnl SkyboxRenderDistanceCodeCavePart2
		cmp eax, 0x03 // checks for rearview mirror
		je SkyboxRenderDistanceCodeCavePart2
		fld dword ptr ds : [0x8F935C]
		jmp SkyboxRenderDistanceCodeCaveExit

	SkyboxRenderDistanceCodeCavePart2:
		fld dword ptr ds : [SkyboxRenderDistance]
		jmp SkyboxRenderDistanceCodeCaveExit
	}
}

void __declspec(naked) ReflShadowOpacityCodeCave()
{
	__asm {
		mov eax, dword ptr ds : [0x982A20]
		cmp dword ptr ds : [eax + 0x04], 0x01
		fld dword ptr ds : [0x89096C]
		jne	ReflShadowOpacityCodeCavePart2
		jmp ReflShadowOpacityCodeCaveExit1
	
	ReflShadowOpacityCodeCavePart2:
		mov eax, dword ptr ds : [esi + 0x40]
		jmp ReflShadowOpacityCodeCaveExit2
	}
}

void __declspec(naked) SunflareCodeCave1()
{
	__asm {
		call sub_6C00D0
		lea ecx, dword ptr ds : [SunflareArray]
		call sub_6C00D0
		jmp SunflareCodeCave1Exit
	}
}

void __declspec(naked) SunflareCodeCave2()
{
	__asm {
		call sub_6C00D0
		lea ecx, dword ptr ds : [SunflareArray]
		call sub_6C00D0
		jmp SunflareCodeCave2Exit
	}
}

void __declspec(naked) SunflareCodeCave3()
{
	__asm {
		lea esi, dword ptr ds : [SunflareArray]
		add esi, 0x24
		call SunflareCodeCave3Sub
		mov esi, 0x93E03C
		call SunflareCodeCave3Sub
		jmp SunflareCodeCave3Exit

	SunflareCodeCave3Sub:
		mov edi, esi
		add edi, 0x10
	
	SunflareCodeCave3SubConditional2:
		mov eax, dword ptr ds : [esi]
		mov edx, dword ptr ds : [eax]
		test eax,eax
		je SunflareCodeCave3SubConditional1
		push eax
		call dword ptr ds : [edx + 0x08]
		mov dword ptr ds : [esi], 0x00000000
	
	SunflareCodeCave3SubConditional1:
		add esi, 0x04
		cmp esi, edi
		jl SunflareCodeCave3SubConditional2
		ret
	}
}

void __declspec(naked) LODBypassCodeCave()
{
	__asm {
		cmp esi, 0x919730
		je LODBypassCodeCavePart2
		mov ecx, [0x982AF4]
		test ecx, ecx
		jne LODBypassCodeCavePart2
		xor ecx, ecx
		cmp ebx, 0x03
		jmp LODBypassCodeCaveExit1

	LODBypassCodeCavePart2:
		jmp LODBypassCodeCaveExit2
	}
}

void __declspec(naked) VisualTreatmentEnablerCodeCave1()
{
	__asm 
	{
		mov ecx, dword ptr ds : [0x982AF0]
		cmp ecx, 0x00 // prevents crash
		je VisualTreatmentEnablerCodeCave1_None
		call sub_0x6DBEC0
		call sub_0x6DBFE0
		movzx ecx, byte ptr ds : [0x901828] // checks if visual treatment is enabled
		push ecx
		mov ecx, dword ptr ds : [0x982AF0]
		call sub_0x6D53D0

		VisualTreatmentEnablerCodeCave1_None:
		mov ecx, dword ptr ds : [0x93DEBC]
		jmp VisualTreatmentEnablerCodeCave1_Exit
	}
}

void __declspec(naked) VisualTreatmentEnablerCodeCave2()
{
	__asm
	{
		mov ecx, dword ptr ds : [0x982A20]
		mov ecx, dword ptr ds : [ecx + 0x04]
		imul ecx, ecx, 0x70
		mov ecx, dword ptr ds : [ecx + 0x9195E4]
		cmp ecx, 0x01 // checks if main render
		je VisualTreatmentEnablerCodeCave2_IsMAIN

		// RVM
		mov ecx, dword ptr ds : [0x982A80]
		mov eax, dword ptr ds : [0x982BDC]
		mov edx, dword ptr ds : [eax]
		push 0x00
		push 0x00
		push ecx
		mov ecx, dword ptr ds : [0x982A84]
		jmp VisualTreatmentEnablerCodeCave2_Exit

		// MAIN
		VisualTreatmentEnablerCodeCave2_IsMAIN :
		mov ecx, dword ptr ds : [0x93DE60]
		mov eax, dword ptr ds : [0x982BDC]
		mov edx, dword ptr ds : [eax]
		push 0x00
		push 0x00
		push ecx
		mov ecx, dword ptr ds : [0x982A28]
		jmp VisualTreatmentEnablerCodeCave2_Exit
	}
}

void __declspec(naked) VisualTreatmentEnablerCodeCave3()
{
	__asm
	{
		mov eax, dword ptr ds : [0x982A20]
		mov eax, dword ptr ds : [eax + 0x04]
		imul eax, eax, 0x70
		mov eax, dword ptr ds : [eax+0x9195E4]
		cmp eax, 0x01 // checks if main render
		je VisualTreatmentEnablerCodeCave3_IsMAIN

		// RVM
		push 0x02
		cmp byte ptr [RestoreVisualTreatment], 0x01
		jne VisualTreatmentEnablerCodeCave3_AA_Smoothing
		lea eax, dword ptr ds : [VisualTreatmentArray]
		push eax
		jmp VisualTreatmentEnablerCodeCave3Part2
	
	VisualTreatmentEnablerCodeCave3_AA_Smoothing:
		push 0x8F9560
	
	VisualTreatmentEnablerCodeCave3Part2:
		push 0x04
		mov eax, dword ptr ds : [0x982A7C]
		mov ecx, dword ptr ds : [0x982A80]
		push eax
		push ecx
		call sub_0x6D2CD0
		add esp, 0x14
		mov al, byte ptr ds : [0x982B26]
		jmp VisualTreatmentEnablerCodeCave3_NotOverBright

		// MAIN
	VisualTreatmentEnablerCodeCave3_IsMAIN: 
		push 0x02
		push 0x8F9560
		push 0x04
		mov eax, dword ptr ds : [0x93DEF8]
		mov ecx, dword ptr ds : [0x982990]
		push eax
		push ecx
		call sub_0x6D2CD0
		add esp, 0x14
		mov al, byte ptr ds : [0x982B26]
		test al, al
		je VisualTreatmentEnablerCodeCave3_NotOverBright
		mov edx, dword ptr ds : [0x93DEF8]
		mov eax, dword ptr ds : [0x982998]
		jmp VisualTreatmentEnablerCodeCave3_Exit
	
	VisualTreatmentEnablerCodeCave3_NotOverBright:
		jmp VisualTreatmentEnablerCodeCave3_Exit2
	}
}

void __declspec(naked) VisualTreatmentEnablerCodeCave4()
{
	__asm
	{
		mov edi, dword ptr ds : [0x982A20]
		mov edi, dword ptr ds : [edi + 0x04]
		imul edi, edi, 0x70
		mov edi, dword ptr ds : [edi + 0x9195E4]
		cmp edi, 0x01 // checks if main render
		je VisualTreatmentEnablerCodeCave4_IsMAIN

		// RVM
		mov edi, dword ptr ds : [0x982A84]
		jmp VisualTreatmentEnablerCodeCave4_Exit


		// MAIN
	VisualTreatmentEnablerCodeCave4_IsMAIN:
		mov edi, dword ptr ds : [0x982A28]
		jmp VisualTreatmentEnablerCodeCave4_Exit
	}
}

void __declspec(naked) VisualTreatmentEnablerCodeCave5()
{
	__asm
	{
		mov edx, dword ptr ds : [0x982A20]
		mov edx, dword ptr ds : [edx + 0x04]
		imul edx, edx, 0x70
		mov edx, dword ptr ds : [edx + 0x9195E4]
		cmp edx, 0x01 // checks if main render
		je VisualTreatmentEnablerCodeCave5_IsMAIN

		// RVM
		mov edx, dword ptr ds : [0x982A7C]
		jmp VisualTreatmentEnablerCodeCave5_Exit


		// MAIN
	VisualTreatmentEnablerCodeCave5_IsMAIN:
		mov edx, dword ptr ds : [0x982964]
		jmp VisualTreatmentEnablerCodeCave5_Exit
	}
}

void __declspec(naked) VisualTreatmentEnablerCodeCave6()
{
	__asm
	{
		push edx
		mov edx, dword ptr ds : [0x982A20]
		mov edx, dword ptr ds : [edx + 0x04]
		imul edx, edx, 0x70
		mov edx, dword ptr ds : [edx + 0x9195E4]
		cmp edx, 0x01 // checks if main render
		je VisualTreatmentEnablerCodeCave6_IsMAIN

		// RVM
		pop edx
		mov edx, dword ptr ds : [0x982A7C]
		jmp VisualTreatmentEnablerCodeCave6_Exit


		// MAIN
	VisualTreatmentEnablerCodeCave6_IsMAIN:
		pop edx
		mov edx, dword ptr ds : [edx * 0x4 + 0x93DEF8]
		jmp VisualTreatmentEnablerCodeCave6_Exit
	}
}

void Init()
{

	// Read values from .ini
	CIniReader iniReader("NFSMWHDReflections.ini");

	// Resolution
	HDReflections = iniReader.ReadInteger("RESOLUTION", "HDReflections", 1);
	OldGPUCompatibility = iniReader.ReadInteger("RESOLUTION", "OldGPUCompatibility", 0);
	VehicleScale = iniReader.ReadFloat("RESOLUTION", "VehicleScale", 1.0f);
	RoadScale = iniReader.ReadFloat("RESOLUTION", "RoadScale", 1.0f);
	MirrorScale = iniReader.ReadFloat("RESOLUTION", "MirrorScale", 1.0f);

	// General
	ImproveReflectionLOD = iniReader.ReadInteger("GENERAL", "ImproveReflectionLOD", 2);
	GeometryFix = iniReader.ReadInteger("GENERAL", "GeometryFix", 1);
	RestoreShaders = iniReader.ReadInteger("GENERAL", "RestoreShaders", 1);
	RestoreVisualTreatment = iniReader.ReadInteger("GENERAL", "RestoreVisualTreatment", 1);
	RestoreDetails = iniReader.ReadInteger("GENERAL", "RestoreDetails", 1);
	OptimizeRenderDistance = iniReader.ReadInteger("GENERAL", "OptimizeRenderDistance", 1);
	VehicleReflectionBrightness = iniReader.ReadFloat("GENERAL", "VehicleReflectionBrightness", 1.0f);

	// Extra
	ExpandMemoryPool = iniReader.ReadInteger("EXTRA", "ExpandMemoryPool", 1);

	if (HDReflections)
	{
		RoadResX = GetSystemMetrics(SM_CXSCREEN);
		RoadResY = GetSystemMetrics(SM_CYSCREEN);
		VehicleRes = GetSystemMetrics(SM_CYSCREEN);
		MirrorResX = GetSystemMetrics(SM_CYSCREEN);
		MirrorResY = GetSystemMetrics(SM_CYSCREEN) / 3;
	}

	// Writes Resolution Values
	{
		injector::MakeJMP(0x6BD4FC, RestoreFEReflectionCodeCave, true);
		injector::MakeJMP(0x6BD52D, VehicleReflectionCodeCave, true);
		// Road Reflection X
		injector::WriteMemory<uint32_t>(0x6CFC26, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BCDF5, RoadResX * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BD17D, RoadResX * RoadScale, true);
		// Road Reflection Y
		injector::WriteMemory<uint32_t>(0x6BD184, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6CFC2D, RoadResY * RoadScale, true);
		injector::WriteMemory<uint32_t>(0x6BCDEF, RoadResY * RoadScale, true);
		// Rearview Mirror
		// Aspect ratio is based on NFSU2 because true aspect ratio is unknown
		injector::WriteMemory<uint32_t>(0x8F9008, MirrorResX * MirrorScale, true);
		injector::WriteMemory<uint32_t>(0x8F900C, MirrorResY * MirrorScale, true);
		// Vehicle Reflection
		injector::WriteMemory<uint32_t>(0x8F8FF4, VehicleRes * VehicleScale, true);
		
		if (OldGPUCompatibility)
		{
			// Rounds vehicle resolution down to the nearest power of two
			static int VehicleRes_POT = (VehicleRes * VehicleScale);
			VehicleRes_POT--;
			VehicleRes_POT |= VehicleRes_POT >> 1;
			VehicleRes_POT |= VehicleRes_POT >> 2;
			VehicleRes_POT |= VehicleRes_POT >> 4;
			VehicleRes_POT |= VehicleRes_POT >> 8;
			VehicleRes_POT |= VehicleRes_POT >> 16;
			VehicleRes_POT++;
			injector::WriteMemory<uint32_t>(0x8F8FF4, VehicleRes_POT, true);
		}
	}

	if (ImproveReflectionLOD >= 1)
	{
		// Vehicle Reflection LOD
		injector::WriteMemory<uint32_t>(0x6BFEBD, 0x00006002, true);
		// RVM LOD
		injector::WriteMemory<uint32_t>(0x6BFE58, 0x00006002, true);
		// Bypasses widescreen fix LOD changes
		injector::MakeJMP(0x6BFE3D, LODBypassCodeCave, true);
		injector::MakeNOP(0x6BFE42, 1, true);
		// Road Reflection LOD
		injector::WriteMemory<uint8_t>(0x4FAE9A, 0xEB, true);
		
		if (ImproveReflectionLOD >= 2)
		// Full RVM LOD
		injector::WriteMemory<uint8_t>(0x4FAEB0, 0xEB, true);
		if (ImproveReflectionLOD >= 2)
		// Full Road Reflection LOD
		if (ImproveReflectionLOD >= 2)
		injector::WriteMemory<uint8_t>(0x6BFF21, 0x06, true);
		if (ImproveReflectionLOD >= 2)
		injector::WriteMemory<uint8_t>(0x7293DB, 0x06, true);
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
		// Adds missing sunflare to the rearview mirror
		// Adds missing destroyable objects to the rearview mirror
		injector::MakeJMP(0x6DB260, SunflareCodeCave1, true);
		injector::MakeJMP(0x6E6DAD, SunflareCodeCave2, true);
		injector::MakeJMP(0x6DB0FE, SunflareCodeCave3, true);
		injector::MakeJMP(0x6DE96D, RestoreDetailsCodeCave, true);
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
		injector::WriteMemory<uint8_t>(0x6DABD6, 0xEB, true);
		// Removes road reflection from other reflections
		injector::MakeJMP(0x6D7289, RemoveRoadReflectionCodeCave, true);
		// Removes dynamic shadows from reflections
		injector::MakeJMP(0x6E0436, ReflShadowOpacityCodeCave, true);
		// Corrects vehicle reflection world brightness
		injector::MakeJMP(0x7696A4, VehicleReflBrightnessCodeCave, true);
		// Fixes flashing when low reflection update rate is used
		injector::MakeJMP(0x6DA528, VehicleReflBrightnessBugFixCodeCave, true);
		// Corrects mirror brightness
		injector::MakeJMP(0x6E7120, MirrorRGBCodeCave, true);
	}

	if (RestoreVisualTreatment)
	{
		// Enables Visual Treatment for Mirror
		injector::MakeJMP(0x6DE9F8, VisualTreatmentEnablerCodeCave1, true);
		injector::MakeJMP(0x6DBF27, VisualTreatmentEnablerCodeCave2, true);
		injector::MakeJMP(0x6DBF83, VisualTreatmentEnablerCodeCave3, true);
		injector::MakeJMP(0x6D542D, VisualTreatmentEnablerCodeCave4, true);
		injector::MakeJMP(0x6D54B7, VisualTreatmentEnablerCodeCave5, true);
		injector::MakeJMP(0x6D5499, VisualTreatmentEnablerCodeCave6, true);
	}

	if (OptimizeRenderDistance)
	{
		injector::MakeJMP(0x6E73A1, RenderDistanceCodeCave, true);
		injector::MakeNOP(0x6DB5AB, 6, true);
		injector::MakeJMP(0x6DB5BE, SkyboxRenderDistanceCodeCave, true);
	}

	if (VehicleReflectionBrightness)
	{
		static float VehicleReflectionIntensity1 = (0.5f * VehicleReflectionBrightness);
		static float VehicleReflectionIntensity2 = (1.0f * VehicleReflectionBrightness);
		injector::WriteMemory(0x6C77CE, &VehicleReflectionIntensity1, true);
		injector::WriteMemory(0x6C77D6, &VehicleReflectionIntensity2, true);
	}

	if (ExpandMemoryPool)
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