/**********************************************************
 *   SDL2Fireworks:  A class to encapsulate creating an 
 *   OpenGL window.
 *   Created by: Edward Charles Eberle <eberdeed@eberdeed.net>
 *   12/2019 San Diego, California USA
 * ********************************************************/

#include "../include/sdl2fireworks.h"


SDL2Fireworks::SDL2Fireworks()
{
    cout << "\n\n\tCreating SDL2Fireworks\n\n";
    quit = false;
    camera = new Camera(SCR_WIDTH, SCR_HEIGHT, vec3(0.0f, 0.0f, 18.0f), vec3(0.0f, 0.0f, 0.0f)); 
    tdata = chrono::system_clock::now();
    // Fill in particle data array
    srand ((long int)chrono::system_clock::to_time_t(tdata));
    // Create the data.
    initFire();
    scaler = mat4(1.0);
    scaler = scale(scaler, vec3((float)SCALE, (float)SCALE, (float)SCALE));
    width = SCR_WIDTH;
    height = SCR_HEIGHT;
    execLoop();
}

SDL2Fireworks::~SDL2Fireworks()
{
    cout << "\n\n\tDestroying SDL2Fireworks\n\n";
    delete camera;
    for (int x = 0; x < NUM_EXPLOSIONS; x++)
    {
        delete shader[x];
    }
    glDeleteVertexArrays(1, &VAO);
}

void SDL2Fireworks::initFire()
{
    bool isUnique = true;
    vec3 *pos;
    cout << "\n\n\tIn initSound.\n\n";
    for (int i = 0; i < NUM_PARTICLES; i++ )
    {
        do
        {
            pos = genCoords();
            // Lifetime of particle
            if (i == 0)
            {
                chckNorm[0] = pos[2];
            }
            else
            {
                isUnique = testNorm(pos[2], i);
            }
        } while(!isUnique);
        // Lifetime of particle
        particles[i].lifetime = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        particles[i].lifetime.x = (float)(rand() % 500) + 500;
        particles[i].startPos = vec4(pos[0], 1.0f);
        particles[i].endPos = vec4(pos[1], 1.0f);
    }
    if (debug1)
    {
        debug();
    }
    timebegin = chrono::system_clock::now();
}

void SDL2Fireworks::debug()
{
        for (int x = 0; x < NUM_PARTICLES; x++)
    {
        cout << "\n\tStart:  " << particles[x].startPos.x 
        << ", " << particles[x].startPos.y << ", "
        << particles[x].startPos.z << ", " 
        << particles[x].startPos.w << "  End:  "
        << particles[x].endPos.x << ", " 
        << particles[x].endPos.y << ", " 
        << particles[x].endPos.z << ", " 
        << particles[x].endPos.w << "  Lifetime:  "
        << particles[x].lifetime.x;
    }
}
vec3* SDL2Fireworks::genCoords()
{
    vec3 *pos, startPos, endPos, normal;
    pos = new vec3[3];
    float startdist, enddist;
        // End position of particle
        if ((rand() % 1000) >= 500)
        {
            sign = -1.0f;
        }
        else
        {
            sign = 1.0f;
        }
        endPos.x = sign *((((float)(rand() % 5000) / 10000.0f) + 0.5f) - 1.0f) / 2.0f;
        if ((rand() % 1000) >= 500)
        {
            sign = -1.0f;
        }
        else
        {
            sign = 1.0f;
        }
        endPos.y = sign * ((((float)(rand() % 5000) / 10000.0f) + 0.5f) - 1.0f) / 2.0f;
        if ((rand() % 1000) >= 500)
        {
            sign = -1.0f;
        }
        else
        {
            sign = 1.0f;
        }
        endPos.z = sign * ((((float)(rand() % 5000) / 10000.0f) + 0.5f) - 1.0f) / 2.0f;
        // Start position of particle
        if ((rand() % 1000) >= 500)
        {
            sign = -1.0f;
        }
        else
        {
            sign = 1.0f;
        }
        startPos.x = sign * (((float)(rand() % 10000) / 40000.0f) - 0.125f) / 2.0f;
        if ((rand() % 1000) >= 500)
        {
            sign = -1.0f;
        }
        else
        {
            sign = 1.0f;
        }
        startPos.y = sign * (((float)(rand() % 10000) / 40000.0f) - 0.125f) / 2.0f;
        if ((rand() % 1000) >= 500)
        {
            sign = -1.0f;
        }
        else
        {
            sign = 1.0f;
        }
        startPos.z = sign * (((float)(rand() % 10000) / 40000.0f) - 0.125f) / 2.0f;
        normal = normalize(endPos - startPos);
        enddist = ((float)(rand() % 10000)) / 20000.0f;
        startdist = ((float)(rand() % 10000)) / 1000000.0f;
        pos[0] = normal * startdist;
        pos[1] = normal * enddist;
        pos[2] = normal;
        return(pos);
}

