#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "notchsensorsample.h"

class RenderContext
{
public:
    virtual void OnRenderNotchFrame(NotchSensorSample*) = 0;
};

#endif // RENDERCONTEXT_H
