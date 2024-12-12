# AV_Mace Engine
Game engine using SDL2, glew, assimp, imgui, glm and devil, made by the Group 4 for the Motors subject for CITM

## Link to the repository

[![Github](https://static.vecteezy.com/system/resources/thumbnails/016/833/880/small_2x/github-logo-git-hub-icon-with-text-on-white-background-free-vector.jpg)](https://github.com/CITM-UPC/AV-Mace_Engine)


## Team members

- Joan Marques - [Link](https://github.com/joanmarquesbesses)
- Pol Celaya - [Link](https://github.com/pcelaya)
- Marc Avante - [Link](https://github.com/MarcoXAvante)

## Controls

- WASD - Move Camera
- Shift - Accelerate movement
- Mouse Wheel - Zoom in and out
- Right Click - Rotate Camera
- Left Click - Mouse Picking
- F - Focus current selected object
- Alt - Orbit selected object
- F1 - Activate/Desactivate Culling (and clear console + print what's been culled)

## Additional functionalities
- Drag and drop .fbx files
- Drag and drop .png and .dds files
- Menu that include links to our github and last release
- Viewport window to avoid mouse picking when changing parameters.
- Bounding Boxes and Parent Boxes for every model group.
- Resize window to your liking.
- Camera is an object and you can edit parameters.
- Added more geometric objects for creating.
- Can paint all face normals and vertex normals.
- Reparent and hierarchy drag and drop.
- Hierarchy pop up menu.
- Create empty game objects.
- Delete models from hierarchy.


## Additional comments
- Dropping a .fbx will create a gameobject.
- Dropping a .png or .dds with a selected gameobject will apply the texture or replace the old one. <-- Important
- We apply all the assimp transforms to the actual object and get rid of unnecessary groups that clouded the hierarchy.
- Components branch has a working Components interface:
- - Components are unique pointers
- - Components use polymorph list
- - Updated interface
- - Parent and owner are pointers
- - All is passed using a reference
- - However, because we finished doing this components part of the project too late before the delivery, we didn't merge it with main and left it aside to merge later. It works as intended.
- We have custom files that in theory save correctly but can't load properly yet.
- When loading the editor, depending on the parentbox, the camera places itself so all the object can be viewed.

## Improvements
- Improved the loading of different fbx
- Texture manager
- Shaders
- You can add component in the inspector
