#define GL_VERSION_4_4
#include <GL/glew.h>
#include <SDL_opengl.h>

// includes
#include <map>
#include <assert.h>
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <memory>

#include "window.hpp"
#include "frame_timer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "entity.hpp"
#include "la_extended.h"
using namespace LA;
#include "camera.hpp"
#include "model.hpp"
#include "frame.hpp"
#include "mesh_renderer.hpp"
#include "first_person.hpp"
#include "resource_manager.hpp"
#include "asset_manager.hpp"


class GraphicsEngine {

    public:
        void* context;
        WindowManager* window;
        frame_timer ft = frame_timer();
        Frame* frame = NULL;
        int width, height;
        Entity* rootEntity = NULL;
        Entity* workspaceCamera = NULL;

        FirstPersonController* fpc;
        Camera* camera;

        GraphicsEngine(WindowManager* window) {
            AttachWindow(window);
            SetViewport(window->width, window->height);
            
            // Initialise GLEW
            // Must be done before any opengl call
            // easier done before application instanced
            glewExperimental = GL_TRUE;
            glewInit();

            frame = new Frame(window->width, window->height);
            frame->SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);
             
            GL_Interface::BindFrameBufferObj(0);

            // load default shader(s)
            resourceShaders.Add(new Shader("base", "shaders/base.vs", "shaders/base.fs"));

            // load default model(s)
            assetManager.Load("models/presets/cone.gltf");
            assetManager.Load("models/presets/cube.gltf");
            assetManager.Load("models/presets/cylinder.gltf");
            assetManager.Load("models/presets/dome.gltf");
            assetManager.Load("models/presets/ico_sphere.gltf");
            assetManager.Load("models/presets/plane.gltf");
            assetManager.Load("models/presets/prism.gltf");
            assetManager.Load("models/presets/sphere.gltf");

            /// TODO: load project shader(s)
            rootEntity = new Entity("scene", NULL);

            LoadScene("scene/Default.scene");

            // Entity* cube = new Entity("cube", rootEntity);
            // rootEntity->AddChild(cube);
            // Component* c = new MeshRenderer(*cube, resourceMeshes.GetId("cube::0::Cube"));
            // cube->AddComponent(c);

            workspaceCamera = new Entity("Workspace Camera", NULL);
            camera = new Camera(*workspaceCamera);
            fpc = new FirstPersonController(*workspaceCamera);

