#pragma once

#include "PumaAnimator.hpp"

namespace application
{

class PumaStraightLineAnimator:
    public PumaAnimator
{
public:
    PumaStraightLineAnimator();
    virtual ~PumaStraightLineAnimator();

protected:
    virtual void onStart();
    virtual void onStep();
};

}

