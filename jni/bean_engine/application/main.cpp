
#ifdef __ANDROID__

#include <jni.h>
#include <android_native_app_glue.h>
#endif

#include "BApplication.h"
#include "Log.h"
#include <stdlib.h>
#include <math.h>

#if defined (__ANDROID__)
void android_main(struct android_app* state)
#elif defined (__SFML__)

#include "SFMLApplication.h"
#include "ResourcePath.hpp"
//#include <boost/program_options.hpp>

struct Options {
    BString op;
    BString *out;
};


static void handle_application_options(int ac, char** av, SFMLApplication::Context* context) {
    
    Options _opts[] = {
        {"--application_name", &context->appName},
        {"--scripts_path", &context->scriptsDir},
        {"--resources_path", &context->path},
        {"--filesystem_root", &context->filesystem},
        {"--main_script_path", &context->script},
        {"", NULL}
    };
    
    for (int i = 0; i < ac - 1; i++) {
        BString arg = av[i];
        if (arg.length() > 2 && arg[0] == '-' && arg[1] == '-') {
            
            for (int index = 0; _opts[index].out != NULL; index++)
                if (arg == _opts[index].op) {
                    *_opts[index].out = BString(av[i + 1]);
                }
        }
    }
    if (context->scriptsDir == "")
        context->scriptsDir = context->path;
    if (context->path == "")
        context->path = context->scriptsDir;
}

int main(int ac, char** av)
#endif
{
#ifdef __SFML__
    SFMLApplication::Context* state = new SFMLApplication::Context();
    state->appName = "BeanEngine";
    state->screenWidth = 960;
    state->screenHeight = 640;
    state->path = resourcePath();
    Log::debug(state->path.c_str());
    state->path = /*"/Volumes/HData/SVN/beanproject/sources/Android/BeanEngine/assets";*/"/Volumes/HData/runtime-EclipseApplication/EIP Forum";
    state->scriptsDir = "/Volumes/HData/runtime-EclipseApplication/EIP Forum/gen";
    state->script = "/scripts/interfaces/main.lua";
    state->debugMode = true;
    state->filesystem = ".";
    handle_application_options(ac, av, state);
    Log::debug(state->path.c_str());
    Log::debug(state->scriptsDir.c_str());
    Log::debug(state->filesystem.c_str());
#endif
    
    BApplication* app = BApplication::instance(state);
	srand(time(NULL));
	app->setIndependentMode(true);
	app->run();
#ifndef __ANDROID__
    return 0;
#endif
}
