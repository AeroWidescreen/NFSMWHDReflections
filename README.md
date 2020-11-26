# NFSMW HD Reflections

## How To Use  
1. Download and extract [NFSMWHDReflections.zip](https://github.com/AeroWidescreen/NFSMWHDReflections/releases).  
2. Move the scripts folder to the game's install folder.  
3. Download and extract [Ultimate-ASI-Loader.zip](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases).  
4. Move dinput8.dll to the game's install folder.  

## Features
- HDReflections : Resolution will be increased based on desktop resolution.
- OldGPUCompatibility : Vehicle reflections will be rounded to the nearest power of two.
- VehicleScale : Controls the resolution multiplier of vehicle reflections.
- RoadScale : Controls the resolution multiplier of road reflections.
- MirrorScale : Controls the resolution multiplier of the rearview mirror.
- ImproveReflectionLOD : Increases the level of detail used in reflections.  
- RestoreVisualTreatment : Adds visual treatment to the rearview mirror.  
- RestoreShaders : Adds shader effects to reflections and enables barrier animations.
- RestoreDetails : Adds lights, car shadows, and particle effects to the rearview mirror.   
- OptimizeRenderDistance : Reduces excessive render distance of reflections to improve performance. 
- GeometryFix : Restores missing geometry, such as frontend reflections and world detail.  
- VehicleReflectionBrightness : Controls the intensity of vehicle reflections.  
- ExpandMemoryPool : Solves an issue that caused objects to disappear when too many polygons were present.  

## FAQ
Q: What are the original resolution values?  
A: It varies from game to game. For this one it's Road: 320x240, Vehicle: 256x256, and Mirror 256x256.  

Q: Why haven't you added RealisticChrome yet?  
A: It requires custom shaders that I haven't found a reliable way to implement. Ideally, I'm waiting for the next release of osdever's shaders.  

Q: Why does the mirror look blurrier than usual?  
A: This is due to how visual treatment works; it acts as a primitive form of anti-aliasing. You can get a sharper image with RestoreVisualTreatment = 2.  

Q: Why did you remove the MirrorTint feature?  
A: It's been replaced with RestoreVisualTreatment. The color of the visual treatment can be changed using Extra Options.  

Q: Is this compatible with other mods?  
A: I can't make any guarantees. It's been tested with Widescreen Fix, MW Fixes, Extra Options, Unlimiter, HUD Adapter, and HUD Resizer. All texture mods and car mods should work.  

Q: What are the system requirements?  
A:  It's difficult to say because it depends on your resolution, settings, and other enhancements being used. For 1080p, anything with a GTX 1060 / RX 480, Core i5 / Ryzen 3 should be able to easily run this.  

## Credits
[Aero_](https://github.com/AeroWidescreen) : Creator of NFSMW HD Reflections.  
[ExOpts Team](https://github.com/ExOptsTeam/) : The project is based on Extra Options and it wouldn't exist without them.

## Thanks
[osdever](https://github.com/osdeverr) : Providing the information needed for RestoreShaders.  
[elaymm4](https://github.com/elaymm4) : Testing and feedback.  
