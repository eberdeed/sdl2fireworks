/**********************************************************
 *   SDL2Fireworks:  A class to encapsulate creating an 
 *   OpenGL fireworks display with sound using SDL2.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   03/2020 San Diego, California USA
 * ********************************************************/

#ifndef SDL3FIREWORKS_H
#define SDL3FIREWORKS_H

#include "commonheader.h"
#include "shader.h"
#include "camera.h"

/** \class SDL2Fireworks Displays an OpenGL fireworks display with 
 *  sound using SDL2.  The amount of particles per display and the 
 *  number of explosions per second can be adjusted from the 
 *  commonheader.h file's defines.  The framebufferSize of the 
 *  screen in pixels can be adjusted in this file.  
 */
class SDL2Fireworks
{
public:
    /** \brief The constructor is where the initialization occurs
     *  and the execution loop is called, making the class 
     *  self-contained.
     */
    SDL2Fireworks();
    /** \brief The destructor is where the pointers for
     *  the camera and shader classes are deleted.
     */
    ~SDL2Fireworks();
    /** \brief The execution loop where the ongoing drawing
     *  of the display occurs.
     */
    void execLoop();
    /** \brief SDL2 variables for window display.
     */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_GLContext context;
    /** \brief initSound defines the particles and their paths
     *  while also providing the lifespan of each particle.
     */
    void initFire();
    /** \brief genCoords generates the start and end points
     *  for the normal for each particle's path and 
     *  then uses testNorm to see if that normal already
     *  exists.  If not, the normal is multiplied by
     *  a small scalar for the start position and 
     *  a large scalar for the end position.
     */
    vec3* genCoords();
    /** \brief testNorm tests each particle's path to 
     *  see if there is already a particle on it.
     *  If there is it returns false, otherwise true.
     */
    bool testNorm(vec3 normal, int x);
    /** \brief update provides the center point of each
     *  explosion, its color, and the total time the
     *  explosion lasts.
     */
    void update(int startup, int shaderVal);
    /** \brief framebufferSize uses glViewport 
     *  to adjust the size of the window.
     */
    void framebufferSize(int width, int height);
    /** \brief keyDown handles key events.
     */
    void keyDown(SDL_Event e);
    /** \brief windowEvent handles window resizing,
     *  closing, minimization and maximization.
     */
	void windowEvent(SDL_Event e);
    /** \brief mouseMove handles changes in the
     *  mouse pointer coordinates.
     */
	void mouseMove(SDL_Event e);
    /** \brief logSDLError takes an output stream and
     *  a message and passes the message through the
     *  provided output stream.
     */
    void logSDLError(ostream &os, const string &msg);
    /** \brief defineObjects defines the OpenGL 
     *  buffer array used to draw the fireworks.
     */
    void defineObjects();
    /** \brief sndMaker creates the explosion sound.
     */
    static void sndMaker();   
    /** \brief debug provides the particle information.
     *  two levels of debug are provided by the 
     *  booleans debug1 and debug2, when set to 
     *  true oodles of debug information is provided.
     */
    void debug();
    //! Initial screen width and height.
    const unsigned int SCR_WIDTH = 1000;
    const unsigned int SCR_HEIGHT = 800;
    //! Ongoing screen width and height.
    unsigned int width, height;
    //! Return value from the sound thread.
    int threadReturnValue;
    //! Initialization data for each explosion.
    InitData explData[NUM_EXPLOSIONS];
    /** The explosion's sound threads, each
     *  containing an explosion sound.
     */
    SDL_Thread *thread[NUM_EXPLOSIONS];
    /** Class booleans, quit determines whether 
     *  the program is running.
     */
    bool quit, isFirst = true, debug2 = false, altSet = false;
    /** \brief Shaders for the display of the explosions,
     *  because we are using framebuffer objects to 
     *  display the explosion, we need two sets of shaders.
     */
    Shader *shader[NUM_EXPLOSIONS];
    /** \brief A camera to allow the viewer to move about.
     */
    Camera *camera;
    //! Transform matrices.
    mat4 view, projection, scaler;
    //! Mouse movement variables.
    float lastX, lastY, xpos, ypos;
    bool firstMouse = true;
    //! The array containing the particle data.
    ParticleData particles[NUM_PARTICLES];
    //! The array containing all the particle path normals.
    vec3 chckNorm[NUM_PARTICLES];
    //! The time point to seed the random number generator.
    chrono::system_clock::time_point tdata;
    //! The time points used for display and mouse timing.
    chrono::system_clock::time_point timebegin, timeend, intend, intbegin;
    //! The OpenGL buffer and vertex array handles.
    GLuint VAO = 0;
    //! Time counter.
    float interval;
    //! Posotive Negative sign for random values.
    float sign = 1.0f;
    //! Color to fill the frame buffer with.
    const GLfloat black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    //! Value to fill the depth buffer with.
    const GLfloat one = 1.0f;
};
bool debug1 = false;
#endif // SDL3FIREWORKS_H
/** \breif The main.  
 *  I keep the main as simple as possible.
 */
int main(int argc, char **argv)
{
    SDL2Fireworks squeaky;
    return 0;
}
