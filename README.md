# SceneGL - Sam Pearson-Smith
### C++ | SDL2 | OpenGL

## Summary
A simple OpenGL rendering engine for experimenting with graphics and game engine features.

Goals:
    ## Tasks

    ### High Level Operation
    - [ ] Create ISystem for systems to inherit
    - [ ] Implement each system to initialise, start, update, stop, shutdown
    - [ ] Create sim abstraction to encapsulate all game/sim runtime systems & scenes
    - [ ] Create simulation to be able to run given simulation, configured at runtime 

    ### Rendering
    - [x] Implement OpenGL rasterization
    - [x] Create graphics layer interface to abstract OpenGL implementation
    - [x] Support lighting
    - [x] Implement Point Lights
    - [x] Implement Directional Lights
    - [ ] Implement Spot Lights
    - [ ] Support materials
    - [ ] Support textures
    - [ ] Wireframe rendering
    - [ ] Impl shader loader as asset_loader
    - [ ] Create renderer interface
    - [ ] Use opengl calls directly in OpenGL rasterizer
    - [ ] Remove gl_interface

    ### Camera
    - [x] Support cameras
    - [x] Support camera movement
    - [ ] Render all cameras in scene
    - [ ] Show camera preview
    - [ ] Show camera depth preview
    - [ ] Show camera stencil preview

    ### Physics
    - [ ] Implement AABB collision detection
    - [ ] Implement Sphere collision detection
    - [ ] Implement RigidBody
    - [ ] Implement collision reponse/fixing
    - [ ] Implement raycasting support
    - [ ] Draw physics

    ### Entity/Components
    - [x] Implement entities
    - [x] Implement components
    - [x] Implement entity-component engine
    - [ ] Implement scene tree
    - [x] Support empty enities
    - [x] Pass as references not pointers
    - [x] Support build pattern creation
    - [x] Children in JSON
    - [x] Ensure all components follow single responsibility
    - [x] Implement using entt
    - [x] Use smart pointers
    - [ ] Implement gizmos, render all component types
    - [ ] 

    ### Asset Manager
    - [x] Implement asset manager
    - [x] Use smart pointers
    - [x] Implement generic, centralise all assets
    - [x] Store each asset type in its own array

    ### Asset Loaders
    - [ ] Support loading and saving
    - [ ] Support json files
    - [ ] Support audio files
    - [ ] Support image files
    - [ ] Support text files
    - [x] Support shader files
    - [x] Support 3D model files
    - [ ] Support animation files

    ### Editor Camera
    - [x] Implement editor camera
    - [ ] Inherit from general camera

    ### Shaders
    - [ ] Live shader editing

    ### Scene
    - [ ] Implement scene manager
    - [ ] Allow multiple scene preloaded
    - [ ] Support dynamic scene switching
    - [x] Load/save scene to file
    - [x] Handle entity management

    ### GUI
    - [x] Implement ImGui
    - [ ] Support dynamic gui loading with interfaces
    - [ ] Create resource preview
    - [ ] Create asset manager
    - [ ] Create resource viewer(s)

    ### Networking
    - [ ] Implement networking socket support

    ### Scripting
    - [ ] Create scripting engine
    - [ ] Support start and update functions
    - [ ] Create gizmo rendering
    - [ ] Implement python scripting support
    - [ ] Implement Lua scripting support

    ### OS Support
    - [x] Support Linux
    - [ ] Support Windows

    ### Experimental
    - [ ] Implement raycasting/raytracing rendering
    - [ ] Implement radiosity lighting


## Compilation
Run compile script:
```
./compile.sh clean
```

Or use CMake/Make directly:
```
cmake -S . -B build
cmake --build build
```

## Usage
Run executable:
```
./build/graphics
```

## License
This project is licensed under the [Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0).

