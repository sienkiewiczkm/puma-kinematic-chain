#pragma once

#include "PumaAnimator.hpp"

namespace application
{

class PumaFastAnimator:
    public PumaAnimator
{
public:
    PumaFastAnimator();
    virtual ~PumaFastAnimator();

protected:
    virtual void onStart();
    virtual void onStep();

    PumaConfiguration _endConfiguration;
};

}

