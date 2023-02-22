# GameEngine3D - Mingyuan Chen
This is the assignment content of the course EAE6320, a project created based on Professor Tony's basic 3D engine template. My main work is to optimize the engine file based on the template, provide a good interface for users to use, and at the same time need to consider the compatibility of OpenGl and D3D graphics libraries and X86 and X64 platforms. And design a new system (animation system, collision system, audio system, etc.) at the end of the course to complete this course.  

***Update(02/21/2023):***   
The final project has been completed. The newly supports MAYA model import engine and analyzes and displayed; the new Mesh file is compiled into binary file output (narrowing file size, speeding up read speed); Add an Audio System system. This system is based on MCI Command Strings. Provided the interface:  
```Play Audio (Can Be Set to loop);```  
```Pause the audio;```  
```Resume playback;```  
```Play Independently (The Same Audio Can Be Played Multiple Times at the Same Time);```  
```Set the volume level;```  
```Check if the audio is playing;```  
Also, I already complete my final game project. Combine with Shankar's Story System and my Audio System to make a small story-driven game. It needs IMGUI to present some pictures on the application screen (IMGUI: https://github.com/ocornut/imgui).



## Getting started

Clone this project, then open ChenMingyuan.sln. Build the project to ensure that both x86 and x64 can be successfully built.
Looking at the Solution Explorer window, there are five folders for the solution:  

1. Engine: It contains multiple projects, and references and dependencies have been set between projects.  

2. ExampleGame: The template's game layer project folder can be ignored and used to create my own MyGame folder.  

3. External: The engine will use Lua as the configuration file, and parsing scheme of Mesh and Shader, so External contains the Lua library; there are also Mcpp and OpenGlExtensions.  

4. MyGame: The project folder I created to serve the game layer, which contains the BuildMyGameAssets and MyGame projects.  

5. Tools: It is used for projects of configuration files such as BuildMesh and Shader. Its functions are mainly to process some data and copy files (copy to the output folder after Build, which is used for game data).  

6. FinalGameProject: The project includes my final game. Use IMGUI to present pictures and follow my Audio System to make the BGM.  


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

***Update(02/21/2023):***   
The Audio System Usage and other stuff are under Roadmap. Please check:  
Maya integration: https://docs.google.com/document/d/1jwN5nu3mrMgjCFf8V6ASwxjLvnuEJyrRBqiPXkmdtHQ/edit?usp=sharing  
Binary File Output: https://docs.google.com/document/d/1QaVXoMhV7YYgbpqhLImKLNcw9A54TfaP0FRh2HEgBQ0/edit?usp=sharing  
Audio System Proposal: https://docs.google.com/document/d/1Bw2K6nJI3luwJcojevRZQZF-G2NmvACwZVqzdY5QoG4/edit?usp=sharing  
Audio System Usage: https://docs.google.com/document/d/13Nue7xKizT1V5U97yfAeUCH26f6xF2DZ7ppZH4XinmQ/edit?usp=sharing  
Final Project Proposal: https://docs.google.com/document/d/1T2G4dWd9V_USri3vMsC9Btuem8MtpiKe71Bbt5lFsjI/edit?usp=sharing  
Final Project Write-Up: https://docs.google.com/document/d/1tOHZBdACA1aNM4xNE1uhg6tb5ROQfblMg-exc_iy5vs/edit?usp=sharing   


## Visuals
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/196015987-54a95557-5884-4e84-9d17-c1d6b8e9b4d2.gif"/></div>
<div align="center"> Camera movement </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/196016514-b5fd6d9f-5761-44c6-abcd-a55f0378b0d5.gif"/></div>
<div align="center"> GameObject movement </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/196016515-97ee19a9-0727-4351-88c9-bd6740d2dc6f.gif"/></div>
<div align="center"> Shader change </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/196016516-3ca2f817-acba-4307-aa9c-cffae0a3af80.gif"/></div>
<div align="center"> Object delete </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/220546317-89469998-9d72-4eec-b71d-191e4fcbd9e3.png"/></div>
<div align="center"> Game running screenshot </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/220546463-f855f3c2-5edc-4548-b7bb-69dd93859190.png"/></div>
<div align="center"> Three model screenshot </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/220547147-80f42b76-8091-48f8-9423-86568950cb0b.png"/></div>
<div align="center"> Final Game Project Screenshot_1 </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/220547149-8bcc11a4-8015-4372-9eb8-94970281b368.png"/></div>
<div align="center"> Final Game Project Screenshot_2 </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/220547151-1bde39c2-1c7a-4953-9b33-881e69d8793b.png"/></div>
<div align="center"> Final Game Project Screenshot_3 </div>  
<div align=center><img src ="https://user-images.githubusercontent.com/58096097/220547153-48bd2a81-4623-49a8-ab90-d0daf8838dac.png"/></div>
<div align="center"> Final Game Project Screenshot_4 </div>  




## Roadmap
Here is a brief record of my steps to complete this project and the Write-up after each task was completed.  
01. Setup project: https://docs.google.com/document/d/1YN8HyhwSQXbwUU7JNtHJv_sArD5YgTDbuh5AaE0m8EQ/edit?usp=sharing  
02. Mesh representation: https://docs.google.com/document/d/1DiRrG-dJIWxnUDd08RRZ6HbffmHbObVnIJdvp26Pn20/edit?usp=sharing  
03. Graphics system completely platform-independent: https://docs.google.com/document/d/1JSovbU6NGR8QS-8mKNRBKN3OAUPy_mBFkIOkXFP_nL8/edit?usp=sharing  
04. Mesh&Effect representation reference count/Interface to submit background color: https://docs.google.com/document/d/1sBaVDOjNMYuWDVzXO5seaJzaeBTEeHzMTCcsv0_S4xs/edit?usp=sharing  
05. Camera representation/Input system implementation: https://docs.google.com/document/d/1SU_LEj1efjUvZmLO9mVoeNiQUj6eFudT0bBhsBNiR_4/edit?usp=sharing  
06. Lua config file for mesh: https://docs.google.com/document/d/1EO1n5JgrJuRKOTjonPwBglcmwTtGqe3vlqC8z6pYA_g/edit?usp=sharing  
07. Maya integration: https://docs.google.com/document/d/1jwN5nu3mrMgjCFf8V6ASwxjLvnuEJyrRBqiPXkmdtHQ/edit?usp=sharing  
08. Binary File Output: https://docs.google.com/document/d/1QaVXoMhV7YYgbpqhLImKLNcw9A54TfaP0FRh2HEgBQ0/edit?usp=sharing  
09. Audio System Proposal: https://docs.google.com/document/d/1Bw2K6nJI3luwJcojevRZQZF-G2NmvACwZVqzdY5QoG4/edit?usp=sharing  
10. Audio System Usage: https://docs.google.com/document/d/13Nue7xKizT1V5U97yfAeUCH26f6xF2DZ7ppZH4XinmQ/edit?usp=sharing  
11. Final Project Proposal: https://docs.google.com/document/d/1T2G4dWd9V_USri3vMsC9Btuem8MtpiKe71Bbt5lFsjI/edit?usp=sharing  
12. Final Project Write-Up: https://docs.google.com/document/d/1tOHZBdACA1aNM4xNE1uhg6tb5ROQfblMg-exc_iy5vs/edit?usp=sharing   

## License
# Lua:
Copyright ?1994?019 Lua.org, PUC-Rio.  
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:  

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  


## Project status
This Project is already finished. But maybe I'll keep updating it later when I have some new stuff I want to do.
