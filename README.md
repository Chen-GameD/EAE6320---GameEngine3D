# GameEngine3D - Mingyuan Chen
This is the assignment content of the course EAE6320, a project created based on Professor Tony's basic 3D engine template. My main work is to optimize the engine file based on the template, provide a good interface for users to use, and at the same time need to consider the compatibility of OpenGl and D3D graphics libraries and X86 and X64 platforms. And design a new system (animation system, collision system, audio system, etc.) at the end of the course to complete this course.


## Getting started

Clone this project, then open ChenMingyuan.sln. Build the project to ensure that both x86 and x64 can be successfully built.
Looking at the Solution Explorer window, there are five folders for the solution:  

1. Engine: It contains multiple projects, and references and dependencies have been set between projects.  

2. ExampleGame: The template's game layer project folder can be ignored and used to create my own MyGame folder.  

3. External: The engine will use Lua as the configuration file, and parsing scheme of Mesh and Shader, so External contains the Lua library; there are also Mcpp and OpenGlExtensions.  

4. MyGame: The project folder I created to serve the game layer, which contains the BuildMyGameAssets and MyGame projects.  

5. Tools: It is used for projects of configuration files such as BuildMesh and Shader. Its functions are mainly to process some data and copy files (copy to the output folder after Build, which is used for game data).  


## Description
This project can perform some basic operations of the game engine:  
1. You can create your Mesh file and then provide it to the engine for rendering at the game layer (the file storage path is: ```MyGame_/Content/Meshes```). Before submitting, you need to update the AssetsToBuild.lua file and add the path for the new file;  

2. You can create your Shader file and then submit the game layer to the engine for binding (the file storage path is: ```MyGame_/Content/Shaders```). Before submitting, you need to update the AssetsToBuild.lua file and add the path for the new file; 

3. The engine provides an interface to submit these new files to the engine and perform rendering:  
```C++
eae6320::Graphics::cEffect::CreateEffect(cEffect*& o_effect, const char* i_vertexShaderAddress, const char* i_fragmentShaderAddress);  
eae6320::Graphics::cMesh::CreateMeshWithLuaFile(cMesh*& o_mesh, const char* const i_path);  
```

4. The engine provides an interface to change the camera position and background color:  
```C++
eae6320::Graphics::SubmitCamera(Camera& i_camera, const float i_elapsedSecondCount_sinceLastSimulationUpdate);  
eae6320::Graphics::SubmitBackBufferColor(const float r, const float g, const float b, const float a);  
```

5. Add the content of the key input to ```eae6320::cMyGame::UpdateSimulationBasedOnInput()``` to update the game operation logic; 

6. Add the logic of game physics update to ```eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)``` for automatic physics update;  

7. Note:  
```C++
struct GameObjectData  
{  
    eae6320::Graphics::cMesh* m_Mesh = nullptr;  
    eae6320::Graphics::cEffect* m_Effect = nullptr;  
    //rigidbody  
    eae6320::Physics::sRigidBodyState m_RigidBodyState;  
};  
```
It is a structure I temporarily created to store Game Object data, which contains mesh information, shader information, and physical information;  

8. Note:  
```C++
struct Camera  
{  
    Physics::sRigidBodyState m_RigidBodyState;  
    float m_verticalFieldOfView_inRadians = Math::ConvertDegreesToRadians(45);  
    float m_aspectRatio = 1.0f;  
    float m_z_nearPlane = 0.1f;  
    float m_z_farPlane = 100;  
};  
```
It is a structure I use to store camera information; changing this data and submitting it to the engine using the interface can change the camera's position, depth, rotation, etc.  


## Visuals
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/196015987-54a95557-5884-4e84-9d17-c1d6b8e9b4d2.gif"/></div>
<div align="center"> Camera movement </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/196016514-b5fd6d9f-5761-44c6-abcd-a55f0378b0d5.gif"/></div>
<div align="center"> GameObject movement </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/196016515-97ee19a9-0727-4351-88c9-bd6740d2dc6f.gif"/></div>
<div align="center"> Shader change </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/196016516-3ca2f817-acba-4307-aa9c-cffae0a3af80.gif"/></div>
<div align="center"> Object delete </div>  

## Roadmap
Here is a brief record of my steps to complete this project and the Write-up after each task was completed.  
01. Setup project: https://docs.google.com/document/d/1YN8HyhwSQXbwUU7JNtHJv_sArD5YgTDbuh5AaE0m8EQ/edit?usp=sharing  
02. Mesh representation: https://docs.google.com/document/d/1DiRrG-dJIWxnUDd08RRZ6HbffmHbObVnIJdvp26Pn20/edit?usp=sharing  
03. Graphics system completely platform-independent: https://docs.google.com/document/d/1JSovbU6NGR8QS-8mKNRBKN3OAUPy_mBFkIOkXFP_nL8/edit?usp=sharing  
04. Mesh&Effect representation reference count/Interface to submit background color: https://docs.google.com/document/d/1sBaVDOjNMYuWDVzXO5seaJzaeBTEeHzMTCcsv0_S4xs/edit?usp=sharing  
05. Camera representation/Input system implementation: https://docs.google.com/document/d/1SU_LEj1efjUvZmLO9mVoeNiQUj6eFudT0bBhsBNiR_4/edit?usp=sharing  
06. Lua config file for mesh: https://docs.google.com/document/d/1EO1n5JgrJuRKOTjonPwBglcmwTtGqe3vlqC8z6pYA_g/edit?usp=sharing  

## License
# Lua:
Copyright ?1994?019 Lua.org, PUC-Rio.  
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:  

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  


## Project status
This project is under development. . .   