bool SDL2Fireworks::testNorm(vec3 normal, int x)
{
    for (int i = 0; i < x; i++)
    {
        if ((normal.x == chckNorm[i].x) && (normal.y == chckNorm[i].y) &&
        (normal.z == chckNorm[i].z))
        {
            return false;
        }
    }
    chckNorm[x] = normal;
    return true;
}

///
//  Update time-based variables
//
void SDL2Fireworks::update(int startup, int shaderVal)
{
    vec4 centerPos;
    vec4 color;
    if (debug2)
    {
        cout << "\n\n\tIn update for " << shaderVal << ".\n\n";
    }
    
    // Pick a new start location and color
    if ((rand() % 1000) >= 500)
    {
        sign = -1.0f;
    }
    else
    {
        sign = 1.0f;
    }
    centerPos.x = sign * (((float) (rand() % 10000) / 10000.0f) - 0.5f);
    if ((rand() % 1000) >= 500)
    {
        sign = -1.0f;
    }
    else
    {
        sign = 1.0f;
    }
    centerPos.y = sign * (((float) (rand() % 10000) / 10000.0f) - 0.5f);
    centerPos.z = -(((float) (rand() % 10000) / 10000.0f) - 0.5f);
    centerPos.w = 1.0f;
    // Random color
    color.r = ((float) (rand() % 60000) / 100000.0f) + 0.3f;
    color.g = ((float) (rand() % 60000) / 100000.0f) + 0.3f;
    color.b = ((float) (rand() % 60000) / 100000.0f) + 0.3f;
    color.a = 0.5f;
    if (debug1)
    {
        cout << "\n\n\tColor: " << color.r << ", " << color.g << ", " 
        << color.b << ", " <<  color.a;
    }
    explData[shaderVal].gap = startup + 500 + rand() % 500;
    for (int x = 0; x < NUM_EXPLOSIONS; x++)
    {
        if ((shaderVal != x) && (explData[shaderVal].gap > (explData[x].gap - EXP_GAP)) 
        && (explData[shaderVal].gap < (explData[x].gap + EXP_GAP)))
        {
            explData[shaderVal].gap = startup + 500 + rand() % 500;
            x = 0;
        }
    }
    explData[shaderVal].centerPos = centerPos;
    explData[shaderVal].color = color;
    explData[shaderVal].start = true;
    shader[shaderVal]->setFloat("u_time", 0.0f);
    shader[shaderVal]->setVec3("u_centerPosition", explData[shaderVal].centerPos);
    shader[shaderVal]->setVec4("u_color", explData[shaderVal].color);
    
}

