#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Window.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Animation.h"
#include "Player.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Cube* Window::cube;
Skeleton* test;
Skin* test2;
Animation* a;
Player* player;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects() {
    // Create a cube
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));

    test = new Skeleton();
    test2 = new Skin(test);
    a = new Animation();
    test->Load("wasp.skel");
    test2->Load("wasp.skin");
    a->Load("wasp_walk.anim");

    player = new Player();
    player->setClip(a, test);

    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete cube;
    delete test;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    //cube->update();
    test->Update();
    test2->Update();
    player->Update(0.005);
 
        
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    //cube->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

    //test->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    test2->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

    ImGui::Begin("This is a Window to Change DOFs");
    //ImGui::Text("Slide to change DOFs");
    float size = 3.0f;
    //ImGui::SliderFloat("Size", &size, 0.f, 5.0f);
    ImGui::Text("X DOFs");
    for (int i = 0; i < test->joints.size(); i++)
    {
        Joint* jt = test->joints.at(i);
        ImGui::SliderFloat(jt->jointName, &jt->dofs.at(0)->value, jt->rotxlimit.x, jt->rotxlimit.y);
    }
    ImGui::Text("Y DOFs");
    for (int i = 0; i < test->joints.size(); i++)
    {
        Joint* jt = test->joints.at(i);
        ImGui::SliderFloat(jt->jointName, &jt->dofs.at(1)->value, jt->rotylimit.x, jt->rotylimit.y);
    }
    ImGui::Text("Z DOFs");
    for (int i = 0; i < test->joints.size(); i++)
    {
        Joint* jt = test->joints.at(i);
        ImGui::SliderFloat(jt->jointName, &jt->dofs.at(2)->value, jt->rotzlimit.x, jt->rotzlimit.y);
    }

    
    ImGui::End();

    

    // Rendering
    //(Your code clears your framebuffer, renders your other stuff etc.)
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // (Your code calls glfwSwapBuffers() etc.)


    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            default:
                break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (!ImGui::GetIO().WantCaptureMouse) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            LeftDown = (action == GLFW_PRESS);
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            RightDown = (action == GLFW_PRESS);
        }
    }
    
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    
    if (!ImGui::GetIO().WantCaptureMouse) {
        int maxDelta = 100;
        int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
        int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

        MouseX = (int)currX;
        MouseY = (int)currY;

        // Move camera
        // NOTE: this should really be part of Camera::Update()
        if (LeftDown) {
            const float rate = 1.0f;
            Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
            Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
        }
        if (RightDown) {
            const float rate = 0.005f;
            float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
            Cam->SetDistance(dist);
        }
    }
    
}