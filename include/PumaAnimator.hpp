#pragma once

#include "PumaCalculator.hpp"

namespace application
{

class PumaAnimator
{
public:
    PumaAnimator();
    virtual ~PumaAnimator();

    double getElapsed() const;
    double getDuration() const;
    void setDuration(double animationDuration);
    void setTarget(const std::shared_ptr<PumaCalculator>& target);

    void startAnimation(
        const PumaInverseKinematicsInput& start,
        const PumaInverseKinematicsInput& end
    );

    void play();
    void pause();
    void restart();

    void update(double deltaSeconds);

protected:
    virtual void onStart() = 0;
    virtual void onStep() = 0;

    bool _isPlaying;
    double _animationElapsed;
    double _animationDuration;

    std::shared_ptr<PumaCalculator> _target;
    PumaInverseKinematicsInput _start;
    PumaInverseKinematicsInput _end;
};

}