            resourceShaders.Get("base")->Use();
            camera->SetResolution(width, height);
        }

        ~GraphicsEngine() {
            // delete context; delete makes error even though pointer?!?
            // delete window; causesd unknown signal error?
            delete frame;
        }

        bool LoadModelResources(std::string filepath) {
            std::ifstream inputFile(filepath);
            std::string inputDir = filepath.substr(0, filepath.find_last_of("/"));

            if (!inputFile.is_open()) {
                std::cout << "WARNING: Unable to open default model file." << std::endl;
                return false;
            }

            std::string line;
            std::istringstream lineStream;
            std::string key, value;
            while (std::getline(inputFile, line)) {
                if (line.length() == 0 || line[0] == '#') {
                    continue; // Skip the line if it starts with "#" or has length 0
                }
                lineStream = std::istringstream(line);
                
                if (std::getline(lineStream, key, '=')) {
                    if (std::getline(lineStream, value)) {
                        value = inputDir + "/" + value;
                        std::cout << "Key: " << key << ", Value: " << value << std::endl;
                        assetManager.Load(value);
                    }
                } else {
                    std::cout << "WARNING: Default models.txt invalid syntax" << std::endl;
                }
            }
            // inputFile.close();
            return true;
        }

        std::string GetFileName(std::string filepath, bool withExtension=false) {
            std::filesystem::path path(filepath);
            std::string filename = path.filename().string();
            if (withExtension)
                return filename;
            size_t lastDotIndex = filename.find_last_of(".");
            if (lastDotIndex != std::string::npos) {
                return filename.substr(0, lastDotIndex);
            }
            return filename;
        }

        bool LoadScene(std::string filepath) {
            std::ifstream inputFile(filepath);
            
            if (!inputFile.is_open()) {
                std::cout << "WARNING: Unable to open scene file." << std::endl;
                return false;
            }
            
            std::string line;
            int lineCount = 0;
            while (std::getline(inputFile, line)) {
                lineCount++;
                std::cout << "Line " << lineCount << ": " << line << std::endl;
                // scene file line
                std::string name;
                std::vector<float> floatValues;

                std::istringstream lineStream(line);
                std::string value;

                if (std::getline(lineStream, name, ',')) {
                    while (std::getline(lineStream, value, ',')) {
                        try {
                            float floatValue = std::stof(value);
                            floatValues.push_back(floatValue);
                        } catch (const std::exception& e) {
                            std::cout << "WARNING (Graphics): Invalid line format (" << lineCount << ")" << std::endl;
                            break;
                        }
                    }
                }

                if (floatValues.size() != 9) {
                    std::cout << "WARNING (Graphics): Invalid line format (" << lineCount << ")" << std::endl;
                } else {
                    ObjId resId = resourceMeshes.GetId(name);
                    Entity* ent = new Entity(name, rootEntity);
                    rootEntity->AddChild(ent);
                    Component* c = new MeshRenderer(*ent, resId);
                    ent->AddComponent(c);
                }
            }
            
            inputFile.close();
            return true;
        }

        bool AttachWindow(WindowManager* window) {
            if (window == NULL) {
                return false;
            }
            this->window = window;
            return true;
        }

        bool DetachWindow() {
            this->window = NULL;
            return true;
        }
        
        // function to reset our viewport after a window resize
        void SetViewport(int _width, int _height) {
            // protect against divide by 0 and no resoltuion
            if (_width == 0) {
                std::cout << "WARNING (Graphics): Trying to set width 0" << std::endl;
                _width = 1;
            }
                
            if (_height == 0) {
                std::cout << "WARNING (Graphics): Trying to set height 0" << std::endl;
                _height = 1;
            }
                
            this->width = _width;
            this->height = _height;
            float ratio = width / height;
        }

        void RenderObject(Entity* entity, mat4 root_trans = mat4()) {
            mat4 model = root_trans *  entity->transform.GetTransform();
            for (int i = 0; i < entity->GetNumChildren(); i++) {
                RenderObject(entity->GetChild(i), model);
            }

            // ensure mesh not empty
            MeshRenderer* renderer = entity->GetComponent<MeshRenderer>();
            if (renderer == nullptr || renderer->GetMeshId() == 0 )
                return; 

            resourceShaders.Get("base")->SetMat4("iModel", &model[0][0]);
            renderer->Render();
        }

        void Render() {
            frame->Bind();
            frame->Clear();
            
            GL_Interface::SetViewport(width, height);

            // GL_Interface::DisableFeature(FEATURE_DEPTH);
            GL_Interface::DisableFeature(FEATURE_CULL);
            GL_Interface::EnableFeature(FEATURE_DEPTH);
            // GL_Interface::EnableFeature(FEATURE_CULL);
            GL_Interface::SetFrontFace(FRONT_CW);

            resourceShaders.Get("base")->Use();

            camera->SetResolution(width, height);
            fpc->Update();
            
            std::cout << fpc->x << ":" << fpc->y << std::endl;

            resourceShaders.Get("base")->SetMat4("iView", &fpc->view[0][0]);
            resourceShaders.Get("base")->SetMat4("iProjection", &camera->proj[0][0]);

            resourceShaders.Get("base")->SetVec3("iResolution", window->width, window->height, 1.0f);
            resourceShaders.Get("base")->SetFloat("iTime", ft.GetTotalElapsed());
            resourceShaders.Get("base")->SetFloat("iTimeDelta", ft.GetFrameElapsed());
            resourceShaders.Get("base")->SetInt("iFrame", ft.GetFrameCount());

            RenderObject(rootEntity);

            frame->Unbind();
            
            ft.Frame();
        }

};