void SDL2Fireworks::execLoop()
{
    cout << "\n\n\tIn execLoop.\n\n";
    SDL_Event e;
    
    quit = false;
    try
    {
        // Setup the window
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0){
            logSDLError(cout, "SDL_Init");
            exit(1);
        }
        else
        {
            cout << "\n\n\tInitialized SDL.\n\n";
        }
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        window = SDL_CreateWindow("OpenGL Fireworks Display", 500, 200, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
        if (window == nullptr){
            logSDLError(cout, "CreateWindow");
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated SDL window.\n\n";
        }
        
        context = SDL_GL_CreateContext(window);
        if (context == nullptr){
            logSDLError(cout, "CreateContext");
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(context);
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated SDL context.\n\n";
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer == nullptr){
            logSDLError(cout, "CreateRenderer");
            SDL_DestroyWindow(window);
            SDL_GL_DeleteContext(context);
            SDL_Quit();
            exit(1);
        }
        else
        {
            cout << "\n\n\tCreated renderer.\n";
        }
        SDL_GL_MakeCurrent(window, context);
        glewExperimental=true;
        GLenum err=glewInit();
        if(err!=GLEW_OK)
        {
            //Problem: glewInit failed, something is seriously wrong.
            cout<<"\n\n\tglewInit failed, aborting."<<endl;
            exit(1);
        }
        else
        {
            cout << "\n\n\tInitialized glew.\n";
        }
        glEnable(GL_DEPTH_TEST);  
        glDepthFunc(GL_LESS);
        glDepthRange(0.1f, 1000.0f);
        for (int x = 0; x < NUM_EXPLOSIONS; x++)
        {
            stringstream ss;
            ss << x + 1;
            shader[x] = new Shader();
            shader[x]->initShader("/usr/share/openglresources/shaders/vertex.glsl",
            "/usr/share/openglresources/shaders/fragment.glsl", "shader-" + ss.str() + ".bin");
            cout << "\n\n\tShader1-" << x + 1 << " created.\n\n";
        }
    }
    catch(exception exc)
    {
        cout << "\n\n\tProgram Initialization Error:  " << exc.what() << "\n\n";
    }
    if (debug2)
    {
        cout << "\n\n\tStarting update cycle.\n\n";
    }
    for (int x = 0; x < NUM_EXPLOSIONS; x++)
    {
        update(0, x);
    }
    explData[0].gap = 0;
    thread[0] = SDL_CreateThread((SDL_ThreadFunction)SDL2Fireworks::sndMaker, "TestThread", (void *)NULL);
    explData[0].start = false;
    defineObjects();
    interval = 0;
    // render loop
    // -----------
    while (!quit)
    {
        if (interval > 100000000)
        {
            for (int x = 0; x < NUM_EXPLOSIONS; x++)
            {
                update(0, x);
            }
            explData[0].gap = 0;
            thread[0] = SDL_CreateThread((SDL_ThreadFunction)SDL2Fireworks::sndMaker, "TestThread", (void *)NULL);
            explData[0].start = false;
            interval = 0;
        }
        for (int x = 0; x < NUM_EXPLOSIONS; x++)
        {
            interval += 10;
            if (debug1)
            {
                cout << "\n\n\tProcessing interval:  " << interval << ".\n\n";
            }
            if ((explData[x].gap < (interval - 30)) && (explData[x].start))
            {
                /* Simply create a thread */
                thread[x] = SDL_CreateThread((SDL_ThreadFunction)SDL2Fireworks::sndMaker, "SoundThread", (void *)NULL);

                if (NULL == thread[x]) {
                    logSDLError(cout, "SDL_CreateThread failed");
                } else {
                    if (debug1)
                    {
                        cout << "\n\n\tThread " << x << " successfully created.\n\n";
                    }
                }
                explData[x].start = false;
            }
            if (interval >= explData[x].gap)
            {
                if (interval > (explData[x].gap + 1000))
                {
                    SDL_WaitThread(thread[x], &threadReturnValue);
                    cout << "\n\n\tThread returned value:  " << threadReturnValue << "\n\n";
                    update(interval, x);
                }
                // render
                // ------
                intbegin = chrono::system_clock::now();
                projection = camera->getPerspective();
                view = camera->getViewMatrix();
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClearDepthf(1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glViewport(0, 0, width, height);
                shader[x]->Use();
                shader[x]->setFloat("u_time", interval - explData[x].gap);
                shader[x]->setVec4("u_color", explData[x].color);
                shader[x]->setVec4("u_centerPosition", explData[x].centerPos);
                shader[x]->setMat4("view", view);
                shader[x]->setMat4("projection", projection);
                shader[x]->setMat4("scaler", scaler);
                glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
                intend = chrono::system_clock::now();
                
                while (SDL_PollEvent(&e))
                {
                    keyDown(e);
                    mouseMove(e);
                    windowEvent(e);
                };
                if (debug1)
                {
                    cout << "\n\tSwapping window for:  " << x;
                }
                SDL_GL_SwapWindow(window);
            }
        }
    }
    //! ------------------------------------------------------------------
    SDL_DestroyWindow(window);
    SDL_GL_DeleteContext(context);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

//! ---------------------------------------------------------------------------------------------------------
// process all input: query SDL2 whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void SDL2Fireworks::keyDown(SDL_Event e)
{
    int delta = (int)
    //! Use timing to create a cameraSpeed variable.
    chrono::duration_cast<chrono::nanoseconds>(intend -
    intbegin).count();
    float deltaTime = (float)delta / 1000000.0f;
    float cameraSpeed = 0.5f * deltaTime;
    //! Motion keys.
    //! Forward motion.
    if (e.type == SDL_KEYDOWN) 
    {
        cout << "\n\n\tIn keyDown:  " << SDL_GetKeyName(e.key.keysym.sym) << "\n\n";
        switch (e.key.keysym.sym)
        {
            case SDLK_w:
                camera->processKeyboard(Camera::Camera_Movement::FORWARD, cameraSpeed);
                break;
            //! Backwards motion.
            case SDLK_s:
                camera->processKeyboard(Camera::Camera_Movement::BACKWARD, cameraSpeed);
                break;
            //! Move left.
            case SDLK_a:
                camera->processKeyboard(Camera::Camera_Movement::LEFT, cameraSpeed);
                break;
            //! Move right.
            case SDLK_d:
                camera->processKeyboard(Camera::Camera_Movement::RIGHT, cameraSpeed);
                break;
            //! Move up.
            case SDLK_r:
                camera->processKeyboard(Camera::Camera_Movement::UP, cameraSpeed);
                break;
            //! Move down.
            case SDLK_f:
                camera->processKeyboard(Camera::Camera_Movement::DOWN, cameraSpeed);
                break;
            //! Reset the camera.
            case SDLK_z:
                camera->resetCamera();
                break;
            //! Reverse the camera.
            case SDLK_x:
                camera->reverseDirection();
                break;
            //! Zoom keys.
            //! Zoom in.
            case SDLK_UP:
                camera->processMouseScroll(Camera::Camera_Movement::CLOSER);
                break;
            //! Zoom out.
            case SDLK_DOWN:
                camera->processMouseScroll(Camera::Camera_Movement::AWAY);
                break;
            case SDLK_ESCAPE:
                quit = true;
                break;
           case SDLK_RALT:
                altSet = true;
                break;
            case SDLK_LALT:
                altSet = true;
                break;
            case SDLK_RETURN:
                if (altSet)
                {
                    cout << "\n\n\tSet window fullscreen.\n\n";
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    altSet = false;
                }
                break;
        }
    }
}    

void SDL2Fireworks::windowEvent(SDL_Event e)
{
    if (e.type == SDL_WINDOWEVENT) {
        switch (e.window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            cout << "\n\n\tWindow " << e.window.windowID << " shown.\n\n";
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            cout << "\n\n\tWindow " << e.window.windowID << " hidden.\n\n";
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            cout << "\n\n\tWindow " << e.window.windowID << " exposed.\n\n";
            break;
        case SDL_WINDOWEVENT_MOVED:
            cout << "\n\n\tWindow " << e.window.windowID << " moved to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            break;
        case SDL_WINDOWEVENT_RESIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " resized to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            width = (unsigned  int)e.window.data1;
            height = (unsigned  int)e.window.data2;
            glViewport(0, 0, e.window.data1, e.window.data2);
            camera->resizeView(e.window.data1, e.window.data2);
            defineObjects();
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            cout << "\n\n\tWindow " << e.window.windowID << " size changed to " 
            << e.window.data1 << ", " << e.window.data2 << "\n\n";
            width = (unsigned  int)e.window.data1;
            height = (unsigned  int)e.window.data2;
            glViewport(0, 0, e.window.data1, e.window.data2);
            camera->resizeView(e.window.data1, e.window.data2);
            defineObjects();
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " minimized.\n\n";
            width = (unsigned  int)0;
            height = (unsigned  int)0;
            glViewport(0, 0, 0, 0);
            camera->resizeView(0, 0);
            defineObjects();
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            cout << "\n\n\tWindow " << e.window.windowID << " maximized.\n\n";
            width = (unsigned int)e.window.data1;
            height = (unsigned int)e.window.data2;
            SDL_SetWindowSize(window, width, height);
            glViewport(0, 0, width, height);
            camera->resizeView(width, height);
            defineObjects();
            break;
        case SDL_WINDOWEVENT_RESTORED:
            cout << "\n\n\tWindow " << e.window.windowID << " restored.\n\n";
            break;
        case SDL_WINDOWEVENT_ENTER:
            cout << "\n\n\tMouse entered window " << e.window.windowID << ".\n\n";
            break;
        case SDL_WINDOWEVENT_LEAVE:
            cout << "\n\n\tMouse left window " << e.window.windowID << ".\n\n";
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            cout << "\n\n\tWindow " << e.window.windowID << " gained keyboard focus.\n\n";
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            cout << "\n\n\tWindow " << e.window.windowID << " lost keyboard focus.\n\n";
            break;
        case SDL_WINDOWEVENT_CLOSE:
            cout << "\n\n\tWindow " << e.window.windowID << " closed.\n\n";
            quit = true;
            break;
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            cout << "\n\n\tWindow " << e.window.windowID << " is offered focus.\n\n";
            break;
        case SDL_WINDOWEVENT_HIT_TEST:
            cout << "\n\n\tWindow " << e.window.windowID << " has a special hit test.\n\n";
            break;
        default:
            cout << "\n\n\tWindow " << e.window.windowID << " received an unknown event.\n\n";
            break;
        }
    }
}

void SDL2Fireworks::mouseMove(SDL_Event e)
{
    if (e.type == SDL_MOUSEWHEEL)
    {
            if(e.wheel.y > 0)
            {
                camera->processMouseScroll(Camera::Camera_Movement::CLOSER);
            }
            else if (e.wheel.y < 0)
            {
                camera->processMouseScroll(Camera::Camera_Movement::AWAY);
            }
    }
    else if( e.type == SDL_MOUSEMOTION)
    {
        camera->processMouseMovement(e.motion.xrel, e.motion.yrel);
    }
    
}    


void SDL2Fireworks::logSDLError(ostream &os, const string &msg)
{
    os << "\n\n\t" << msg << " error: " << SDL_GetError() << "\n\n";
}

void SDL2Fireworks::defineObjects()
{
    if (VAO > 0)
    {
        cout << "\n\n\tDeleted VAO.\n\n";
        glDeleteBuffers(1, &VAO);
    }
    //Create vertex buffer.
    glGenBuffers(1, &VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT,
    GL_FALSE, sizeof(ParticleData), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT,
    GL_FALSE, sizeof(ParticleData), (GLvoid*) (4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT,
    GL_FALSE, sizeof(ParticleData), (GLvoid*) (8 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}    

void SDL2Fireworks::sndMaker()
{
    Uint32 wave_len; // length of our sample
    Uint8 *wave_buf; // buffer containing our audio file
    SDL_AudioSpec wave_spec, ret_spec; // the specs of our piece of music
    if (debug1)
    {
        cout << "\n\n\tStarting sound production.";
        cout << "\n\tLoading WAV file.\n\n";
    }
    
    if (SDL_LoadWAV("/usr/share/openglresources/sounds/explosion.wav", &wave_spec, &wave_buf, &wave_len) ==  NULL)
    {
        cout << "\n\n\tUnable to load WAV file:  " << SDL_GetError() << "\n\n";
        exit(-1);
    }
    else
    {
        if (debug1)
        {
            cout << "\n\n\tWAV file successfully loaded.\n\n";
        }
    }
	// set the callback function
	/*
	wave_spec.callback = SDL2Triangle::my_audio_callback;
	wave_spec.userdata = NULL;
	// set our global static variables
	audio_buf = wave_buf; // copy sound buffer
	audio_len = wave_len; // copy file length
	*/
    string name  = SDL_GetAudioDeviceName(0, 0);
    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(name.c_str(), 0, &wave_spec, &ret_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
    if (dev < 0)
    {
        cout << "\n\n\tUnable to open audio device:  " << SDL_GetError() << "\n\n";
        exit(-1);
    }
    else
    {
        if (debug1)
        {
            if (debug1)
            {
                cout << "\n\n\tAudio device successfully opened.\n\n";
            }
        }    
        int result = SDL_QueueAudio(dev, wave_buf, wave_len);
        if (result != 0)
        {
            cout <<  "\n\n\tUnable to queue audio data.\n\n";
            exit(-1);
        }
        else
        {
            if (debug1)
            {
                cout << "\n\n\tAudio data successfully queued.\n\n";
            }
        }
    }
    SDL_PauseAudioDevice(dev, 0); /* start audio playing. */
    if (debug1)
    {
        cout << "\n\n\tPlayed the audio.\n\n";
    }
    SDL_Delay(800); /* let the audio callback play some sound for 5 seconds. */
    SDL_CloseAudioDevice(dev);
    if (debug1)
    {
        cout << "\n\n\tClosed audio device.\n\n";
    }
	SDL_FreeWAV(wave_buf);
}
